#include "stdio.h"
#include "stdlib.h"
#include "Spline.h"
#include "math.h"

#define OK		   0		/* no problems */
#define	NOMEMORY	  -1		/* no memory available for buffers */
#define EQALXES		  -2		/* two x-coordinates are equal */

#define true		   1
#define false		   0
#define MAXTXTLEN	  80
#define fint int

#define finit( fc , len ) { fc.a = malloc( ( len + 1 ) * sizeof( char ) ) ;  \
                            fc.l = len ; }
#define N 10
#define N2 100    /*N'i kaç kat arttýrmak istiyorsak o kadar büyük olacak*/
#define TIMESTEP 25    /* sn*/

void main () {

	float x[N], time[N], y2[N],z[N2], time2[N2];

	int i, j, k,out;
	for (i = 0; i < N; i++) {
		x[i] = 1.0 + 0.2*rand() / RAND_MAX;
		time[i] = i*TIMESTEP;
	}

	for (i = 0; i < N2; i++) {
		time2[i] = i*TIMESTEP*N/N2;
	}
	out = spline(time, x, N, 1e40, 1e40, y2);
	for (i = 0; i < N2; i++) {
		out = splint(time, x, y2, N, time2[i], &z[i]);
	}

	i = 1;

};