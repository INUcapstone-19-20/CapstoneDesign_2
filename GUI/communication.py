import serial
from PyQt5.QtCore import pyqtSignal
import random
import time

BUTTONPAD_NUM = 12 * 12        # 총 버튼 갯수
mineCiper = 3             # 지뢰 자릿수
count_turn = 0            # 턴 수

ser = serial.Serial(
    port='/dev/ttyACM0',         # 라즈베리파이 포트
    baudrate=115200,
    timeout = 1
)

# 모드 설정 전달
def mode_toArduino(mode):
    ModeTrans = "Mode" + mode
    ModeTrans = ModeTrans.encode('utf-8')

    # 아두이노로 전달
    ser.write(ModeTrans)


# 디코드 함수
def Decode(x):
    result = x.decode()
    result = str(result)            # string으로 변환
    return result


# 버튼패드 클릭 신호 전달
def click_FromArduino():
    if ser.readable():
        global count_turn
        LINE = ser.readline()
        code = Decode(LINE)
        # 값 확인
        # print("code : ", code, end='\n')
        
        # 버튼패드를 클릭했다면
        if "Click" in code:
            if count_turn > 0:
                count_turn -= 1
                # print("count_turn : ",count_turn)
            return 1
        elif "Boom" in code:
            if "Red" in code: return 99
            elif "Blue" in code: return -99
            elif "Single" in code: return 99

    else :
        print("읽기 실패 from_click_FromArduino_")
    return 0


# 어떤 플레이어 턴인지 아두이노로 전달
def turn_ToArduino(turn) :
    TurnTrans = "Turn" + turn               # 아두이노에서 수신할 때 식별하기 용이하도록 앞에 Turn 삽입
    TurnTrans = TurnTrans.encode('utf-8')
    # print('TurnTrans :', TurnTrans)

    # 아두이노로 전달
    ser.write(TurnTrans)

# 버튼 잠그기 함수
# def lock_ToArduino () :
#     lock = "Lock"                      
#     LockTrans = lock.encode('utf-8')
#     #print('LockTrans :', LockTrans)

#     # 아두이노로 전달
#     ser.write(LockTrans)

# 지뢰 설정 랜덤함수
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
    # print('red_mine :', red_mine)
    # print('blue_mine :', blue_mine)
    # print('')

    # 아두이노로 전달
    mine_ToArduino(red_mine, blue_mine)

# 지뢰 위치에 대한 정보를 아두이노로 전달
def mine_ToArduino (red_mine, blue_mine) :
    MineTrans = "Mine" + red_mine + blue_mine       # 아두이노에서 수신할 때 식별하기 용이하도록 앞에 Mine 삽입
    #print('MineTrans :', MineTrans)
    MineTrans = MineTrans.encode('utf-8')
    #print('MineTrans(utf-8) :', MineTrans)

    # 아두이노로 전달
    ser.write(MineTrans)

# 싱글모드 지뢰탐색횟수, 탐색시간 초과 위기 전달 함수
def Warn_ToArduino(delay) :
    delay = str(delay).zfill(3)
    Warn = "Warn" + delay
    WarnTrans = Warn.encode('utf-8')

    # 아두이노로 전달
    ser.write(WarnTrans)

# 싱글모드 게임 실패 전달 함수
def fail_ToArduino() :
    Fail = "Fail"
    FailTrans = Fail.encode('utf-8')

    # 아두이노로 전달
    ser.write(FailTrans)

'''
gui.py에서
if time < 10 :
    danger_ToArduino()

if 탐색횟수 < 5 :
    danger_ToArduino()
식으로 가면 될 듯
'''
