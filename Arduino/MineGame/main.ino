#include "Adafruit_NeoTrellis.h"
#include "MineGame.h"

#define Y_DIM 8 // number of rows of keys
#define X_DIM 8 // number of columns of keys

// create a matrix of neotrellis boards
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
    { Adafruit_NeoTrellis(0x31) }

    // { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32)},
    // { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E)}

    // { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x33) },
    // { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E), Adafruit_NeoTrellis(0x3F) },
    // { Adafruit_NeoTrellis(0x4A), Adafruit_NeoTrellis(0x4B), Adafruit_NeoTrellis(0x4C) }

};

// pass this matrix to the multitrellis constructor
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

// mine button color
const static uint32_t mine_colors[4] = { 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF };

static uint32_t blue_colors[Y_DIM*X_DIM], red_colors[Y_DIM*X_DIM];
static uint32_t single_colors[Y_DIM*X_DIM];

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

// 모드 저장 변수
static char mode;

// 플레이어, 게임 객체
static Player pRed, pBlue;
static Player pSingle;
static MineGame game;

// mine LED effect
void showMine(Player p) {
    // keynumber -> x,y 좌표로 변경
    uint8_t mine_x = p.mine % X_DIM;
    uint8_t mine_y = p.mine / X_DIM;

    Serial.println(p.ID + "Boom");
    // 첫번째 영역 on
    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            if(x == mine_x && y == mine_y) continue;                    
            if(distance(mine_x, mine_y, x, y) < DISTANCE1) {
                trellis.setPixelColor(x, y, p.color[0]);
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
                trellis.setPixelColor(x, y, p.color[1]);
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
                trellis.setPixelColor(x, y, p.color[2]);
                trellis.show();
            }            
        }
    }
    delay(500);

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

        if(s.substring(0,4) == "Mode") {    // 게임모드 변경 == 새 게임
            initButtonState();
            game.initPlayer(pSingle);
            game.initPlayer(pRed);
            game.initPlayer(pBlue);

            if(game.mode == "single") { // 싱글모드였으면 배틀모드로
                mode = 'B';
                game = new MineGame(pRed, pBlue, Y_DIM, X_DIM);
            }
            else(game.mode == "battle") { // 배틀모드였으면 싱글모드로
                mode = 'S';
                game = new MineGame(pSingle, Y_DIM, X_DIM);
            }
            break;
        }

        if(s.substring(0,4) == "Mine") {    // 같은 게임 재시작
            initButtonState();
            if(game.mode == "single") 
                game.setPlayerColorArray(pSingle, single_colors);
            else if(game.mode == "battle") {
                game.setPlayerColorArray(pRed, red_colors);
                game.setPlayerColorArray(pBlue, blue_colors);
            }
            break;
        }
    }
}

void initButtonState()
{
    for (int i = 0; i < Y_DIM * X_DIM; i++) {
        // all neopixels off
        trellis.setPixelColor(i, 0x000000);
        trellis.show();
        ispressed[i] = 0;
    }
}

// define a callback for single player's key presses
TrellisCallback led_ON(keyEvent evt) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
                ispressed[evt.bit.NUM] = 1;
                if(evt.bit.NUM == pSingle.mine) { // 지뢰 탐색 성공
                    showMine(pSingle);
                    // 파이썬에 '게임 종료' 전송
                } 
                else {
                    trellis.setPixelColor(evt.bit.NUM, single_colors[evt.bit.NUM]);
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
                if(evt.bit.NUM == pRed.mine) { // 빨간 플레이어 패배(본인 지뢰 클릭)
                    showMine(pRed);
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == pBlue.mine) { // 빨간 플레이어 승리
                    showMine(pBlue);
                    // 파이썬에 '게임 종료' 전송
                }
                else {
                    trellis.setPixelColor(evt.bit.NUM, red_colors[evt.bit.NUM]);
                    trellis.show();
                    Serial.println("Click"); // 버튼 클릭시 파이썬에 '버튼 클릭 이벤트 발생' 전송
                }                
            }
        }
    return 0;
}

// define a callback player2's key presses
TrellisCallback blue_ON(keyEvent evt) {
        if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
            if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
                ispressed[evt.bit.NUM] = 1;
                // 누른 버튼이 지뢰일 경우
                if(evt.bit.NUM == pRed.mine) { // 파란 플레이어 승리
                    showMine(pRed);
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == pBlue.mine) { // 파란 플레이어 패배(본인 지뢰 클릭)
                    showMine(pBlue);
                    // 파이썬에 '게임 종료' 전송
                } 
                else {
                    trellis.setPixelColor(evt.bit.NUM, blue_colors[evt.bit.NUM]);
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


void setup() {
    Serial.begin(115200);
    
    if(!trellis.begin()) {
        // Serial.println("failed to begin trellis");
        while(1);
    }

    // initialize button state
    initButtonState();
    
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

void loop() {
    while(Serial.available())
    {
       // 시리얼 읽어서 문자열로 저장
       char wait = Serial.read();
       sig.concat(wait);
    }
    // 문자열 슬라이싱 (Mode or Mine or Turn)
    sig.substring(0,4).toCharArray(check,5);        // 문자열 끝은 NULL
    // 문자열 string으로 저장
    String temp = check;

    // 싱글모드 시리얼 -> 게임모드, 지뢰1개, 턴 정보는 필요하지 않을 듯?
    // 배틀모드 시리얼 -> 게임모드, 지뢰2개, 턴 정보 3 종류(R, B, L)

    // 게임모드 시리얼을 수신한 경우
    // if(temp == "Mode")
    // {

        // 테스트 코드
        String test = "BATTLE"; // 배틀모드로 테스트
        if (test == "SINGLE") {
            mode = 'S';
            game = new MineGame(pSingle, Y_DIM, X_DIM);
        }
        else {
            mode = 'B';
            game = new MineGame(pRed, pBlue, Y_DIM, X_DIM);
        } // end 테스트 코드
        
    // } // end 게임모드 시리얼

   
    // 지뢰 설정 시리얼을 수신한 경우
    if (temp == "Mine")
    {   // 의도하지않은 값 방지
        if(game.mode == "battle") {   // 배틀모드
            if(sig.length()==10) {
                sig.substring(4,7).toCharArray(red,4);          // red_mine 부분 슬라이싱
                sig.substring(7,10).toCharArray(blue,4);        // blue_mine 부분 슬라이싱
                // int로 변환
                pRed.mine = atoi(red);
                pBlue.mine = atoi(blue);

                // 테스트
                // Serial.print("red : ");
                // Serial.print(red_mine);
                // Serial.print("\t");
                // Serial.print("blue : ");
                // Serial.print(blue_mine);

                initButtonState();
                // set color array
                game.setPlayerColorArray(pRed, red_colors);
                game.setPlayerColorArray(pBlue, blue_colors);
            }
            // 초기화
            sig = "";
            temp = "";
        }
        else if(game.mode == "single") {   // 싱글모드
                // mine 받는 코드
                // pRed.mine = 
                pSingle.mine = 8;  // 테스트 임시 지뢰 위치

                initButtonState();
                // set color array
                game.setPlayerColorArray(pSingle, single_colors);
        }
        // 시리얼이 잘못 수신된 경우
        else 
        {
           // 초기화
           sig = "";
           temp = "";
       }
    } // end 지뢰 설정 시리얼

   // 턴에 대한 정보를 수신한 경우
    else if (temp == "Turn")
    {   
        if(game.mode == "battle") {  // 배틀모드
           // 의도하지않은 값 방지
            if (sig.length() == 5)
            {
                sig.substring(4, 5).toCharArray(turnT, 2); // turn 부분 슬라이싱 (trunT : 'R' or 'B')

                // 테스트
                //    Serial.print("turnT : ");
                //    Serial.print(turnT);

                //    // 턴 저장
                //    turn = turnT[0];        // turnT[1] = NULL

                //    // 테스트
                //    Serial.print("\t");
                //    Serial.print("turn : ");
                //    Serial.println(turn);

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
        }
    } // end 턴 정보 시리얼

   // 게임진행에 필요없는 시리얼인 경우
   else
    {
       // 초기화
       sig = "";
       temp = "";
    }

    // register a callback for all keys
    if(game.mode == "single") {
        for(uint8_t i=0; i<Y_DIM*X_DIM; i++) {
            // if(turn == 'S') 
                trellis.registerCallback(i, led_ON);
            // else if(turn == 'L') 
            //     terllis.registerCallback(i, lock_ON);   // 잠글 일이 없을 듯?
        }
        // 탐색시간, 탐색 횟수 모두 남아있는 경우만 read
        trellis.read();
    }
    if(game.mode == "battle") {
        for(uint8_t i=0; i<Y_DIM*X_DIM; i++) {
            if(turn == 'R') 
                trellis.registerCallback(i, red_ON);
            else if(turn == 'B') 
                trellis.registerCallback(i, blue_ON);
            else if(turn == 'L') 
                terllis.registerCallback(i, lock_ON);
        }
        trellis.read();
    }
    
    delay(20);
}