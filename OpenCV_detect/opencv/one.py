import cv2
import numpy as np

# 创建 VideoCapture 对象
cap = cv2.VideoCapture(0)  # 0 代表默认摄像头

# 检查摄像头是否成功开启
if not cap.isOpened():
    print("Error Cannot ioen the camera")
    exit()

while True:
    # 读取当前帧
    ret, frame = cap.read()

    # 将图像从 BGR 转换到 HSV 颜色空间
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 定义蓝色在 HSV 中的范围
    lower_blue = np.array([110, 50, 50])
    upper_blue = np.array([130, 255, 255])

    # 根据定义的蓝色阈值创建掩模
    mask = cv2.inRange(hsv, lower_blue, upper_blue)

    # 仅保留蓝色
    blue = cv2.bitwise_and(frame, frame, mask=mask)

    # 查找掩模中的轮廓
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 在原始图像上绘制轮廓
    cv2.drawContours(frame, contours, -1, (0, 255, 0), 3)

    # 显示结果
    cv2.imshow("Frame", frame)
    cv2.imshow("Mask", mask)
    cv2.imshow("Blue Color", blue)

    # 显示帧
    #cv2.imshow("Camera", frame)

    # 按 'q' 键退出循环
    if cv2.waitKey(1) == ord("q"):
        break

# 释放摄像头
cap.release()

# 关闭所有 OpenCV 窗口
cv2.destroyAllWindows()
