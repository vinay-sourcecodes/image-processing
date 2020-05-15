// colorTracking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<opencv2/opencv.hpp>
 
using namespace std;
using namespace cv;
int main(int argc,char* argv[])
{
    VideoCapture cap(0);
    if (!cap.isOpened()) { cout << "cannot open the web cam"<<endl; return -1; }
    namedWindow("control", WINDOW_AUTOSIZE);

    int lowH = 0;
    int highH = 179;
    int lowS = 0;
    int highS = 255;
    int lowV = 0;
    int highV = 255;
    createTrackbar("lowH", "control", &lowH, 179);//HUE 0-179
    createTrackbar("highH", "control", &highH, 179);
    createTrackbar("lowS", "control", &lowS, 255);//SATURATION 0-255
    createTrackbar("highS", "control", &highS, 255);
    createTrackbar("lowV", "control", &lowV, 255);//VALUE 0-255
    createTrackbar("highV", "control", &highV, 255);

    while (1)
    {
        Mat frame;
        bool bSuccess = cap.read(frame);//read new frame from video
        if (!bSuccess) { cout << "cant read a frame from the video stream" << endl; break; }
        Mat HSV;
        cvtColor(frame, HSV, COLOR_BGR2HSV);
        Mat imgT;
        inRange(HSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), imgT);

        //MORPHOLOGICAL OPENING!!!!
        //removing small objects from the fore grounds

        erode(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        dilate(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        //MORPHOLOGICAL CLOSING!!
        //fill small holes in the foregrounds

        dilate(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
        erode(imgT, imgT, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

        imshow("thresholded image", imgT);
        imshow("original", frame);
        if (waitKey(30) == 27)//waiting for esc
        {
            cout << "esc key is pressed!!!" << endl;
            break;
        }
    }
    return 0;
}
