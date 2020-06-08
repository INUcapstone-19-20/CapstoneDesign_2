#include "Adafruit_NeoTrellis.h"

#define Y_DIM 12 // number of rows of keys
#define X_DIM 12 // number of columns of keys
#define COLORS 3  // number of colors

// define color
#define SINGLE1 0x00FF12
#define SINGLE2 0x55FF66
//#define SINGLE3 0x99FFCC
//#define SINGLE3 0x94E594
#define SINGLE3 0x557755
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

    // 12 x 12
     { Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x32), Adafruit_NeoTrellis(0x33) },
     { Adafruit_NeoTrellis(0x3D), Adafruit_NeoTrellis(0x3E), Adafruit_NeoTrellis(0x3F) },
     { Adafruit_NeoTrellis(0x4A), Adafruit_NeoTrellis(0x4B), Adafruit_NeoTrellis(0x4C) }

};

// pass this matrix to the multitrellis constructor
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);
TrellisCallback blue_ON(keyEvent evt);

static uint8_t ispressed[Y_DIM*X_DIM]; // button state. 1 is pressed, 0 is not pressed

// 애니메이션
static int sunglasses[] = {38,39,40,43,44,45,48,49,50,51,52,53,54,55,56,57,58,59,62,63,64,67,68,69};
static int lenSunglasses = sizeof(sunglasses)/sizeof(sunglasses[0]);

static int eye[] = {38,40,43,45,51,56,62,64,67,69};
static int lenEye = sizeof(eye)/sizeof(eye[0]);

static int mouseHappy[] = {86,93,99,104,112,113,114,115};
static int lenMouseHappy = sizeof(mouseHappy)/sizeof(mouseHappy[0]);

static int mouseDepressed[] = {88,89,90,91,99,104,110,117};
static int lenMousDepressede = sizeof(mouseDepressed)/sizeof(mouseDepressed[0]);

static int background[] = {0,1,2,9,10,11,12,13,22,23,24,35,108, 119,120,121,130,131,132,133,134,141,142,143};
static int lenBackground = sizeof(background)/sizeof(background[0]);

// static char fail[Y_DIM*X_DIM] = {
//     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
//     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
//     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
//     'f', 'f', 'f', '0', 'a', '0', 'i', 'i', 'i', 'l', '0', '0', 
//     'f', '0', '0', 'a', '0', 'a', '0', 'i', '0', 'l', '0', '0', 
//     'f', 'f', 'f', 'a', '0', 'a', '0', 'i', '0', 'l', '0', '0', 
//     'f', '0', '0', 'a', 'a', 'a', '0', 'i', '0', 'l', '0', '0', 
//     'f', '0', '0', 'a', '0', 'a', '0', 'i', '0', 'l', '0', '0', 
//     'f', '0', '0', 'a', '0', 'a', 'i', 'i', 'i', 'l', 'l', 'l', 
//     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
//     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
//     '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
// };

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

static int warningDelay = 0;
static boolean isOver = false;

// 테스트용 -> 테스트 후 삭제
int cnt = 10;

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

    // 첫번째 영역 on
    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            if(x == mine_x && y == mine_y) continue;                    
            if(distance(mine_x, mine_y, x, y) < DISTANCE1) {
                if(mode == "Single" && !isOver) {   // 싱글모드에서 지뢰탐색에 성공한 경우 초록색으로 
                    trellis.setPixelColor(x, y, 0x00FF00);
                }
                else {  // 배틀 모드인 경우 & 싱글모드 실패
                    trellis.setPixelColor(x, y, p.colorcode[0]);
                }
            }
        }
    }
    trellis.show();
    delay(500);

    // 두번째 영역 on
    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            if(x == mine_x && y == mine_y) continue;
            if(distance(mine_x, mine_y, x, y) >= DISTANCE1 && distance(mine_x, mine_y, x, y) <= DISTANCE2) {
                if(mode == "Single" && !isOver) {   // 싱글모드에서 지뢰탐색에 성공한 경우 초록색으로 
                    trellis.setPixelColor(x, y, 0x00FF00);
                }
                else {  // 배틀 모드인 경우 & 싱글모드 실패
                    trellis.setPixelColor(x, y, p.colorcode[1]);
                }
            }
        }
    }
    trellis.show();
    delay(500);

    // 세번째 영역 on
    for(uint8_t y=0; y<Y_DIM; y++) {
        for(uint8_t x=0; x<X_DIM; x++) {
            if(x == mine_x && y == mine_y) continue;
            if(distance(mine_x, mine_y, x, y) > DISTANCE2) {
                if(mode == "Single" && !isOver) {   // 싱글모드에서 지뢰탐색에 성공한 경우 초록색으로 
                    trellis.setPixelColor(x, y, 0x00FF00);
                }
                else {  // 배틀 모드인 경우 & 싱글모드 실패
                    trellis.setPixelColor(x, y, p.colorcode[2]);
                }            
            }            
        }
    }
    trellis.show();
    delay(500);
}

// 지뢰 효과
void showMine(Player p) {
    uint8_t i = 0;
    uint8_t interval = 1;
    String s = "";

    Serial.print(p.ID + "Boom");
    while (1)
    {
        if(mode == "Single") {  // 싱글모드
            if(isOver) {    // 지뢰 찾기 실패 -> 여러색으로 지뢰 표시
                trellis.setPixelColor(pSingle.mine, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255)));
                if(i > X_DIM*Y_DIM) i = 0;
                else i++;
            }
            else {  // 지뢰 찾기 성공 -> 초록색 안에서만 지뢰 표시
                trellis.setPixelColor(pSingle.mine, seesaw_NeoPixel::Color(i, 255, i)); // 초록색 사이에서 지뢰 색 변동
                if(i == 255) interval = -1;
                else if(i == 0) interval = 1;
                i += interval;
            }
        }
        else {  // 배틀 모드
            // 레드 -> 빨간색 안에서만 , 블루 -> 파란색 안에서만 지뢰 표시
            trellis.setPixelColor(pRed.mine, seesaw_NeoPixel::Color(255, i, i));  // 레드
            trellis.setPixelColor(pBlue.mine, seesaw_NeoPixel::Color(i, i, 255)); // 블루
            if(i == 255) interval = -1;
            else if(i == 0) interval = 1;
            i += interval;
        }
        trellis.show();
        delay(5);

        while(Serial.available()) {
          // 시리얼 읽어서 문자열로 저장
          s = Serial.readString();
        }

         if(s.substring(0,4) == "Mine") {
             turn = "Lock";
             pSingle.mine = atoi(s.substring(4,7).c_str());
             initButtonState();
             setPlayerColors(&pSingle);
             break;
         }
    }
}

void showFail() {
   for(int i=0; i<Y_DIM*X_DIM; i++)
   {
        // starting effect
        // 눈 부분
        if (isExist(eye, lenEye, i))
    //                    if (isExist(sunglasses, i))
            trellis.setPixelColor(i, 0x9B00F7);

        // 입 부분
        else if (isExist(mouseDepressed, lenMouseDepressed, i))
    //                    if (isExist(mouse, i))
            trellis.setPixelColor(i, 0x9B00F7);

        // 배경 부분
        else if (isExist(background, lenBackground, i))
    //                    else if (isExist(background, i))
            trellis.setPixelColor(i, 0x000000);

        // 이모티콘 얼굴 부분
        else trellis.setPixelColor(i, 0xAAAA00);
        
        trellis.show();
        delay(10);
    }
}

// 배열 속 값 존재여부
int isExist(int a[], int n, int key){
    for(int i = 0; i < n; i++){
        if(a[i]==key){
            return true;
        }
    }
    return false;
}

//void animation(uint32_t sunglassesColor){
//     for(int i=0; i<Y_DIM*X_DIM; i++) 
//     {
//        // starting effect
//        // 선글라스 부분
//        if (isExist(sunglasses, lensunglasses, i))
////                    if (isExist(sunglasses, i))
//            trellis.setPixelColor(i, sunglassesColor);
//
//        // 입 부분
//        else if (isExist(mouse, lenmouse, i))
////                    if (isExist(mouse, i))
//            trellis.setPixelColor(i, 0xFFFFFF);
//
//        // 배경 부분
//        else if (isExist(background, lenBackground, i))
////                    else if (isExist(background, i))
//            trellis.setPixelColor(i, 0x000000);
//
//        // 이모티콘 얼굴 부분
//        else trellis.setPixelColor(i, 0xFFFF00);
//        
//        trellis.show();
//        delay(30);    
//    }   
//}



// 라즈베리파이와의 통신 함수
void communication()
{
    while(Serial.available()) {
    // 시리얼 읽어서 문자열로 저장
    Serial.setTimeout(20);
    sig = Serial.readString();
    // Serial.println(sig);
    }

    // 문자열 슬라이싱 (Mode or Mine or Turn or Dang or Over)
    check = sig.substring(0,4);

    // 모드 설정 시리얼을 수신한 경우
    if(check == "Mode")
    {   // 의도하지않은 값 방지
        if (sig.length()==10)
        {
            // 모드 저장
            mode = sig.substring(4,10);

            if(mode == "Loding") {
                setPlayer(&pSingle, "Single");
                setPlayer(&pRed, "Red");
                setPlayer(&pBlue, "Blue");

                for(int i=0; i<Y_DIM*X_DIM; i++) 
                {
                    // starting effect
                    // 선글라스 부분
                    if (isExist(sunglasses, lenSunglasses, i))
            //                    if (isExist(sunglasses, i))
                        trellis.setPixelColor(i, 0x0085FF);

                    // 입 부분
                    else if (isExist(mouseHappy, lenMouseHappy, i))
            //                    if (isExist(mouse, i))
                        trellis.setPixelColor(i, 0x0085FF);

                    // 배경 부분
                    else if (isExist(background, lenBackground, i))
            //                    else if (isExist(background, i))
                        trellis.setPixelColor(i, 0x000000);

                    // 이모티콘 얼굴 부분
                    else trellis.setPixelColor(i, 0xAAAA00);
                    
                    trellis.show();
                    delay(30);    
                }   
                delay(2500);
                for(int i=0; i<Y_DIM*X_DIM; i++) {
                    // all neopixels off
                    trellis.setPixelColor(i, 0x000000);
                    trellis.show();

                    // activate rising edge on all keys
                    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING, true);
                    delay(10);
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
                
                pRed.mine = red_mine;
                pBlue.mine = blue_mine;

                initButtonState();
                // set color array
                setPlayerColors(&pRed);
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
        }
    }
    // 싱글모드 지뢰탐색횟수, 탐색시간 초과 위기를 수신한 경우
    else if (check == "Warn")
    {
        // 딜레이 시간 같이 보내주세요
        // ex) delay(100) -> Dang100, delay(40) -> Dang040
        // 의도하지않은 값 방지
        if (sig.length()==7)
        {
            warningDelay = atoi(sig.substring(4, 7).c_str());   // 딜레이 시간 저장
        }
    }
    // 싱글모드 게임 실패 신호를 수신한 경우
    else if (check == "Fail") {
        turn = "Lock";
        isOver = true;
        showFail();
        showMine(pSingle);
    }

    // 게임진행에 필요없는 시리얼인 경우
    // 초기화
    sig = "";
    check = "";
}

// define a callback for key presses in SingleMode
TrellisCallback led_ON(keyEvent evt) {
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        if(ispressed[evt.bit.NUM] == 0) { // 눌리지 않은 버튼일 때
            ispressed[evt.bit.NUM] = 1;
            cnt--;
            // 누른 버튼이 지뢰일 경우
            if(evt.bit.NUM == pSingle.mine) { // 지뢰 탐색 성공
                cnt = 10;
                showMine(pSingle);
                // 파이썬에 '게임 종료' 전송
            } 
            // 누른 버튼이 지뢰가 아닐 경우
            else {
                // 테스트 -> 테스트 후 if문 삭제, else 괄호 삭제
                if(cnt == 0) {
                    Serial.println("Click");
                    cnt = 10;
                    isOver = true;
                    showColors(pSingle);
                    showFail();
                    showMine(pSingle);
                    
                } // end 테스트
                else {
                  trellis.setPixelColor(evt.bit.NUM, pSingle.colors[evt.bit.NUM]);
                  trellis.show();
                  Serial.println("Click"); // 버튼 클릭시 파이썬에 '버튼 클릭 이벤트 발생' 전송
                }
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
                    showMine(pRed);
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == pBlue.mine) { // 빨간 플레이어 승리
                    showColors(pBlue);
                    showMine(pBlue);
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
                    showMine(pRed);
                    // 파이썬에 '게임 종료' 전송
                } 
                else if(evt.bit.NUM == pBlue.mine) { // 파란 플레이어 패배
                    showColors(pBlue);
                    showMine(pBlue);
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
    communication();
    
    // register a callback for all keys
    if(mode == "Single") {
        for(int i=0; i<Y_DIM*X_DIM; i++) {
            if(turn == "Solo")
                trellis.registerCallback(i, led_ON);
            else if(turn == "Lock")
                trellis.registerCallback(i, lock_ON);
        }
        if(warningDelay) { // -> danger 수신 했을 때 true
            // led blink
            for(int i=0; i<Y_DIM*X_DIM; i++) {
                trellis.setPixelColor(i, 0x000000); // off
            }
            trellis.show();
            trellis.read();
            delay(warningDelay);
            for(int i=0; i<Y_DIM*X_DIM; i++) {
                if(ispressed[i]) 
                    trellis.setPixelColor(i, pSingle.colors[i]); // on
            }
            trellis.show();
        }
        trellis.read();
    }
    else if(mode == "Battle") 
    {
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
