/*
 * unix.c
 *
 *  Created on: 8 Nov 2012
 *      Author: Andrew Bell
 */

#include "unix.h"

// component functions

void behaviour_start(args_s *args) // wrapper for behaviour entry point, to pass args in
{
	void (*behaviour)() = args->behaviour;
	Ithis_s *this_ptr = args->this_ptr;
	int argc = args->argc;
	void **argv = args->argv;
	pthread_mutex_t* init = args->init;

	(*behaviour)(this_ptr, argc, argv, init);

	free(args);
	free(init);
}

void *component_create(void(*behaviour)(void*), int struct_size, int stack_size, int argc, void *argv[])
{
    Ithis_s *this_ptr;
    extern sem_t can_exit;
    extern int  num_threads;

    if((this_ptr = (Ithis_s*)DAL_alloc(struct_size, true)) == NULL)
    {
    	return NULL;
    }
    else
    {
    	memset(this_ptr, 0, (size_t)struct_size);
    }

    // need to wait until the thread has finished initialising its channels, etc.
    pthread_mutex_t* init = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(init, NULL);

    this_ptr->stopped = false;

    args_s *args = malloc(sizeof(args_s));
    args->behaviour = behaviour;
    args->this_ptr = this_ptr;
    args->argc = argc;
    args->argv = argv;
    args->init = init;

    pthread_t *thread = malloc(sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_trylock(init); // lock init so it will block until after pthread_create returns. locked here so it's definitely locked on thread start. note: do not want to block here.
    pthread_create(thread, &attr, (void*)behaviour_start, (void*)args);

    // wait for component initialisation to complete; this is so that channels that aren't yet initialised can't be bound
    pthread_mutex_lock(init);
    // init has served its purpose, clean up
    pthread_mutex_destroy(init);
    free(init);

    num_threads++;
    int val = 0;
    sem_getvalue(&can_exit, &val);
    if(val == 1) 	// lock if not locked
    {
        sem_wait(&can_exit);
    }
    return this_ptr;
}

void component_stop(void *this_ptr)
{
	((Ithis_s *)this_ptr)->stopped = true;
	return;
}

void component_exit()
{
	extern int num_threads;
	extern sem_t can_exit;

	num_threads--;
	if(num_threads == 0)
	{
		sem_post(&can_exit);
	}
	pthread_exit(NULL);
}

void component_yield(void)
{
	// don't need this in unix; scheduler is cleverer than we are
	return;
}

// channel functions
static void Channel_decRef(Channel_PNTR pntr){
        DAL_decRef(pntr->connections);
}

Channel_PNTR channel_create(int typesize, chan_dir direction)
{
	Channel_PNTR this = (Channel_PNTR)DAL_alloc(sizeof(Channel), true);
        if(this == (void*) 0){
                DAL_error(CHAN_OUT_OF_MEMORY_ERROR);
                return NULL;
        }

        this->decRef = Channel_decRef;
	this->direction = direction;
	this->typesize = typesize;
	this->ready = false;
	this->nd_received = false;
        DAL_assign(&(this->connections), Construct_List());	// empty list of connections
	sem_init(&(this->conns_sem), 0, 0);		// initialise mutexes and semaphores
	pthread_mutex_init(&(this->mutex), NULL);
	sem_init(&(this->blocked), 0, 0);
	sem_init(&(this->actually_received), 0, 0);

#if DEBUG > 1
        printf("Construct_HalfChannel: ch=%p ready=%b connections=%p typesize=%d \n",
                        this, this->ready, this->connections, this->typesize);
#endif
        return(this);
}

bool channel_bind(Channel_PNTR id1, Channel_PNTR id2)
{
	pthread_mutex_lock(&conn_op_mutex);

	// check not both CHAN_IN or CHAN_OUT
	if(id1->direction == id2->direction || id1->typesize != id2->typesize)
	{
		pthread_mutex_unlock(&conn_op_mutex);
		return false;
	}

	pthread_mutex_lock(id1->direction == CHAN_IN ? &(id1->mutex) : &(id2->mutex));
	pthread_mutex_lock(id1->direction == CHAN_IN ? &(id2->mutex) : &(id1->mutex));

	// check not already connected
	// assuming bind always adds to both channels' lists, we only need to check one channel for the other
	if(containsElement(id1->connections, (void*)id2))
	{
		pthread_mutex_unlock(&conn_op_mutex);
		return false;
	}

	// add to conns lists
	insertElement(id1->connections, id2);
	insertElement(id2->connections, id1);

	// unlock conns mutex in both channels
	int val = 0;
	sem_getvalue(&(id1->conns_sem), &val);
        if(val == 0) 	// never allow semaphore to go above 1; make it act like a mutex
	{
		sem_post(&(id1->conns_sem));
	}

	sem_getvalue(&(id2->conns_sem), &val);
	if(val == 0)
	{
		sem_post(&(id2->conns_sem));
	}

	pthread_mutex_unlock(&(id1->mutex));
	pthread_mutex_unlock(&(id2->mutex));

	pthread_mutex_unlock(&conn_op_mutex);
	return true;
}

void channel_unbind(Channel_PNTR id)
{
	pthread_mutex_lock(&conn_op_mutex);

	// iterate through list, locking then disconnecting
	unsigned int length = getListLength(id->connections);
	Channel_PNTR opposite; // channel on the opposite side of current connection
	int i;
	for(i = 0; i < length; i++)
	{
		opposite = getElementN(id->connections, i);	// fetch current opposite half-channel

       		sem_wait(id->direction == CHAN_IN ? &(id->conns_sem) : &(opposite->conns_sem) );
       		sem_wait(id->direction == CHAN_IN ? &(opposite->conns_sem) : &(id->conns_sem) );
       		pthread_mutex_lock(id->direction == CHAN_IN ? &(id->mutex) : &(opposite->mutex) );
       		pthread_mutex_lock(id->direction == CHAN_IN ? &(opposite->mutex) : &(id->mutex) );

		removeElement(id->connections, opposite);
		removeElement(opposite->connections, id);

       		pthread_mutex_unlock(&(id->mutex));
       		pthread_mutex_unlock(&(opposite->mutex));
		
		if(!isEmpty(id->connections))
		{
			sem_post(&(id->conns_sem));
		}
		if(!isEmpty(opposite->connections))
		{
			sem_post(&(opposite->conns_sem));
		}
	}

	pthread_mutex_unlock(&conn_op_mutex);
	return;
}

int channel_select(struct select_struct *s)
{
	return 0;
}

int channel_send(Channel_PNTR id, void *data, jmp_buf *ex_handler)
{
	sem_wait(&(id->conns_sem));
	pthread_mutex_lock(&(id->mutex));

	id->buffer = data;
	id->ready = true;
	pthread_mutex_unlock(&(id->mutex));
	
	// iterate through connection list, looking for receiver that is ready
	unsigned int length = getListLength(id->connections);
	Channel_PNTR match; // current receiver
	int i;
	for(i = 0; i < length; i++)
	{
		match = getNextElement(id->connections);	// fetch next channel in conns list (which keeps state across calls)
		
		pthread_mutex_lock(&(match->mutex));
		pthread_mutex_lock(&(id->mutex));
		
		if(match->ready && id->ready)
		{
			match->buffer = id->buffer;
			match->ready = false;
			id->ready = false;
			id->nd_received = true;
			
			sem_post(&(match->blocked));
			sem_wait(&(match->actually_received));
			pthread_mutex_unlock(&(id->mutex));
			pthread_mutex_unlock(&(match->mutex));

			
			int val = 0;
			sem_getvalue(&(id->conns_sem), &val);
			if(val == 0)
			{
				sem_post(&(id->conns_sem));
			}
			return 0;
		}
			
		pthread_mutex_unlock(&(id->mutex));
		pthread_mutex_unlock(&(match->mutex));		
	}

	int val = 0;
	sem_getvalue(&(id->conns_sem), &val);
	if(val == 0)
	{
		sem_post(&(id->conns_sem));
	}
	sem_wait(&(id->blocked));	// blocked should always be locked, so wait here until data is taken by active part of a receive

	return 0;
}

int channel_receive(Channel_PNTR id, void *data, bool in_ack_after)
{
	sem_wait(&(id->conns_sem));
	pthread_mutex_lock(&(id->mutex));

	id->ready = true;

	pthread_mutex_unlock(&(id->mutex));
	
	// iterate through connection list, looking for receiver that is ready
	unsigned int length = getListLength(id->connections);
	Channel_PNTR match; // current receiver
	int i;
	for(i = 0; i < length; i++)
	{
		match = getNextElement(id->connections);	// fetch next channel in conns list (which keeps state across calls)
		
		pthread_mutex_lock(&(id->mutex));
		pthread_mutex_lock(&(match->mutex));
		
		if(match->ready && id->ready)
		{
			id->buffer = match->buffer;		// found a ready sender, get pointer
			memcpy(data, id->buffer, id->typesize);	// got pointer from sender; copy data

			match->ready = false;
			id->ready = false;
			
			sem_post(&(match->blocked));
			pthread_mutex_unlock(&(match->mutex));
			pthread_mutex_unlock(&(id->mutex));

			int val = 0;
			sem_getvalue(&(id->conns_sem), &val);
			if(val == 0)
			{
				sem_post(&(id->conns_sem));
			}
			return 0;
		}
			
		pthread_mutex_unlock(&(id->mutex));
		pthread_mutex_unlock(&(match->mutex));		
	}

	int val = 0;
	sem_getvalue(&(id->conns_sem), &val);
	if(val == 0)
	{
		sem_post(&(id->conns_sem));
	}
	sem_wait(&(id->blocked));	// wait here until data is ready in active part of a send

	memcpy(data, id->buffer, id->typesize);	// receiver now has pointer; copy data
	
	sem_post(&(id->actually_received));
	
	return 0;
}

int channel_multicast_send(Channel_PNTR id, void *buffer)
{
	return 0;
}

