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

    this_ptr->stopped = false;

    args_s *args = malloc(sizeof(args_s));
    args->behaviour = behaviour;
    args->this_ptr = this_ptr;
    args->argc = argc;
    args->argv = argv;

    pthread_t *thread = malloc(sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(thread, &attr, (void*)behaviour_start, (void*)args);

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

static void Channel_decRef(Channel_PNTR pntr){
        channel_unbind(pntr);
	DAL_decRef(pntr->buffer);
        DAL_decRef(pntr->connections);
}

bool channel_bind(Channel_PNTR id1, Channel_PNTR id2)
{
	return false;
}

void channel_unbind(Channel_PNTR id)
{
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
