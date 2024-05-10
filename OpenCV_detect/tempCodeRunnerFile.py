serial_port = serial.Serial("/dev/ttyACM0", 115200, timeout=0.5)
# serial_port_state = serial_port.is_open