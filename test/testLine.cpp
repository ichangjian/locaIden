#include "stdafx.h"

void main()
{
    //{
    //    Mat view = imread("E:\\shm2\\" );
    //    Mat viewGray;
    //    vector<Point2f> pointbuf;
    //    cvtColor(view, viewGray, COLOR_BGR2GRAY);

    //    bool found = findChessboardCorners(view, Size(1,1), pointbuf, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);


    //    // improve the found corners' coordinate accuracy
    //    if (found)
    //    {
    //        cornerSubPix(viewGray, pointbuf, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
    //        drawChessboardCorners(view, boardSize, Mat(pointbuf), found);
    //        imagePoints.push_back(pointbuf);
    //        imshow("Image View", view);
    //        waitKey(100);
    //    }
    //}
    string corners_window = "aa0";
    Mat src = imread("G:/DSC01048/line.png");
    Mat src_gray;
    GaussianBlur(src, src, Size(21, 21), 2, 2);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(src.size(), CV_32FC1);
    int blockSize = 11;
    int apertureSize = 7;
    double k = 0.04;
    cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

        vector<Point2f> corners;
    for (int j = 0; j < dst_norm.rows; j++)
    {
        for (int i = 0; i < dst_norm.cols; i++)
        {
            if (dst_norm.at<float>(j, i) >55)
            {
                corners.push_back(Point2f(i,j));
                circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
            }
        }
    }
    Size zeroZone = Size(-1, -1);
    TermCriteria criteria = TermCriteria(
        CV_TERMCRIT_EPS + CV_TERMCRIT_ITER,
        40, //maxCount=40  
        0.001);  //epsilon=0.001  
    cornerSubPix(src_gray, corners, Size(57, 57), zeroZone, criteria);
    for (int i = 0; i < corners.size(); i++)
    {
 
        circle(src, corners[i], 15, Scalar(0,255,255), 1, 8, 0);
 
    }
    namedWindow(corners_window, WINDOW_AUTOSIZE);
    imshow(corners_window, dst_norm_scaled);
    Mat img = imread("G:/DSC01048/line.png",0);
    GaussianBlur(img,img,Size(21,21),2,2);
    //Mat dst;
    adaptiveThreshold(img, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,137,1);
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(
        dst,
        contours,
        cv::noArray(),
        cv::RETR_LIST,
        cv::CHAIN_APPROX_SIMPLE
        );
    vector<Rect> rests;
    for (size_t i = 0; i < contours.size(); i++)
    {
       Rect rect= boundingRect(contours[i]);
       if ((rect.width>30 && rect.height>300) || (rect.width>300 && rect.height>30))
       {
           rests.push_back(rect);
           rectangle(img, rect, Scalar(255), 5);
       }
    }
    //cv::drawContours(img, contours, -1, cv::Scalar::all(255));
    Sobel(img, dst, CV_8UC1, 0, 1,7);
    int xx = 0;
    xx = 1;
}