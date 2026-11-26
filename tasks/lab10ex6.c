#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/inotify.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN ( 1024 * ( EVENT_SIZE + 16 ) )

int main() {
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    // create the inotify instance
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // add a watch on /tmp specifically for file creation (IN_CREATE)
    wd = inotify_add_watch(fd, "/tmp", IN_CREATE);
    if (wd < 0) {
        perror("inotify_add_watch");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // loop until a file is created in /tmp
    int file_created = 0;
    while (!file_created) {
        // read() blocks until at least one event occurs
        length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
            break;
        }

        i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            
            // check if the event has a name and is a file creation, and not a directory
            if (event->len) {
                if ((event->mask & IN_CREATE) && !(event->mask & IN_ISDIR)) {
                    printf("New file created.\n");
                    file_created = 1;
                    break;
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    // clean up watch descriptor and file descriptor
    inotify_rm_watch(fd, wd);
    close(fd);
}
