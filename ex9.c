#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_CREAT | O_RDWR, 0644);
    if (fd < 0) return 1;

	char* text = "Hello, this is a sample text.";
	write(fd, text, strlen(text));

    lseek(fd, 0, SEEK_SET);
	
    char buffer[100];
    read(fd, buffer, sizeof(buffer)-1);
    buffer[100] = '\0';
    printf("%s\n", buffer);

    close(fd);
    return 0;
}
