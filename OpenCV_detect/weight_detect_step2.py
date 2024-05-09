'''2024 物流赛 砝码识别检测
    @Time 2024.5.1
'''
import cv2
import numpy as np
import time
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
lower_weight = np.array([0, 68, 11])
upper_weight = np.array([179, 164, 110])
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
        """
            4. 二值化
        """
        weight_gray = cv2.cvtColor(weight_img, cv2.COLOR_BGR2GRAY)
        weight_gray = cv2.GaussianBlur(weight_gray, (3, 3), 0)
        weight_thre = cv2.adaptiveThreshold(weight_gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 5, -10)
        res, weight_thre = cv2.threshold(
            weight_gray, 0, 255, cv2.THRESH_BINARY)
        # cv2.imshow('result', weight_thre)
        """
            5.开闭运算
        """
        weight_thre = cv2.morphologyEx(weight_thre, cv2.MORPH_OPEN, kernel)
        weight_thre = cv2.morphologyEx(weight_thre, cv2.MORPH_CLOSE, kernel)
        cv2.imshow("result_thre_01", weight_thre)
        """
            6.canny边缘检测
        """

        """
        #   7. 砝码检测
        """
        # 先进行霍夫圆变换
        weight_circles = cv2.HoughCircles(weight_thre, cv2.HOUGH_GRADIENT_ALT,
                                          1.5, 20, param1=30, param2=0.50, minRadius=10, maxRadius=200)

        # 如果找到了球，则直接判定
        if weight_circles is not None:
            filter_circle = []
            weight_circles = np.uint16(np.around(weight_circles))  # 将检测到的圆的坐标和半径转换为整数类型，并四舍五入（OpenCV要求）
            for cir_ in weight_circles:
                cir = cir_[0]
                x = cir[0]
                y = cir[1]
                r = cir[2]
                # 判断圆的半径是否大于22
                if r > 22:
                    filter_circle.append((x, y, r))
            if len(filter_circle) > 0:
                for cir_ in filter_circle:
                    weight_x = cir_[0]
                    weight_y = cir_[1]
                    weight_r = cir_[2]
                    # 在原图上绘制检测到的圆
                    cv2.circle(
                        color_image, (cir_[0], cir_[1]), cir_[2], (0, 255, 255), 2)  # 线条颜色为黄色
                    # 在圆心处绘制标记点
                    cv2.circle(color_image, (cir_[0], cir_[1]), 2, (255, 255, 0), 2)  # 颜色为青色

            else:
                # 提取轮廓
                contours_weight, hierarchy_weight = cv2.findContours(
                    weight_thre, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
                # 在图像上绘制所有轮廓
                cv2.drawContours(
                    color_image, contours_weight, -1, (0, 255, 255), 1)  # 线条颜色为黄色
                if not contours_weight:
                    pass
                else:
                    # 寻找最大面积的轮廓
                    areas_weight = []
                    for c in range(len(contours_weight)):
                        areas_weight.append(cv2.contourArea(contours_weight[c]))

                    max_id_weight = areas_weight.index(max(areas_weight))
                    # 椭圆拟合
                    if contours_weight[max_id_weight].size < 10:
                        pass
                    else:
                        # (x_weight, y_weight), radius_weight = cv2.fitEllipse(
                        #     contours_weight[max_id_weight])
                        ellipse = cv2.fitEllipse(contours_weight[max_id_weight])
                        center = ellipse[0]
                        x_weight = center[0]
                        y_weight = center[1]
                        axes = ellipse[1]
                        radius_weight = axes[0] / 2 + axes[1] / 2
                        if radius_weight > 22:
                            center_weight = (int(x_weight), int(y_weight))
                            radius_weight = int(radius_weight)
                            weight_x = x_weight
                            weight_y = y_weight
                            weight_r = radius_weight
                            # 在图像上绘制拟合出的圆
                            cv2.circle(color_image, center_weight,
                                       radius_weight, (0, 0, 255), 3)  # 线条颜色为红色

        # 显示结果图像
        cv2.imshow('result', color_image)
        print(weight_x, weight_y, weight_r)  # 输出检测到的球体位置信息
        weight_data = [weight_x, weight_y, weight_r]
        # pack_data = struct.pack('<BfffB', 0xFF, weight_data[0], weight_data[1], weight_data[2], 0xEE)
        # serial_port.write(pack_data)  # 将数据打包发送到串口

        key = cv2.waitKey(1)
        if key == 27:
            break   

        time.sleep(0.05)
    else:
        break
    
cv2.destroyAllWindows()
cap.release()
