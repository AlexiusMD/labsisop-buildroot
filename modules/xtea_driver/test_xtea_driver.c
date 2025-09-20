/**
 * @brief  A Linux user space program that communicates with the LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/xtea_driver.
 * 
 * Modified from Derek Molloy (http://www.derekmolloy.ie/ )
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)

int main(){
	int ret, fd;
	char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
	char stringToSend[BUFFER_LENGTH];

	printf("Starting device test code example...\n");

	fd = open("/dev/xtea_driver", O_RDWR);             // Open the device with read/write access
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}

	printf("This is a xtea encryption/decryption driver.\n");

	
	while (1) {
		printf("Type in your command:\n");
		scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)

		if (strcmp(stringToSend, "exit") == 0) {
			printf("Exiting the program.\n");
			close(fd);
			return 0;
		}

		if (strlen(stringToSend) == 0) {
			continue; // Skip empty input
		}

		if ((strncmp(stringToSend, "enc", 3) != 0) && (strncmp(stringToSend, "dec", 3) != 0)) {
			printf("Invalid command. Please use 'enc' or 'dec'.\n");
			continue;
		}

		ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
		if (ret < 0){
			perror("Failed to write the message to the device.");
			return errno;
		}	
		
		printf("Press ENTER to read back from the device...\n");
		getchar();	
		printf("Reading from the device...\n");
		ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
		if (ret < 0){
			perror("Failed to read the message from the device.");
			return errno;
		}

		printf("The received message is: [%s]\n", receive);
		printf("End of the program\n");
	} 

	return 0;
}
