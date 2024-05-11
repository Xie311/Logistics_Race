import struct
import time
import serial
import serial.tools.list_ports

import serial.tools.list_ports

# 列出所有可用的串口设备
ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(ports):
    print(f"{port}: {desc} [{hwid}]")


# ser = serial.Serial("/dev/ttyAMA0", 115200)   
# serial_port_state = ser.is_open# 打开ttyUSB0，将波特率配置为115200，其余参数使用默认值
# while 1:
#     weight_data = [2, 3, 4]
#     pack_data = struct.pack('<BBfffBB', 0xFF,0xFE, weight_data[0], weight_data[1], weight_data[2], 0xFE,0xFF)
#     ser.write(pack_data)  # 将数据打包发送到串口
#     time.sleep(0.1)