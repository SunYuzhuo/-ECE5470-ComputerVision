/*******************************************************************/
/* vtemp      Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */
VisXfile_t *VXin,            /* input file structure                 */
           *VXout;           /* output file structure                */
VisXelem_t *VXlist,*VXptr;   /* VisionX data structure               */
VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vgrow: "},
{  "of=",   0,   " output file "},
{  "r=",    0,   " set the region pixel range"},
{  "-p",    0,   " value of the first pixel in the region"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  RANGE  par[2].val
#define  PFLAG  par[3].val

int first;

void setlabel(int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */

main(argc, argv)
int argc;
char *argv[];
{
    int start;
    int        y,x;                   /* index counters              */
    VXparse(&argc, &argv, par);       /* parse the command line      */
    Vfread(&im, IVAL);                /* read image file              */
    Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
    int label = 1;                        /* region sequence              */ 
    first = im.u[im.ylo][im.xlo];
       
    if ( im.type != VX_PBYTE ) {      /* check image format           */
       fprintf(stderr, "vtemp: no byte image data in input file\n");
       exit(-1);
    }

    for (y = im.ylo ; y <= im.yhi ; y++) 
        for (x = im.xlo; x <= im.xhi; x++)  
	    im.u[y][x] = 0;

    for (y = im.ylo ; y <= im.yhi ; y++) {
        for (x = im.xlo; x <= im.xhi; x++)  {
	    if(im.u[y][x] == 0 && tm.u[y][x] != 0){
		first = tm.u[y][x];
	        if(PFLAG){
		
		setlabel(x, y, first);
		}
		else{
		    setlabel(x, y, label);
		    if(label == 255){
			label = 1;
		    }else
			label++;
		}
	    }
        }
    }

    Vfwrite(&im, OVAL);             /* write image file              */
    exit(0);
}

/* function to set the label */
void setlabel(int x, int y, int l) 
{						  
    im.u[y][x] = l;
    if((tm.u[y][x + 1] != 0 && im.u[y][x + 1] == 0) && abs(tm.u[y][x + 1] - first) < atoi(RANGE))
	setlabel(x + 1, y, l);
    if((tm.u[y][x - 1] != 0 && im.u[y][x - 1] == 0) && abs(tm.u[y][x - 1] - first) < atoi(RANGE))
	setlabel(x - 1, y, l);
    if((tm.u[y + 1][x] != 0 && im.u[y + 1][x] == 0) && abs(tm.u[y + 1][x] - first) < atoi(RANGE))
	setlabel(x, y + 1, l);
    if((tm.u[y - 1][x] != 0 && im.u[y - 1][x] == 0) && abs(tm.u[y - 1][x] - first) < atoi(RANGE))
	setlabel(x, y - 1, l);
}
