/*
 * unix.c
 *
 *  Created on: 8 Nov 2012
 *      Author: Andrew Bell
 */

#include "unix.h"

// component functions

void behaviour_start(args_s *args)
{
	void (*behaviour)() = args->behaviour;
	Ithis_s *this_ptr = args->this_ptr;
	int argc = args->argc;
	void **argv = args->argv;

	(*behaviour)(this_ptr, argc, argv);

	free(args);
}

void *component_create(void(*behaviour)(void*), int struct_size, int stack_size, int argc, void *argv[])
{
    Ithis_s *this_ptr;

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
    pthread_mutex_lock(init); // lock init so it will block after pthread_create returns. locked here so it's definitely locked on thread start
    pthread_create(thread, &attr, (void*)behaviour_start, (void*)args);

    // wait for component initialisation to complete
    pthread_mutex_lock(init);
    // init has served its purpose, clean up
    pthread_mutex_destroy(init);
    free(init);

    void **temp;
    //pthread_join(*thread, temp);
    return this_ptr;
}

void component_stop(void *this_ptr)
{
	((Ithis_s *)this_ptr)->stopped = true;
	return;
}

void component_exit()
{
	//pthread_exit(NULL);
	return;
}

void component_yield(void)
{
	return;
}

// channel functions
static void Channel_decRef(Channel_PNTR pntr){
        channel_unbind(pntr);
	DAL_decRef(pntr->buffer);
        DAL_decRef(pntr->connections);
}

Channel_PNTR channel_create(int typesize)
{
	Channel_PNTR this = (Channel_PNTR)DAL_alloc(sizeof(Channel), true);
        if(this == (void*) 0){
                DAL_error(OUT_OF_MEMORY_ERROR);
                return NULL;
        }

        this->decRef = Channel_decRef;
	this->typesize = typesize;
	this->buffer = DAL_alloc(typesize, false); // sending data by value in this implementation, so allocate buffer
	this->ready = false;
	this->connections = DAL_alloc(sizeof(List_PNTR), false);
        DAL_assign(this->connections, Construct_List()); // empty list of connections
	sem_init(&(this->conns_sem), 0, 0);	// initialise semaphore to 0 connections
	pthread_mutex_init(&this->mutex, NULL);
	pthread_mutex_init(&this->blocked, NULL);

#if DEBUG > 1
        printf("Construct_HalfChannel: ch=%p ready=%b connections=%p typesize=%d \n",
                        this, this->ready, this->connections, this->typesize);
#endif
        return(this);
}

bool channel_bind(Channel_PNTR id1, Channel_PNTR id2)
{
	pthread_mutex_lock(&(id1->mutex));
	pthread_mutex_lock(&(id2->mutex));

	// check not both IN or OUT
	if(id1->direction == id2->direction)
	{
		return false;
	}

	// check not already connected
	// assuming bind always adds to both channels' lists, we only need to check one channel for the other
	if(containsElement(id1->connections, (void*)id2))
	{
		return false;
	}

	// add to conns lists
	insertElement(id1->connections, id2);
	insertElement(id2->connections, id1);

	// increment conns sems
	sem_post(&(id1->conns_sem));
	sem_post(&(id2->conns_sem));

	pthread_mutex_unlock(&(id1->mutex));
	pthread_mutex_unlock(&(id2->mutex));
	return true;
}

void channel_unbind(Channel_PNTR id)
{
        pthread_mutex_lock(&(id->mutex));

	// iterate through list, locking then disconnecting
	unsigned int length = getListLength(id->connections);
	Channel_PNTR opposite; // channel on the opposite side of current connection

	int i;
	for(i = 0; i < length; i++)
	{
		opposite = getElementN(id->connections, i);
		pthread_mutex_lock(&(opposite->mutex));
		removeElement(opposite->connections, id);
		pthread_mutex_unlock(&(opposite->mutex));
		removeElement(id->connections, opposite);
	}

        pthread_mutex_unlock(&(id->mutex));
	return;
}

int channel_select(struct select_struct *s)
{
	return 0;
}

int channel_send(Channel_PNTR id, void *buffer, jmp_buf *ex_handler)
{
	return 0;
}

int channel_receive(Channel_PNTR id, void *buffer, bool in_ack_after)
{
	return 0;
}

int channel_multicast_send(Channel_PNTR id, void *buffer)
{
	return 0;
}

void remoteAnonymousUnbind_proc(Channel_PNTR id, void* var)
{
	return;
}
