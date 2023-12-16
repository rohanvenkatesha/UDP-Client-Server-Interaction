#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

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

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    unsigned long long receivedValue;
    //char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        die("Error creating socket");
    }

    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    // Bind socket to address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        die("Error binding socket");
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);

    while (1) {
        // Receive data from the client
        ssize_t recvSize = recvfrom(serverSocket, &receivedValue, sizeof(unsigned long long), 0, (struct sockaddr *)&clientAddr, &addrLen);

        if (recvSize == -1) {
            die("Error receiving message");
        }

        // Print current system time and the received value
        printCurrentTime();
        printf("Received value: %llu from %s\n", receivedValue, inet_ntoa(clientAddr.sin_addr));
    }

    // Close the socket
    close(serverSocket);

    return 0;
}
