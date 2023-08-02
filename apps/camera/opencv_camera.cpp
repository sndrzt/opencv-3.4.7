#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap(0);
    Mat frame;

    if (!cap.isOpened()) {
        cerr << "Cannot open camera";
        return -1;
    }

    while (true) {
        cap.read(frame);
        imshow("Camera", frame);
        if (waitKey(5) >= 0)
            break;
    }

    cap.release();

    return 0;
}

