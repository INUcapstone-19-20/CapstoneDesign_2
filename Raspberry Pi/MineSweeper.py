import random
import math

#DiceResult = 0                                           # 주사위 결과치
#TurnNum = DiceResult                                     # 턴 횟수
#KEYPAD_EDGE_RISING = 0                                   # 버튼 클릭
#numPixels = 16
KEYPAD_ROW_NUM = 4                                       # 키패드 행 갯수
KEYPAD_COL_NUM = 4                                       # 키패드 열 갯수
red_turn = 1                                             # 빨강 플레이어 턴                   
blue_turn = 0                                            # 파랑 플레이어 턴  

# 키패드                                   
KEYPAD = [  [0, 1, 2, 3],                                           
            [4, 5, 6, 7],
            [8, 9, 10, 11],
            [12, 13, 14, 15]]
print('키패드 배열')
for i in KEYPAD :
    for j in i:
        print("%3d"%j, end=' ')
    print()
print('')                                     

# 지뢰와의 거리에 따른 LED 색_거리 짧은 순
Red = ['빨1','빨2','빨3']                   #[0xFFC6C6, 0xFF6363, 0xFF0000]
Blue = ['파1','파2','파3']                  #[0xC6C6FF, 0x6363FF, 0x0000FF]

# 플레이어 별 패드색 초기 선언
RKEYPAD_COLOR = [['0' for col in range(KEYPAD_COL_NUM)] for row in range(KEYPAD_ROW_NUM)]       # 0 for ~
BKEYPAD_COLOR = [['0' for col in range(KEYPAD_COL_NUM)] for row in range(KEYPAD_ROW_NUM)]       # 0 for ~

print('빨간플레이어 패드색 선언')
for i in RKEYPAD_COLOR :
    for j in i:
        print(j, end=' ')
    print()
print('')
print('파랑플레이어 패드색 선언')
for i in BKEYPAD_COLOR :
    for j in i:
        print(j, end=' ')
    print()
print('')

# 지뢰 설정
# 빨강플레이어의 지뢰
RMine_ROW = random.randint(0,KEYPAD_ROW_NUM-1)    
RMine_COL = random.randint(0,KEYPAD_COL_NUM-1)
RMine = KEYPAD[RMine_ROW][RMine_COL]
# 파랑플레이어의 지뢰
BMine_ROW = random.randint(0,KEYPAD_ROW_NUM-1)    
BMine_COL = random.randint(0,KEYPAD_COL_NUM-1)
BMine = KEYPAD[BMine_ROW][BMine_COL]
# 지뢰중복 방지
while (RMine_ROW == BMine_ROW and RMine_COL == BMine_COL):
    BMine_ROW = random.randint(0,KEYPAD_ROW_NUM-1)    
    BMine_COL = random.randint(0,KEYPAD_COL_NUM-1)
    BMine = KEYPAD[BMine_ROW][BMine_COL]

print('RMine_ROW :', RMine_ROW)
print('RMine_COL :', RMine_COL)
print('RMine :', RMine)
print('')
print('BMine_ROW :', BMine_ROW)
print('BMine_COL :', BMine_COL)
print('BMine :', BMine)
print('')

# 거리 알고리즘
if red_turn :
    for r in range(0, KEYPAD_ROW_NUM):
        for c in range(0, KEYPAD_COL_NUM):
            if ((r != RMine_ROW or c != RMine_COL) and (r != BMine_ROW or c != BMine_COL)):         # 지뢰가 아닌 곳
                distance = math.sqrt(math.pow((RMine_ROW - r), 2) + math.pow((RMine_COL - c), 2))
                if 0 < distance < 2 :
                    RKEYPAD_COLOR[r][c] = Red[0]
                elif 2 <= distance < 3 :
                    RKEYPAD_COLOR[r][c] = Red[1]
                else :
                    RKEYPAD_COLOR[r][c] = Red[2]
            elif (r == RMine_ROW and c == RMine_COL):           # 빨강지뢰
                RKEYPAD_COLOR[r][c] = '빨뢰'
            elif (r == BMine_ROW and c == BMine_COL):           # 파랑지뢰
                RKEYPAD_COLOR[r][c] = '파뢰'
elif blue_turn :                                                # 맨위에서 red_turn = 0, blue_turn = 1 하면 구현됌
    for r in range(0, KEYPAD_ROW_NUM):
        for c in range(0, KEYPAD_COL_NUM):
            # 지뢰가 아닌 곳
            if ((r != RMine_ROW or c != RMine_COL) and (r != BMine_ROW or c != BMine_COL)):
                distance = math.sqrt(math.pow((BMine_ROW - r), 2) + math.pow((BMine_COL - c), 2))
                if 0 < distance < 2 :
                    BKEYPAD_COLOR[r][c] = Blue[0]
                elif 2 <= distance < 3 :
                    BKEYPAD_COLOR[r][c] = Blue[1]
                else :
                    BKEYPAD_COLOR[r][c] = Blue[2]
            # 지뢰인 곳
            elif (r == RMine_ROW and c == RMine_COL):
                BKEYPAD_COLOR[r][c] = '빨뢰'
            elif (r == BMine_ROW and c == BMine_COL):
                BKEYPAD_COLOR[r][c] = '파뢰'

print('빨간플레이어 패드색')
for i in RKEYPAD_COLOR :
    for j in i:
        print("%5s"%j, end = ' ')             #print("%9d"%j, end=' ')
    print()
print('')

print('파랑플레이어 패드색')
for i in BKEYPAD_COLOR :
    for j in i:
        print("%5s"%j, end = ' ')             #print("%9d"%j, end=' ')
    print()
print('')
