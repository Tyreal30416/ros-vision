#ifndef BOLD_RECOGNIZER_HPP
#define BOLD_RECOGNIZER_HPP

#include "bold.hpp"
#include "bold_feature.hpp"

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <math.h>
#include <cfloat>


using namespace std;
using namespace cv;

namespace BOLD{
  
  enum BOLDRecognizerConstants{
   K_NEAREST_NEIGHBORS = 5,
  };
  
  class BOLDRecognizer{
  private:
    BOLDescriptor descriptor;
    vector<BOLDFeature> trainedFeatures;
    
  public:
    BOLDRecognizer();
    string classify(BOLDFeature f);
    void addLabeledFeature(BOLDFeature f);
    void addLabeledFeatureFromFile(string fileName,string label);
  };
  
  
}

#endif