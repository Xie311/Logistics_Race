'''2024 物流赛 砝码识别检测
    @Time 2024.5.1
    @Author X311
'''
import cv2
import numpy as np
# import time
# import struct

'''
# 全局定义段
'''
# 1. 饱和度增强定义
# 调节通道强度
lutEqual = np.array([i for i in range(256)]).astype("uint8")
lutRaisen = np.array([int(102+0.6*i) for i in range(256)]).astype("uint8")
# 调节饱和度
# 一个三通道的查找表，其中蓝色通道和红色通道采用了 lutEqual，而绿色通道采用了 lutRaisen。这样就实现了对图像的饱和度进行调节，同时保持了图像的亮度和色调。
lutSRaisen = np.dstack((lutEqual, lutRaisen, lutEqual))  # Saturation raisen
# 2. 掩膜阈值定义
lower_weight = np.array([0, 80, 17])
upper_weight = np.array([179, 160, 119])
# 3. 结构元素定义
kernel = np.ones((7, 7), np.uint8)
# 4. Serial Port Definition
# serial_port = serial.Serial("/dev/ttyACM0", 115200, timeout=0.5)
# serial_port_state = serial_port.is_open
# 5. Capture Definition
cap = cv2.VideoCapture(1)
cap.set(10, -2)
# 6. Weight Definition
weight_x = 0.0
weight_y = 0.0
weight_r = 0.0

"""
# 运行段
"""
while cap.isOpened():
    '''
    #接收颜色图像数据
    '''
    ret, color_image = cap.read()

    if ret is True:
        """
            图像处理段
        """
        # color_image = cv2.flip(color_image, 0)  # 将图像水平翻转
        """
            1. 饱和度增强
        """
        hsv = cv2.cvtColor(color_image, cv2.COLOR_RGB2HSV)  # 色彩空间转换, RGB->HSV
        blendSRaisen = cv2.LUT(hsv, lutSRaisen)             # 饱和度增大
        # cv2.imshow("hsv1", hsv)
        # img_enhance_saturation = cv2.cvtColor(blendSRaisen, cv2.COLOR_HSV2RGB)
        # cv2.imshow('img_enhance_saturation', img_enhance_saturation)
        """
            2. 掩膜创建
        """
        weight_mask = cv2.inRange(blendSRaisen, lower_weight, upper_weight)
        weight_img = cv2.bitwise_and(color_image, color_image, mask=weight_mask)
        """
            3. 滤波
        """
        weight_img = cv2.GaussianBlur(weight_img, (3, 3), 0)
        weight_img = cv2.medianBlur(weight_img, 5)
        cv2.imshow("temp", weight_img)
        """
            4. 二值化
        """
        weight_gray = cv2.cvtColor(weight_img, cv2.COLOR_BGR2GRAY)
        # weight_gray = cv2.GaussianBlur(weight_gray, (3, 3), 0)
        # cv2.imshow('result',purple_gray)
        # weight_thre = cv2.adaptiveThreshold(weight_gray,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,5,-10)
        res, weight_thre = cv2.threshold(
            weight_gray, 0, 255, cv2.THRESH_BINARY)
        # cv2.imshow('result', purple_thre)
        # cv2.imshow('result', weight_thre)
        weight_thre = cv2.morphologyEx(weight_thre, cv2.MORPH_OPEN, kernel)
        weight_thre = cv2.morphologyEx(weight_thre, cv2.MORPH_CLOSE, kernel)
        cv2.imshow('result_thre', weight_thre)
        """
        #         5. 球的检测
        #     """
        #     # 先进行霍夫圆变换
        #     weight_circles = cv2.HoughCircles(weight_thre, cv2.HOUGH_GRADIENT_ALT,
        #                                       1.5, 20, param1=30, param2=0.50, minRadius=10, maxRadius=200)
        #     # 如果找到了球，则直接判定
        #     if weight_circles is not None:
        #         filter_circle = []
        #         weight_circles = np.uint16(np.around(weight_circles))
        #         for cir_ in weight_circles:
        #             cir = cir_[0]
        #             # 凹凸判断，通过创建掩膜进行逻辑与进行凹凸性判断
        #             x = cir[0]
        #             y = cir[1]
        #             r = cir[2]
        #             weight_circle_mask = np.zeros_like(weight_thre)
        #             cv2.circle(weight_circle_mask, (x, y), r, 255, thickness=-1)
        #             pixel_count = np.sum(np.logical_and(
        #                 weight_circle_mask, weight_thre) > 0)
        #             if pixel_count > 0.9 * np.pi * r * r:
        #                 filter_circle.append((x, y, r))
        #         if len(filter_circle) > 0:
        #             for cir_ in filter_circle:
        #                 weight_x = cir_[0]
        #                 weight_y = cir_[1]
        #                 weight_r = cir_[2]
        #                 cv2.circle(
        #                     color_image, (cir_[0], cir_[1]), cir_[2], (0, 255, 255), 2)
        #                 cv2.circle(color_image, (cir_[0], cir_[
        #                     1]), 2, (255, 255, 0), 2)

        #         else:
        #             # 提取轮廓
        #             contours_weight, hierarchy_weight = cv2.findContours(
        #                 weight_thre, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
        #             cv2.drawContours(
        #                 color_image, contours_weight, -1, (0, 255, 255), 1)
        #             if not contours_weight:
        #                 pass
        #             else:
        #                 # 寻找最大面积的轮廓
        #                 areas_weight = []
        #                 for c in range(len(contours_weight)):
        #                     areas_weight.append(cv2.contourArea(contours_weight[c]))

        #                 max_id_weight = areas_weight.index(max(areas_weight))
        #                 # 圆拟合
        #                 if contours_weight[max_id_weight].size < 10:
        #                     pass
        #                 else:
        #                     (x_weight, y_weight), radius_weight = cv2.minEnclosingCircle(
        #                         contours_weight[max_id_weight])
        #                     center_weight = (int(x_weight), int(y_weight))
        #                     radius_weight = int(radius_weight)
        #                     weight_x = x_weight
        #                     weight_y = y_weight
        #                     weight_r = radius_weight
        #                     cv2.circle(color_image, center_weight,
        #                                radius_weight, (0, 0, 255), 3)

        #     cv2.imshow('result', color_image)
        #     print(weight_x, weight_y, weight_r)
        #     weight_data = [weight_x, weight_y, weight_r]
        #     pack_data = struct.pack('<BfffB', 0xFF, weight_data[0], weight_data[1], weight_data[2], 0xEE)
        #     # serial_port.write(pack_data)

        key = cv2.waitKey(1)
        if key == 27:
            break

    #     time.sleep(0.05)
    # else:
    #     break
cv2.destroyAllWindows()
cap.release()
