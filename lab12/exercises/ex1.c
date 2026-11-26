#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdbool.h>

#define DEVICE_PATH "/dev/input/by-path/platform-i8042-serio-0-event-kbd"

int main()
{
    // open the keyboard event device file in read-only mode
    int fd = open(DEVICE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("failed to open device\n");
        return EXIT_FAILURE;
    }

    // open the output file to save the logs
    FILE *output = fopen("ex1.txt", "a");
    if (output == NULL) {
        perror("failed to open the file\n");
        close(fd);
        return EXIT_FAILURE;
    }

    // state flags to track whether each shortcut key is currently held down
    struct input_event ev;
    bool e_pressed = false;
    bool x_pressed = false;
    bool p_pressed = false;
    bool a_pressed = false;
    bool c_pressed = false;
    bool m_pressed = false;

    printf("the available shortcuts of the program: P+E, C+A+P, A+M\n");
    printf("E+X for exit\n");
    fprintf(output, "the available shortcuts of the program: P+E, C+A+P, A+M, E+X for exit\n");

    while (1) {        
        ssize_t bytes = read(fd, &ev, sizeof(struct input_event));
        if (bytes < (ssize_t)sizeof(struct input_event)) {
            perror("failed to read the event");
            break;
        }

        if (ev.type == EV_KEY) {
            const char *event_type = NULL;

            if (ev.value == 1) event_type = "PRESSED";
            if (ev.value == 0) event_type = "RELEASED";
            if (ev.value == 2) event_type = "REPEATED";

            if (event_type != NULL) {
                // print to stdout
                printf("%s 0x%04x (%d)\n", event_type, ev.code, ev.code);
                fflush(stdout);

                // write to ex1.txt
                fprintf(output, "%s 0x%04x (%d)\n", event_type, ev.code, ev.code);
                fflush(output);

                // watch keys
                // update key states: true when PRESSED (1) or REPEATED (2), false when RELEASED (0)
                if (ev.code == KEY_E) {
                    e_pressed = (ev.value == 1 || ev.value == 2);
                } else if (ev.code == KEY_X) {
                    x_pressed = (ev.value == 1 || ev.value == 2);
                } else if (ev.code == KEY_P) {
                    p_pressed = (ev.value == 1 || ev.value == 2);
                } else if (ev.code == KEY_A) {
                    a_pressed = (ev.value == 1 || ev.value == 2);
                } else if (ev.code == KEY_C) {
                    c_pressed = (ev.value == 1 || ev.value == 2);
                } else if (ev.code == KEY_M) {
                    m_pressed = (ev.value == 1 || ev.value == 2);
                }

                // check shortcuts only on a new key press
                if (ev.value == 1) {
                    // terminate if both E and X are pressed simultaneously
                    if (e_pressed && x_pressed) {
                        printf("Exiting\n");
                        fprintf(output, "Exiting\n");
                        fflush(output);
                        break;
                    }

                    if (p_pressed && e_pressed) {
                        printf("I passed the Exam!\n");
                        fprintf(output, "I passed the Exam!\n");
                        fflush(output);
                    }

                    if (c_pressed && a_pressed && p_pressed) {
                        printf("Get some cappuccino!\n");
                        fprintf(output, "Get some cappuccino!\n");
                        fflush(output);
                    }

                    if (a_pressed && m_pressed) {
                        printf("i drink americano..\n");
                        fprintf(output, "i drink americano..\n");
                        fflush(output);
                    }
                }
            }
        }
    }

    fclose(output);
    close(fd);
}
