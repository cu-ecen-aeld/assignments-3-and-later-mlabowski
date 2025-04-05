#include "systemcalls.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd) {
    int ret = system(cmd);
    if (ret == -1) {
        perror("system() failed");
        return false;
    }
    return WEXITSTATUS(ret) == 0;
}


/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool _do_exec_v(const char *outputfile, int count, va_list args) {
    char **command = (char **)malloc((count + 1) * sizeof(char *));
    if (!command) {
        perror("malloc() failed");
        return false;
    }

    for (int i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork() failed");
        free(command);
        return false;
    }

    if (pid == 0) {
        if (outputfile) {
            int fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (fd == -1) {
                perror("open() failed");
                free(command);
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2() failed");
                close(fd);
                free(command);
                exit(EXIT_FAILURE);
            }

            close(fd);
        }

        if (execv(command[0], command) == -1) {
            perror("execv() failed");
            free(command);
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        if (waitpid(pid, &status, 0) != pid) {
            perror("waitpid() failed");
            free(command);
            return false;
        }

        bool success = WEXITSTATUS(status) == 0;
        free(command);
        return success;
    }
    return true;
}


bool do_exec(int count, ...) {
    va_list args;
    va_start(args, count);
    /*
    * TODO:
    *   Execute a system command by calling fork, execv(),
    *   and wait instead of system (see LSP page 161).
    *   Use the command[0] as the full path to the command to execute
    *   (first argument to execv), and use the remaining arguments
    *   as second argument to the execv() command.
    *
*/
    bool result = _do_exec_v(NULL, count, args);
    va_end(args);
    return result;
}


/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...) {
    va_list args;
    va_start(args, count);
    /*
    * TODO
    *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
    *   redirect standard out to a file specified by outputfile.
    *   The rest of the behaviour is same as do_exec()
    *
    */
    bool result = _do_exec_v(outputfile, count, args);
    va_end(args);
    return result;
}
