import cv2
import numpy as np

# 打开摄像头
cap = cv2.VideoCapture(1)

while True:
    # 读取图像
    ret, frame = cap.read()
    if not ret:
        break

    # 假设砝码为特定颜色，设置过滤阈值
    lower_color = np.array([83, 26, 95])  # 示例为黑色物体的阈值
    upper_color = np.array([138, 92, 218])

    # 创建掩膜以过滤出特定颜色的物体
    mask = cv2.inRange(frame, lower_color, upper_color)

    # 在掩膜上找到轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 找到面积最大的轮廓
    if contours:
        max_contour = max(contours, key=cv2.contourArea)

        # 使用多边形逼近方法简化轮廓
        epsilon = 0.01 * cv2.arcLength(max_contour, True)
        approx = cv2.approxPolyDP(max_contour, epsilon, True)

        # 计算逼近轮廓的最小外接圆
        (x, y), radius = cv2.minEnclosingCircle(approx)
        center = (int(x), int(y))
        radius = int(radius)

        # 绘制最小外接圆来近似物体的边缘
        cv2.circle(frame, center, radius, (0, 255, 0), 2)

        # 打印圆心坐标
        print(f"Center of the largest object: {center}")

    # 显示结果
    cv2.imshow('Frame', frame)

    # 按 'q' 键退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头并关闭所有窗口
cap.release()
cv2.destroyAllWindows()
