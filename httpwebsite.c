#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 80
#define BUFFER_SIZE 1024

void handle_client(int client_socket){
        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received<0){
                perror("Error in receiving");
                exit(EXIT_FAILURE);
        }
        printf("Received request: \n%s\n", buffer);

	FILE *html_file = fopen("index.html","r");
	if (html_file == NULL){
		perror("Error opening HTML file");
		exit(EXIT_FAILURE);
	}
	char html_response[50000] = {0};
	size_t bytes_read = fread(html_response, sizeof(char), 50000, html_file);
    	fclose(html_file);
	const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(client_socket, response, strlen(response), 0);
	send(client_socket, html_response, bytes_read, 0);
        printf("Response sent\n");

        close(client_socket);
}

int main(){
        int server_fd, client_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);


        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


        int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY; // Bind to any available interface
        address.sin_port = htons(PORT);

        // Bind the socket to the specified address and port
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }

        // Listen for incoming connections
        if (listen(server_fd, 3) < 0) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }
	
	system("hostname");
	system("hostname -I");


        printf("Server listening on port %d...\n", PORT);

        while(1){
                 if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New connection accepted\n");

        // Handle client request
        handle_client(client_socket);

        }




}
