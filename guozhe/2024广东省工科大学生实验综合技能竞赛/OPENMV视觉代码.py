import sensor, image, time,math,pyb
from pyb import UART
import ustruct

  # 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
  # 关闭自动增益和白平衡
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
uart = UART(1,115200)
uart.init(115200, bits=8, parity=None, stop=1) #串口初始化
  # 设置颜色阈值（这里以红色为例）
green_threshold  =(15, 44, -35, -7, 10, 22)
red_threshold    =(1, 11, 6, 30, 0, 13)
blue_threshold  = (0, 25, 4, 33, -56, -17) # 注意：这些值需要根据实际情况调整
threshold = (0,0,0,0,0,0)

rx_buff=[]
state = 0
tx_flag = 0
recognize_flag = 0       #1
data = [0x00,0x00,0x00]

# 定义状态
STATE_IDLE = 0
STATE_COLOR_RECOGNIZE = 1
STATE_BALL_RECOGNIZE = 2
current_state = STATE_IDLE  #当前状态为初始态

paused = False  # 控制是否暂停的标志

  # 初始化时钟对象
clock = time.clock()
########串口接收颜色识别函数#########
def sending_data(cx,cy,ball_count):
    global uart;
    #frame=[0x2C,18,cx%0xff,int(cx/0xff),cy%0xff,int(cy/0xff),0x5B];
    #data = bytearray(frame)
    data = ustruct.pack("<bbhhhb",      #格式为俩个字符三个短整型(2字节)1个字符
                   0x2C,                      #帧头1
                   0x12,                      #帧头2
                   int(blob.cx()), # up sample by  2 #数据1
                   int(blob.cy()), # up sample by  2 #数据2
                   int(ball_count),
                   0x5B)
    uart.write(data);   #必须要传入一个字节数组

########串口接收颜色识别函数#########
def Receive_order():  # For first data type
    global state
    global data
    global recognize_flag
    if state == 0:
        data[0] = uart.readchar()
        if data[0] == 0x0C:  # Frame header
            state = 1
        else:
            state = 0
            rx_buff.clear()
    elif state == 1:
        data[1] = uart.readchar()
        recognize_flag = data[1]  # Adjust according to the actual index
        state = 2
    elif state == 2:
        data[2] = uart.readchar()
        if data[2] == 0x6b:
            tx_flag1 = int(data[0])
            state = 3
        else:
            state = 0  # Reset if not the expected byte
            rx_buff.clear()
    elif state == 3:
        state = 0
    else:
        state = 0
        rx_buff.clear()


while True:
  clock.tick()  # 更新时钟
  img = sensor.snapshot().lens_corr(1.42)  # 捕获图像

  if uart.any() > 0:
      Receive_order()  # 接收命令并更新识别标志
      if recognize_flag == 0xff:
          current_state = STATE_IDLE
      elif recognize_flag == 0x05:
          print('color')
          current_state = STATE_COLOR_RECOGNIZE
      elif recognize_flag == 0x04:
          print('ball')
          current_state = STATE_BALL_RECOGNIZE
      elif recognize_flag == 0x06:
          paused = True
          print('paulsed')
      elif recognize_flag == 0x07:
          paused = False
          print('continued')

  if paused:
      time.sleep_ms(100)  # 可选：在暂停时降低CPU使用率
      continue

  if current_state == STATE_IDLE:
      print("State init.")
  # 状态处理
  elif current_state == STATE_COLOR_RECOGNIZE:  #识别颜色
      if recognize_flag == 0x01:
          threshold = red_threshold
      elif recognize_flag == 0x02:
          threshold = blue_threshold
      elif recognize_flag == 0x03:
          threshold = green_threshold

  elif current_state == STATE_BALL_RECOGNIZE: #识别小球
      # 查找指定球色
      # 查找色块，限制最小面积为270像素
      blobs = img.find_blobs([threshold], area_threshold=90)
      if blobs:
          min_radius = 1000
          for blob in blobs:
              radius = int((blob.area() / 4) ** 0.5)
              if radius < min_radius:
                  min_radius = radius
          min_radius += 4.48

          ball_count = 0
          # 统计识别到的小球个数

          for blob in blobs:
              radius = int((blob.area() / 4) ** 0.5)
              if radius < min_radius:
                  img.draw_circle((blob.cx(), blob.cy(), int((blob.w() + blob.h()) / 4)))
                  img.draw_cross(blob.cx(), blob.cy(), size=2)
                  cx = blob.cx()
                  cy = blob.cy()
                  # 在串口返回中心坐标
                  sending_data(cx, cy,ball_count)
                  # 增加小球计数器
                  ball_count += 1
          print(ball_count)
