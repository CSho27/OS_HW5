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

void shubertRun(int begin, int end, int index);
double shubert(double x1, double x2);

int main()
{
	struct timeval start_time, stop_time, elapsed_time;  // timers
  	gettimeofday(&start_time,NULL); // Unix timer
	
	pid_t pid = 0;
	int index = 0;
	
	const int SIZE = 4096;
    const char *name = "minArray";
	
	int shm_fd;
    void *ptr;
    
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
	
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
    }
	
	pid = fork();
	
	if(pid == 0){
		index = 1;
		shubertRun(-500, -250, index);
	}
	else{
		shubertRun(-250, 0, index);
	}
	
	pid = fork();
	if(pid == 0 && index == 0){
		index = 2;
		shubertRun(0, 250, index);
	}
	else{
		if(pid == 0 && index == 1){
			index = 3;
			shubertRun(250, 500, index);
		}
	}
	waitpid(-1, NULL, 0);
	waitpid(-1, NULL, 0);
	
	if(index == 0){
		ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
		if (ptr == MAP_FAILED) {
			printf("Map failed receive\n");
			return -1;
		}
		
		double min = atof(ptr);
		
		gettimeofday(&stop_time,NULL);
		timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
		printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
		printf("minimum = %f\n", min);
	}
}

void shubertRun (int begin, int end, int index) {
	const int SIZE = 4096;
	int shm_fd;
    void *ptr;
	const char *name = "minArray";
    
    shm_fd = shm_open(name, O_RDWR, 0666);
    if(shm_fd ==-1){
        printf("shared memory failed \n");
        exit(-1);
    }
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
    }
		
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
	if(local_min<atof(ptr)){
		sprintf(ptr, "%f", local_min);
	  
    //printf("\n"); // Print on next row
 	}
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
		