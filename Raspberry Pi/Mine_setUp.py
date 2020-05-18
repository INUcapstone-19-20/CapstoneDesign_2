import random
import serial

ser = serial.Serial(
    port='/dev/tty.ACM0',
    baudrate=115200,
)

BUTTONPAD_NUM = 144
start = 1
mineCiper = 3             # 지뢰 자릿숫 (4x4 : 1, 8x8 : 2, 12x12 :3) -> 일단 3으로 쭉

def Mine_ToArduino (red_mine, blue_mine) :
    MineTrans = "Mine" + red_mine + blue_mine
    print('MineTrans :', MineTrans)
    MineTrans = MineTrans.encode('utf-8')
    print('MineTrans(utf-8) :', MineTrans)

    # 아두이노로 전송
    ser.write(MineTrans)

def Turn_ToArduino (turn) :
    TurnTrans = "Turn" + turn
    TurnTrans = TurnTrans.encode('utf-8')
    print('TurnTrans :', TurnTrans)

    # 아두이노로 전송
    ser.write(TurnTrans)

def set_Mine ():
    # 빨강플레이어의 지뢰
    red_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 파랑플레이어의 지뢰
    blue_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 지뢰 중복 제거
    while (red_mine == blue_mine):
        blue_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 문자열 변환 후 자릿수 통일
    red_mine = str(red_mine).zfill(mineCiper)
    blue_mine = str(blue_mine).zfill(mineCiper)
    # 값 확인
    print('red_mine :', red_mine)
    print('blue_mine :', blue_mine)
    print('')

    Mine_ToArduino(red_mine, blue_mine)




    
