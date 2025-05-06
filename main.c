#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/param.h>
#include <dirent.h>

#define BUFFER_SIZE 4096

int main(int argc, char const *argv[])
{
    NAME_MAX;
    return 0;
}

void explore_and_backup(DIR *dest_dir, DIR *old_dir, DIR *actual_dir)
{
    struct dirent *entry;
    while ((entry = readdir(actual_dir)))
    {
        struct stat st;
        stat(&st, entry->d_name);
    }
}

int copy_file(int fd_src, int fd_dest);
int copy_if_change(char *maybe_dest, char *old_file, char *actual_file)
{
    struct stat st_old, st_actual;
    stat(old_file, &st_old);
    long old_last_modification = st_old.st_mtime;

    int fd_actual = open(actual_file, O_RDONLY);
    flock(fd_actual, LOCK_SH);
    fstat(fd_actual, &st_actual);
    long actual_last_modification = st_actual.st_mtime;

    if (actual_last_modification <= old_last_modification)
    {
        // If the file didn't changed
        flock(fd_actual, LOCK_UN);
        close(fd_actual);
        return 0;
    }
    int fd_dest = open(maybe_dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    copy_file(fd_actual, fd_dest);
    flock(fd_actual, LOCK_UN);
    close(fd_actual);
    close(fd_dest);
    return 1;
}
int copy_file(int fd_src, int fd_dest)
{
    unsigned char buffer[BUFFER_SIZE];
    ssize_t nb_read = 0;

    // cpy main loop
    while ((nb_read = read(fd_src, buffer, BUFFER_SIZE)) == BUFFER_SIZE)
    {
        write(fd_dest, buffer, BUFFER_SIZE);
    }

    // cpy the nb_total_byte % BUFFER_SIZE last bytes
    if (nb_read > 0)
    {
        write(fd_dest, buffer, nb_read);
    }
    return nb_read;
}