#include "Adafruit_NeoTrellis.h"

#define Y_DIM 8 // number of rows of keys
#define X_DIM 8 // number of columns of keys
#define COLORS 3  // number of colors

// create a matrix of neotrellis boards
// 12 x 12
// Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
    // { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x33) },
    // { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E), Adafruit_NeoTrellis(0x3F) },
    // { Adafruit_NeoTrellis(0x4A), Adafruit_NeoTrellis(0x4B), Adafruit_NeoTrellis(0x4C) }
// };

// 8 x 8
 Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
    { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32)},
    { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E)}
 };

// 4 x 4
//Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
//   { Adafruit_NeoTrellis(0x4A) }
//};

// pass this matrix to the multitrellis constructor
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

// mine button color
const static uint32_t mine_colors[4] = { 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF };

// set players' color
//static uint32_t blue_colorcode[COLORS] = { 0x0000FF, 0x5A5AFF, 0xB4B4FF };
//static uint32_t red_colorcode[COLORS] = { 0xFF0000, 0xFF5A5A, 0xFFB4B4 };
// test
static uint32_t blue_colorcode[COLORS] = { 0x0000FF, 0x333377, 0x000022 };
static uint32_t red_colorcode[COLORS] = { 0xFF0000, 0x770303, 0xAA5050 };

static uint32_t blue_colors[Y_DIM*X_DIM], red_colors[Y_DIM*X_DIM];
//static uint32_t blue_colors[16] = { 0x6363FF, 0x0000FF, 0x0000FF, 0x0000FF,     // - - - -
//                                    0x6363FF, 0x0000FF, 0x000000, 0x0000FF,     // - - * -
//                                    0x6363FF, 0x0000FF, 0x0000FF, 0x0000FF,     // - - - -
//                                    0xC6C6FF, 0x6363FF, 0x6363FF, 0x6363FF };   // - - - -    ( * is mine) 
//
//static uint32_t red_colors[16] = { 0xFFC6C6, 0xFFC6C6, 0xFFC6C6, 0xFFC6C6,      // - - - -
//                                    0xFF6363, 0xFF6363, 0xFF6363, 0xFFC6C6,     // - - - -
//                                    0xFF0000, 0xFF0000, 0xFF0000, 0xFF6363,     // - - - -
//                                    0xFF0000, 0x000000, 0xFF0000, 0xFF6363 };   // - * - -    ( * is mine) 

static uint8_t red_mine, blue_mine; // location of mine

//static uint8_t red_turn, blue_turn; // number of turns
//static boolean firstturn;

static uint8_t ispressed[Y_DIM*X_DIM]; // button state. 1 is pressed, 0 is not pressed

// 시리얼 수신 변수
static String sig = "";

// 시리얼 수신 시 문자열 슬라이싱 용 변수_문자열의 끝은 NULL
static char check[5];
static char red[4];
static char blue[4];
static char turnT[2];

// 턴 저장 변수
static char turn;

//  Input a value 0 to 255 to get a color value
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   /* uint32_t Color(uint8_t r, uint8_t g, uint8_t b) : Convert R, G, B into packed 32-bit RGB color. */
   return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
   WheelPos -= 85;
   return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
   WheelPos -= 170;
   return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}

double distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

void setColor() {

    for(int i=0; i<Y_DIM*X_DIM; i++) {

        // all neopixels off
        trellis.setPixelColor(i, 0x000000);
        trellis.show();
        ispressed[i] = 0;
    }
  
    uint8_t b_mine_x = blue_mine % X_DIM;
    uint8_t b_mine_y = blue_mine / X_DIM;
    for(uint8_t y=0; y<Y_DIM; y++) {
      for(uint8_t x=0; x<X_DIM; x++) {
        if(distance(b_mine_x, b_mine_y, x, y) <= sqrt(2)) 
          blue_colors[y * X_DIM + x] = blue_colorcode[0];
        if(distance(b_mine_x, b_mine_y, x, y) > sqrt(2) && distance(b_mine_x, b_mine_y, x, y) < sqrt(8)) 
          blue_colors[y * X_DIM + x] = blue_colorcode[1];
        if(distance(b_mine_x, b_mine_y, x, y) >= sqrt(8)) 
          blue_colors[y * X_DIM + x] = blue_colorcode[2];
      }
    }
    uint8_t r_mine_x = red_mine % X_DIM;
    uint8_t r_mine_y = red_mine / X_DIM;
    for(uint8_t y=0; y<Y_DIM; y++) {
      for(uint8_t x=0; x<X_DIM; x++) {
        if(distance(r_mine_x, r_mine_y, x, y) <= sqrt(2)) 
          red_colors[y * X_DIM + x] = red_colorcode[0];
        if(distance(r_mine_x, r_mine_y, x, y) > sqrt(2) && distance(r_mine_x, r_mine_y, x, y) < sqrt(8)) 
          red_colors[y * X_DIM + x] = red_colorcode[1];
        if(distance(r_mine_x, r_mine_y, x, y) >= sqrt(8)) 
          red_colors[y * X_DIM + x] = red_colorcode[2];
      }
    }

}

// mine LED effect
void showMine(uint16_t mine_key, String color) {
    // keynumber -> x,y 좌표로 변경
    uint8_t mine_x = mine_key % X_DIM;
    uint8_t mine_y = mine_key / X_DIM;

    if(color == "red") {
        Serial.println("RedBoom");
        // 첫번째 영역 on
        for(uint8_t y=0; y<Y_DIM; y++) {
            for(uint8_t x=0; x<X_DIM; x++) {
                if(x == mine_x && y == mine_y) continue;                    
                if(distance(mine_x, mine_y, x, y) <= sqrt(2)) {
                    trellis.setPixelColor(x, y, red_colors[y * X_DIM + x]);
                    trellis.show();
                }
            }
        }
        delay(500);

        // 두번째 영역 on
        for(uint8_t y=0; y<Y_DIM; y++) {
            for(uint8_t x=0; x<X_DIM; x++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) > sqrt(2) && distance(mine_x, mine_y, x, y) < sqrt(8)) {
                    trellis.setPixelColor(x, y, red_colors[y * X_DIM + x]);
                    trellis.show();
                }
            }
        }
        delay(500);

        // 세번째 영역 on
        for(uint8_t y=0; y<Y_DIM; y++) {
            for(uint8_t x=0; x<X_DIM; x++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) >= sqrt(8)) {
                    trellis.setPixelColor(x, y, red_colors[y * X_DIM + x]);
                    trellis.show();
                }            
            }
        }
        delay(500);

    }
    else { // color == "blue"
        Serial.println("BlueBoom");
        // 첫번째 영역 on
        for(uint8_t y=0; y<Y_DIM; y++) {
            for(uint8_t x=0; x<X_DIM; x++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) <= sqrt(2)) {
                    trellis.setPixelColor(x, y, blue_colors[y * X_DIM + x]);
                    trellis.show();
                }
            }
        }
        delay(500);

        // 두번째 영역 on
        for(uint8_t y=0; y<Y_DIM; y++) {
            for(uint8_t x=0; x<X_DIM; x++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) > sqrt(2) && distance(mine_x, mine_y, x, y) < sqrt(8)) {
                    trellis.setPixelColor(x, y, blue_colors[y * X_DIM + x]);
                    trellis.show();
                }
            }
        }
        delay(500);

        // 세번째 영역 on
        for(uint8_t y=0; y<Y_DIM; y++) {
            for(uint8_t x=0; x<X_DIM; x++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) >= sqrt(8)) {
                    trellis.setPixelColor(x, y, blue_colors[y * X_DIM + x]);
                    trellis.show();
                }
            }
        }
        delay(500);


    }

    // 지뢰 효과
    uint8_t i = 0;
    String s = "";
    while (1)
    {
        trellis.setPixelColor(mine_key, mine_colors[i]);
        trellis.show();
        delay(500);
        if(i >= 3) i = 0;
        else i++;
        // 종료 조건 추가
        
        while(Serial.available()) {
          // 시리얼 읽어서 문자열로 저장
          s = Serial.readString();
        }

        if(s.substring(0,4) == "Mine") {
            setColor();
            break;
        }
    }
}

// define a callback for player1's key presses
TrellisCallback red_ON(keyEvent evt) {
//    if(red_turn > 0) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
                ispressed[evt.bit.NUM] = 1;
                // 누른 버튼이 지뢰일 경우
                if(evt.bit.NUM == red_mine) { // 빨간 플레이어 패배(본인 지뢰 클릭)
                    showMine(evt.bit.NUM, "red");
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == blue_mine) { // 빨간 플레이어 승리
                    showMine(evt.bit.NUM, "blue");
                    // 파이썬에 '게임 종료' 전송
                }
                // 누른 버튼이 지뢰가 아닐 경우
                else {
                    trellis.setPixelColor(evt.bit.NUM, red_colors[evt.bit.NUM]);
                    trellis.show();
                    Serial.println("Click"); // 버튼 클릭시 파이썬에 '버튼 클릭 이벤트 발생' 전송
//                    // 테스트용
//                    red_turn--;
//                    if(red_turn == 0) {
//                        firstturn = !firstturn; // toggle turn state
//                    }// end 테스트용
                }                
                
            }
        }
//    }
    return 0;
}

// define a callback player2's key presses
TrellisCallback blue_ON(keyEvent evt) {
//    if(blue_turn > 0) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
                ispressed[evt.bit.NUM] = 1;
                // 누른 버튼이 지뢰일 경우
                if(evt.bit.NUM == red_mine) { // 파란 플레이어 승리
                    showMine(evt.bit.NUM, "red");
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == blue_mine) { // 파란 플레이어 패배(본인 지뢰 클릭)
                    showMine(evt.bit.NUM, "blue");
                    // 파이썬에 '게임 종료' 전송
                }
                // 누른 버튼이 지뢰가 아닐 경우
                else {
                    trellis.setPixelColor(evt.bit.NUM, blue_colors[evt.bit.NUM]);
                    trellis.show();
                    Serial.println("Click"); // 버튼 클릭시 파이썬에 '버튼 클릭 이벤트 발생' 전송
                    // 테스트용
//                    blue_turn--;
//                    if(blue_turn == 0) {
//                        firstturn = !firstturn; // toggle turn state
//                    }// end 테스트용
                }
            }
        }
//    }
    return 0;
}

void setup() {
    Serial.begin(115200);
    
    if(!trellis.begin()) {
//        Serial.println("failed to begin trellis");
        while(1);
    }

    // set location of mine
    // blue_mine = 전달값
    // red_mine = 전달값
    // blue_mine = 0;
    // red_mine = 8;

    // set color array
    // setColor();
    
    // set turn state
    //firstturn = true;

    // initialize button state
    for(uint8_t i=0; i<Y_DIM*X_DIM; i++) {
      ispressed[i] = 0;
    }
    
    // starting effect
    for(int i=0; i<Y_DIM*X_DIM; i++) {
        trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255)));
        trellis.show();
        delay(30);
    }

    for(int i=0; i<Y_DIM*X_DIM; i++) {
        // activate rising edge on all keys
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);

        // all neopixels off
        trellis.setPixelColor(i, 0x000000);
        trellis.show();

        delay(30);
    }
}

void lockOn() {
    for(int i=0; i<Y_DIM*X_DIM; i++) {
        // deactivate rising edge on all keys
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, false);
    }
}

void lockOff() {
    for(int i=0; i<Y_DIM*X_DIM; i++) {
        // activate rising edge on all keys
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
    }
}

// 테스트용 임시 변수
// uint8_t r_turns[6] = {0, 10, 0, 10, 0, 10};
// uint8_t b_turns[6] = {10, 0, 10, 0, 10, 0};
// uint8_t it = 0;
// String ch, number;

void loop() {

//    if(Serial.available()){
//        ch = Serial.readString();
//    }
//    // serial 받은 값에 "red"가 포함되면
//    if(ch.indexOf("red") >= 0) {
//        number = ch.substring(3,ch.length());
//        // 빨강 지뢰 재설정
//        red_mine = number.toInt();
//    }
//
//    else if(ch.indexOf("blue") >= 0) {
//        number = ch.substring(4,ch.length());
//        blue_mine = number.toInt();
//    }
//    setColor();
    
   while(Serial.available())
   {
       // 시리얼 읽어서 문자열로 저장
       char wait = Serial.read();
       sig.concat(wait);
   }
   
   // 문자열 슬라이싱 (Mine or Turn)
   sig.substring(0,4).toCharArray(check,5);        // 문자열 끝은 NULL
   // 문자열 string으로 저장
   String temp = check;
   
   // 지뢰 설정 시리얼을 수신한 경우
   if (temp == "Mine")
   {   // 의도하지않은  방지
       if (sig.length()==10)
       {
           sig.substring(4,7).toCharArray(red,4);          // red_mine 부분 슬라이싱
           sig.substring(7,10).toCharArray(blue,4);        // blue_mine 부분 슬라이싱
           // int로 변환
           red_mine = atoi(red);
           blue_mine = atoi(blue);

           // 테스트
           Serial.print("red : ");
           Serial.print(red_mine);
           Serial.print("\t");
           Serial.print("blue : ");
           Serial.print(blue_mine);

           // set color array
           setColor();

           // 초기화
           sig = "";
           temp = "";
       }
       // 시리얼이 잘못 수신된 경우
       else 
       {
           // 초기화
           sig = "";
           temp = "";
       }
       Serial.println();
   }
   // 턴에 대한 정보를 수신한 경우
   else if (temp == "Turn")
   {   // 의도하지않은 값 방지
       if (sig.length()==5)
       {
           sig.substring(4,5).toCharArray(turnT,2);    // turn 부분 슬라이싱 (trunT : 'R' or 'B')

           // 테스트
           Serial.print("\t");
           Serial.print("turnT : ");
           Serial.print(turnT);
           
           // 턴 저장
           turn = turnT[0];        // turnT[1] = NULL

           // 테스트
           Serial.print("\t");
           Serial.print("turn : ");
           Serial.print(turn);

           // 초기화
           sig = "";
           temp = "";
       }
       // 시리얼이 잘못 수신된 경우
       else 
       {
           // 초기화
           sig = "";
           temp = "";
       }
       Serial.println();
   }
   // 게임진행에 필요없는 시리얼인 경우
   else
   {
       // 초기화
       sig = "";
       temp = "";
   }
//     String sline = "";
    
//     while(Serial.available())
//     {
//         sline = Serial.readString();
//     }
    
//     String temp = sline.substring(0,4);
//     turn = '0';
//     // 지뢰 설정 시리얼을 수신한 경우
//     if (temp == "Mine")
//     {   // 의도하지않은  방지
//         if (sline.length()==10)
//         {
//             // int로 변환
// //            sline.substring(4,7).toCharArray(red,4);
// //            sline.substring(7,10).toCharArray(blue,4);
//             red_mine = sline.substring(4, 7).toInt();
//             blue_mine = sline.substring(7, 10).toInt();

//             // set color array
//             setColor();
//         }
//     }
//     // 턴에 대한 정보를 수신한 경우
//     else if (temp == "Turn")
//     {   // 의도하지않은 값 방지
//         if (sline.length()==5)
//         {
//             turn = sline[4];
//         }
//     }
    

    // 테스트용
    // if(firstturn) { // 턴이 바뀌면 해당 플레이어의 턴 횟수 설정
    //     //red전달값 > blue전달값 ? red_turn = red전달값 : blue_turn = blue전달값;
    //     r_turns[it] > b_turns[it] ? red_turn = r_turns[it] : blue_turn = b_turns[it];
    //     firstturn = !firstturn; // toggle turn state
    //     it++;
    // }// end 테스트용
    
    // register a callback for all keys
    for(int i=0; i<Y_DIM*X_DIM; i++) 
    {
        if (turn == 'R')
        //if(red_turn > 0) { // 차례가 아니면 0
        {
            trellis.registerCallback(i, red_ON);
        }
        else if(turn == 'B')
        //else if(blue_turn > 0) {
        {
            trellis.registerCallback(i, blue_ON);
        }
    }
    
    trellis.read();
    delay(20);
}