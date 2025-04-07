// --- client.c ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in saddr;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    FILE *fp = fopen("file.txt", "r");
    if (!fp) {
        perror("File open error");
        close(sockfd);
        exit(1);
    }

    int count = 0;
    while (fscanf(fp, "%s", buffer) != EOF) {
        count++;
    }
    rewind(fp);

    write(sockfd, &count, sizeof(int)); // Send word count

    while (fscanf(fp, "%s", buffer) != EOF) {
        write(sockfd, buffer, strlen(buffer) + 1); // Send each word with null terminator
    }

    printf("File sent successfully!\n");

    fclose(fp);
    close(sockfd);
    return 0;
}
