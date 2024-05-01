import cv2
import numpy as np


# 创建窗口
cv2.namedWindow("image")


# 打开摄像头
cap = cv2.VideoCapture(1)

lutEqual = np.array([i for i in range(256)]).astype("uint8")
lutRaisen = np.array([int(102 + 0.6 * i) for i in range(256)]).astype("uint8")
lutSRaisen = np.dstack((lutEqual, lutRaisen, lutEqual))


def trackbar_callback(x):
    pass


# 创建 RGB 阈值的滑块
cv2.createTrackbar("R Min", "image", 0, 255, trackbar_callback)
cv2.createTrackbar("R Max", "image", 255, 255, trackbar_callback)
cv2.createTrackbar("G Min", "image", 0, 255, trackbar_callback)
cv2.createTrackbar("G Max", "image", 255, 255, trackbar_callback)
cv2.createTrackbar("B Min", "image", 0, 255, trackbar_callback)
cv2.createTrackbar("B Max", "image", 255, 255, trackbar_callback)


while True:
    # 读取图像
    ret, frame = cap.read()
    if not ret:
        break
    hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)  # 色彩空间转换, RGB->HSV
    frame = cv2.LUT(hsv, lutSRaisen)

    # 获取滑块的值
    r_min = cv2.getTrackbarPos("R Min", "image")
    r_max = cv2.getTrackbarPos("R Max", "image")
    g_min = cv2.getTrackbarPos("G Min", "image")
    g_max = cv2.getTrackbarPos("G Max", "image")
    b_min = cv2.getTrackbarPos("B Min", "image")
    b_max = cv2.getTrackbarPos("B Max", "image")

    # 创建阈值掩膜
    lower_bound = np.array([b_min, g_min, r_min])
    upper_bound = np.array([b_max, g_max, r_max])
    mask = cv2.inRange(frame, lower_bound, upper_bound)

    # 应用掩膜
    # mask = cv2.bitwise_and(frame, frame, mask=mask)

    # # 在掩膜上找到轮廓
    # contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # # 找到面积最大的轮廓
    # if contours:
    #     max_contour = max(contours, key=cv2.contourArea)

    #     # 使用多边形逼近方法简化轮廓
    #     epsilon = 0.01 * cv2.arcLength(max_contour, True)
    #     approx = cv2.approxPolyDP(max_contour, epsilon, True)

    #     # 计算逼近轮廓的最小外接圆
    #     (x, y), radius = cv2.minEnclosingCircle(approx)
    #     center = (int(x), int(y))
    #     radius = int(radius)

    #     # 绘制最小外接圆来近似物体的边缘
    #     cv2.circle(frame, center, radius, (0, 255, 0), 2)

    #     # 打印圆心坐标
    #     print(f"Center of the largest object: {center}")

    # 显示结果
    cv2.imshow("Frame", mask)

    # 按 'q' 键退出
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

# 释放摄像头并关闭所有窗口
cap.release()
cv2.destroyAllWindows()
