#include <stdio.h>
#include <math.h>
#include <gsl/gsl_integration.h>  /* GSL Integration Library */
#include "funs.c"                 /* Integration functions   */
#include "timing_funs.h"          /* Timing functions        */

#define NMAX 100



int main(void)
{
	/* Declaration and initialization */
	int i;
	double vals1[NMAX+1], vals2[NMAX+1];

	/*Uses functions declared in "funs.c" to compute I(n) using the recurrence and the 		generic integrator methods, and stores the results in their respective arrays.*/
	integral_recur(0, 100, vals1);
	integral_gen(0, 100, vals2);
	
	/*Prints "n" and the result I(n) for each method above.  The absolute error 		multiplied by a factor of 1.e9 is reported in the last column.*/
	
	printf("n    Recurrence    Generic       Scaled Abs Err\n");

	for(i = 0; i <=NMAX; i++)
	{
		printf("%d    %.8f    %.8f    %f\n", i, vals1[i], vals2[i], 
		(vals1[i]-vals2[i])*1.e9);
	}


	/* Define the intial number of counts, tmin, tmax*/
	double time, trr, tgsl;
	int tmin = 1;
	int tmax = 2;
	int count = 1000;

	/* Computes the time per function call of the recurrence integration method.  Calls 		the function integral_recur() "count" times and adjusts count until the total time 		is between 1 and 2 seconds.*/
	printf("\n Timing for Recurrence Relation:\n");
	do
	{
		timer_start ();
		for(i = 0; i < count; i++)
		{
			integral_recur(0, 100, vals1);
		}
		time = timer_stop ();
		trr = time /count;
		printf (" %10.2f usec     %10.6f sec    %10d\n",trr * 1.e6, time, count);
		count = adjust_rep_count (count, time, tmin, tmax);
	}
	while ((time >tmax) || (time <tmin));

	
	/* Computes the time per function call of the GSL integrator method.  Calls 		the function integral_gen() "count" times and adjusts count until the total time is 		between 1 and 2 seconds. */
	printf("\n Timing for GSL Integrator:\n");
	do
	{
		timer_start ();
		for(i = 0; i < count; i++)
		{
			integral_gen(0, 100, vals1);
		}
		time = timer_stop ();
		tgsl = time /count;
		printf (" %10.2f usec     %10.6f sec    %10d\n",tgsl * 1.e6, time, count);
		count = adjust_rep_count (count, time, tmin, tmax);
	}
	while ((time >tmax) || (time <tmin));

	/*Prints time per function call for leibniz and bbp series, and then takes
	the ratio between the times to compare the relative speed*/
	printf("\nTime per function call for Recur:      %10.6f usec", trr*1.e6);
	printf("\nTime per function call for GSL:        %10.6f usec", tgsl*1.e6);
	int ratio = round(tgsl/trr);
	printf("\nRatio of function times tgsl/trr:        %d\n",ratio);

}
