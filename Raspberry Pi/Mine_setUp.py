import random
import serial

ser = serial.Serial(
    port='/dev/tty.ACM0',
    baudrate=115200,
)

BUTTONPAD_NUM = 144
start = 1

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

# 지뢰 설정
if (start) :                                                    # start : 게임시작 or 재시작
    # 빨강플레이어의 지뢰
    red_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 파랑플레이어의 지뢰
    blue_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 지뢰 중복 제거
    while (red_mine == blue_mine):
        blue_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 문자열 변환 후 자릿수 3자리로 통일
    red_mine = str(red_mine).zfill(3)
    blue_mine = str(blue_mine).zfill(3)
    # 값 확인
    print('red_mine :', red_mine)
    print('blue_mine :', blue_mine)
    print('')

    Mine_ToArduino(red_mine, blue_mine)



    
