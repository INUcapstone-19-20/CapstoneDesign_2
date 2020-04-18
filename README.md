
# 회의록
### 20.04.18

[GUI]

04.22 까지 완성

[버튼패드]

Adafruit NeoTrellis RGB Driver PCB for 4x4 Keypad

https://www.adafruit.com/product/3954 제품 신청 완료


매뉴얼 숙지

https://learn.adafruit.com/adafruit-neotrellis


Arduino Code 분석하기

https://github.com/adafruit/Adafruit_Seesaw/tree/master/examples/NeoTrellis/basic

https://github.com/adafruit/Adafruit_Seesaw/blob/master/examples/NeoTrellis/ripples/ripples.ino

***

### 20.04.11
[파트분배]

GUI - 이예은

PWM - 김수연, 손병수, 김병민


[GUI]

pyqt로 변경 완료.

화면 구성 진행


[PWM]

exercise3를 analogWrite와 millis 사용해 한 행에 다양한 색 출력.

-> 다양한 색 나오지만 LED 출력이 약하고 불안정함.



불안정(깜빡이는 것이 육안으로 보임)한 문제

  -> 원인 파악: 1ms마다 LED를 껐다 켰다하면서 발생하는 것이라 생각함.
  
  -> 그러나 R, G, B 단일 색 255 밝기로 출력 시에는 깜빡이지 않음. 출력이 약해짐.

LED 출력이 약해지는 문제

  -> R, G, B 단일 색 각각 255 밝기 사용 시 : 같은 값임에도 0행에서 3행으로 갈수록 밝기 약해짐.
  
  -> 문제 해결을 위해 각 RGB 값의 합을 255 이하로 조정했지만 여전히 같은 문제가 발생.


지뢰게임 구현에 있어서 매트릭스로 구현된 회로는 한계점이 있다고 판단함.

적당한 기판을 다시 조사하여 회로도를 분석해보고, 해당 기판으로의 변경을 논의함.


***

### 20.04.05
[파트 분배]

GUI - 김병민, 이예은

PWM - 김수연, 손병수

[GUI]

Tkinter->pyqt로 변경

변경 사유 : 기능이 적어서 GUI 디자인에 어려움을 느낌


[PWM]

색깔을 담당하는 행, 온/오프를 담당하는 열

같은 행에서는 같은 색깔만 켤 수 있다는 한계점

->한 행에서 여러 색깔을 켜기 위해 시간 차 알고리즘을 이용.

밝기 조절

단일색에서 같은 analog value 값을 주었을 시에는 빛이 잘 나옴

하지만 각 행에 다른 analog value 값을 주었을 시에는 한 행, 혹은 여러 개의 행에서 불빛이 나오지 않는 현상이 발생

색을 조합했을 때는 다른 analog value 값을 주었을 시에도 모든 행의 불빛이 잘 나옴

색을 조합한 상태에서 아두이노 코드 내에서 특정 색(R, G, B 중 하나)를 껐을 때 의도와는 다른 행에 다른 색이 나오는 현상이 발생

3단계 밝기 구분은 가능하다고 보여지나 4단계 밝기 구분은 힘들다고 판단됨

---

### 20.03.30
[파트 분배]   
GUI - 김병민, 이예은   
PWM - 김수연, 손병수   

[GUI]   
Tkinter(GUI 모듈)_파이썬 이용   
이전에 GUI 짰던 거를 토대로 구성   
\- 30일까지   
화면에 싱글모드, 배틀모드 띄워진 상태에서 누르면 화면 넘어가는 디스플레이 구성

[PWM]   
개인 개발환경 구축   
코드 완벽 이해   
\- 30일까지   
LED RGB와 PWM value 수치를 조정하여 지뢰찾기에서 최종적으로 사용할 3가지 색 정하기 (색 or 밝기 or 색 + 밝기)   
한 행에 여러가지 색, 여러가지 밝기 켜지도록 하기   
Advanced - 1행 4열을 지뢰라고 생각하고 부분적으로 밝기 다르게 구성   
다음에 - 턴의 의미 부여(어떤 버튼이든 누를 때마다 빨강, 파랑 번갈아 가면서 나오도록)   

[기타]   
회의록 - 한글파일 업로드 방식 -> GitHub에 바로 바로 적기   
GitHub 깔끔하게 파일 분류하기
***
