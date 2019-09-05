#include <algorithm>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

int getMaxAreaContourId(vector <vector <Point>> contours);

int main()
{
    VideoCapture cap(0);
    double c;

    if (!cap.isOpened())
    {
        cout << "can not open the camera!" << endl;
        exit(1);
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    int start_x = 0, start_y = 240, w = 640, h = 240;

    while (true)
    {
        Mat frame;
        cap >> frame;
        Mat ROI(frame, Rect(start_x, start_y, w, h));

        Mat crop;
        ROI.copyTo(crop);
        Mat reverse;
        flip(crop, reverse, 1);

        Mat gray;
        cvtColor(reverse, gray, CV_BGR2GRAY);
        Mat blur;
        GaussianBlur(gray, blur, {3, 3}, 0, 0, BORDER_DEFAULT);
        Mat thresh;
        threshold(blur, thresh, 60, 255, THRESH_BINARY_INV);

        vector<vector<Point>> contours;
        vector<Vec4i> hierachy;
        findContours(thresh, contours, hierachy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

        if (contours.size() > 0)
        {
            Moments M;
            M = moments(contours.at(getMaxAreaContourId(contours)), true);
            Point p(M.m10 / M.m00, M.m01 / M.m00);

            drawContours(reverse, contours, -1, Scalar(128, 0, 0), 1);
            circle(reverse, p, 3, Scalar(255, 255, 0), 3);
            
            if (p.x >= 370)
            {
                cout << "RIGHT! " << endl;
            }
            else if (p.x <= 270)
            {
                cout << "LEFT! " << endl;
            }
            else
            {
                cout << "ON TRACK! " << endl;
            }
            
        }

        namedWindow("frame");
        namedWindow("thr");
        imshow("frame", reverse);
        imshow("thr", thresh);
        moveWindow("frame", 0, 0);
        moveWindow("thr", 641, 0);
        char k;
        k = waitKey(16) & 0xFF;
        
        if (k == 27)
        {
            break;
        }
    }
    
    destroyAllWindows();
    cap.release();
    return 0;
}

int getMaxAreaContourId(vector <vector <Point>> contours)
{
    double maxArea;
    int maxAreaContourId = -1;
    for (int j = 0; j < contours.size(); j++)
    {
        double newArea = contourArea(contours.at(j));
        if (newArea > maxArea)
        {
            maxArea = newArea;
            maxAreaContourId = j;
        }
    }
    return maxAreaContourId;
}
