#include "vision.hpp"
using namespace std;
using namespace cv;

int main() {
    VideoCapture source("/home/linux/lsy/vscode_ws/simulation/7_lt_ccw_100rpm_in.mp4");
    if (!source.isOpened()) {
        cerr << "video open failed!" << endl;
        return -1;
    }

    double fps = source.get(CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);

    bool first_run = true;
    Point tmp_point;
    Mat frame, gray, thresh, result, stats, centroids;

    while (true) {
        preprocess(source, frame, gray, thresh);  // 전처리
        if (thresh.empty()) break;

        // 첫 실행시 기준점 설정
        if (first_run) {
            tmp_point = Point(thresh.cols / 2, thresh.rows - 1);
            first_run = false;
        }

        // 객체 찾기
        findObjects(thresh, tmp_point, result, stats, centroids);  // stats와 centroids 추가

        // 객체 그리기
        drawObjects(result, stats, centroids, tmp_point, result);  // stats와 centroids 추가

        // 결과 출력
        // imshow("frame", frame);
        // imshow("gray", gray);
        imshow("thresh", result);

        // 'Esc' 키를 눌러 종료
        if (waitKey(delay) == 27) break;
    }

    return 0;
}
