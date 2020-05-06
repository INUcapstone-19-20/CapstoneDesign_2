import serial

ser = serial.Serial(
    port='/dev/tty.ACM0',
    baudrate=115200,
)

while True:
    print("color :", end=' ')
    color = input()
    ser.write(color.encode())
