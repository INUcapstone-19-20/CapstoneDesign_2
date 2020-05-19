import random
import serial
import time

ser = serial.Serial(
    # port='/dev/tty.ACM0', # 라즈베리파이 포트
    port='/dev/cu.usbmodem14201', # 테스트용 노트북 포트
    baudrate=115200,
)

BUTTONPAD_NUM = 64        # 총 버튼 갯수
mineCiper = 3             # 지뢰 자릿수
count_turn = 0

# 디코드 함수
def Decode(x):
    result = x.decode()
    result = str(result)            # string으로 변환
    return result

# 버튼패드 클릭 신호 수신
def click_FromArduino():
    if ser.readable():
        try:    
            global count_turn
            LINE = ser.readline()
            code = Decode(LINE)
            # 값 확인
            print("code : ", code, end='\n')
            
            # 버튼패드를 클릭했다면
            if "Click" in code :
                # 턴 수 감소
                count_turn -= 1
                print("count_turn : ", count_turn)
            
        except serial.serialutil.SerialException:
            time.sleep(1)
    else :
        print("읽기 실패 from_click_FromArduino_")


# 어떤 플레이어 턴인지 아두이노로 전달하기 위한 함수
def turn_ToArduino (turn) :
    TurnTrans = "Turn" + turn                       # 아두이노에서 수신할 때 식별하기 용이하도록 앞에 Turn 삽입
    TurnTrans = TurnTrans.encode('utf-8')
    #print('TurnTrans :', TurnTrans)

    # 아두이노로 전달
    ser.write(TurnTrans)

# 지뢰 위치에 대한 정보를 아두이노로 전달하기 위한 함수
def mine_ToArduino (red_mine, blue_mine) :
    MineTrans = "Mine" + red_mine + blue_mine       # 아두이노에서 수신할 때 식별하기 용이하도록 앞에 Mine 삽입
    #print('MineTrans :', MineTrans)
    MineTrans = MineTrans.encode('utf-8')
    #print('MineTrans(utf-8) :', MineTrans)

    # 아두이노로 전달
    ser.write(MineTrans)

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
    print('red_mine :', red_mine)
    print('blue_mine :', blue_mine)
    # print('')

    # 아두이노로 전달
    mine_ToArduino(red_mine, blue_mine)




    
