// Chris Fietkiewicz. Searches for a minimum in Shubert function.
// Note that an interval of 0.5 gives a very inaccurate result but
// works as a fast demonstration. For a realistic search, we would
// use a much small interval and remove the printing from the loop.
// NOTE: Compile with -lm option for math library.
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <sys/time.h>

// Prototype for computation
double shubert(double x1, double x2);

int main () {
  double x1, x2, y, min = 0;
  struct timeval start_time, stop_time, elapsed_time;  // timers
  gettimeofday(&start_time,NULL); // Unix timer
  // Loops 
  for (x1 = -500; x1 <= 500; x1 += 0.5) { // x1 parameter of Shubert function
    for (x2 = -500; x2 <= 500; x2 += 0.5) { // x2 parameter of Shubert function
      y = shubert(x1, x2); // Evaluate Shubert function defined below
      //printf("%.2f ", y); // Print value for demonstration purposes
      if (y < min) // Check for global minimum
        min = y;
    }
    //printf("\n"); // Print on next row
  }
  gettimeofday(&stop_time,NULL);
  timersub(&stop_time, &start_time, &elapsed_time); // Unix time subtract routine
  printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
  printf("minimum = %f\n", min);
}

// Compute Shubert value for x1,x2 combination
double shubert(double x1, double x2) {
	double sum1 = 0; 
	double sum2 = 0;
	int i;
	for (i = 1; i <= 5; i++) {
		sum1 += i * cos((i + 1) * x1 + i);
		sum2 += i * cos((i + 1) * x2 + i);
	}
	return sum1 * sum2;
}