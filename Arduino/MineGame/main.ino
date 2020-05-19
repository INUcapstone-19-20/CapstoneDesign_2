#include "Adafruit_NeoTrellis.h"
#include "MineGame.h"

#define Y_DIM 4 // number of rows of keys
#define X_DIM 4 // number of columns of keys

// create a matrix of neotrellis boards
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
    { Adafruit_NeoTrellis(0x31) }

    // { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32)},
    // { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E)}

    // { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x33) },
    // { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E), Adafruit_NeoTrellis(0x3F) },
    // { Adafruit_NeoTrellis(0x4A), Adafruit_NeoTrellis(0x4B), Adafruit_NeoTrellis(0x4C) }

};

// pass matrix to the multitrellis constructor
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

// 시리얼 수신 변수
static String sig = "";
//static String temp;

// 시리얼 수신 시 문자열 슬라이싱 용 변수_문자열의 끝은 NULL
static char check[5];
static char red[4];
static char blue[4];
static char turnT[2];

// 턴 저장 변수
static char turn;

// 모드 저장 변수
static char mode;

//  Input a value 0 to 255 to get a color value
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}

void setup() {
    Serial.begin(115200);
    
    if(!trellis.begin()) {
        // Serial.println("failed to begin trellis");
        while(1);
    }
    
    // starting effect
    for(int i=0; i<Y_DIM*X_DIM; i++) {
        trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255)));
        trellis.show();
        delay(50);
    }

    for(int i=0; i<Y_DIM*X_DIM; i++) {
        // activate rising edge on all keys
        trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);

        // all neopixels off
        trellis.setPixelColor(i, 0x000000);
        trellis.show();

        delay(50);
    }
}

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

    // 게임모드 시리얼을 수신한 경우
    // if(temp == "Mode")
    {
        // 테스트 코드
        String test = "BATTLE"; // 배틀모드로 테스트
        if (test == "BATTLE") {
            mode = 'B';
            BattleMode game;    // 배틀 모드 생성
        }
        else {
            mode = 'S';
            SingleMode game;    // 싱글 모드 생성
        } // end 테스트 코드
    } // end 게임모드 시리얼

    // 지뢰 설정 시리얼을 수신한 경우
    if (temp == "Mine")
    {   // 의도하지않은 값 방지
        if (sig.length()==10)
        {
            if(mode == 'B') {   // 배틀모드
                sig.substring(4,7).toCharArray(red,4);          // red_mine 부분 슬라이싱
                sig.substring(7,10).toCharArray(blue,4);        // blue_mine 부분 슬라이싱
                // int로 변환
                uint16_t red_mine = atoi(red);
                uint16_t blue_mine = atoi(blue);

                // 테스트
                Serial.print("red : ");
                Serial.print(red_mine);
                Serial.print("\t");
                Serial.print("blue : ");
                Serial.print(blue_mine);

                // 지뢰 전달
                game = BattleMode(red_mine, blue_mine);
            }

            if(mode == 'S') {   // 싱글모드
                // mine 받는 코드
                // uint16_t mine = 
                uint16_t mine = 8;  // 테스트 임시 지뢰 위치

                // 지뢰 전달
                game = SingleMode(mine);
                // 콜백 설정
                game.setCallback();
            }

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
    } // end 지뢰 설정 시리얼

    // 턴에 대한 정보를 수신한 경우 --> mode == 'B' 일 때만 실행
    else if (temp == "Turn")
    {   
        if(mode == 'B') { 
            // 의도하지않은 값 방지
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

                // 턴에 따라 콜백 지정
                game.setCallback(turn);
                
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
    } // end 턴 정보 시리얼

    // 게임진행에 필요없는 시리얼인 경우
    else
    {
        // 초기화
        sig = "";
        temp = "";
    }

    // 게임 시작
    game.startGame();
}
