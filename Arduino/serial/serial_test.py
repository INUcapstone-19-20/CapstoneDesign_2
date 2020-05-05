import serial

ser = serial.Serial(
    port='/dev/tty.usbmodem14201',
    baudrate=115200,
)

while True:
    print("color :", end=' ')
    color = input()
    ser.write(color.encode())