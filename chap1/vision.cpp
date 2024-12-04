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
void findObjects(Mat& thresh, Point& tmp_pt, Mat& stats, Mat& centroids) {
    // 객체검출
    Mat labels;
    int cnt = connectedComponentsWithStats(thresh, labels, stats, centroids);

    // 1채널 -> 3채널 변환
    cvtColor(thresh, thresh, COLOR_GRAY2BGR);

    int min_index = -1;
    int min_dist = thresh.cols; // 거리 최소값 저장

    for (int i = 1; i < cnt; i++) {
        int area = stats.at<int>(i, 4); // 객체 면적

        if (area > 100) {
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));
            int dist = norm(Point(x, y) - tmp_pt);   //거리 계산
            
            if (dist < min_dist && dist <= 150) {   //거리가 최소값보다 작고 설정한 최소 거리이내인 경우
                min_dist = dist;
                min_index = i;
            }
        }
    }
    if (min_index != -1 && min_dist <= 150) { // 설정한 최소 거리 내에 객체가 있는 경우
        tmp_pt = Point(cvRound(centroids.at<double>(min_index, 0)), cvRound(centroids.at<double>(min_index, 1)));    //tmp_pt 갱신
    }
    else circle(thresh, Point(tmp_pt.x, tmp_pt.y), 5, Scalar(0, 0, 255), -1);

}

// 객체 표시
void drawObjects(Mat& stats, Mat& centroids, Point& tmp_pt, Mat& thresh) {
    for (int i = 1; i < stats.rows; i++) {
        int area = stats.at<int>(i, 4);
        if (area > 100) {
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));

            if (x == tmp_pt.x) {
                rectangle(thresh, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(0, 0, 255));
                circle(thresh, Point(x, y), 5, Scalar(0, 0, 255), -1);
            }
            else {
                rectangle(thresh, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(255, 0, 0));
                circle(thresh, Point(x, y), 5, Scalar(255, 0, 0), -1);
            }
            
        }
    }
}

// error 계산
int getError(Mat& thresh, Point& tmp_pt) {
    return ((thresh.cols / 2) - tmp_pt.x);
}
