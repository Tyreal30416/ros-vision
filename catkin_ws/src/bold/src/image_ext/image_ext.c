#include "image_ext.h"


/*----------------------------------------------------------------------------*/
/*----------------------------- Write EPS File -------------------------------*/
/*----------------------------------------------------------------------------*/
/** Write line segments into an EPS file.
    If the name is "-" the file is written to standard output.

    According to

      Adobe "Encapsulated PostScript File Format Specification",
      Version 3.0, 1 May 1992,

    and

      Adobe "PostScript(R) LANGUAGE REFERENCE", third edition, 1999.
 */

void write_eps( double * segs, int n, int dim, char * filename, int xsize, int ysize, double width )
{
  FILE * eps;
  int i;

  /* check input */
  if( segs == NULL || n < 0 || dim <= 0 )
    printf("Error: invalid line segment list in write_eps.\n");
  if( xsize <= 0 || ysize <= 0 )
    printf("Error: invalid image size in write_eps.\n");

  /* open file */
  if( strcmp(filename,"-") == 0 ) eps = stdout;
  else eps = fopen(filename,"w");
  if( eps == NULL ) printf("Error: unable to open EPS output file.\n");

  /* write EPS header */
  fprintf(eps,"%%!PS-Adobe-3.0 EPSF-3.0\n");
  fprintf(eps,"%%%%BoundingBox: 0 0 %d %d\n",xsize,ysize);
  fprintf(eps,"%%%%Creator: LSD, Line Segment Detector\n");
  fprintf(eps,"%%%%Title: (%s)\n",filename);
  fprintf(eps,"%%%%EndComments\n");

  /* write line segments */
  for(i=0;i<n;i++)
    {
      fprintf( eps,"newpath %f %f moveto %f %f lineto %f setlinewidth stroke\n",
	      segs[i*dim+0],
	      (double) ysize - segs[i*dim+1],
	      segs[i*dim+2],
	      (double) ysize - segs[i*dim+3],
	      width <= 0.0 ? segs[i*dim+4] : width );
    }

  /* close EPS file */
  fprintf(eps,"showpage\n");
  fprintf(eps,"%%%%EOF\n");
  if( eps != stdout && fclose(eps) == EOF )
    printf("Error: unable to close file while writing EPS file.\n");
}

double* char_to_image_double_ptr( unsigned int xsize, 
				    unsigned int ysize, char * data ){

  int size = xsize*ysize;
  int i;
  double* image;
  /* check parameters */
  if( xsize == 0 || ysize == 0 )
    printf("new_image_double_ptr: invalid image size.\n");
  if( data == NULL ) printf("new_image_double_ptr: NULL data pointer.\n");

  /* get memory */
  image = (double*) malloc(size* sizeof(double) );
  if( image == NULL ) printf("not enough memory.\n");

  /* set image */

  for(i=0; i < size; ++i){
    image[i] = (double) data[i];
  }
  return image;
}
