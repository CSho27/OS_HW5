//Christopher Shorter
//HW5

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>




#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h> 

double shubertRun(int begin, int end);
double shubert(double x1, double x2);



void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[])
{
	int index = atoi(argv[0]);
	char buffer[1024];
	// Get socket
    int portno = 8081;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Set up for connect()
    struct hostent *server;
    server = gethostbyname("127.0.0.1");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    // Make connection
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR connecting");
	}
	
	double xvals[4] = {-500, -250, 0, 250};
	 double local_min = shubertRun(xvals[index], xvals[index+1]);	
	sprintf(buffer, "%f", local_min);
	 write(sockfd, buffer, sizeof(buffer));
}

double shubertRun (int begin, int end) {
	double x1, x2, y, local_min = 0;
 	// Loops 
 	for (x1 = begin; x1 <= end; x1 += 0.5) { // x1 parameter of Shubert function
   		for (x2 = begin; x2 <= end; x2 += 0.5) { // x2 parameter of Shubert function
   	   	y = shubert(x1, x2); // Evaluate Shubert function defined below
     	//printf("%.2f ", y); // Print value for demonstration purposes
     	if (y < local_min) // Check for global minimum
       	local_min = y;
		}
	}
	
	return local_min;
	  
    //printf("\n"); // Print on next r
}


// Compute Shubert value for x1,x2 combination
double shubert(double x1, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((double)(i + 1) * x1 + i);
		sum2 += i * cos(((double)i + 1) * x2 + i);
	}
	return sum1 * sum2;
}
		