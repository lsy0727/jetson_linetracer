#include <sys/time.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "vision.hpp"
#include "dxl.hpp"
using namespace std;
using namespace cv;

bool ctrl_c_pressed= false;
void ctrlc_handler(int) { ctrl_c_pressed= true; }
int main() {
    VideoCapture source("/home/linux/lsy/vscode_ws/simulation/7_lt_ccw_100rpm_in.mp4");
    if (!source.isOpened()) {
        cerr << "video open failed!" << endl;
        return -1;
    }

    bool first_run = true;
    Point tmp_pt;
    Mat frame, gray, thresh, result, stats, centroids;
    int error;

    Dxl mx;
    if(!mx.open()) { cout <<"dynamixel open error"<<endl; return-1; }
    bool control = false;

    // 시간 관련 변수
    struct timeval start, end1;
    double time1;
    int vel1 = 0, vel2 = 0;
    // 시그널
    signal(SIGINT, ctrlc_handler);

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
        findObjects(thresh, tmp_pt, result, stats, centroids);  // stats와 centroids 추가
        // 객체 그리기
        drawObjects(stats, centroids, tmp_pt, result);  // stats와 centroids 추가
        // error 계산
        error = getError(result, tmp_pt);

        if (mx.kbhit()) // 키보드입력체크
        {
            char c = mx.getch();    // 키입력받기
            if (c == 's') {
                vel1 = 100;
                vel2 = 100;
                control = true;
            }
        }

        // 모터 제어**************************************************************************
        if (control) {
            vel1 = vel1-error;
            vel2 = -(vel2+error);
            mx.setVelocity(vel1, vel2);
        }   //********************************************************************************

        if (ctrl_c_pressed) break;  //ctrl+c 입력시 탈출
        usleep(30*1000);

        gettimeofday(&end1,NULL);   //종료시간
        time1=end1.tv_sec + end1.tv_usec/1000000.0 - start.tv_sec - start.tv_usec/1000000.0;
        cout << "error : " << error << "\tvel1:" << vel1 << ',' << "\tvel2:" << vel2 << "\ttime : " << time1 << endl;

        // 결과 출력
        imshow("frame", frame);
        // imshow("gray", gray);
        imshow("thresh", result);
    }
    mx.close();
    return 0;
}
