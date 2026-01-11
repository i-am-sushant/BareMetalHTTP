#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h> // Included this for close()

#include <netinet/in.h> // needed for internet address structures
#define PORT 8080

#include <string.h>

#include <stdbool.h>

int main() {
	int server_fd;
	int new_socket;
	struct sockaddr_in address; //Structure to hold our address info
	int addrlen = sizeof(address); //Needed for accept()

//1. CREATE THE SOCKET
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET: Tells the system we are using IPv4 (e.g., 127.0.0.1).
	//SOCK_STREAM: Tells the system we want a TCP connection (reliable, in-order data).
	//0: Lets the system choose the default protocol (which is IP for this combination).

//2. Define the Address and Port
	address.sin_family = AF_INET; // We are using IPv4
	address.sin_addr.s_addr = INADDR_ANY; //Listen on all interfaces (0.0.0.0)
										  // Accept connections on ANY of my network cards (Wi-Fi, Ethernet, etc.)
	address.sin_port = htons(PORT); //Converts port to network byte order... htons stands for Host TO Network Short
									//The Port number (flipped to Network Byte Order)

//3. Bind the socket to the address
	//I want to reserve this specific port (8080) for my application.
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

//4. Start Listening
	//The '3' is the backlog: how many pending connections to keep in queue
    if (listen(server_fd, 3) < 0) { //"Hey Operating System, if people call while I'm busy, you can hold up to 3 of them on the line for me."
        perror("Listen failed");
        return 1;
    }


    printf("Server is listening on port %d\n", PORT);

while (true) {
	//5. Accept the connection
	//Pauses the program until a client (like your web browser) tries to connect
    printf("Waiting for a connection...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }
    printf("Connection accepted!\n");

    // A. Create a buffer to store the data (30,000 bytes is plenty for headers)
    char buffer[30000] = {0}; 
     
    // B. Read the data from the socket into the buffer
    read(new_socket, buffer, 30000); 
     
    // C. Print what we received so we can see the HTTP Request
    printf("--- CLIENT REQUEST ---\n%s\n----------------------\n", buffer);

//6. Send Response
    /*char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    
    write(new_socket, hello, strlen(hello));
    printf("Hello message sent\n");
    */

    //Serving Real Content (Read the file)
    char method[100] = {0};
    char path[100] = {0};
    char version[100] = {0};
    sscanf(buffer, "%s %s %s", method, path, version);
    //printf debugging
    printf("DEBUG: Path extracted from HTTP request: '%s'\n", path);

    //Constructing the path
    char file_path[200];
    sprintf(file_path, "public%s", path);
	//printf debugging
    printf("DEBUG: Attempting to open file at path: '%s'\n", file_path);
    
    //Open the file
    FILE *file = fopen(file_path, "r");
	if (file == NULL) {
        // 1. Create a 404 error message
        char *error_response = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 13\n\n404 Not Found";
        
        // 2. Send it to the client
        write(new_socket, error_response, strlen(error_response));
        
        // 3. Close this specific connection and jump to the next loop iteration
        close(new_socket);
        continue;
    }

    //Sending the File
		//i. Create a buffer to hold the file contents
		char file_buffer[30000] = {0};
		//ii. Read the file into the buffer
		//fread returns the number of bytes read
		int bytes_read;
		//bytes_read = fread(file_buffer, 1, 30000, file);
		//iii. Close the file (we have the data in memory now)
		//fclose(file);
		// iv. Send the Headers
    	// Note: We are hardcoding "text/html" for now. 
    	//char *headers = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";

    	char *dot = strrchr(path, '.');
    	char *content_type = "text/html";
    	if ((dot != NULL) && (strcmp(dot, ".css") == 0)) {
    		content_type = "text/css";
    	}
    	//char *headers; //points to a random spot in memory. 
    				   //When sprintf tries to write data there, your program will crash (Segmentation Fault) 
    				   //because it's trying to write to memory it doesn't own.
    	char headers[1024];
    	sprintf(headers, "HTTP/1.1 200 OK\nContent-Type: %s\n\n", content_type);
    	write(new_socket, headers, strlen(headers));

    	// 5. Send the File Content
    	while((bytes_read = fread(file_buffer, 1, 30000, file)) > 0) {
			write(new_socket, file_buffer, bytes_read);
		}
		fclose(file);
    	

    close(new_socket); 
}

/* //2. Check if the socket was created successfully.
	if (server_fd < 0) {
		perror("Socket Creation Failed!");
		return 1;
	}
	printf("Socket Created Successfully!");
*/

//#. Close the socket
	// Good practice to close the socket when done, though OS handles it on exit
    close(server_fd);

    return 0;

}