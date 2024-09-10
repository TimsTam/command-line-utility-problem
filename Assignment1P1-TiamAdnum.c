#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#ifdef _WIN32
    #include <direct.h> // windows-specific directory functions
    #define MKDIR(path) _mkdir(path) // define MKDIR to use _mkdir on Windows
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define MKDIR(path) mkdir(path, 0700) // define MKDIR to use mkdir with mode on POSIX systems
#endif

// Function to evaluate a prefix expression recursively
int evaluatePrefixExpression(char** expr, int* index) {
    char* token = expr[(*index)++]; // get current token & increment index
    if (token[0] == '+') {
        // recursively evaluate left and right operands
        return evaluatePrefixExpression(expr, index) + evaluatePrefixExpression(expr, index);
    } else if (token[0] == '-') {
        // recursively evaluate left and right operands
        return evaluatePrefixExpression(expr, index) - evaluatePrefixExpression(expr, index);
    } else {
        return atoi(token); // convert to integer and return token
    }
}

// Function for the calculate command
void calculate(char* input) {
    char* expr[100]; // array for tokens from prefix expression
    int index = 0;

    // tokenize input string
    char* token = strtok(input, " ");
    while (token != NULL) {
        expr[index++] = token; // store each token in the expr array
        token = strtok(NULL, " ");
    }

    int startIndex = 0;
    int result = evaluatePrefixExpression(expr, &startIndex); // evaluate prefix expression
    printf("Result: %d\n", result);
}

// Function for the time command
void displayTime() {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime); // get the current time
    localTime = localtime(&currentTime); // convert to local time structure
    printf("Current local time and date: %s", asctime(localTime));
}

// Function for the path command
void displayPath() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) { // get the current working directory
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
    }
}

// Function for the sys command
void displaySysInfo() {
    // print basic system information
    printf("Operating System: %s\n", "Linux");
    printf("CPU Type: %s\n", "x86_64");
}

// Function for the put command
void putFiles(char* dirname, char** filenames, int numFiles, int force) {
    struct stat st = {0};

    // if directory does not exist, create it
    if (stat(dirname, &st) == -1) {
        if (MKDIR(dirname) == -1) {
            perror("mkdir failed"); // Print error if directory creation fails
            return;
        }
    // if directory exists and force is specified, remove it and recreate
    } else if (force) {
        char command[1024];
        snprintf(command, sizeof(command), "rm -rf %s", dirname); // create a command to remove the directory
        system(command); // execute command
        if (MKDIR(dirname) == -1) {
            perror("mkdir failed");
            return;
        }
    } else {
        printf("Directory %s already exists. Use -f to overwrite.\n", dirname); // inform user that directory exists
        return;
    }

    // copy each file into created directory
    for (int i = 0; i < numFiles; i++) {
        FILE *src = fopen(filenames[i], "r"); // open source file for reading
        if (src == NULL) {
            printf("File %s not found.\n", filenames[i]);
            continue;
        }

        char destPath[1024];
        snprintf(destPath, sizeof(destPath), "%s/%s", dirname, filenames[i]); // create destination path
        FILE *dest = fopen(destPath, "w"); // open destination file for writing

        char ch;
        // copy content from source to destination
        while ((ch = fgetc(src)) != EOF) {
            fputc(ch, dest);
        }

        fclose(src); 
        fclose(dest); 
        printf("File %s copied to %s\n", filenames[i], destPath);
    }
}

// Function for the get command
void getFileContent(char* filename) {
    FILE *file = fopen(filename, "r"); // open file for reading
    if (file == NULL) {
        printf("File %s not found.\n", filename); 
        return;
    }

    char line[256];
    int lineCount = 0;
    // read & print file content, 40 lines each time
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        if (++lineCount % 40 == 0) {
            printf("Press any key to continue...\n");
            getchar(); // wait for user input
        }
    }

    fclose(file);
}

// Main function
int main() {
    char input[1024];
    char command[100];
    char args[1024];

    while (1) {
        printf("> ");
        fgets(input, sizeof(input), stdin); // get user input
        input[strlen(input) - 1] = '\0'; // remove newline character

        sscanf(input, "%s %[^\n]", command, args); // split input into command and arguments

        if (strcmp(command, "calculate") == 0) {
            calculate(args); // call calculate function
        } else if (strcmp(command, "time") == 0) {
            displayTime(); // call time function
        } else if (strcmp(command, "path") == 0) {
            displayPath(); // call path function
        } else if (strcmp(command, "sys") == 0) {
            displaySysInfo(); // call sys function
        } else if (strcmp(command, "put") == 0) {
            char* dirname = strtok(args, " "); // get directory name
            char* filenames[100];
            int numFiles = 0;
            int force = 0;

            char* token;
            while ((token = strtok(NULL, " ")) != NULL) {
                if (strcmp(token, "-f") == 0) {
                    force = 1; // set force flag if -f is found
                } else {
                    filenames[numFiles++] = token; // store each filename listed
                }
            }

            putFiles(dirname, filenames, numFiles, force); // call put function
        } else if (strcmp(command, "get") == 0) {
            getFileContent(args); // call get function
        } else if (strcmp(command, "quit") == 0) {
            break; // exit loop and quit program
        } else {
            printf("Unknown command: %s\n", command); // error message for unknown command input
        }
    }

    return 0; 
}
