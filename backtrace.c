#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <execinfo.h>
#include <time.h>


#define MAX_SAVE_CALL_LAYERS 40
void append_backtrace(const char *path, char *fmt, ...)
{
	int fd;
	va_list vl;
	char  msg[512];
	int len;
	time_t timep;
	void *array[MAX_SAVE_CALL_LAYERS];
	size_t size;

	time (&timep);
	sprintf(msg, "\n[%s", ctime(&timep));
	len = strlen(msg);
	msg[len -1] = ']';
	va_start( vl, fmt );
	vsnprintf( msg+len, sizeof(msg)-len, fmt, vl );
	msg[sizeof(msg)-1] = '\0';
	va_end( vl );

	fd = open(path, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	if(fd < 0) return;
	write(fd, msg, strlen(msg));

	size = backtrace (array, MAX_SAVE_CALL_LAYERS);
	backtrace_symbols_fd (array, size, fd);

	close(fd);
}

#define MEDIA_DEBUG_DIR "/media/debug"

static void core_dump_save(void)
{
	struct stat st;

	if (stat(MEDIA_DEBUG_DIR, &st) < 0) {
		mkdir(MEDIA_DEBUG_DIR, 0755);
	}

    printf("core_dump_save\n");

	append_backtrace(MEDIA_DEBUG_DIR"/dp_backtrace.log", "dp pid(%d):\n", getpid());
}

void test3(void)
{
    core_dump_save();
}

void test2(void)
{
    test3();
}

void test1(void)
{
    test2();
}

int main(int argc, char const *argv[])
{
    test1();

    /* code */
    return 0;
}
