#include "Adafruit_NeoTrellis.h"

#define Y_DIM 12 // number of rows of keys
#define X_DIM 12 // number of columns of keys
#define COLORS 3  // number of colors

// define color
#define SINGLE1 0x00FFFF
#define SINGLE2 0x8AFFFF
#define SINGLE3 0xE0FFFF
#define RED1 0x990000
#define RED2 0xDD1111
#define RED3 0x772222
#define BLUE1 0x0000FF
#define BLUE2 0x2222DD
#define BLUE3 0x333355

// define distance
#define DISTANCE1 sqrt(8)
#define DISTANCE2 sqrt(20)

typedef struct Player {
    String ID;
    uint32_t colorcode[COLORS];
    uint32_t colors[Y_DIM*X_DIM];
    uint16_t mine;
} Player;

static Player pSingle, pRed, pBlue;

// create a matrix of neotrellis boards
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
    // 4 x 4
    // { Adafruit_NeoTrellis(0x31) }

    // 8 x 8
//    { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32)},
//    { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E)}

    // 12 x 12
     { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x33) },
     { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E), Adafruit_NeoTrellis(0x3F) },
     { Adafruit_NeoTrellis(0x4A), Adafruit_NeoTrellis(0x4B), Adafruit_NeoTrellis(0x4C) }

};

// pass this matrix to the multitrellis constructor
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);
TrellisCallback blue_ON(keyEvent evt);

static uint8_t ispressed[Y_DIM*X_DIM]; // button state. 1 is pressed, 0 is not pressed

// 시리얼 수신 변수
static String sig = "";

// 시리얼 수신 시 문자열 슬라이싱 용 변수_문자열의 끝은 NULL
static String check;
static char red[4];
static char blue[4];
static char mine[4];    // 싱글모드 지뢰
static char turnT[2];
static char modeM[2];   // 모드

// 턴 저장 변수
static String turn;

// 모드 저장 변수
static String mode;

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

double distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

void initButtonState() {
        for(int i=0; i<Y_DIM*X_DIM; i++) {
        // all neopixels off
        trellis.setPixelColor(i, 0x000000);
        trellis.show();
        ispressed[i] = 0;
    }
}

void setPlayer(Player *p, String id) {
    p->ID = id;

    if(id == "Single") {
        p->colorcode[0] = SINGLE1;
        p->colorcode[1] = SINGLE2;
        p->colorcode[2] = SINGLE3;
    }
    else if(id == "Red") {
        p->colorcode[0] = RED1;
        p->colorcode[1] = RED2;
        p->colorcode[2] = RED3;
    }
    else if(id == "Blue") {
        p->colorcode[0] = BLUE1;
        p->colorcode[1] = BLUE2;
        p->colorcode[2] = BLUE3;
    }
    
}

void setPlayerColors(Player *p) {
    uint8_t mine_x = p->mine % X_DIM;
    uint8_t mine_y = p->mine / X_DIM;

    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            double d = distance(mine_x, mine_y, x, y);
            if(d < DISTANCE1) 
                p->colors[y * X_DIM + x] = p->colorcode[0];
            if(d >= DISTANCE1 && d <= DISTANCE2) 
                p->colors[y * X_DIM + x] = p->colorcode[1];
            if(d > DISTANCE2) 
                p->colors[y * X_DIM + x] = p->colorcode[2];
        }
    }
}

// 플레이어의 colors 단계적으로 on
void showColors(Player p) {
    // keynumber -> x,y 좌표로 변경
    uint8_t mine_x = p.mine % X_DIM;
    uint8_t mine_y = p.mine / X_DIM;

    Serial.println(p.ID + "Boom");
    // 첫번째 영역 on
    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            if(x == mine_x && y == mine_y) continue;                    
            if(distance(mine_x, mine_y, x, y) < DISTANCE1) {
                trellis.setPixelColor(x, y, p.colorcode[0]);
                trellis.show();
            }
        }
    }
    delay(500);

    // 두번째 영역 on
    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            if(x == mine_x && y == mine_y) continue;
            if(distance(mine_x, mine_y, x, y) >= DISTANCE1 && distance(mine_x, mine_y, x, y) <= DISTANCE2) {
                trellis.setPixelColor(x, y, p.colorcode[1]);
                trellis.show();
            }
        }
    }
    delay(500);

    // 세번째 영역 on
    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            if(x == mine_x && y == mine_y) continue;
            if(distance(mine_x, mine_y, x, y) > DISTANCE2) {
                trellis.setPixelColor(x, y, p.colorcode[2]);
                trellis.show();
            }            
        }
    }
    delay(500);
}

// 지뢰 효과
void showMine(uint16_t mine_key) {
    uint8_t i = 0;
    String s = "";
    
    while (1)
    {
        trellis.setPixelColor(mine_key, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255)));
        trellis.show();
        delay(100);
        if(i >= X_DIM*Y_DIM) i = 0;
        else i++;
        // 종료 조건 추가
        
        while(Serial.available()) {
          // 시리얼 읽어서 문자열로 저장
          s = Serial.readString();
        }

//        if(s.substring(0,4) == "Mine") {
//            setColor();
//            break;
//        }

         if(s.substring(0,4) == "Mine") {
             initButtonState();
             turn = "Lock";
             pSingle.mine = atoi(s.substring(4,7).c_str());
             break;
         }
    }

}

// define a callback for key presses in SingleMode
TrellisCallback led_ON(keyEvent evt) {
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
            ispressed[evt.bit.NUM] = 1;
            // 누른 버튼이 지뢰일 경우
            if(evt.bit.NUM == pSingle.mine) { // 지뢰 탐색 성공
                showColors(pSingle);
                showMine(pSingle.mine);
                // 파이썬에 '게임 종료' 전송
            } 
            // 누른 버튼이 지뢰가 아닐 경우
            else {
                    trellis.setPixelColor(evt.bit.NUM, pSingle.colors[evt.bit.NUM]);
                    trellis.show();
                    Serial.println("Click"); // 버튼 클릭시 파이썬에 '버튼 클릭 이벤트 발생' 전송
            }                
        }
    }
    return 0;
}

// define a callback for player1's key presses
TrellisCallback red_ON(keyEvent evt) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
                ispressed[evt.bit.NUM] = 1;
                // 누른 버튼이 지뢰일 경우
                if(evt.bit.NUM == pRed.mine) { // 빨간 플레이어 패배
                    showColors(pRed);
                    showMine(pRed.mine);
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == pBlue.mine) { // 빨간 플레이어 승리
                    showColors(pBlue);
                    showMine(pBlue.mine);
                    // 파이썬에 '게임 종료' 전송 
                }
                // 누른 버튼이 지뢰가 아닐 경우
                else {
                    trellis.setPixelColor(evt.bit.NUM, pRed.colors[evt.bit.NUM]);
                    trellis.show();
                    Serial.println("Click"); // 버튼 클릭시 파이썬에 '버튼 클릭 이벤트 발생' 전송
                }                
            }
        }
    return 0;
}

// define a callback player2's key presses
TrellisCallback blue_ON(keyEvent evt) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) 
        {
            if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
                ispressed[evt.bit.NUM] = 1;
                // 누른 버튼이 지뢰일 경우
                if(evt.bit.NUM == pRed.mine) { // 파란 플레이어 승리
                    showColors(pRed);
                    showMine(pRed.mine);
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == pBlue.mine) { // 파란 플레이어 패배
                    showColors(pBlue);
                    showMine(pBlue.mine);
                    // 파이썬에 '게임 종료' 전송
                }
                // 누른 버튼이 지뢰가 아닐 경우
                else {
                    trellis.setPixelColor(evt.bit.NUM, pBlue.colors[evt.bit.NUM]);
                    trellis.show();
                    Serial.println("Click"); // 버튼 클릭시 파이썬에 '버튼 클릭 이벤트 발생' 전송
                }
            }
        }
    return 0;
}

// define a callback key lock
TrellisCallback lock_ON(keyEvent evt) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            return 0;
        }
    return 0;
}

void setup() 
{
    Serial.begin(115200);
    
    if(!trellis.begin()) {
        while(1);
    }

    initButtonState();
    turn = "Lock";
    
}

void loop() 
{
    while(Serial.available()) {
        // 시리얼 읽어서 문자열로 저장
        Serial.setTimeout(20);
        sig = Serial.readString();
        // Serial.println(sig);
    }
   // 문자열 슬라이싱 (Mode or Mine or Turn)
//    sig.substring(0,4).toCharArray(check,5);        // 문자열 끝은 NULL
   check = sig.substring(0,4);

    // 모드 설정 시리얼을 수신한 경우
    if(check == "Mode")
    {   // 의도하지않은 값 방지
        if (sig.length()==10)
        {
            // 모드 저장
            mode = sig.substring(4,10);

//                // 테스트
//                Serial.print("\t");
//                Serial.print("mode : ");
//                Serial.println(mode);

            if(mode == "Loding") {
                setPlayer(&pSingle, "Single");
                setPlayer(&pRed, "Red");
                setPlayer(&pBlue, "Blue");
                
                // starting effect
                for(int i=0; i<Y_DIM*X_DIM; i++) {
                    trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255)));
                    trellis.show();
                    delay(20);
                }
            
                for(int i=0; i<Y_DIM*X_DIM; i++) {
                    // activate rising edge on all keys
                    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
            
                    // all neopixels off
                    trellis.setPixelColor(i, 0x000000);
                    trellis.show();
            
                    delay(20);
                }                 
            }
             
        }
    }
    // 지뢰 설정 시리얼을 수신한 경우
    else if (check == "Mine")
    {   
        // 의도하지않은 값 방지
        if (sig.length()==10)
        {
            if(mode == "Single") 
            {
//                sig.substring(4,7).toCharArray(solo,4);     // single_mine 부분 슬라이싱
                // int로 변환
                uint16_t single_mine = atoi(sig.substring(4,7).c_str());

                // 테스트
                // Serial.print("single : ");
                // Serial.println(single_mine);
                
                pSingle.mine = single_mine;

                initButtonState();
                // set color array
                setPlayerColors(&pSingle);

                // 초기화
                sig = "";
                check = "";
            }
            else if(mode == "Battle") 
            {
                sig.substring(4,7).toCharArray(red,4);          // red_mine 부분 슬라이싱
                sig.substring(7,10).toCharArray(blue,4);        // blue_mine 부분 슬라이싱
                // int로 변환
                uint16_t red_mine = atoi(red);
                uint16_t blue_mine = atoi(blue);
                
                // 테스트
                //    Serial.print("red : ");
                //    Serial.print(red_mine);
                //    Serial.print("\t");
                //    Serial.print("blue : ");
                //    Serial.println(blue_mine);

                pRed.mine = red_mine;
                pBlue.mine = blue_mine;

                initButtonState();
                // set color array
                setPlayerColors(&pRed);  // 기존 setColor()를 initButtonState()와 setPlayerColors(player) 로 변경
                setPlayerColors(&pBlue);
            }
        }
    }
    // 턴에 대한 정보를 수신한 경우
    else if (check == "Turn")
    {   // 의도하지않은 값 방지
        if (sig.length()==8)
        {
            // turn 부분 슬라이싱 (trunT : 'Solo' or 'Red_' or 'Blue' or 'Lock')
            turn = sig.substring(4,8);
            // 테스트
            // Serial.print("turn : ");
            // Serial.println(turn);
        }
    }
    // 게임진행에 필요없는 시리얼인 경우
    // 초기화
    sig = "";
    check = "";
        
    // register a callback for all keys
    if(mode == "Single") {
        for(int i=0; i<Y_DIM*X_DIM; i++) {
            if(turn == "Solo")
                trellis.registerCallback(i, led_ON);
            else if(turn == "Lock")
                trellis.registerCallback(i, lock_ON);
        }
        trellis.read();
    }
    else if(mode == "Battle") {
        for(int i=0; i<Y_DIM*X_DIM; i++) 
        {
            if (turn == "Red_") 
                trellis.registerCallback(i, red_ON);
            else if(turn == "Blue")
                trellis.registerCallback(i, blue_ON);
            else if(turn == "Lock") {    // key lock
                trellis.registerCallback(i, lock_ON);
            }
        }
        trellis.read();
    }

    delay(20);
}