#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "devices/shutdown.h"

static void syscall_handler (struct intr_frame *);

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
	struct thread *cur = current_thread();
	if (stay_alive(cur)){ // if stay alive
		cur->status = status;
	}
	thread_exit();
}

int read (int fd, void *buffer, unsigned size){
	/*
	int read_size = 0;
	if (fd == 0){
		// data = input_getc();
		// read
	}
	else if (){ // able to read
		// // file_read (struct file *, void *, off_t);
		read_size = file_read(fd , buffer, size);
		return size;
	}
	else if (read_size == 0){ //if error
		return -1;
	}
	*/
	enum intr_level old_level = intr_disable ();

	int read_size = file_read(fd , buffer, size);

	intr_set_level (old_level);
}



int write (int fd, const void *buffer, unsigned size){
	//file_write (struct file *file, const void *buffer, off_t size) 
	
	/*
	int write_size = 0;
	if (fd == 1){
		// data = putbuf();
		// read
	}
	else if (){ // able to read
		// // file_read (struct file *, void *, off_t);
		write_size = file_write(fd , buffer, size);
		return write_size;
	}
	else if (write_size == 0){ //if error

		return -1;
	}
	*/
	enum intr_level old_level = intr_disable();
	int write_size = file_write(fd , buffer, size);
	intr_set_level(old_level);
}
