import cv2
import numpy as np

# 打开摄像头
cap = cv2.VideoCapture(0)

while True:
    # 读取图像
    ret, frame = cap.read()
    if not ret:
        break

    # 转换到 HSV 色彩空间
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # 选择一个通道进行边缘检测，这里选择 V 通道（亮度）
    v_channel = hsv[:, :, 2]

    # 应用 Canny 边缘检测
    edges = cv2.Canny(v_channel, 50, 150)

    # 在原始图像上绘制边缘（将边缘线设为红色）
    frame[edges == 255] = [255, 0, 0]

    # 显示结果
    cv2.imshow('Frame with Edges', frame)

    # 按 'q' 键退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头并关闭所有窗口
cap.release()
cv2.destroyAllWindows()