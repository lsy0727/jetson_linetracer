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
    // string src = "nvarguscamerasrc sensor-id=0 ! \
    //     video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
    //     format=(string)NV12, framerate=(fraction)30/1 ! \
    //     nvvidconv flip-method=0 ! video/x-raw, \
    //     width=(int)640, height=(int)360, format=(string)BGRx ! \
    //     videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    // VideoCapture source(src, CAP_GSTREAMER);
    // if (!source.isOpened()) { cout << "Camera error" << endl; return -1; }

    // string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
    //     nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
    //     h264parse ! rtph264pay pt=96 ! \
    //     udpsink host=203.234.58.164 port=8001 sync=false";
    // VideoWriter writer1(dst1, 0, (double)30, Size(640, 360), true);
    // if (!writer1.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}

    // string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
    //     nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
    //     h264parse ! rtph264pay pt=96 ! \
    //     udpsink host=203.234.58.164 port=8002 sync=false";
    // VideoWriter writer2(dst2, 0, (double)30, Size(640, 360), false);
    // if (!writer2.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}

    // string dst3 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
    //     nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
    //     h264parse ! rtph264pay pt=96 ! \
    //     udpsink host=203.234.58.164 port=8003 sync=false";
    // VideoWriter writer3(dst3, 0, (double)30, Size(640, 360), false);
    // if (!writer3.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}

    VideoCapture source("/home/linux/lsy/vscode_ws/simulation/7_lt_ccw_100rpm_in.mp4");
    if (!source.isOpened()) {
        cerr << "video open failed!" << endl;
        return -1;
    }

    //vision
    bool first_run = true;
    Point tmp_pt;
    Mat frame, gray, thresh, result, stats, centroids;
    int error;
    
    double k = 1;   // 게인값

    //dxl
    Dxl mx;
    if(!mx.open()) { cout <<"dynamixel open error"<<endl; return-1; }
    bool mode = false;
    int vel1 = 0, vel2 = 0; // 좌우 바퀴 속도

    //  time
    struct timeval start, end1;
    double time1;

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
            if (c == 's') { mode = true; }
            else if (c == 'q') break;
        }

        // 모터 제어
        k = (error/10.0) * 10;  // 게인값 조정(10미만은 *10, 10이상은 10~100으로 제한) / 대안2 : error범위마다 k값 임의로 지정
        vel1 = 100 - k*error;
        vel2 = -(100 + k*error);
        if (control) mx.setVelocity(vel1, vel2);

        if (ctrl_c_pressed) break;  //ctrl+c 입력시 탈출
        usleep(30*1000);

        gettimeofday(&end1,NULL);   //종료시간
        time1=end1.tv_sec + end1.tv_usec/1000000.0 - start.tv_sec - start.tv_usec/1000000.0;
        cout << "error : " << error << "\tvel1:" << vel1 << ',' << "\tvel2:" << vel2 << "\ttime : " << time1 << endl;

        // 결과 출력
        imshow("frame", frame);
        // imshow("gray", gray);
        imshow("thresh", result);
        // writer1 << frame;
        // writer2 << gray;
        // writer3 << thresh;
    }
    mx.close();
    return 0;
}
