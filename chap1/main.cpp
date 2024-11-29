#include "vision.hpp"
#include <sys/time.h>
using namespace std;
using namespace cv;

int main() {
    VideoCapture source("/home/linux/lsy/vscode_ws/simulation/7_lt_ccw_100rpm_in.mp4");
    if (!source.isOpened()) {
        cerr << "video open failed!" << endl;
        return -1;
    }
    // double fps = source.get(CAP_PROP_FPS);
    // int delay = cvRound(1000 / fps);

    bool first_run = true;
    Point tmp_pt;
    Mat frame, gray, thresh, stats, centroids;
    int error;

    struct timeval start, end1;
    double diff1;

    while (true) {
        gettimeofday(&start,NULL);  //시작

        preprocess(source, frame, gray, thresh);  // 전처리
        if (thresh.empty()) break;

        // 첫 실행시 기준점 설정
        if (first_run) {
            tmp_pt = Point(thresh.cols / 2, thresh.rows - 1);
            first_run = false;
        }

        // 객체 찾기
        findObjects(thresh, tmp_pt, stats, centroids);  // stats와 centroids 추가
        // 객체 그리기
        drawObjects(stats, centroids, tmp_pt, thresh);  // stats와 centroids 추가
        // error 계산
        error = getError(thresh, tmp_pt);

        // 'Esc' 키를 눌러 종료
        if (waitKey(30) == 27) break;

        gettimeofday(&end1,NULL);   //종료시간
        diff1=end1.tv_sec + end1.tv_usec/1000000.0 - start.tv_sec - start.tv_usec/1000000.0;
        cout << "error : " << error << "\ttime : " << diff1 << endl;

        // 결과 출력
        imshow("frame", frame);
        // imshow("gray", gray);
        imshow("thresh", thresh);
    }

    return 0;
}
