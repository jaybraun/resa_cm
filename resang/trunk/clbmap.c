#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define handle_error(msg) \
	{ perror(msg); exit(EXIT_FAILURE); }
#define PERMS 0666     /* RW for owner, group, others */
#define BUFSIZE 512
#define bboard_loc 0x10000000
#define dtedboard_loc 0x13000000
#define geomapboard_loc 0x16000000


char *c_mmap(int *fd, long loc_array, int *size);
char *c_mmap_dted(int *fd, long loc_array, int *size);
char *c_mmap_geomap(int *fd, long loc_array, int *size);

int mmap_file_open(int *fd, char file[], int *size, int *rc)
{
	size_t len;
	struct stat sb;
	int ifd, i;
	char buf[BUFSIZE] = {'\0'};
	len = *size * 512;      				 /* pages to bytes */
	/* check for existence of bb */
	ifd = open(file, O_RDONLY);
	if (ifd == -1) {   /* create file with given size */
		ifd = creat(file, PERMS);
		if (ifd == -1)
			handle_error("mmap creat");
		for (i=1; i<=len; i=i+BUFSIZE)  /* zero file contents */
			if (write (ifd, buf, BUFSIZE) != BUFSIZE)
				handle_error("mmap zero file");				
	}
	/* close(ifd);  */

	ifd = open(file, O_RDWR);
	if (ifd == -1)
		handle_error("mmap open");
	if (fstat(ifd, &sb) == -1)
		handle_error("mmap fstat");
	if (sb.st_size < len)
		handle_error(" mmap size");
	*fd = ifd;
}

char *c_mmap(int *fd, long loc_array, int *size)
/* map given file into shared memory */
{
	char *addr;
	size_t len;
	int ifd;
	off_t offset, pa_offset;
	len = *size;
	pa_offset = 0;
	ifd = *fd;
	void *tloc = (char*)bboard_loc;

 	addr=mmap(tloc,len,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_FIXED, ifd,pa_offset);
 
	if (addr == MAP_FAILED)
		handle_error("mmap");
	return(addr);
}

int c_munmap(size_t len)
{
	int rc;
	void *tloc = (char*)bboard_loc;
	rc = munmap(tloc, len);
	if (rc < 0)
		handle_error("munmap");
	return(rc);
}

int c_msync(size_t len)
{
	int rc;
	void *tloc = (char*)bboard_loc;
	rc = msync(tloc, len, MS_SYNC);
	if (rc < 0)
		handle_error("msync");
	return(rc);
}

/* DTED mapping routines below same as BB routines above except for fixed starting address
   and dted appended to routine name
 */

char *c_mmap_dted(int *fd, long loc_array, int *size)
/* map given file into shared memory */
{
	char *addr;
	size_t len;
	int ifd;
	off_t offset, pa_offset;
	len = *size;
	pa_offset = 0;
	ifd = *fd;
	void *tloc = (char*)dtedboard_loc;

 	addr=mmap(tloc,len,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_FIXED, ifd,pa_offset);
 
	if (addr == MAP_FAILED)
		handle_error("mmap");
	return(addr);
}

int c_munmap_dted(size_t len)
{
	int rc;
	void *tloc = (char*)dtedboard_loc;
	rc = munmap(tloc, len);
	if (rc < 0)
		handle_error("munmap");
	return(rc);
}

int c_msync_dted(size_t len)
{
	int rc;
	void *tloc = (char*)dtedboard_loc;
	rc = msync(tloc, len, MS_SYNC);
	if (rc < 0)
		handle_error("msync");
	return(rc);
}

/* GEOMAP mapping routines below same as BB routines above except for fixed starting address
   and dted appended to routine name
 */

char *c_mmap_geomap(int *fd, long loc_array, int *size)
/* map given file into shared memory */
{
	char *addr;
	size_t len;
	int ifd;
	off_t offset, pa_offset;
	len = *size;
	pa_offset = 0;
	ifd = *fd;
	void *tloc = (char*)geomapboard_loc;

 	addr=mmap(tloc,len,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_FIXED, ifd,pa_offset);
 
	if (addr == MAP_FAILED)
		handle_error("mmap_geomap");
	return(addr);
}

int c_munmap_geomap(size_t len)
{
	int rc;
	void *tloc = (char*)geomapboard_loc;
	rc = munmap(tloc, len);
	if (rc < 0)
		handle_error("munmap_geomap");
	return(rc);
}

int c_msync_geomap(size_t len)
{
	int rc;
	void *tloc = (char*)geomapboard_loc;
	rc = msync(tloc, len, MS_SYNC);
	if (rc < 0)
		handle_error("msync_geomap");
	return(rc);
}

