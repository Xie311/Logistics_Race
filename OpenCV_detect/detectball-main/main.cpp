#include "FindBall.hpp"
#include <iostream>
#include <memory>
#include <opencv2/highgui.hpp>

int main()
{
    std::shared_ptr<FindBallServer> findball_server_handler = std::make_shared<FindBallServer>();
    findball_server_handler->main_init();
    findball_server_handler->imgshow_DEBUG_INIT();
    //cv::namedWindow("Colored Labels", cv::WINDOW_NORMAL);
    while(true)
    {
        cv::Vec3d ball_result;
        (findball_server_handler->findball_with_Kalman(1, ball_result));
        //    std::cout << "ball_result: " << ball_result << std::endl;
        //findball_server_handler->find_ball(1, ball_result);
        findball_server_handler->imgshow_DEBUG();
        cv::waitKey(1);
    }
    return 0;
}