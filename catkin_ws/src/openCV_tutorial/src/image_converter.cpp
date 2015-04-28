#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    ros::init(argc,argv,"image_converter");
    cv::initModule_nonfree();
    
    const cv::Mat input = cv::imread("choco.jpg", 0); //Load as grayscale
    std::cout <<"Halllo1\n";
    cv::SiftFeatureDetector detector;
    std::cout <<"Halllo2\n";
    std::vector<cv::KeyPoint> keypoints;
    std::cout <<"Halllo3\n";
    detector.detect(input, keypoints);
    
    std::cout <<"Halllo4\n";
    // Add results to image and save.
    cv::Mat output;
    
    std::cout <<"Halllo5\n";
    cv::drawKeypoints(input, keypoints, output);
    std::cout <<"Halllo6\n";
    cv::imwrite("sift_result.jpg", output);
    std::cout <<"Halllo7\n";
    
    
    return 0;
}

