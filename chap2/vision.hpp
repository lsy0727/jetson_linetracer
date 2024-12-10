#ifndef _VISION_HPP_
#define _VISION_HPP_

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

void preprocess(VideoCapture& source, Mat& frame, Mat& gray, Mat& thresh);
void findObjects(Mat& thresh, Point& tmp_pt, Mat& stats, Mat& centroids);
void drawObjects(Mat& stats, Mat& centroids, Point& tmp_pt, Mat& thresh);
int getError(Mat& thresh, Point& tmp_pt);

#endif //_VISION_HPP_