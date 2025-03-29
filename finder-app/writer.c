#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    openlog("writer", LOG_PID, LOG_USER);

    if (argc < 3) {
        syslog(LOG_ERR, "Not enough arguments. Usage: %s writefile writestr", argv[0]);
        closelog();
        return 1;
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];
    
    FILE *file = fopen(writefile, "w");
    if (!file) {
        syslog(LOG_ERR, "Error opening file: %s", writefile);
        closelog();
        return 1;
    }

    fprintf(file, "%s\n", writestr);
    fclose(file);
    
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
    closelog();
    
    return 0;
}
