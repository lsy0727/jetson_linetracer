#include "vision.hpp"
using namespace std;
using namespace cv;

void video_open()
{
    VideoCapture source("/home/linux/lsy/vscode_ws/simulation/7_lt_ccw_100rpm_in.mp4");
    if (!source.isOpened()) { cerr << "video open failed!" << endl; return; }

    Mat frame, gray, thresh;
    //딜레이 구하기
    double fps = source.get(CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);

    //트랙바 생성
    // namedWindow("thresh");
    // int pos;
    // createTrackbar("threshold", "thresh", &pos, 255);
    
    bool first_run = true; // 최초 실행 여부 플래그

    int index;
    double dist;
    vector<double> dist_list;
    Point tmp_point;
    while (1)
    {
        source >> frame;
        if (frame.empty()) { cerr << "empty frame" << endl; break; }
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        Scalar bright_avg = mean(gray); //gray영상의 평균 밝기 값
        gray = gray + (100 - bright_avg[0]);
        threshold(gray, thresh, 130, 255, THRESH_BINARY);

        // 자를 사각형 범위
        int r_pts1 = 0; //좌상단 x좌표
        int r_pts2 = thresh.rows / 4 * 3;   //좌상단 y좌표
        int r_width = thresh.cols;  //너비
        int r_height = thresh.rows - r_pts2;    //높이
        Rect r(r_pts1, r_pts2, r_width, r_height);
        thresh = thresh(r);
        if (first_run)
        {
            tmp_point = Point(thresh.cols / 2, thresh.rows - 1);    // 이전 위치 저장 좌표
            first_run = false;
        }

        //객체 검출
        Mat labels, stats, centroids;
        int cnt = connectedComponentsWithStats(thresh, labels, stats, centroids);

        cvtColor(thresh, thresh, COLOR_GRAY2BGR);   //1채널 -> 3채널로 다시 변환
        double min_dist = 50000;
        for (int i = 1; i < cnt; i++)
        {
            int area = stats.at<int>(i, 4); // 객체 면적

            // 면적이 1000 이상인 객체만 처리
            if (area > 1000)
            {
                double x = centroids.at<double>(i, 0);  // 객체의 무게중심 x 좌표
                double y = centroids.at<double>(i, 1);  // 객체의 무게중심 y 좌표

                //거리 구하기
                dist = sqrt(pow(x - tmp_point.x, 2) + pow(y - tmp_point.y, 2));

                if (dist < min_dist)
                {
                    min_dist = dist;
                    index = i;
                }
                dist_list.push_back(dist);
            }
        }
        for (int i = 1; i < cnt; i++)
        {
            int area = stats.at<int>(i, 4); // 객체 면적

            if (area > 1000)
            {
                if (i == index)
                {
                    rectangle(thresh, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(0, 0, 255));
                    double x = centroids.at<double>(i, 0);  // 객체의 무게중심 x 좌표
                    double y = centroids.at<double>(i, 1);  // 객체의 무게중심 y 좌표
                    circle(thresh, Point(cvRound(x), cvRound(y)), 5, Scalar(0, 0, 255), -1); // 무게중심에 빨간색 점 그리기
                    
                    cout << "x:" << stats.at<int>(i, 0) << "\ty:" << stats.at<int>(i, 1)
                        << "\tw:" << stats.at<int>(i, 2) << " \th:" << stats.at<int>(i, 3) << "\tarea:" << stats.at<int>(i, 4) << endl;

                    //error = thresh.cols / 2 - (stats.at<int>(i, 0) + stats.at<int>(i, 3) / 2);  //error = 로봇의 중심 x좌표 - 라인의 중심 x좌표
                    tmp_point = Point(x, y);
                }
                else
                {
                    rectangle(thresh, Rect(stats.at<int>(i, 0), stats.at<int>(i, 1), stats.at<int>(i, 2), stats.at<int>(i, 3)), Scalar(255, 0, 0));
                    double x = centroids.at<double>(i, 0);  // 객체의 무게중심 x 좌표
                    double y = centroids.at<double>(i, 1);  // 객체의 무게중심 y 좌표
                    circle(thresh, Point(cvRound(x), cvRound(y)), 5, Scalar(255, 0, 0), -1); // 무게중심에 빨간색 점 그리기
                }
            }
        }
        dist_list.clear();  //벡터 초기화

        imshow("frame", frame);
        imshow("gray", gray);
        imshow("thresh", thresh);

        if (waitKey(delay) == 27)
            break;
    }
}
