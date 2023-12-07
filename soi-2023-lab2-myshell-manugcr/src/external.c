#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define BACKGROUND 4

int flags[BACKGROUND]   = {0};
int job_bg[BACKGROUND]  = {0};
int pid_bg[BACKGROUND]  = {0};
int job_ID              = 0;
int background_flag     = 0;

static void get_flag(char *command)
{
    if (strrchr(command, '&'))
    {
        background_flag = 1;
        command[strcspn(command, "&")] = '\0';
    }
}

static void CHLD_Handler()
{
    pid_t pid;
    for (int i = 0; i < BACKGROUND; i++)
    {
        if ((pid = waitpid(-1, NULL, WNOHANG)) == pid_bg[i] && pid_bg[i] != 0)
        {
            flags[i] = 1;
            break;
        }
    }
}

void external_command(char *command)
{
    signal(SIGCHLD, CHLD_Handler);

    get_flag(command);

    int pipe_flag = 0;
    char *pipe_token = strchr(command, '|');
    if (pipe_token != NULL)
    {
        pipe_flag = 1;
        *pipe_token = '\0'; // Split command into two parts at the pipe symbol
    }

    int input_fd = -1;
    int output_fd = -1;

    // Handle input redirection
    char *input_token = strchr(command, '<');
    if (input_token != NULL)
    {
        *input_token = '\0';
        input_fd = open(input_token + 1, O_RDONLY);
        if (input_fd == -1)
        {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
    }

    // Handle output redirection
    char *output_token = strchr(command, '>');
    if (output_token != NULL)
    {
        *output_token = '\0';
        char *output_file_path = output_token + 1;

        char full_output_path[1024];
        if (output_file_path[0] == '/')
        {
            // Absolute path
            snprintf(full_output_path, sizeof(full_output_path), "%s", output_file_path);
        }
        else
        {
            // Relative path, construct full path based on current working directory
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL)
            {
                perror("getcwd");
                exit(EXIT_FAILURE);
            }

            // Ensure that the destination buffer has enough space for both strings
            if (snprintf(full_output_path, sizeof(full_output_path), "%s/%s", cwd, output_file_path) >= sizeof(full_output_path))
            {
                fprintf(stderr, "Error: Output path is too long\n");
                exit(EXIT_FAILURE);
            }
        }

        output_fd = open(full_output_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (output_fd == -1)
        {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
    }
    pid_t fork_ID = fork();

    switch (fork_ID)
    {
        case -1:
            perror("Fork failed.\n");
            exit(EXIT_FAILURE);
            break;

        case 0:
            // Set up input redirection
            if (input_fd != -1)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }

            // Set up output redirection
            if (output_fd != -1)
            {
                dup2(output_fd, STDOUT_FILENO);
                close(output_fd);
            }

            // Handle pipe
            if (pipe_flag)
            {
                int pipefd[2];
                if (pipe(pipefd) == -1)
                {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }

                pid_t pipe_pid = fork();

                if (pipe_pid == -1)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                else if (pipe_pid == 0) // Child process for the first command
                {
                    close(pipefd[0]); // Close unused read end

                    // Redirect stdout to the pipe
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]); // Close the write end of the pipe

                    execl("/bin/sh", "sh", "-c", command, NULL);

                    perror("execl");
                    exit(EXIT_FAILURE);
                }
                else // Parent process
                {
                    close(pipefd[1]); // Close the write end of the pipe

                    // Redirect stdin to read from the pipe
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[0]); // Close the read end of the pipe

                    execl("/bin/sh", "sh", "-c", pipe_token + 1, NULL);

                    perror("execl");
                    exit(EXIT_FAILURE);
                }
            }
            else // No pipe
            {
                execl("/bin/sh", "sh", "-c", command, NULL);
                perror("execl");
                exit(EXIT_FAILURE);
            }
            break;

        default:
            if (background_flag == 0)
                waitpid(fork_ID, 0, 0); // Wait for the child process to finish in the foreground
            else
            {
                job_ID++;
                printf("[%d] %d\n", job_ID, fork_ID);

                for (int i = 0; i < BACKGROUND; i++)
                {
                    if (pid_bg[i] == 0)
                    {
                        pid_bg[i] = fork_ID;
                        job_bg[i] = job_ID;
                        break;
                    }
                }

                // Non-blocking wait for background processes
                waitpid(-1, NULL, WNOHANG);
            }

            // Close file descriptors in the parent process
            if (input_fd != -1)
                close(input_fd);
            if (output_fd != -1)
                close(output_fd);

            for (int i = 0; i < BACKGROUND; i++)
            {
                if (flags[i] == 1)
                {
                    printf("[%d]+ Done \n", job_bg[i]);
                    flags[i] = 0;
                    pid_bg[i] = 0;
                }
            }
            break;
    }
}
