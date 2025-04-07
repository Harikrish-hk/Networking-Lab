// --- server.c ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main() {
    int sockfd, newfd;
    struct sockaddr_in saddr, caddr;
    socklen_t len = sizeof(caddr);
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    listen(sockfd, 3);
    printf("Server listening on port %d...\n", PORT);

    newfd = accept(sockfd, (struct sockaddr *)&caddr, &len);
    if (newfd < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }

    FILE *fp = fopen("rec.txt", "w");
    if (!fp) {
        perror("File open error");
        close(newfd);
        close(sockfd);
        exit(1);
    }

    int count = 0, ch = 0;
    read(newfd, &count, sizeof(int)); // Receive word count

    while (ch < count) {
        bzero(buffer, SIZE);
        read(newfd, buffer, SIZE);    // Receive each word
        fprintf(fp, "%s ", buffer);   // Write word with space
        ch++;
    }

    printf("File received successfully!\n");

    fclose(fp);
    close(newfd);
    close(sockfd);
    return 0;
}
