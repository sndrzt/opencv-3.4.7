#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

cv::Mat g_image, g_image1;
int g_contrast = 10;
int g_threshold = 215;
int g_brightness = 0;
std::string g_name = "Trackbar";

void onTrackbar(int pos, void *)
{
    for (size_t row = 0; row < g_image.rows; ++row)
    {
        for (size_t col = 0; col < g_image.cols; ++col)
        {
            for (size_t rgb = 0; rgb < 3; ++rgb)
            {
                size_t n;
                if (g_image.at<cv::Vec3b>(row, col)[rgb] > g_threshold)
                    n = g_image.at<cv::Vec3b>(row, col)[rgb] * (1 + g_contrast * 0.01) + g_brightness;
                else
                    n = g_image.at<cv::Vec3b>(row, col)[rgb] * (1 - g_contrast * 0.01) + g_brightness;
                n = cv::saturate_cast<uchar>(n);
                g_image1.at<cv::Vec3b>(row, col)[rgb] = n;
            }
        }
    }
    cv::imshow(g_name, g_image1);
}

int main()
{
    g_image = cv::imread("a.jpg");
    if (g_image.empty())
    {
        std::cout << "can not open image!" << std::endl;
        return -1;
    }
    g_image.copyTo(g_image1);

    cv::namedWindow(g_name, cv::WINDOW_AUTOSIZE);

    cv::createTrackbar("contrast", g_name, &g_contrast, 100, onTrackbar);
    cv::createTrackbar("threshold", g_name, &g_threshold, 255, onTrackbar);
    cv::createTrackbar("brightness", g_name, &g_brightness, 255, onTrackbar);
    while (true)
    {
        onTrackbar(0, 0);
        auto c = cv::waitKey(10);
        if (c == 's')
            cv::imwrite("b.jpg", g_image1);
        else if (c >= 0 && c != 's')
            break;
    }
    cv::destroyAllWindows();
    return 0;
}
