/*
 * Exercise 2.26:
 *
 * In Section 2.3, we described a program that copies the contents of one file
 * to a destination file. This program works by first prompting the user for
 * the name of the source and destination files. Write this program using
 * either the Windows or POSIX API. Be sure to include all necessary error
 * checking, including ensuring that the source file exists.
 *     Once you have correctly designed and tested the program, if you
 * used a system that supports it, run the program using a utility that traces
 * system calls. Linux systems provide the strace utility, and Solaris and
 * Mac OS X systems use the dtrace command. As Windows systems do
 * not provide such features, you will have to trace through the Windows
 * version of this program using a debugger.
 */

/* POSIX headers */
#include <unistd.h>

/* read/write */
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <errno.h>
#include <stdbool.h>

#define DEBUG false
#if DEBUG
#include <stdio.h>
#define debug(args) printf args
#else
#define debug(args)
#endif

#define READ_MAX_SIZE 1024
#define BLOCK_SIZE 1024

void output_string(char * buf) {
    int bytes = 0;
    while(buf[bytes] != '\0') {
        bytes++;
    }
    write(STDOUT_FILENO, buf, bytes);
}

void input_string(char * buf) {
    size_t bytes_read;
    bytes_read = read(STDIN_FILENO, buf, READ_MAX_SIZE);
    buf[bytes_read - 1] = '\0'; /* replace newline */
}

void copy_file(int from_fd, int to_fd) {
    size_t bytes_read;
    char buf[BLOCK_SIZE];
    do {
        bytes_read = read(from_fd, buf, READ_MAX_SIZE);
        write(to_fd, buf, bytes_read);
    } while(bytes_read == BLOCK_SIZE);
}

int main() {
    char path_from[READ_MAX_SIZE], path_to[READ_MAX_SIZE];
    int input_fd, output_fd;

    output_string("input file: ");
    input_string(path_from);
    input_fd = open(path_from, O_RDONLY);
    debug(("%d, %d, %s\n", input_fd, errno, path_from));
    if(input_fd == -1) {
        output_string("An error occured: ");
        switch errno {
            case EACCES:
                output_string("Insufficent permissions.\n");
                break;
            case ENOENT:
                output_string("File not found.\n");
                break;
            default:
                output_string("Unknown error.\n");
        }
        return 1;
    }

    output_string("output file: ");
    input_string(path_to);
    output_fd = open(path_to,
                     O_WRONLY | O_CREAT | O_EXCL,
                     S_IRWXU | S_IRGRP | S_IROTH);
    debug(("%d, %d, %s\n", output_fd, errno, path_to));
    if(output_fd == -1) {
        switch errno {
            case EACCES:
                output_string("Insufficent permissions.\n");
                break;
            case EEXIST:
                output_string("File already exists.\n");
                break;
            default:
                output_string("Unknown error.\n");
        }
        return 1;
    }

    copy_file(input_fd, output_fd);

    close(input_fd);
    close(output_fd);

    return 0;
}
