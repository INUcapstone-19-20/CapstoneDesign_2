import serial
from PyQt5.QtCore import pyqtSignal
import random
import time

# 시리얼 포트 설정
ser = serial.Serial(
    # port='/dev/ttyACM0',          # 라즈베리파이 포트
    port='/dev/cu.usbmodem14201',   # 테스트용 노트북 포트
    baudrate=115200,
    timeout = 1
)

BUTTONPAD_NUM = 12 * 12        # 총 버튼 갯수
toCiper = 3                    # 전달 자릿수
count_turn = 0                 # 턴 수

# 디코드 함수
def Decode(x):
    result = x.decode()             # 디코드
    result = str(result)            # string으로 변환
    return result


# 모드 설정 전달
def mode_toArduino(mode):                       # mode = "Loding" or "Single" or "Battle"
    ModeTrans = "Mode" + mode                   # 시리얼 분류 문자 "Mode" 삽입
    ModeTrans = ModeTrans.encode('utf-8')       # 인코드

    # 아두이노로 전달
    ser.write(ModeTrans)


# 싱글모드 지뢰탐색횟수, 탐색시간 초과 위기 전달
def warn_toArduino(delay) :                     # delay : 탐색시간이 줄어들수록 깜빡이는 정도를 증가시키기 위해 전달
    delay = str(delay).zfill(toCiper)           # 문자열 변환 후 자릿수 통일
    Warn = "Warn" + delay                       # 시리얼 분류 문자 "Warn" 삽입
    WarnTrans = Warn.encode('utf-8')            # 인코드

    # 아두이노로 전달
    ser.write(WarnTrans)


# 싱글모드 게임 실패 전달
def fail_toArduino() :
    Fail = "Fail"
    FailTrans = Fail.encode('utf-8')            # 인코드

    # 아두이노로 전달
    ser.write(FailTrans)


# 어떤 플레이어 턴인지 아두이노로 전달
def turn_toArduino(turn) :
    TurnTrans = "Turn" + turn                   # 시리얼 분류 문자 "Turn" 삽입
    TurnTrans = TurnTrans.encode('utf-8')       # 인코드

    # 아두이노로 전달
    ser.write(TurnTrans)


# 지뢰 위치에 대한 정보를 아두이노로 전달
def mine_toArduino (red_mine, blue_mine) :
    MineTrans = "Mine" + red_mine + blue_mine       # 시리얼 분류 문자 "Mine" 삽입
    MineTrans = MineTrans.encode('utf-8')           # 인코드

    # 아두이노로 전달
    ser.write(MineTrans)


# 지뢰 설정 및 전달
def set_mine ():
    # 빨강플레이어의 지뢰
    red_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 파랑플레이어의 지뢰
    blue_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 지뢰 중복 제거
    while (red_mine == blue_mine):
        blue_mine = random.randint(0,BUTTONPAD_NUM-1)
    # 문자열 변환 후 자릿수 통일
    red_mine = str(red_mine).zfill(toCiper)
    blue_mine = str(blue_mine).zfill(toCiper)

    # 아두이노로 전달
    mine_toArduino(red_mine, blue_mine)


# 버튼패드 클릭 신호 수신
def click_fromArduino():
    if ser.readable():
        global count_turn                       # 턴 수를 바꿔주기 위해 전역변수로 정의
        LINE = ser.readline()                   # 시리얼 읽기
        code = Decode(LINE)                     # 디코드
        
        # 지뢰가 아닌 버튼을 클릭했다면 (code = "Click")
        if "Click" in code:
            if count_turn > 0:
                count_turn -= 1                 # 턴 수 1 감소
            return 1

        # 지뢰인 버튼을 클릭했다면 (code = "RedBoom" or "BlueBoom" or "SingleBoom")
        elif "Boom" in code:
            if "Red" in code: return 99         
            elif "Blue" in code: return -99
            elif "Single" in code: return 99
    else :
        print("읽기 실패 from_click_FromArduino_")
    return 0
