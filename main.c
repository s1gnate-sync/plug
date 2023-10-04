#include "dtach.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
 
char *progname = "plugctl";
char *sockname;
int detach_char = -1;
int no_suspend = 1;
int redraw_method = REDRAW_WINCH;
struct termios orig_term;
int dont_have_tty = 1;

#define PATH_MAX 255

static int fd = -1;
static char *fifo = NULL;

void sig_handler(int sig) {
	if (fd != -1) {
		close(fd);
	}

	if (fifo != NULL) {
		unlink(fifo);
	}
	
	fifo = NULL;

	exit(0);
}

int run_fifo() {
	if (fifo == NULL) {
		return 1;
	}
	
	signal(SIGINT, sig_handler);
	signal(SIGABRT, sig_handler);
	signal(SIGHUP, sig_handler);

	while (1) {	
		if (fifo == NULL) {
			break;
		}
		
		mkfifo(fifo, 0600);

		char code[PATH_MAX] = "";
		char *c = code;
		char socket[PATH_MAX] = "";
		char *s = socket;
		size_t x = PATH_MAX;

		fd = open(fifo, O_RDONLY);			
		FILE *stream = fdopen(fd, "r");

		getdelim(&c, &x, '\0', stream);
		getdelim(&s, &x, '\0', stream);	

		fclose(stream);
		close(fd);	
		fd = -1;

		if (strcmp(code,"") != 0 && strcmp(socket,"") != 0) {
			pid_t p = fork();
			if (p < 0) break;
			
			if (p == 0) {
				execlp(progname, progname, "create", socket, code, (char *)NULL);
				return 1;
			}
		}

		if (fifo == NULL) {
			break;
		}
		
		fd = open(fifo, O_WRONLY);
		write(fd, progname, 1); 
		close(fd);
		fd = -1;
	}
	

	if (fd != -1) {
		close(fd);
	}

	if (fifo != NULL) {
		unlink(fifo);
	}

	return 1;
}


int main(int argc, char **argv)
{
	progname = *argv;
	++argv;--argc;
	
	if (argc < 1) {
		printf("Usage: %s listen <fifo>|create <socket> <cmd> <args>|connect <socket>\n\n", progname);
		printf("To spawn a command in listener environment: \n\n");
		printf("  echo -e -n \"cmd\\0socket\\0\" > fifo # without arguments \n");
		printf("  cut -c1 fifo > /dev/null\n");
		printf("  %s connect <socket>\n\n", progname);
		return 1;
	}
	
	char *mode = *argv;
	++argv;--argc;

	if (strcmp(mode, "listen") == 0) {
		fifo = *argv;
		return run_fifo();
	} 

	sockname = *argv;
	++argv; --argc;
	if (tcgetattr(0, &orig_term) < 0) {
		memset(&orig_term, 0, sizeof(struct termios));
	}

	
	if (strcmp(mode, "create") == 0) {
		if (argc < 1) {
			return 1;
		}

		return master_main(argv, 0, 0);
	} else if (strcmp(mode, "connect") == 0) {
		if (argc > 0) {
			return 1;
		}

		return attach_main(0);
	}

	return 1;
}
