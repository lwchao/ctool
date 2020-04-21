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
#include <sys/mman.h>

#define SHM_TEST_FILE "shm_test_file"
#define MAP_FAILED ((void *) -1)
#  ifndef  PROT_READ
#   define PROT_READ            0x01
#  endif
#  ifndef  PROT_WRITE
#   define PROT_WRITE           0x02
#  endif
#  ifndef  MAP_SHARED
#   define MAP_SHARED           0x01
#  endif
#  ifndef  MAP_PRIVATE
#   define MAP_PRIVATE          0x02
#  endif

struct sg_card_info {
    int card;
    int ports;
	int hw_type;
};

void *shm_attach(const char *path, int flags, int mode, size_t size)
{
    int fd;
    struct stat buf;
	void *ptr = NULL;
	char tmp[1024];

    do {
        fd = shm_open(path, flags, mode);
        if (fd < 0) {
            printf("open file %s failed\n", path);
            if (flags & O_CREAT)
                abort();
            sleep(1);
        }
    } while (fd < 0);

    if (flags & O_CREAT) {
        ftruncate(fd, size);
    } else {
        snprintf(tmp, sizeof(tmp), "/dev/shm/%s", path);
        while (1) {
            if (stat(tmp, &buf) >= 0 && buf.st_size == size)
                break;
            printf("stat tmp: %s buf.st_size:%u\n", tmp, buf.st_size);
            sleep(1);
        }
    }

    ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        close(fd);
        printf("mmap file: %s failed\n", path);
        return NULL;
    }

    close(fd);
    return ptr;
}


int card_shm_init(void)
{
    struct sg_card_info *card = NULL;
    int mode = S_IRUSR | S_IWUSR;
	int flags = O_RDWR | O_CREAT | O_TRUNC;

	size_t size = sizeof(struct sg_card_info);
	card = shm_attach(SHM_TEST_FILE, flags, mode, size);
    if (!card) {
        return -1;
    }

    card->card = 1;
    card->ports = 20;
    card->hw_type = -1;
    return 0;
}

int card_shm_attach(void)
{
    struct sg_card_info *card = NULL;
    int mode = S_IRUSR | S_IWUSR;
	int flags = O_RDWR;

	size_t size = sizeof(struct sg_card_info);
	card = shm_attach(SHM_TEST_FILE, flags, mode, size);
    if (!card) {
        return -1;
    }

    printf("card: %d, ports: %d, hw_type:%d\n", card->card, card->ports,
	   card->hw_type);
   
    return 0;
}

int main(void) 
{
	// card_shm_init();
	card_shm_attach();
	return 0;
}
