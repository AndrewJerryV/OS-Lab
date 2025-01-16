#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_CREAT | O_RDWR, 0644);
    if (fd < 0) return 1;

    write(fd, "Hello, this is a sample text.", 29);
    lseek(fd, 0, SEEK_SET);

    char buffer[30];
    read(fd, buffer, 29);
    buffer[29] = '\0';
    printf("%s\n", buffer);

    close(fd);
    return 0;
}
