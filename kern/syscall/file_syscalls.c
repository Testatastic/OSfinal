/*
 * File-related system call implementations.
 */

#include <types.h>
#include <kern/errno.h>
#include <kern/fcntl.h>
#include <kern/limits.h>
#include <kern/seek.h>
#include <kern/stat.h>
#include <lib.h>
#include <uio.h>
#include <proc.h>
#include <current.h>
#include <synch.h>
#include <copyinout.h>
#include <vfs.h>
#include <vnode.h>
#include <openfile.h>
#include <filetable.h>
#include <syscall.h>

/*
 * open() - get the path with copyinstr, then use openfile_open and
 * filetable_place to do the real work.
 */
int
sys_open(const_userptr_t upath, int flags, mode_t mode, int *retval)
{
	const int allflags = O_ACCMODE | O_CREAT | O_EXCL | O_TRUNC | O_APPEND | O_NOCTTY;

	char *kpath;
	struct openfile *file;
	int result;

	if(flags != allflags){
		*retval = EINVAL;
		return -1;
	}
	if(upath == NULL){
		*retval = EFAULT;
		return -1;
	}

	kpath = (void *)kmalloc(32+1);
	result = copyin(upath, (void *)kpath, 32+1);
	if(result){
		*retval = result;
		return -1;
	}

	result = openfile_open(kpath, flags, mode, &file);
	if(result){
		*retval = result;
		return -1;
	}
	result = filetable_place(curthread->t_proc->p_filetable, file, retval);

	/*
	 * Your implementation of system call open starts here.
	 *
	 * Check the design document design/filesyscall.txt for the steps
	 */
	 //retval = *file;

	return 0;
}

/*
 * read() - read data from a file
 */
int
sys_read(int fd, userptr_t buf, size_t size, int *retval)
{
       int result = 0;

			 if(fd < 0 || fd > MAX_FILE_DESCRIPTOR){
				 *retval = EBADF;
				 return -1;
			 }
			 result = filetable_get(ft, fd, file);


       /*
        * Your implementation of system call read starts here.
        *
        * Check the design document design/filesyscall.txt for the steps
        */
       (void) fd; // suppress compilation warning until code gets written
       (void) buf; // suppress compilation warning until code gets written
       (void) size; // suppress compilation warning until code gets written
       (void) retval; // suppress compilation warning until code gets written

       return result;
}

/*
 * write() - write data to a file
 */

/*
 * close() - remove from the file table.
 */

/*
* meld () - combine the content of two files word by word into a new file
*/
