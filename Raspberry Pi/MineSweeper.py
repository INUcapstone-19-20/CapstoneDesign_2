import random

DiceResult = 0                                                      # 주사위 결과치
Turn = DiceResult                                                   # 턴 횟수
KEYPAD_EDGE_RISING = 0                                              # 버튼 클릭
numPixels = 16

#KEYPAD = list(range(numPixels))                                    # 키패드         
KEYPAD = [  0, 1, 2, 3,
            7, 6, 5, 4,
            8, 9, 10, 11,
            15, 14, 13, 12]                                         # 키패드

Red = [[255, 0 , 0], [255, 99, 99], [255, 198, 198]]
Blue = [[0, 0 , 255], [99, 99, 255], [198, 198, 255]]

Mine = random.choice(KEYPAD)                                       # 지뢰 설정
print(Mine)
