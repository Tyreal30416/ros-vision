#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv/cv.hpp>


#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

extern "C"{
   #include "lsd_1.6/lsd.h"
}


#include <vector>
#include <iostream>
#include <string>


using namespace std;
using namespace cv;


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
static void write_eps( double * segs, int n, int dim,
                       char * filename, int xsize, int ysize, double width )
{
  FILE * eps;
  int i;

  /* check input */
  if( segs == NULL || n < 0 || dim <= 0 )
    cout << "Error: invalid line segment list in write_eps.";
  if( xsize <= 0 || ysize <= 0 )
    cout << "Error: invalid image size in write_eps.";

  /* open file */
  if( strcmp(filename,"-") == 0 ) eps = stdout;
  else eps = fopen(filename,"w");
  if( eps == NULL ) cout << "Error: unable to open EPS output file.";

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
    cout << "Error: unable to close file while writing EPS file.";
}


void printSIFTVector(std::vector<cv::KeyPoint> kp){
     int i,size;
     size = kp.size();
     for(i=0;i<size;i++){
        std::cout << kp[i].pt << "\n";   
     }
     std::cout << "there were " << size << " features\n";
}

void performSIFT(cv::Mat input){
    SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);
    //printVector(keypoints);
    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("sift_result.jpg", output);
}

double* char_to_image_double_ptr( unsigned int xsize, 
                                     unsigned int ysize, char * data ){

  int size = xsize*ysize;
  int i;
  double* image;
  /* check parameters */
  if( xsize == 0 || ysize == 0 )
    cout << "new_image_double_ptr: invalid image size.";
  if( data == NULL ) cout << "new_image_double_ptr: NULL data pointer.";

  /* get memory */
  image = (double*) malloc(size* sizeof(double) );
  if( image == NULL ) cout << "not enough memory.";

  /* set image */

  for(i=0; i < size; ++i){
    image[i] = (double) data[i];
  }
  return image;
}

void performLSD(cv::Mat m){
  
  double * image = char_to_image_double_ptr(m.cols,m.rows,(char*)m.data);
  double * out;
  int x,y,i,j,n;
  int X = m.cols;  /* x image size */
  int Y = m.rows;  /* y image size */

 
  /* LSD call */
  out = lsd(&n,image,X,Y);


  /* print output */

  /*for(i=0;i<n;i++)
    {
      for(j=0;j<7;j++)
        printf("%f ",out[7*i+j]);
      printf("\n");
    }*/
  printf("%d line segments found:\n",n);
  /* free memory */
  write_eps(out,n,7,"LSDout.eps",X,Y,.1);
  free( (void *) image );
  free( (void *) out );
}


int main(int argc, char* argv[]){
    cv::Mat output;
    
    ros::init(argc,argv,"image_converter");
    cv::initModule_nonfree();
    const cv::Mat input = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE); //Load as grayscale
   // performSIFT(input);  
    performLSD(input);
    
    return 0;
}

