import cv2
import numpy as np

def nothing(x):
    pass


# 创建窗口
cv2.namedWindow('image')

# 创建 RGB 阈值的滑块
cv2.createTrackbar('R Min', 'image', 0, 255, nothing)
cv2.createTrackbar('R Max', 'image', 255, 255, nothing)
cv2.createTrackbar('G Min', 'image', 0, 255, nothing)
cv2.createTrackbar('G Max', 'image', 255, 255, nothing)
cv2.createTrackbar('B Min', 'image', 0, 255, nothing)
cv2.createTrackbar('B Max', 'image', 255, 255, nothing)

# 打开摄像头
cap = cv2.VideoCapture(0)

while True:
    # 读取图像
    ret, frame = cap.read()
    if not ret:
        break

    # 获取滑块的值
    r_min = cv2.getTrackbarPos('R Min', 'image')
    r_max = cv2.getTrackbarPos('R Max', 'image')
    g_min = cv2.getTrackbarPos('G Min', 'image')
    g_max = cv2.getTrackbarPos('G Max', 'image')
    b_min = cv2.getTrackbarPos('B Min', 'image')
    b_max = cv2.getTrackbarPos('B Max', 'image')

    # 创建阈值掩膜
    lower_bound = np.array([b_min, g_min, r_min])
    upper_bound = np.array([b_max, g_max, r_max])
    mask = cv2.inRange(frame, lower_bound, upper_bound)

    # 应用掩膜
    result = cv2.bitwise_and(frame, frame, mask=mask)

    # 显示结果
    cv2.imshow('image', result)

    # 按 'q' 退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 释放摄像头并关闭窗口
cap.release()
cv2.destroyAllWindows()