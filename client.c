#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define DELAY_SECONDS 10

void die(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

void printCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *timeinfo = localtime(&tv.tv_sec);

    // Format the time using strftime
    char timeString[80]; // Buffer to store the formatted time
    strftime(timeString, sizeof(timeString), "%a %b %e %T %Y", timeinfo);

    printf("[%s]\n", timeString);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *inputFileName = argv[1];
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Open the file for reading
    FILE *file = fopen(inputFileName, "r");
    if (!file) {
        die("Error opening file");
    }

    // Create UDP socket
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        die("Error creating socket");
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        die("Error converting IP address");
    }

    // Read and send multiple strings to the server
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove newline character at the end
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Trim any whitespace at the end of the string
        size_t i = len - 1;
        while (i > 0 && (buffer[i] == ' ' || buffer[i] == '\t')) {
            buffer[i--] = '\0';
        }

        // Convert hexadecimal string to integer
        unsigned long long value;
        sscanf(buffer, "%llx", &value);

        // Print current system time and the value before sending
        printCurrentTime();
        printf("Sending value: %llu\n", value);

        // Send the integer value to the server
        if (sendto(clientSocket, &value, sizeof(unsigned long long), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
            die("Error sending message");
        }

        // 10-second delay
        sleep(DELAY_SECONDS);
    }

    // Close the file
    fclose(file);

    // Close the socket
    close(clientSocket);

    return 0;
}
