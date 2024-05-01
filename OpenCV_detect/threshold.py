"""双目摄像头饱和度增强实验
    @Time   2023.10.24
    @Author SSC
"""
import numpy as np
import cv2

cap = cv2.VideoCapture(1)
cap.set(10, -2)

# 全局定义段
# 1. 饱和度增强定义
# 调节通道强度
lutEqual = np.array([i for i in range(256)]).astype("uint8")
lutRaisen = np.array([int(102+0.6*i) for i in range(256)]).astype("uint8")
# 调节饱和度
lutSRaisen = np.dstack((lutEqual, lutRaisen, lutEqual))  # Saturation raisen
# 2. 滑动条定义
cv2.namedWindow("TrackBars")


def empty(a):
    pass


cv2.resizeWindow("TrackBars", 640, 240)
cv2.createTrackbar("Hue Min", "TrackBars", 0, 179, empty)
cv2.createTrackbar("Hue Max", "TrackBars", 0, 179, empty)
cv2.createTrackbar("Sat Min", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Sat Max", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Val Min", "TrackBars", 0, 255, empty)
cv2.createTrackbar("Val Max", "TrackBars", 0, 255, empty)

#############################
# 运行段
#############################
while cap.isOpened():
    ret, frame = cap.read()
    if ret is True:
        # 图像处理段
        # # 1. 饱和度增强
        hsv = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)  # 色彩空间转换, RGB->HSV

        cv2.imshow('hsv', hsv)
        blendSRaisen = cv2.LUT(hsv, lutSRaisen)             # 饱和度增大
        # img_enhance_saturation = cv2.cvtColor(blendSRaisen, cv2.COLOR_HSV2RGB)
        # cv2.imshow('img_enhance_saturation',img_enhance_saturation)

        # 2. 掩膜生成
        h_min = cv2.getTrackbarPos("Hue Min", "TrackBars")
        h_max = cv2.getTrackbarPos("Hue Max", "TrackBars")
        s_min = cv2.getTrackbarPos("Sat Min", "TrackBars")
        s_max = cv2.getTrackbarPos("Sat Max", "TrackBars")
        v_min = cv2.getTrackbarPos("Val Min", "TrackBars")
        v_max = cv2.getTrackbarPos("Val Max", "TrackBars")
        lower = np.array([h_min, s_min, v_min])
        upper = np.array([h_max, s_max, v_max])
        mask = cv2.inRange(blendSRaisen, lower, upper)
        imgResult = cv2.bitwise_and(frame, frame, mask=mask)
        cv2.imshow('result', imgResult)

        key = cv2.waitKey(1)
        if key == 27:
            break
    else:
        break

cv2.destroyAllWindows()
cap.release()
