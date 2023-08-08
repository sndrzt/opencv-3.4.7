#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img;
Mat templ;
Mat result;

int match_method = 4;
int max_Trackbar = 5;
void MatchingMethod( int a, void *b );
void MatchingMethod( int a, void *b )
{
  a = a;
  b = b;
  Mat img_display;
  img.copyTo( img_display ); //将img的内容copy到img_display

  // 创建输出结果矩阵
  int result_cols =  img.cols - templ.cols + 1;//计算用于储存匹配结果的输出图像矩阵的大小
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_cols, result_rows, CV_32FC1 );//被创建矩阵的列数,行数,以CV_32FC1形式储存

  // 进行匹配和标准化
  matchTemplate( img, templ, result, match_method ); //待匹配图像,模版图像,输出结果图像,匹配方法
  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );//归一化

  // 通过函数 minMaxLoc 定位最匹配的位置
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );//检测矩阵中最大值和最小值的像素位置

  // 对于方法 SQDIFF和SQDIFF_NORMED, 越小的数值代表更高的匹配结果. 而对于其他方法, 数值越大匹配越好
  if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  // 最终结果
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 );//用矩形框起来框出最佳匹配位置
  rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar(0,0,255), 2, 8, 0 );

  imshow( "Original Image", img_display );//输出最终的到的结果
  imwrite("result.png", img_display); //将得到的结果写到源代码目录下
  //imshow( "Result window", result );   //输出匹配结果矩阵

  return;
}

int main()
{
    VideoCapture cap(1);

    if (!cap.isOpened()) {
        cerr << "Cannot open camera";
        return -1;
    }

    while (true) {
        cap.set(CAP_PROP_FRAME_WIDTH, 1920);
        cap.set(CAP_PROP_FRAME_HEIGHT, 1080);
        cap.read(img);
		resizeWindow("Original Image", 1920, 1080);
        imshow("Original Image", img);

        templ = imread("2.jpg");//载入模版图像

        // 创建窗口
        namedWindow( "Original Image", WINDOW_AUTOSIZE ); //窗口名称,窗口标识CV_WINDOW_AUTOSIZE是自动调整窗口大小以适应图片尺寸
        //namedWindow( "Result window", WINDOW_AUTOSIZE );

        // 创建滑动条
        createTrackbar("jackchen", "Original Image", &match_method, max_Trackbar, MatchingMethod ); 
        MatchingMethod( 0, 0 );//初始化显示

        if (waitKey(5) >= 0)
            break;
    }

    cap.release();

    return 0;
}
