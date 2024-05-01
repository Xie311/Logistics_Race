import cv2
import numpy as np

# 打开摄像头
cap = cv2.VideoCapture(0)

while True:
    # 读取图像
    ret, frame = cap.read()
    if not ret:
        break

    # 假设砝码为黑色，设置过滤阈值
    # 调整这些阈值以匹配砝码的颜色
    lower_black = np.array([0, 0, 0])
    upper_black = np.array([91, 95, 115])

    # 创建掩膜以过滤出黑色砝码
    mask = cv2.inRange(frame, lower_black, upper_black)

    # 在掩膜上找到轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 找到面积最大的轮廓
    if contours:
        max_contour = max(contours, key=cv2.contourArea)

        # 计算最大轮廓的外接圆
        (x, y), radius = cv2.minEnclosingCircle(max_contour)
        center = (int(x), int(y))
        radius = int(radius)

        # 绘制一个完美的圆来近似最大物体的边缘
        cv2.circle(frame, center, radius, (0, 255, 0), 3)

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