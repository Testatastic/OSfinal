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

	char *kpath = (char *)kmalloc(sizeof(char)*PATH_MAX);
	struct openfile *file;
	int result;
	size_t T;

	if(flags == allflags){
		return -1;
	}


	if(upath == NULL){
		*retval = EFAULT;
		return -1;
	}

	result = copyinstr((const_userptr_t)upath, kpath, PATH_MAX, &T);
	if(result){
		*retval = EFAULT;
		return -1;
	}

	result = openfile_open((char *)kpath, flags, mode, &file);
	if(result){
		*retval = result;
		return -1;
	}
	result = filetable_place(curthread->t_proc->p_filetable, file, retval);
	if(result){
		*retval = result;
		return -1;
	}

kfree(kpath);
	return 0;
}

/*
 * read() - read data from a file
 */
int
sys_read(int fd, userptr_t buf, size_t size, int *retval)
{
       int result = 0;
			 struct openfile *file;

			 if(fd < 0 || fd > PATH_MAX){
				 *retval = EBADF;
				 return -1;
			 }
			 result = filetable_get(curthread->t_proc->p_filetable, fd, &file);
			 if(result){
				 *retval = result;
				 return -1;
			 }

			 spinlock_acquire(&file->of_reflock);
			 if(file->of_accmode == O_WRONLY){
				 *retval = EBADF;
				 spinlock_release(&file->of_reflock);
				 return EBADF;
			 }

			 struct uio x;
			 struct iovec y;
			 uio_kinit(&y, &x, buf, size, 0, UIO_READ);
			 result = VOP_READ(file->of_vnode, &x);
			 if(result){
				 *retval = result;
				 return -1;
			 }
			 file->of_offset = x.uio_offset;
			 spinlock_release(&file->of_reflock);
			 *retval = size - x.uio_resid;


       /*
        * Your implementation of system call read starts here.
        *
        * Check the design document design/filesyscall.txt for the steps
        */


       return 0;
}

/*
 * write() - write data to a file
 */
 int
 sys_write(int fd, userptr_t buf, size_t size, int *retval){
	 int result = 0;
	 struct openfile *file;

	 if(fd < 0 || fd > PATH_MAX){
		 *retval = EBADF;
		 return -1;
	 }
	 result = filetable_get(curthread->t_proc->p_filetable, fd, &file);

	 spinlock_acquire(&file->of_reflock);
	 if(file->of_accmode == O_RDONLY){
		 *retval = EBADF;
		 spinlock_release(&file->of_reflock);
		 return EBADF;
	 }

	 struct uio x;
	 struct iovec y;
	 uio_kinit(&y, &x, buf, size, 0, UIO_WRITE);
	 //x.uio_rw = UIO_WRITE;
	 result = VOP_WRITE(file->of_vnode, &x);
	 if(result){
		 *retval = result;
		 return -1;
	 }
	 file->of_offset = x.uio_offset;
	 spinlock_release(&file->of_reflock);
	 *retval = size - x.uio_resid;

	 return 0;
 }

/*
 * close() - remove from the file table.
 */
 int sys_close(int fd){
	 (void) fd;
	 return 0;
 }

/*
* meld () - combine the content of two files word by word into a new file
*/
int sys_meld(char *pn1, char *pn2, char *pn3){
	//im guessing we create file 3 if it doesnt not exist and then writing into file 3 what we read into file one and writing in what we read from file 2 into
	(void) pn1;
	(void) pn2;
	(void) pn3;

	return 0;
}
