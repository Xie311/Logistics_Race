#ifndef FINDBALL_HPP
#define FINDBALL_HPP

// #define ENABLE_THRESHOLD

#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/ximgproc.hpp"


extern cv::Scalar lower_purple;
extern cv::Scalar upper_purple;
extern cv::Scalar lower_red;
extern cv::Scalar upper_red;
extern cv::Scalar lower_blue;
extern cv::Scalar upper_blue;

class FindBallServer
{
    public:
    std::shared_ptr<cv::VideoCapture> cap;

    FindBallServer();
    ~FindBallServer();   // 析构函数

    bool find_ball(int type, cv::Vec3d &ball_result);

    bool usbcamera_init();
    bool usbcamera_deinit();
    bool usbcamera_getImage(cv::Mat &frame);
    bool usbcamera_getImage();

    void EDinit(cv::Ptr<cv::ximgproc::EdgeDrawing> &ed,
    std::shared_ptr<cv::ximgproc::EdgeDrawing::Params> &EDParams);

    void put_text(cv::Mat frame ,int &frame_number);
    bool findball_with_Kalman(int type, cv::Vec3d &data);
    bool main_init();
    void imgshow_DEBUG_INIT();
    void imgshow_DEBUG();


    cv::Vec3d ball_result;
    cv::Mat thre;
    cv::Mat color_image;

    private:
    void lut_init();
    std::vector<cv::Scalar> lower = {lower_red, lower_purple, lower_blue};
    std::vector<cv::Scalar> upper = {upper_red, upper_purple, upper_blue};
    
    std::vector<uint8_t> lutEqual;
    std::vector<uint8_t> lutZero;
    std::vector<uint8_t> lutRaisen;
    cv::Mat lutSRaisen;
    
    

    // variables
    cv::Mat temp_frame;
    
    cv::Mat hsv;
    cv::Mat blendSRaisen;
    cv::Mat mask;
    cv::Mat img;
    cv::Mat gray;

    cv::Mat edge_image;
    std::vector<std::vector<cv::Point>> contours;

    //边缘检测variables
    std::vector<cv::Vec6d> ellipses;
    std::vector<cv::Vec3d> filter_ellipses;

    // create output array
    std::vector<cv::Vec6f> ells;

    // Kalman variables
    cv::Vec2f last_measurement;
    cv::Vec2f current_measurement ;
    cv::Vec4f last_prediction ;
    cv::Vec4f current_prediction ;
    float current_radius;
    std::shared_ptr<cv::KalmanFilter> Kalman;

        // 计时开始
    double start_time;
    double current_time;
    int frame_number;
    int frame_number_record;
    int frames_per_second;
    double elapsed_seconds;

    //DEGUB
    // Global variables to hold the threshold values
    #ifdef ENABLE_THRESHOLD
    int lowH, lowS, lowV;
    int highH, highS, highV;
    #endif // ENABLE_THRESHOLD
};

#endif // FINDBALL_HPP