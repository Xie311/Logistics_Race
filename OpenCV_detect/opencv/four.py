import cv2
import numpy as np

# 打开摄像头
cap = cv2.VideoCapture(1)

while True:
    # 读取图像
    ret, frame = cap.read()
    if not ret:
        break

    # 转换到 HSV 色彩空间
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 定义 HSV 中蓝色的范围
    lower_blue = np.array([110, 50, 50])
    upper_blue = np.array([130, 255, 255])

    # 创建蓝色掩膜
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    # 将掩膜应用于原始图像
    masked_image = cv2.bitwise_and(frame, frame, mask=mask)

    # 在掩膜上找到轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 在掩膜后的图像上绘制轮廓
    cv2.drawContours(masked_image, contours, -1, (0, 255, 0), 3)

    # 显示结果
    cv2.imshow('Masked Image with Contours', masked_image)

    # 按 'q' 键退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头并关闭所有窗口
cap.release()
cv2.destroyAllWindows()