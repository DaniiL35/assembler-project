#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void removeFiles() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, "file1", 5) == 0 && strcmp(entry->d_name, "file.as") != 0) {
            if (remove(entry->d_name) != 0) {
                perror("Unable to remove file");
            } else {
                printf("File %s removed successfully\n", entry->d_name);
            }
        }
    }

    closedir(dir);
}

int main() {
    removeFiles();
    return 0;
}