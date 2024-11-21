#ifndef _VISION_HPP_
#define _VISION_HPP_

#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void preprocess(VideoCapture& source, Mat& frame, Mat& gray, Mat& thresh);
void findObjects(const Mat& thresh, Point& tmp_point, Mat& result, Mat& stats, Mat& centroids);
void drawObjects(const Mat& labels, const Mat& stats, const Mat& centroids, const Point& tmp_points, Mat& result);
int getError(const Mat& result, const Point& tmp_points);

#endif //_VISION_HPP_
