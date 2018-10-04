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

int main()
{
	struct timeval start_time, stop_time, elapsed_time;  // timers
  	gettimeofday(&start_time,NULL); // Unix timer
	
	char* buffer[1024];
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     struct sockaddr_in serv_addr; // Server address struct
     bzero((char *) &serv_addr, sizeof(serv_addr));
     int portno = 8081; // Port number is commandline argument
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
          error("ERROR on binding");
     listen(sockfd, 5);

     // Service phase
     struct sockaddr_in cli_addr;
     socklen_t clilen = sizeof(cli_addr); // Address struct length
     int sockfd_array[3];
	 int i = 0;
	 for(; i<3; i++){
		 sockfd_array[i] = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		 if (sockfd_array[i] < 0) 
			  error("ERROR on accept");
	 }
	 double local_min = shubertRun(250, 500);
	double min = local_min;
	i = 0;
	for(; i<3; i++){
	 	read(sockfd_array[i], buffer, sizeof(buffer));
		double check = atof((char*)buffer);
		if(check<min)
			check = min;
	}
     gettimeofday(&stop_time,NULL);
 	 timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
   	 printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
	 printf("minimum = %f\n", min);
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
		