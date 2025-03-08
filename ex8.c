#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }
    printf("File Status in Current Directory:\n");
    printf("--------------------------------\n");
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (stat(entry->d_name, &filestat) == -1) {
            perror("stat failed");
            continue;
        }
        printf("Name: %s\n", entry->d_name);
        printf("Size: %ld bytes\n", fileStat.st_size);
        printf("Permissions: %o\n", fileStat.st_mode & 0777);
        printf("Last Modified: %ld\n", fileStat.st_mtime);
        printf("--------------------------------\n");
    }
    closedir(dir);
    return 0;
}
