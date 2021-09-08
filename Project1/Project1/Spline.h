#pragma once
#define fint int
#define OK		   0		/* no problems */
#define	NOMEMORY	  -1		/* no memory available for buffers */
#define EQALXES		  -2		/* two x-coordinates are equal */

#define true		   1
#define false		   0
#define MAXTXTLEN	  80
static fint klo = -1;
static fint khi = -1;

/*
spline constructs a cubic spline given a set of x and y values, through
these values.

*/
static fint spline(float *x, float *y, fint  n,
	float  yp1, float  ypn, float *y2)
{
	fint  i, k;
	float p, qn, sig, un, *u;

	u = (float *)malloc((unsigned)(n - 1) * sizeof(float));
	if (!u) return(NOMEMORY);
	if (yp1 > 0.99e30)
		y2[0] = u[0] = 0.0;
	else {
		y2[0] = -0.5;
		u[0] = (3.0 / (x[1] - x[0]))*((y[1] - y[0]) / (x[1] - x[0]) - yp1);
	}
	for (i = 1; i <= n - 2; i++) {
		sig = (x[i] - x[i - 1]) / (x[i + 1] - x[i - 1]);
		p = sig*y2[i - 1] + 2.0;
		y2[i] = (sig - 1.0) / p;
		u[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) - (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
		u[i] = (6.0*u[i] / (x[i + 1] - x[i - 1]) - sig*u[i - 1]) / p;
	}
	if (ypn > 0.99e30)
		qn = un = 0.0;
	else {
		qn = 0.5;
		un = (3.0 / (x[n - 1] - x[n - 2]))*(ypn - (y[n - 1] - y[n - 2]) / (x[n - 1] - x[n - 2]));
	}
	y2[n - 1] = (un - qn*u[n - 2]) / (qn*y2[n - 2] + 1.0);
	for (k = n - 2; k >= 0; k--)
		y2[k] = y2[k] * y2[k + 1] + u[k];
	free(u);
	return(OK);
}


/*
splint uses the cubic spline generated with spline to interpolate values
in the XY  table.

*/
static fint splint(float *xa, float *ya, float *y2a,
	fint n, float x, float *y)
{
	fint  r = 0;
	fint  k;
	float h, b, a;

	if (klo < 0) {
		klo = 0;
		khi = n - 1;
	}
	else {
		if (x < xa[klo]) klo = 0;
		if (x > xa[khi]) khi = n - 1;
	}
	while (khi - klo > 1) {
		k = (khi + klo) >> 1;
		if (xa[k] > x) khi = k;
		else klo = k;
	}
	h = xa[khi] - xa[klo];
	if (h == 0.0) {
		//setfblank_c(y);
		r = EQALXES;
	}
	else {
		a = (xa[khi] - x) / h;
		b = (x - xa[klo]) / h;
		*y = a*ya[klo] + b*ya[khi] + ((a*a*a - a)*y2a[klo] +
			(b*b*b - b)*y2a[khi]) * (h*h) / 6.0;
	}
	return(r);
}