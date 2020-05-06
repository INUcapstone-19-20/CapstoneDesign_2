#include "Adafruit_NeoTrellis.h"

#define Y_DIM 4 // number of rows of keys
#define X_DIM 4 // number of columns of keys

// create a matrix of neotrellis boards
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {

    { Adafruit_NeoTrellis(0x2E) }

};

// pass this matrix to the multitrellis constructor
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

// mine button color
const static uint32_t mine_colors[3] = { 0xFF0000, 0x00FF00, 0x0000FF };

// set players' color
static uint32_t blue_colors[16] = { 0x6363FF, 0x0000FF, 0x0000FF, 0x0000FF,     // - - - -
                                    0x6363FF, 0x0000FF, 0xFFFFFF, 0x0000FF,     // - - * -
                                    0x6363FF, 0x0000FF, 0x0000FF, 0x0000FF,     // - - - -
                                    0xC6C6FF, 0x6363FF, 0x6363FF, 0x6363FF };   // - - - -    ( * is mine) 

static uint32_t red_colors[16] = { 0xFFC6C6, 0xFFC6C6, 0xFFC6C6, 0xFFC6C6,      // - - - -
                                    0xFF6363, 0xFF6363, 0xFF6363, 0xFFC6C6,     // - - - -
                                    0xFF0000, 0xFF0000, 0xFF0000, 0xFF6363,     // - - - -
                                    0xFF0000, 0xFFFFFF, 0xFF0000, 0xFF6363 };   // - * - -    ( * is mine) 

static uint8_t red_mine, blue_mine; // location of mine

static uint8_t red_turn, blue_turn; // number of turns
static boolean firstturn;

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
    return abs(sqrt(pow(x1 - x2) + pow(y1 - y2)));
}

// mine LED effect
void showMine(uint16_t mine_key, String color) {
    // keynumber -> x,y 좌표로 변경
    uint8_t mine_x = mine_key / Y_DIM;
    uint8_t mine_y = mine_key % Y_DIM;

    if(color == "red") {
        // 첫번째 영역 on
        for(uint8_t x=0; x<X_DIM; x++) {
            for(uint8_t y=0; y<Y_DIM; y++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) <= sqrt(2)) {
                    trellis.setPixelColor(x, y, red_colors[x * Y_DIM + y]);
                    trellis.show();
                }
            }
        }
        delay(50);

        // 두번째 영역 on
        for(uint8_t x=0; x<X_DIM; x++) {
            for(uint8_t y=0; y<Y_DIM; y++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) >= 2 && distance(mine_x, mine_y, x, y) <= sqrt(5)) {
                    trellis.setPixelColor(x, y, red_colors[x * Y_DIM + y]);
                    trellis.show();
                }
            }
        }
        delay(50);

        // 세번째 영역 on
        for(uint8_t x=0; x<X_DIM; x++) {
            for(uint8_t y=0; y<Y_DIM; y++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) >= 3) {
                    trellis.setPixelColor(x, y, red_colors[x * Y_DIM + y]);
                    trellis.show();
                }            
            }
        }
        delay(50);
    }
    else { // color == "blue"
        // 첫번째 영역 on
        for(uint8_t x=0; x<X_DIM; x++) {
            for(uint8_t y=0; y<Y_DIM; y++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) <= sqrt(2)) {
                    trellis.setPixelColor(x, y, blue_colors[x * Y_DIM + y]);
                    trellis.show();
                }
            }
        }
        delay(50);

        // 두번째 영역 on
        for(uint8_t x=0; x<X_DIM; x++) {
            for(uint8_t y=0; y<Y_DIM; y++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) >= 2 && distance(mine_x, mine_y, x, y) <= sqrt(5)) {
                    trellis.setPixelColor(x, y, blue_colors[x * Y_DIM + y]);
                    trellis.show();
                }
            }
        }
        delay(50);

        // 세번째 영역 on
        for(uint8_t x=0; x<X_DIM; x++) {
            for(uint8_t y=0; y<Y_DIM; y++) {
                if(x == mine_x && y == mine_y) continue;
                if(distance(mine_x, mine_y, x, y) >= 3) {
                    trellis.setPixelColor(x, y, blue_colors[x * Y_DIM + y]);
                    trellis.show();
                }
            }
        }
        delay(50);
    }

    // 지뢰 효과
    uint8_t i = 0;
    while (1)
    {
        trellis.setPixelColor(mine_key, mine_colors[i]);
        trellis.show();
        if(i > 2) i = 0;
        else i++;
        // 종료 조건 추가
    }
}

// define a callback for player1's key presses
TrellisCallback red_ON(keyEvent evt) {
    if(red_turn > 0) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            // pressed key is mine
            if(evt.bit.NUM == red_mine) { // 빨간 플레이어 패배(본인 지뢰 클릭)
                showMine(evt.bit.NUM, "red");
                // 파이썬에 게임 종료 메시지 전송
            } 
            else if(evt.bit.NUM == blue_mine) { // 빨간 플레이어 승리
                showMine(evt.bit.NUM, "blue");
                // 파이썬에 게임 종료 메시지 전송
            }
            else {
                trellis.setPixelColor(evt.bit.NUM, red_colors[evt.bit.NUM]);
                trellis.show();
                red_turn--;
                if(red_turn == 0) {
                    firstturn = !firstturn; // toggle turn state
                    // loop 중단
                    // 파이썬에 다음 턴에 대한 데이터 요청
                }
            }
        }
    }
    return 0;
}

// define a callback player2's key presses
TrellisCallback blue_ON(keyEvent evt) {
    if(blue_turn > 0) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            // pressed key is mine
            if(evt.bit.NUM == red_mine) { // 파란 플레이어 승리
                showMine(evt.bit.NUM, "red");
                // 파이썬에 게임 종료 메시지 전송
            } 
            else if(evt.bit.NUM == blue_mine) { // 파란 플레이어 패배(본인 지뢰 클릭)
                showMine(evt.bit.NUM, "blue");
                // 파이썬에 게임 종료 메시지 전송
            }
            else {
                trellis.setPixelColor(evt.bit.NUM, blue_colors[evt.bit.NUM]);
                trellis.show();
                blue_turn--;
                if(blue_turn == 0) {
                    firstturn = !firstturn; // toggle turn state
                    // loop 중단
                    // 파이썬에 다음 턴에 대한 데이터 요청
                }
            }
        }
    }
    return 0;
}

void setup() {
    Serial.begin(115200);

    if(!trellis.begin()) {
        Serial.println("failed to begin trellis");
        while(1);
    }

    // set colors and location of mine
    // for(int i=0; i<Y_DIM*X_DIM; i++) {
    //     blue_colors[i] =
    //     red_colors[i] =
    // }
    // blue_mine =
    // red_mine =
    blue_mine = 13;
    red_mine = 6;
    
    // set turn state
    firstturn = true;
    
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

// 테스트용 임시 변수
uint8_t r_turns[6] = {2, 3, 1, 2, 0, 1};
uint8_t b_turns[6] = {3, 2, 2, 1, 1, 2};
uint8_t it = 0;

void loop() {
    if(firstturn) { // 턴이 바뀌면 해당 플레이어의 턴 횟수 설정
        //red전달값 > blue전달값 ? red_turn = red전달값 : blue_turn = blue전달값;
        r_turns[it] > b_turns[it] ? red_turn = r_turns[it] : blue_turn = b_turns[it];
        firstturn = !firstturn; // toggle turn state
        it++;
    }
    
    // register a callback for all keys
    for(int i=0; i<Y_DIM*X_DIM; i++) {
        if(red_turn > 0) { // 차례가 아니면 0
            trellis.registerCallback(i, red_ON);
        }
        else if(blue_turn > 0) {
            trellis.registerCallback(i, blue_ON);
        }
    }

    trellis.read();
    delay(20);
}
