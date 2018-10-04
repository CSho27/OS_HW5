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

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

void shubertRun(int begin, int end, int index, int fd[]);
double shubert(double x1, double x2);

int main()
{
	struct timeval start_time, stop_time, elapsed_time;  // timers
  	gettimeofday(&start_time,NULL); // Unix timer
	pid_t pid = 0;
	int fd[4];
	int index = 0;
	
	/* create the pipe */
    if (pipe(fd) == -1) {
            fprintf(stderr,"Pipe failed");
            return 1;
    }
	
	
	pid = fork();
	
	if(pid == 0){
		index = 1;
		shubertRun(-500, -250, index, fd);
	}
	else{
		shubertRun(-250, 0, index, fd);
	}
	
	pid = fork();
	if(pid == 0 && index == 0){
		index = 2;
		shubertRun(0, 250, index, fd);
	}
	else{
		if(pid == 0 && index == 1){
			index = 3;
			shubertRun(250, 500, index, fd);
		}
	}
	waitpid(-1, NULL, 0);
	waitpid(-1, NULL, 0);
	
	double min = 0;
	char* minstr = malloc (10);
	read(fd[1], minstr, 10-1);
	minstr[10] ='\0';
	printf("%s", minstr);
	fflush(stdout);
	min = atof(minstr);
			
	if(index == 0){
		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
		printf("minimum = %f\n", min);
	}
}

void shubertRun (int begin, int end, int index, int fd[]) {
	double x1, x2, y, local_min = 0;
	char* write_data = malloc(20);
 	// Loops 
 	for (x1 = begin; x1 <= end; x1 += 0.5) { // x1 parameter of Shubert function
   		for (x2 = begin; x2 <= end; x2 += 0.5) { // x2 parameter of Shubert function
   	   	y = shubert(x1, x2); // Evaluate Shubert function defined below
     	//printf("%.2f ", y); // Print value for demonstration purposes
     	if (y < local_min) // Check for global minimum
       	local_min = y;
		}
	}
	char* minstr = malloc(10);
	read(fd[1], minstr, 10-1);
	minstr[10] ='\0';
	double min = atof(minstr);
	
	if(local_min<min){
		sprintf(write_data, "%f", local_min);
		write(fd[1], write_data, sizeof(write_data));
	}
	  
    //printf("\n"); // Print on next row

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
		