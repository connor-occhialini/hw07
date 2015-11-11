/*funs.c - Library of function declarations which are used in the main file "hw07.c".*/


/* Definition of the recurrence relation used to calculate I(n) for n < 100. The "initial condition" is the value of the integral for I(100) - computed using a brute force numerical integrator. */

double I(int n)
{
	if(n==100)
	{
		return .00367843;
	}
	else
	{
		return (I(n+1) + 1/exp(1))/(n+1);
	}	
}


/* Uses the definition of the recurrence relation above to compute and store I(n) in the nth element of the provided array "vals".*/

void integral_recur(int nmin, int nmax, double vals[])
{
	int i;
	for(i = nmin; i <= nmax; i++)
	{
		vals[i] = I(i);
	}
}

/* Function declaration that "builds" the integrands for each integral to be evaluated by the generic integrator. */

double f (double x, void *params)
{
  double n = *(double *) params;
  double f = exp (-x) * pow (x, n);
  return f;
}

/* The GSL generic integrator.  Computes each integral I(n) numerically and stores the value in the nth element of the provided array "vals".*/

void integral_gen(int nmin, int nmax, double vals[])
{
	double result, error;
	double a = 0., b = 1.;
	double abserr = 1.e-9, relerr = 1.e-9;
	double n;
	size_t np = 1000;
	gsl_integration_workspace *w = gsl_integration_workspace_alloc (np);

	gsl_function F;
 	F.function = &f;

	for (int i = nmin; i <= nmax; i++)
 	{
		n = (double) i;
		F.params = &n;

 		gsl_integration_qag (&F, a, b, abserr, relerr, np, GSL_INTEG_GAUSS15, w,
   		          &result, &error);
		vals[i] = result;
	}

  	gsl_integration_workspace_free (w);

}
