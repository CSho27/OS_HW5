//Christopher Shorter
//This file is for problem 1

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

double shubertRun(int begin, int end);
void *pShubertRun(void *nothing);
double shubert(double x1, double x2);
double p_min=0;
double p_begin = -500;
double p_end = 0;

int main()
{
	struct timeval start_time, stop_time, elapsed_time;  // timers
  	gettimeofday(&start_time,NULL); // Unix timer
	
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /* set of attributes for the thread */

	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the thread */
	pthread_create(&tid,&attr,pShubertRun, NULL);

	//Run Shubert run on our own
	double local_min = shubertRun(0, 500);

	/* now wait for the thread to exit */
	pthread_join(tid,NULL);
	
	printf("main : min=%f\n", local_min);
	printf("thread : min=%f\n", p_min);
	fflush(stdout);
	
	double min = 0;
	if(local_min<p_min)
		min = local_min;
	else
		min = p_min;
	

	gettimeofday(&stop_time,NULL);
	timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
	printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
	printf("minimum = %f\n", min);
}

double shubertRun (int begin, int end) {
	double x1, x2, y, local_min;
 	// Loops 
 	for (x1 = begin; x1 <= end; x1 += 0.5) { // x1 parameter of Shubert function
   		for (x2 = begin; x2 <= end; x2 += 0.5) { // x2 parameter of Shubert function
   	   	y = shubert(x1, x2); // Evaluate Shubert function defined below
     	//printf("%.2f ", y); // Print value for demonstration purposes
     	if (y < local_min) // Check for global minimum
       	local_min = y;
		}
	  
    //printf("\n"); // Print on next row
 	}
	return local_min;
}

void *pShubertRun (void *nothing) {
  double x1, x2, y;
  // Loops 
  for (x1 = p_begin; x1 <= p_end; x1 += 0.5) { // x1 parameter of Shubert function
    for (x2 = p_begin; x2 <= p_end; x2 += 0.5) { // x2 parameter of Shubert function
      y = shubert(x1, x2); // Evaluate Shubert function defined below
      //printf("%.2f ", y); // Print value for demonstration purposes
      if (y < p_min) // Check for global minimum
        p_min = y;
    }
    //printf("\n"); // Print on next row
  }
  pthread_exit(0);

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
