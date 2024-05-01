import cv2
import numpy as np

# 打开摄像头或读取图像
cap = cv2.VideoCapture(0)

"""
def nothing(x):
    pass

# 创建一个窗口
cv2.namedWindow('image')

# 创建滑块，用于控制 HSV 阈值
cv2.createTrackbar('H Min', 'image', 0, 179, nothing)
cv2.createTrackbar('H Max', 'image', 179, 179, nothing)
cv2.createTrackbar('S Min', 'image', 0, 255, nothing)
cv2.createTrackbar('S Max', 'image', 255, 255, nothing)
cv2.createTrackbar('V Min', 'image', 0, 255, nothing)
cv2.createTrackbar('V Max', 'image', 255, 255, nothing)

# 打开摄像头
cap = cv2.VideoCapture(0)

while True:
    # 读取图像
    ret, frame = cap.read()
    if not ret:
        break

    # 转换到 HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # 获取滑块的当前位置
    h_min = cv2.getTrackbarPos('H Min', 'image')
    h_max = cv2.getTrackbarPos('H Max', 'image')
    s_min = cv2.getTrackbarPos('S Min', 'image')
    s_max = cv2.getTrackbarPos('S Max', 'image')
    v_min = cv2.getTrackbarPos('V Min', 'image')
    v_max = cv2.getTrackbarPos('V Max', 'image')

    # 创建 HSV 阈值掩模
    lower_hsv = np.array([h_min, s_min, v_min])
    upper_hsv = np.array([h_max, s_max, v_max])
    mask = cv2.inRange(hsv, lower_hsv, upper_hsv)

    # 将掩模和原始图像结合
    result = cv2.bitwise_and(frame, frame, mask=mask)

    # 显示结果
    cv2.imshow('image', result)

    # 按 'q' 键退出循环
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

"""
while True:
    _, frame = cap.read()

    # 预处理：高斯模糊
    blurred = cv2.GaussianBlur(frame, (5, 5), 0)

    # 转换到 HSV 色彩空间
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    # 定义蓝色的 HSV 阈值范围
    lower_blue = np.array([100, 45, 28])
    upper_blue = np.array([153, 231, 154])

    # 定义黑色的 HSV 阈值范围
    lower_black = np.array([96, 14, 23])
    upper_black = np.array([153, 88, 172])

    # 颜色过滤，创建掩膜图像
    mask = cv2.inRange(hsv, lower_black, upper_black)

    # 形态学操作：膨胀后腐蚀
    kernel = np.ones((5, 5), np.uint8)
    mask = cv2.dilate(mask, kernel, iterations=1)
    mask = cv2.erode(mask, kernel, iterations=1)

    # 将掩膜应用于原始图像
    masked_image = cv2.bitwise_and(frame, frame, mask=mask)

    # 轮廓检测
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    #contours_1, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # 轮廓筛选和识别
    

    #for contour in contours:
        # area = cv2.contourArea(contour)
        # if area > 500:  # 假设砝码面积至少为 x 个像素
        # (x, y), radius = cv2.minEnclosingCircle(contour)
        # center = (int(x), int(y))
        # if 0.75 < area / (np.pi * radius * radius) < 1.25:  # 圆度检测
        # cv2.circle(frame, center, int(radius), (0, 255, 0), 2)
        # cv2.putText(frame, "Weight", center, cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        # 绘制轮廓
    cv2.drawContours(frame, contours, -1, (255, 0, 0), 0)
    cv2.drawContours(mask, contours, -1, (255, 0, 0), 0)
    cv2.drawContours(masked_image, contours, -1, (255, 0, 0), 0)
       

    # for contour_1 in contours_1:
    # area_1 = cv2.contourArea(contour_1)
    # if area_1 > 500:

    # 显示结果S
    cv2.imshow("Frame", frame)
    cv2.imshow("Mask", mask)
    cv2.imshow("Masked_iamge",masked_image)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release(),
cv2.destroyAll, Windows()