/*******************************************************************/
/* vtemp      Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
void mark_region(int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

main(argc, argv)
int argc;
char *argv[];
{
int        y,x;                     /* index counters               */
  VXparse(&argc, &argv, par);       /* parse the command line       */

  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  int i = 1;                        /* region sequence              */        
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }
  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
           if(tm.u[y][x] == 0) {continue; }
           mark_region(x, y, i);
           i++;
     }
   }

   Vfwrite(&im, OVAL);             /* write image file              */
   exit(0);
}

/* function to mark the region */
void mark_region(int x, int y, int cnt) 
{						
   if (tm.u[y][x] == 0) {return; }
   im.u[y][x] = cnt;
   tm.u[y][x] = 0;                /* tag the visited pixel         */          
   mark_region(x + 1, y, cnt);
   mark_region(x - 1, y, cnt);
   mark_region(x, y + 1, cnt);
   mark_region(x, y - 1, cnt);   
}
