import serial
import numpy as np
import cv2

# 串口參數
SERIAL_PORT = 'COM9'  # 修改為您的 COM 埠
BAUD_RATE = 115200
IMAGE_WIDTH = 160
IMAGE_HEIGHT = 120
IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT

def receive_image():
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=5) as ser:
        print("waiting for data...")
        data = ser.read(IMAGE_SIZE)  # 接收影像資料
        if len(data) == IMAGE_SIZE:
            print("receive successfully, data length:", len(data))
            print(data)
            return np.frombuffer(data, dtype=np.uint8).reshape((IMAGE_HEIGHT, IMAGE_WIDTH))
        else:
            print("receive failed, data length:", len(data))
            return None

image = receive_image()
if image is not None:
    cv2.imshow("OV7670 Image", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()