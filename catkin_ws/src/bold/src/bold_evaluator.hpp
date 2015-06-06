#ifndef BOLD_EVALUATOR_HPP
#define BOLD_EVALUATOR_HPP


#include <opencv/cv.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <math.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cfloat>

#include "bold_recognizer.hpp"
#include "bold_report.hpp"
#include "bold_datum.hpp"

using namespace std;

namespace BOLD{
  
  
  
  class BOLDEvaluator{
  private:
    string mainDatasetDirectory;
    vector<string> labels;
    
    vector<BOLDDatum> data;
    vector<BOLDDatum> trainingSet;
    vector<BOLDDatum> testSet;
    
    BOLDReport report;
    
    int nLabels;
    int nCorrect;
    int nFalse;
    //vector<int> nItems;
  public:
    BOLDRecognizer bold;
    void readDataset(string mainDatasetDirectory,int firstN);
    void readObject(string label);
    void splitData(float fracTest);
    void train(int curFold,int totFold);
    void test();
    void nTests(int n,float fracTest,int nItems);
  };
}

#endif