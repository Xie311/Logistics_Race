#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>


int main ()
{
    // 创建VideoCapture对象
    auto cap = std::make_shared<cv::VideoCapture>();
    cap->open(0);
    //cv::VideoCapture cap(0); // 参数0代表默认摄像头
    // 检查摄像头是否成功打开
    if (!cap->isOpened()) {
        std::cerr << "ERROR: 摄像头无法打开" << std::endl;
        return -1;
    }
    // 创建窗口
    cv::namedWindow("摄像头", cv::WINDOW_AUTOSIZE);
    // 捕获和显示图像
    cv::Mat frame;
    while (true) {
        cap->read(frame); // 或者使用 cap.read(frame);
        if (frame.empty()) {
            std::cerr << "ERROR: 捕获到空帧" << std::endl;
            break;
        }
        // 显示帧
        cv::imshow("摄像头", frame);
        // 按 'q' 退出循环
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    // 释放窗口资源
    cv::destroyWindow("摄像头");
    return 0;
}

    // 进行连通组件标记
    cv::Mat labels, stats, centroids;
    int num_labels = cv::connectedComponentsWithStats(thre, labels, stats, centroids);

    // 输出结果
    std::cout << "Number of independent color blobs: " << num_labels - 1 << std::endl;

    // 可视化连通组件标记结果（可选）
    cv::Mat colored_labels = cv::Mat::zeros(labels.size(), CV_8UC3);
    for (int i = 1; i < num_labels; ++i) {
        cv::Scalar color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
        colored_labels.setTo(color, labels == i);
    }

    // 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thre, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 遍历每个轮廓，计算边界框并绘制
    for (const auto& contour : contours) {
        // 计算边界框
        cv::Rect bounding_box = cv::boundingRect(contour);

        // 绘制边界框
        cv::rectangle(colored_labels, bounding_box, cv::Scalar(0, 255, 0), 2);
    }

    // 创建凸包容器
    std::vector<std::vector<int>> convex_hulls(contours.size());

    // 寻找凸包并绘制凸边
    for (int i = 0; i < contours.size(); ++i) {
        cv::convexHull(contours[i], convex_hulls[i],true,false);
        //cv::drawContours(colored_labels, convex_hulls, i, cv::Scalar(0, 255, 255), 2);
        
        //检测凸缺陷
        std::vector<cv::Vec4i> defects;
        if (contours[i].size() > 4 && convex_hulls[i].size() > 4) { // 至少需要 4 个点来构成凸包
        {
            cv::convexityDefects(contours[i], convex_hulls[i], defects);
            // 绘制凸缺陷
            for (const auto& defect : defects) {
                cv::Point start = contours[i][defect[0]];
                cv::Point end = contours[i][defect[1]];
                cv::Point far = contours[i][defect[2]];
                float depth = defect[3] / 256.0f;  // 将围绕缺陷的深度缩放到 [0, 1] 范围

                if (depth > 10) {  // 只绘制深度大于某个阈值的凸缺陷
                    cv::line(colored_labels, start, end, cv::Scalar(0, 255, 0), 2);
                    cv::circle(colored_labels, far, 5, cv::Scalar(0, 0, 255), -1);
                }
            }
        }   
        }
    }
    cv::imshow("Colored Labels", colored_labels);

/*
     // 寻找凸包并绘制凸边
    for (int i = 0; i < contours.size(); ++i) {
        cv::convexHull(contours[i], convex_hulls[i],true,false);
        //cv::drawContours(colored_labels, convex_hulls, i, cv::Scalar(0, 255, 255), 2);
        //检测凸缺陷
        std::vector<cv::Vec4i> defects;
        if (contours[i].size() > 3 && convex_hulls[i].size() > 3) { // 至少需要 4 个点来构成凸包
        {
            cv::convexityDefects(contours[i], convex_hulls[i], defects);
            // 绘制凸缺陷
            for (const auto& defect : defects) {
                cv::Point start = contours[i][defect[0]];
                cv::Point end = contours[i][defect[1]];
                cv::Point far = contours[i][defect[2]];
                float depth = defect[3] / 256.0f;  // 将围绕缺陷的深度缩放到 [0, 1] 范围

                if (depth > 6) {  // 只绘制深度大于某个阈值的凸缺陷
                    cv::line(color_image, start, end, cv::Scalar(0, 255, 0), 2);
                    cv::circle(color_image, far, 5, cv::Scalar(0, 0, 255), -1);
                }
            }
        }   
        }
    }

*/

    //cv::ximgproc::findEllipses(thre, ells, 0.4f, 0.7f, 0.02f);
    //for (int i = 0;i< ells.size();i++)
    //{
    //   cv::circle(color_image, cv::Point((int)ells[i][0],(int)ells[i][1]), ells[i][2]/2.0+ells[i][3]/2.0, cv::Scalar(0, 255, 255), 2);
    //}

    // 进行连通组件标记
    //cv::Mat labels, stats, centroids;
    //int num_labels = cv::connectedComponentsWithStats(thre, labels, stats, centroids);

    // // 输出结果
    // std::cout << "Number of independent color blobs: " << num_labels - 1 << std::endl;

    // // 可视化连通组件标记结果（可选）
    // cv::Mat colored_labels = cv::Mat::zeros(labels.size(), CV_8UC3);
    // for (int i = 1; i < num_labels; ++i) {
    //     cv::Scalar color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
    //     colored_labels.setTo(color, labels == i);
    // }