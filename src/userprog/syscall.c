#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"
#include "filesys/file.c"
static void syscall_handler (struct intr_frame *);

struct file* process_get_file (int fd);

struct process_file // file of process
{
	struct file *file; // the file
	int fd; // fd that link to the file
	struct list_elem elem; // store it as list
};

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();
}

void halt(void){
	shutdown_power_off();
}

void exit (int status){
	
	// check if the current thread is still in the pool or not (stay_alive?) // looping in the pool to check
	// if alive --> we have to add status to the thread to "status"
	// printf("%s: exit(%d\n", );
	//thread_exit();
	/*
	struct thread *cur = current_thread();
	if (stay_alive(cur)){ // if stay alive
		cur->status = status;
	}*/
	thread_exit();
}

int read (int fd, void *buffer, unsigned size){
	
	
	if (fd == 0){ // if it is STANDARD_INPUT
		uint8_t* new_buff = (uint8_t*) buffer;
		for (int i = 0; i < size; ++i)
		{
			new_buff[i] = input_getc();
		
		}
		return size; 
	}

	enum intr_level old_level = intr_disable ();

	struct file *f = process_get_file (int fd);
	if (!f){ // if there is no match-fd file in process collection
		intr_set_level(old_level);
		return -1;
	}
	
	intr_set_level (old_level);

	int read_size = file_read(file , buffer, size);
	return read_size;
}



int write (int fd, const void *buffer, unsigned size){
	//file_write (struct file *file, const void *buffer, off_t size) 
	
	if (fd == 1){ // STANDARD_OUTPUT

		putbuf(buffer, size); // write data into buffer by "size" byte
		return size;
	}
	
	enum intr_level old_level = intr_disable();
	struct file *f = process_get_file (int fd);
	if (!f){
		intr_set_level(old_level);
		return -1;
	}
	intr_set_level(old_level);


	int write_size = file_write(f , buffer, size);
	return write_size;
}

struct file* process_get_file (int fd)
{
	stuct thread *t = thread_current();
	sturct list_elem *e;

	for (e = list_begin(&t->file_list); e != list_end(&t->file_list); e = list_next(e))
	{
		struct process_file* pf = list_entry(e, struct process_file, elem);
		if (fd == pf->fd){ // if the wanted file is in process_file
			return pf->file;
		}
	}
	return NULL;
}