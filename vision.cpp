#include "vision.hpp"
using namespace std;
using namespace cv;

// 전처리, 자르기
void preprocess(VideoCapture& source, Mat& frame, Mat& gray, Mat& thresh) {
    source >> frame;
    if (frame.empty()) {
        cerr << "empty frame" << endl;
        return;
    }
    //전처리
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    Scalar bright_avg = mean(gray); // 밝기 평균
    gray = gray + (100 - bright_avg[0]);
    threshold(gray, thresh, 130, 255, THRESH_BINARY);

    //자르기
    int r_pts = thresh.rows / 4 * 3;
    Rect r(0, r_pts, thresh.cols, thresh.rows - r_pts);
    thresh = thresh(r);
}

// 객체 찾기
void findObjects(const Mat& thresh, Point& tmp_point, Mat& result, Mat& stats, Mat& centroids) {
    Mat labels;
    int cnt = connectedComponentsWithStats(thresh, labels, stats, centroids);

    result = thresh.clone();
    cvtColor(result, result, COLOR_GRAY2BGR);

    int min_dist = result.cols; // 거리 최소값 저장
    for (int i = 1; i < cnt; i++) {
        int area = stats.at<int>(i, 4); // 객체 면적

        if (area < 5000 && area > 1000) {
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));
            int dist = abs(x - tmp_point.x);

            if (dist < min_dist) {
                min_dist = dist;
                tmp_point = Point(x, y);
            }
        }
    }
}

// 객체 표시
void drawObjects(const Mat& labels, const Mat& stats, const Mat& centroids, const Point& tmp_point, Mat& result) {
    for (int i = 0; i < stats.rows; i++) {
        int area = stats.at<int>(i, 4);
        if (area < 5000 && area > 1000) {
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));

            if (x == tmp_point.x) {
                rectangle(result, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(0, 0, 255));
                circle(result, Point(x, y), 5, Scalar(0, 0, 255), -1);
            }
            else {
                rectangle(result, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(255, 0, 0));
                circle(result, Point(x, y), 5, Scalar(255, 0, 0), -1);
            }
        }
    }
}
