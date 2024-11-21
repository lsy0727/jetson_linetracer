#ifndef _VISION_HPP_
#define _VISION_HPP_

#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void preprocess(VideoCapture& source, Mat& frame, Mat& gray, Mat& thresh);
void findObjects(const Mat& thresh, Point& tmp_pt, Point& prev_pt, Mat& result, Mat& stats, Mat& centroids);
void drawObjects(const Mat& stats, const Mat& centroids, const Point& tmp_pt, Mat& result);
int getError(const Mat& result, const Point& prev_pt);

#endif //_VISION_HPP_
