/*********************************************************************/
/* vtpeak:     Threshold image between two most sig. hgram peaks     */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "d=",    0,   " min dist between hgram peaks (default 10)"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  TVAL   par[2].val
#define  VFLAG  par[3].val

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i,j;

    int hist[256];                 /* histogram bins                 */
    int thresh;                    /* threshold                      */
    int avg1 = 0;
    int avg2 = 0;
    int sum1 = 0;
    int sum2 = 0;
    int cnt1 = 0;
    int cnt2 = 0;
			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }
	
	if (TVAL) thresh = atoi(TVAL);  /* if thresh= was specified, get value */
    	else{
	    int sum = 0;
	    for (y = im.ylo ; y <= im.yhi ; y++) {
	        for (x = im.xlo; x <= im.xhi; x++)  {
		    sum += im.u[y][x];
	        }
	    }
	    thresh = sum / ((im.yhi - im.ylo + 1) * (im.xhi - im.xlo + 1));
        }
	if (thresh < 0 || thresh > 255) {
	    fprintf(stderr, "thresh= must be between 0 and 255\nUsing d=10\n");
	    thresh = 10;
	}

        /* clear the histogram */
        for (i = 0; i < 256; i++) hist[i] = 0;
 
        /* compute the histogram */
        for (y = im.ylo; y <= im.yhi; y++)
            for (x = im.xlo; x <= im.xhi; x++)
                 hist[im.u[y][x]]++;
  
         /* compute the threshold */
  
	for(i = 0; i < 10000; i++){
	    sum1 = 0;
	    sum2 = 0;
	    cnt1 = 0;
	    cnt2 = 0;
	    for(j = 0; j < thresh; j++){
		sum1 += j * hist[j];
		cnt1 += hist[j];
	    }
	    for(j = 255; j >= thresh; j--){
		sum2 += j * hist[j];
		cnt2 += hist[j];
	    }
	    if(cnt1 == 0)	avg1 = 0;
	    else	avg1 = sum1/cnt1;
	    if(cnt2 == 0)	avg2 = 0;
	    else	avg2 = sum2/cnt2;
	    if(thresh == (avg1 + avg2) /2){
	        fprintf(stderr, "Break at %d,%d\n", i, thresh);
		break;
	    }	
	    else	thresh = (avg1 + avg2) / 2;
	}
  
        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= thresh) im.u[y][x] = 255;
                 else                      im.u[y][x] = 0;
            }
        }
  
        Vfwrite( &im, OVAL);
    } /* end of every frame section */
    exit(0);
}
