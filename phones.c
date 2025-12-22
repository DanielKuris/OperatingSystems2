#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define NAME_SIZE 19
#define PHONE_SIZE 13

struct phone_rec {
    char num[2];
    char name[NAME_SIZE];
    char phone_num[PHONE_SIZE];
};

void print_record(int fd, off_t offset, int whence) {
    struct phone_rec phone;

    if (lseek(fd, offset, whence) == -1) {
        perror("lseek");
        return;
    }

    if (read(fd, &phone, sizeof(phone)) != sizeof(phone)) {
        perror("read");
        return;
    }

    /* Name: remove comma and trailing spaces */
    write(1, "Name: ", sizeof("Name: ") - 1);
    for (int i = 0; i < NAME_SIZE; i++) {
	    if (phone.name[i] == ',')      // replace commas with space    
		write(1, " ", 1);             
	    else if (phone.name[i] != ' ')    // skip trailing spaces at end of name
		write(1, &phone.name[i], 1);
	}

    write(1, ", Phone: ", sizeof(", Phone: ") - 1);
    write(1, phone.phone_num, 12);
    write(1, "\n", 1);
}

int main(void) {
    int fd;
    off_t rec = sizeof(struct phone_rec);

    fd = open("phones.dat", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    /* 2nd record (SEEK_SET) */
    print_record(fd, rec, SEEK_SET);

    /* one before last (SEEK_END) */cl
    print_record(fd, -2 * rec, SEEK_END);

    /* 50th record (SEEK_SET) */
    print_record(fd, 49 * rec, SEEK_SET);

    /* 49th record (SEEK_CUR backwards) */
    print_record(fd, -2 * rec, SEEK_CUR);

    /* 51st record (SEEK_CUR forward) */
    print_record(fd, rec, SEEK_CUR);

    close(fd);
    return 0;
}
