#include "vision.hpp"
#include "dxl.hpp"
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
using namespace std;
using namespace cv;

bool ctrl_c_pressed= false;
void ctrlc_handler(int) { ctrl_c_pressed= true; }
int main() {
    string src = "nvarguscamerasrc sensor-id=0 ! \
        video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
        format=(string)NV12, framerate=(fraction)30/1 ! \
        nvvidconv flip-method=0 ! video/x-raw, \
        width=(int)640, height=(int)360, format=(string)BGRx ! \
        videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    VideoCapture source(src, CAP_GSTREAMER);
    // VideoCapture source("/home/jetson/workspace/linedetect_sim/simulation/5_lt_cw_100rpm_out.mp4");
    // if (!source.isOpened()) { cerr << "video open failed!" << endl; return -1; }

    string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
        nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
        h264parse ! rtph264pay pt=96 ! \
        udpsink host=203.234.58.164 port=8001 sync=false";
    VideoWriter writer1(dst1, 0, (double)30, Size(640, 360), true);
    if (!writer1.isOpened()) { cerr << "Writer open failed!" << endl; return -1; }

    string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
        nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
        h264parse ! rtph264pay pt=96 ! \
        udpsink host=203.234.58.164 port=8002 sync=false";
    VideoWriter writer2(dst2, 0, (double)30, Size(640, 360), false);
    if (!writer2.isOpened()) { cerr << "Writer open failed!" << endl; return -1; }

    string dst3 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
        nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
        h264parse ! rtph264pay pt=96 ! \
        udpsink host=203.234.58.164 port=8003 sync=false";
    VideoWriter writer3(dst3, 0, (double)30, Size(640, 90), true);
    if (!writer3.isOpened()) { cerr << "Writer open failed!" << endl; return -1; }

    bool first_run = true;
    Point tmp_pt;
    Mat frame, gray, thresh, stats, centroids;
    int error;

    struct timeval start, end, runtime_start, runtime_end;
    double diff, runtime_diff;

    // 시그널
    signal(SIGINT, ctrlc_handler);

    Dxl mx;
    if(!mx.open()) { cout <<"dynamixel open error"<<endl; return-1; }
    bool mode =false;
    double lval = 0, rval = 0;
    double k = 0.185;    //0.185

    while (true) {
        gettimeofday(&start,NULL);  //시작
        gettimeofday(&runtime_start,NULL);

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

        if(mx.kbhit())
        {
            char ch = mx.getch();
            if(ch == 'q') mode = false;
            else if(ch == 's') mode = true;
        }
        if (ctrl_c_pressed) break;  //ctrl+c 입력시 탈출

        lval = 100 - k* error;
        rval = -(100 + k* error);
        if(mode) mx.setVelocity(lval, rval);
        else mx.setVelocity(0, 0);

        gettimeofday(&runtime_end, NULL);
        runtime_diff = 30 - (runtime_end.tv_sec + runtime_end.tv_usec/1000000.0 - runtime_start.tv_sec - runtime_start.tv_usec/1000000.0)* 1000;
        usleep(runtime_diff*1000);

        gettimeofday(&end,NULL);   //종료시간
        diff=end.tv_sec + end.tv_usec/1000000.0 - start.tv_sec - start.tv_usec/1000000.0;
        cout << "error : " << error << "\tlval : " << lval << "\trval : " << rval << "\ttime : " << diff << endl;

        // 결과 출력
        writer1 << frame;
        writer2 << gray;
        writer3 << thresh;
    }
    mx.close();
    return 0;
}
