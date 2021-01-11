#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include "data.h"
#include <time.h>
#include <locale.h>
//#include "semaphore.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include  <fcntl.h>

#define q	11		    /* for 2^11 points */
#define N	(1<<q)		/* N-point FFT, iFFT */

typedef float real;
typedef struct{real Re; real Im;} complex;

int S[2];

#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif


void fft( complex *v, int n, complex *tmp )
{
  if(n>1) {			/* otherwise, do nothing and return */
    int k,m;    complex z, w, *vo, *ve;
    ve = tmp; vo = tmp+n/2;
    for(k=0; k<n/2; k++) {
      ve[k] = v[2*k];
      vo[k] = v[2*k+1];
    }
    fft( ve, n/2, v );		/* FFT on even-indexed elements of v[] */
    fft( vo, n/2, v );		/* FFT on odd-indexed elements of v[] */
    for(m=0; m<n/2; m++) {
      w.Re = cos(2*PI*m/(double)n);
      w.Im = -sin(2*PI*m/(double)n);
      z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	/* Re(w*vo[m]) */
      z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	/* Im(w*vo[m]) */
      v[  m  ].Re = ve[m].Re + z.Re;
      v[  m  ].Im = ve[m].Im + z.Im;
      v[m+n/2].Re = ve[m].Re - z.Re;
      v[m+n/2].Im = ve[m].Im - z.Im;
    }
  }
  return;
}

int main(void)
{
  complex v[N], scratch[N];
  ssize_t letti;
  ///char numeroDaLeggere[sizeof(int)];
  int numeroDaLeggere;
  float abs[N];
  int k;
  int m;
  int i;
  int j;
  int totale;
  int minIdx, maxIdx;
  
  //clock_t start;
  //clock_t end;
  //double diff;
  // devo aprire il file per leggere la device

  char *app_name = "Application";
  char *dev_name = "/dev/cddrecipe";
  int fd = -1;
  char c;
  if ((fd = open(dev_name, O_RDWR)) < 0)
  {
  fprintf(stderr, "%s: unable to open %s: %s\n", app_name, dev_name, strerror(errno));
  return( 1 );
  }

// Initialize the complex array for FFT computation
  
 while(1)
{
	 
	 //start = clock();
        for(k=0; k<N; k++) {  
	letti=read(fd,&numeroDaLeggere,4);
	if(letti == -1)
	 {
	    perror("qualcosa è andato storto \n");
	 }
        v[k].Re = numeroDaLeggere;
        v[k].Im = 0;
        usleep(20000);
       // end = clock();
        //diff= ((double)(end-start))/CLOCKS_PER_SEC;
        //printf("\t%.10f K = %d\n",diff,k);
    //fflush(stdout);
   
   }

   

 // FFT computation
   fft( v, N, scratch );

 // PSD computation
   for(k=0; k<N; k++) {
	abs[k] = (50.0/2048)*((v[k].Re*v[k].Re)+(v[k].Im*v[k].Im));
   }

   minIdx = (0.5*2048)/50;   // position in the PSD of the spectral line corresponding to 30 bpm
   maxIdx = 3*2048/50;       // position in the PSD of the spectral line corresponding to 180 bpm

 // Find the peak in the PSD from 30 bpm to 180 bpm
   m = minIdx;
   for(k=minIdx; k<(maxIdx); k++) {
     if( abs[k] > abs[m] )
 	 m = k;
   }

 // Print the heart beat in bpm
   printf( "\n\n\n%d bpm\n\n\n", (m)*60*50/2048 );

   close(fd);

   if ((fd = open(dev_name, O_RDWR)) < 0)
     {
     fprintf(stderr, "%s: unable to open %s: %s\n", app_name, dev_name, strerror(errno));
     return( 1 );
     }
}


  exit(EXIT_SUCCESS);
}


