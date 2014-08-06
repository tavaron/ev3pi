#ifndef __PIDFILE_HPP
#define __PIDFILE_HPP

#include <stdio.h>


int createPidFile(char *s);
int removePidFile(char *s);

int createPidFile(char *s) {
	/* open or overwrite file */
    FILE *f = fopen(s, "w");
    if(f = NULL)
        return 1;
	
	/* write pid to file */
    fprintf(f, "%d", getpid());
    
    /* close file */
    fclose(f);
    return 0;
}

int removePidFile(char *s) {
	/* remove file and return error code */
    int r = remove(s);
    return r;
}

#endif // __PIDFILE_HPP
