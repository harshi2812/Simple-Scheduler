#include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/mman.h>
    #include <fcntl.h>
    #include <semaphore.h>
   

    struct SharedMemoryData {
        char executableName[256];
    };
    void sendExecutableToSharedMemory(const char *executableName) {
        //printf("Sent executable name: %s\n", executableName);
        int shm_fd = shm_open("/executablename", O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) { 
            perror("shm_open");
            return;
        }
        ftruncate(shm_fd, sizeof(struct SharedMemoryData));
        struct SharedMemoryData *sharedData = (struct SharedMemoryData *)mmap(NULL, sizeof(struct SharedMemoryData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (sharedData == MAP_FAILED) {
            perror("mmap");
            return;
        }

        strncpy(sharedData->executableName, executableName, sizeof(sharedData->executableName) - 1);
        //printf("Sent executable name: %s\n", executableName);
        munmap(sharedData, sizeof(struct SharedMemoryData));
        close(shm_fd);
    }

    void savehistory(char history[100][100], int *historyCount, const char *command) {
        if (*historyCount < 100) {
            int i = 0;
            while (command[i] != '\0' && i < 99) {
                history[*historyCount][i] = command[i];
                i++;
            }
            history[*historyCount][i] = '\0';
            (*historyCount)++;
        } else {
            printf("History is full. Cannot add more commands.\n");
            printf("");
        }
    }

    int Process(char *command[]) {
        pid_t pid, wpid;
        int status;

        pid = fork();
        if (pid == 0) {
            if (execvp(command[0], command) == -1) {
                perror("KSTM shell");
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("KSTM shell");
        } else {
            if (wait(&status) == -1) {
                perror("KSTM shell");
            }
        }

        return 0;
    }

    void PrintHistory(char history[100][100], int historyCount) {
        printf("Command History:\n");
        for (int i = 0; i < historyCount; i++) {
            printf("%d: %s\n", i + 1, history[i]);
        }
    }

    int main() {
        char inputarr[100];
        char words[20][20];
        int wordCount = 0;
        char history[100][100];
        char* executable;
        int historyCount = 0;
        char* before[50];  
        char* between[50];
        char* after[50]; 
        int ncpu, tslice;

        printf("No. of CPU resources: ");
        scanf("%d", &ncpu);
        printf("Time Slice in milliseconds: ");
        scanf("%d", &tslice);
        
        char ncpu_str[20];
        char tslice_str[20];

        snprintf(ncpu_str, sizeof(ncpu_str), "%d", ncpu);
        snprintf(tslice_str, sizeof(tslice_str), "%d", tslice);

        if (fork() == 0) {
            execl("./SimpleScheduler", "./SimpleScheduler", ncpu_str, tslice_str, (char *)0);
            perror("execl");
            exit(1);
        }
    
        while (1) {
            printf("<KSTM shell>: ");

            if (scanf(" %[^\n]", inputarr) != 1) {
                printf("Input read failed.\n");
                return 1;
            }
            if (strlen(inputarr) > 0) {
                savehistory(history, &historyCount, inputarr);
            }   
            //printf("%d",&inputarr[0]);

            if (strcmp(inputarr, "history") == 0) {
                // Print the entire command history internally
                PrintHistory(history, historyCount);

            }
            if (strncmp(inputarr, "submit", 6) == 0) {
           
                executable = strtok(inputarr + 6, " \t\n");
            if (executable == NULL) {
                printf("Missing executable name.\n");
                continue;
    }
                printf("Executing: %s\n", executable);
                sendExecutableToSharedMemory(executable);   

              }
            else {
                int i = 0;
                int j = 0;
                while (inputarr[i] != '\0' && wordCount < 20) {
                    if (inputarr[i] != ' ') {
                        words[wordCount][j] = inputarr[i];
                        j++;
                    } else if (j > 0) {
                        words[wordCount][j] = '\0';
                        wordCount++;
                        j = 0;
                    }
                    i++;
                }
                if (j > 0) {
                    words[wordCount][j] = '\0';
                    wordCount++;
                }

                char *command[wordCount + 1];
                for (int k = 0; k < wordCount; k++) {
                    command[k] = words[k];
                }
                command[wordCount] = NULL;

                Process(command);

        int beforeIndex = 0;
        int betweenIndex = 0;
        int afterIndex = 0;
        int pipeCount = 0;
        int ok=1;

                
        for (int m = 0; m < wordCount; m++) {
        if (strcmp(words[m], "|") == 0) {
            pipeCount++;
        }

        if (pipeCount == 0) {
            before[beforeIndex] = words[m];
            beforeIndex++;
            ok++;
        } else if (pipeCount == 1) {
            between[betweenIndex] = words[m];
            betweenIndex++;
            ok--;
        } else if (pipeCount == 2) {
            after[afterIndex] = words[m];
            afterIndex++;
            ok--;
        }
    }
        

        before[beforeIndex] = NULL;
        between[betweenIndex] = NULL;
        after[afterIndex] = NULL;
                
            }

            wordCount = 0;
        }

        return 0;
    }   
