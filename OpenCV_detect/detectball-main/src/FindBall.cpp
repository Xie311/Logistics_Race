#include "FindBall.hpp"
#include "ximgproc/find_ellipses.hpp"
#include <iostream>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>


#define ENABLE_IMSHOW


#ifdef ENABLE_THRESHOLD
// Trackbar callback function
void onTrackbar(int, void*) {
    // Nothing to do here, just for trackbar functionality
}
#endif // ENABLE_THRESHOLD

enum BallType { RED = 0, PURPLE = 1, BLUE = 2 };

cv::Scalar lower_purple = cv::Scalar(180, 160, 0);
cv::Scalar upper_purple = cv::Scalar(225, 255, 255);
cv::Scalar lower_red = cv::Scalar(117, 143, 0);
cv::Scalar upper_red = cv::Scalar(133, 255, 255);
cv::Scalar lower_blue = cv::Scalar(0, 159, 0);
cv::Scalar upper_blue = cv::Scalar(16, 255, 255);

FindBallServer::FindBallServer():   lutEqual(256), lutZero(256, 0), lutRaisen(256), lutSRaisen(256, 256, CV_8UC3), 
                                    start_time(0.0), current_time(0.0), frame_number(-1), frame_number_record(-1), 
                                    frames_per_second(0), elapsed_seconds(0.0)
{
    std::cout << "FindBallServer created" << std::endl;
    cap = std::make_shared<cv::VideoCapture>();
    ball_result = cv::Vec3d(0, 0, 0);

    Kalman = std::make_shared<cv::KalmanFilter>(4, 2);
    Kalman->measurementMatrix = (cv::Mat_<float>(2, 4) << 1, 0, 0, 0, 0, 1, 0, 0);
    Kalman->transitionMatrix = (cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
    Kalman->processNoiseCov = (cv::Mat_<float>(4, 4) << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    #ifdef ENABLE_THRESHOLD
    lowH = 184, lowS = 118, lowV = 150;
    highH = 194, highS = 218, highV = 255;
    #endif // ENABLE_THRESHOLD
}

FindBallServer::~FindBallServer()
{
    std::cout << "FindBallServer destroyed" << std::endl;
    usbcamera_deinit();
}

bool FindBallServer::find_ball(int type, cv::Vec3d &ball_result_)
{
    cv::Ptr<cv::ximgproc::EdgeDrawing> ed = cv::ximgproc::createEdgeDrawing();
    std::shared_ptr<cv::ximgproc::EdgeDrawing::Params> EDParams = std::make_shared<cv::ximgproc::EdgeDrawing::Params>();
    EDinit(ed, EDParams);

    usbcamera_getImage(color_image);
    if (color_image.empty()) {
        return false;
    }
    color_image.size();

    hsv.release();
    blendSRaisen.release();
    mask.release();
    img.release();
    gray.release();
    thre.release();
    edge_image.release();
    contours.clear();
    filter_ellipses.clear();
    ellipses.clear();

    cv::cvtColor(color_image, hsv, cv::COLOR_BGR2HSV);
    cv::LUT(hsv, lutRaisen ,blendSRaisen);
    cv::inRange(blendSRaisen, this->lower[type], this->upper[type], mask);
    cv::bitwise_and(color_image, color_image, img, mask);
    cv::medianBlur(img, img, 7);
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, thre, 0, 255, cv::THRESH_BINARY);
    cv::morphologyEx(thre, thre, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)), cv::Point(-1, -1), 2);

    cv::findContours(thre, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (auto contour : contours) {
        cv::fillPoly(thre, contour, cv::Scalar(255, 255, 255));
    }


    std::vector<cv::Vec3d> colorblocks;
    // 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thre, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 遍历每个轮廓，计算边界框并绘制
    for (const auto& contour : contours) {
        cv::Point2f center_;
        float radius_;
        cv::minEnclosingCircle(contour, center_, radius_);
        cv::Mat circle_region;
        cv::Mat mask_  = cv::Mat::zeros(thre.size(), CV_8UC1);
        cv::circle(mask_, cv::Point(int(center_.x), int(center_.y)), int(radius_), cv::Scalar(255), -1);
        cv::bitwise_and(thre, mask_, circle_region);
        float white_pixels = cv::countNonZero(circle_region);
        if(white_pixels > 1000)
        {
            cv::fillPoly(thre, contour, cv::Scalar(255, 255, 255));
            cv::circle(color_image, center_, radius_, cv::Scalar(255, 255, 0), 2);
            colorblocks.push_back(cv::Vec3d(center_.x, center_.y,radius_));
        }
    }
    std::sort(colorblocks.begin(), colorblocks.end(), 
        [](const cv::Vec3d& a, const cv::Vec3d& b) {
        return a[1] > b[1]; // 按 center.y 降序排序
        });


    // 开始检测ellipses
    ed->detectEdges(thre);
    ed->getEdgeImage(edge_image);
    ed->detectEllipses(ellipses);

    for (size_t i=0; i<ellipses.size(); i++)
    {
        cv::Point center((int)ellipses[i][0], (int)ellipses[i][1]);
        cv::Size axes((int)ellipses[i][2] + (int)ellipses[i][3], (int)ellipses[i][2] + (int)ellipses[i][4]);
        filter_ellipses.push_back(cv::Vec3d(center.x, center.y, axes.width/2.0 + axes.height/2.0));
        cv::ellipse(color_image, center, axes, ellipses[i][5], 0, 360, cv::Scalar(0, 255, 0), 2);
        //cv::ellipse(thre, center, axes, ellipses[i][5], 0, 360, cv::Scalar(255, 255, 255), -1);
    }

    // 使用 lambda 表达式进行排序
    std::sort(filter_ellipses.begin(), filter_ellipses.end(), 
            [](const cv::Vec3d& a, const cv::Vec3d& b) {
            return a[1] > b[1]; // 按 center.y 降序排序
            });

    if(filter_ellipses.size() > 0)
    {
        for (auto &max_ellipse : filter_ellipses)
        {
            cv::Mat circle_region;
            //cv::Vec3d max_ellipse = filter_ellipses[0];
            cv::Mat mask_  = cv::Mat::zeros(thre.size(), CV_8UC1);
            cv::circle(mask_, cv::Point(int(max_ellipse[0]), int(max_ellipse[1])), int(max_ellipse[2]), cv::Scalar(255), -1);
            cv::bitwise_and(thre, mask_, circle_region);
            float white_pixels = cv::countNonZero(circle_region);
            float total_pixels = cv::countNonZero(mask_);

            if (white_pixels / total_pixels > 0.4)
            {
                this->ball_result = max_ellipse;
                ball_result_ = max_ellipse;
                break;
            }
        }
    }else if (filter_ellipses.size() == 0)
    {
        std::vector<cv::Vec3d> filter_ellipses_ ;
        std::vector<std::vector<cv::Point>> contours_;
        cv::findContours(thre, contours_, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        for (auto contour : contours_) {
            if(contour.size() > 50 && cv::contourArea(contour) >100)
            {
                cv::Point2f center_;
                float radius_;
                cv::minEnclosingCircle(contour, center_, radius_);
                filter_ellipses_.push_back(cv::Vec3d(center_.x, center_.y, radius_));
            }
        }
        // 使用 lambda 表达式进行排序 
        std::sort(filter_ellipses_.begin(), filter_ellipses_.end(), 
                [](const cv::Vec3d& a, const cv::Vec3d& b) {
                return a[1] > b[1]; // 按 center.y 降序排序
                });
        
        if(filter_ellipses_.size() > 0)
        {
            this->ball_result = filter_ellipses_[0];
            ball_result_ = filter_ellipses_[0];
        }
    }
    std::cout << "ball_result: " << this->ball_result << std::endl;
    return true;
}

bool FindBallServer::usbcamera_init()
{
    if(!this->cap->open(0))
        if(!this->cap->open(1))
            return false;
    if (!cap->isOpened()) {
        return false;
    }
    return true;
}

bool FindBallServer::usbcamera_deinit()
{
    this->cap->release();
    return true;
}

bool FindBallServer::usbcamera_getImage(cv::Mat &frame)
{
    this->cap->read(frame);
    if (frame.empty()) {
        return false;
    }
    return true;
}

bool FindBallServer::usbcamera_getImage()
{
    this->cap->read(temp_frame);
    if (temp_frame.empty()) {
        return false;
    }
    return true;
}

void FindBallServer::EDinit(cv::Ptr<cv::ximgproc::EdgeDrawing> &ed,
    std::shared_ptr<cv::ximgproc::EdgeDrawing::Params> &EDParams)
{
    EDParams->MinPathLength = 60;
    EDParams->MinLineLength = 20;
    EDParams->PFmode = false;
    EDParams->NFAValidation = true;
    ed->setParams(*EDParams);
}

void FindBallServer::put_text(cv::Mat frame ,int &frame_number)
{
    cv::putText(frame, "Frames per second: " + std::to_string(frame_number), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
}

bool FindBallServer::findball_with_Kalman(int type, cv::Vec3d &data)
{
    frame_number = frame_number + 1;

    cv::Vec3d ref;
    if(!find_ball(type, ref))
        return false;
    last_prediction = current_prediction;
    last_measurement = current_measurement;
    current_measurement = cv::Vec2f(ref[0], ref[1]);
    Kalman->correct(cv::Mat(current_measurement));
    current_prediction = Kalman->predict();
    data[0] = current_prediction[0];
    data[1] = current_prediction[1];
    data[2] = ref[2];
    current_radius = ref[2];

    current_time = (double)cv::getTickCount() / cv::getTickFrequency();
    elapsed_seconds = (current_time - start_time);
    if (elapsed_seconds >= 1.0)
    {
        frames_per_second =  frame_number - frame_number_record;
        // 重置计时器
        start_time = current_time;
        frame_number_record = frame_number;
    }
    return true;
}

bool FindBallServer::main_init()
{
    if(!usbcamera_init())
        return false;
    for(int i = 0; i < 10; i++)
        usbcamera_getImage();
    lut_init();
    start_time = (double)cv::getTickCount() / cv::getTickFrequency();
    return true;
}

void FindBallServer::imgshow_DEBUG_INIT()
{
    #ifdef ENABLE_THRESHOLD
    cv::namedWindow("Threshold Adjustments", cv::WINDOW_NORMAL);
    // Create trackbars
    cv::createTrackbar("Low H", "Threshold Adjustments", &lowH, 255, onTrackbar);
    cv::createTrackbar("High H", "Threshold Adjustments", &highH, 255, onTrackbar);
    cv::createTrackbar("Low S", "Threshold Adjustments", &lowS, 255, onTrackbar);
    cv::createTrackbar("High S", "Threshold Adjustments", &highS, 255, onTrackbar);
    cv::createTrackbar("Low V", "Threshold Adjustments", &lowV, 255, onTrackbar);
    cv::createTrackbar("High V", "Threshold Adjustments", &highV, 255, onTrackbar);
    #endif // ENABLE_THRESHOLD

    #ifdef ENABLE_IMSHOW
    cv::namedWindow("color_image", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("color", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("hsv", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("blendSRaisen", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("thre", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("edge", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("mask",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("prediction",cv::WINDOW_AUTOSIZE);
    #endif // ENABLE_IMSHOW
}

void FindBallServer::imgshow_DEBUG()
{
    #ifdef ENABLE_THRESHOLD
    cv::Mat thresholdImage(100, 600, CV_8UC3, cv::Scalar(0, 0, 0));
    // Draw rectangles for each trackbar
    cv::rectangle(thresholdImage, cv::Rect(0, 0, 100, 100), cv::Scalar(lowH, lowS, lowV), -1);
    cv::rectangle(thresholdImage, cv::Rect(100, 0, 100, 100), cv::Scalar(highH, highS, highV), -1);
    cv::rectangle(thresholdImage, cv::Rect(200, 0, 100, 100), cv::Scalar(lowH, 255, 255), -1);
    cv::rectangle(thresholdImage, cv::Rect(300, 0, 100, 100), cv::Scalar(highH, 255, 255), -1);
    cv::rectangle(thresholdImage, cv::Rect(400, 0, 100, 100), cv::Scalar(255, lowS, 255), -1);
    cv::rectangle(thresholdImage, cv::Rect(500, 0, 100, 100), cv::Scalar(255, highS, 255), -1);
    cv::rectangle(thresholdImage, cv::Rect(600, 0, 100, 100), cv::Scalar(255, 255, lowV), -1);
    cv::rectangle(thresholdImage, cv::Rect(700, 0, 100, 100), cv::Scalar(255, 255, highV), -1);

    lower[1] = cv::Scalar(lowH, lowS, lowV);
    upper[1] = cv::Scalar(highH, highS, highV);
    // Show the image
    cv::imshow("Threshold Adjustments", thresholdImage);

    #endif // ENABLE_THRESHOLD


    #ifdef ENABLE_IMSHOW
    cv::Mat prediction = cv::Mat::zeros(480, 640, CV_8UC3);
    if(current_prediction[0] != 0)
        cv::circle(prediction, cv::Point(current_prediction[0], current_prediction[1]), current_radius, cv::Scalar(0, 255, 0), -1);
    
    cv::line(color_image, cv::Point(161,333), cv::Point(161,480), cv::Scalar(0, 255, 0), 2);
    cv::line(color_image, cv::Point(329,0), cv::Point(329,480), cv::Scalar(0, 255, 0), 2);
    cv::line(color_image, cv::Point(468,333), cv::Point(468,480), cv::Scalar(0, 255, 0), 2);
    cv::imshow("prediction", prediction);
    cv::imshow("color_image", color_image);
    cv::imshow("hsv", hsv);
    cv::imshow("mask", mask);
    cv::imshow("color", blendSRaisen);
    cv::imshow("thre", thre);
    cv::imshow("edge", edge_image);
    cv::imshow("blendSRaisen", blendSRaisen);
    #endif // ENABLE_IMSHOW
}

void FindBallServer::lut_init()
{
    // Fill lutEqual
    for (int i = 0; i < 256; ++i) {
        lutEqual[i] = static_cast<uint8_t>(i);
    }
    // Fill lutRaisen
    for (int i = 0; i < 256; ++i) {
        lutRaisen[i] = static_cast<uint8_t>(102 + 0.6 * i);
    }
    // Fill lutSRaisen
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            lutSRaisen.at<cv::Vec3b>(i, j) = cv::Vec3b(lutEqual[i], lutRaisen[j], lutEqual[i]);
        }
    }
}