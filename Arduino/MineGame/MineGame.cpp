#include "MineGame.h"

SingleMode::SingleMode()
{
    this->_trellis = (Adafruit_NeoTrellis *)Adafruit_MultiTrellis::_trelli;
}

void SingleMode::setGame(uint16_t mine)
{
    // initialize button state
    for(uint8_t i=0; i<ydim*xdim; i++)
        this->ispressed[i] = 0;

    // set player
    this->player.ID = "single";
    this->player.color[0] = SINGLE1;
    this->player.color[1] = SINGLE2;
    this->player.color[2] = SINGLE3;
    this->player.mine = mine;

    setColorArray(this->singlecolors);
}

void SingleMode::setColorArray(uint32_t *arr)
{
    // number에서 x, y로 변경
    uint8_t mine_x = this->player.mine % xdim;
    uint8_t mine_y = this->player.mine / xdim;
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            double d = distance(mine_x, mine_y, x, y);
            uint32_t keynum = y * xdim + x;
            if (d <= DISTANCE1) 
                arr[keynum] = SINGLE1;
            if (d > DISTANCE1 && DISTANCE2) 
                arr[keynum] = SINGLE2;
            if (d >= DISTANCE2) 
                arr[keynum] = SINGLE3;
        }
    }
}

void SingleMode::setCallback(void)
{
    for (int i = 0; i < ydim * xdim; i++)
        this->_trellis.registerCallback(i, led_ON);
}

void SingleMode::(TrellisCallback *)led_ON(keyEvent evt)
{
    Adafruit_NeoTrellis *_t = this->_trellis;

    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        Serial.println("Click"); // 버튼 클릭시 Raspberrypi에 전달하기 위해 출력
        if (ispressed[evt.bit.NUM] == 0) {
            if (evt.bit.NUM == player.mine) {
                showMine(player);
            } else {
                ispressed[evt.bit.NUM] = 1;
                _t.show(); 
            }
            // 파이썬에 '버튼 클릭 이벤트 발생' 전송
        }
    }
    return 0;
}

void SingleMode::startGame(void)
{
    // '제한시간 안끝남 && 탐색횟수 남음' 일 때만 read
    this->_trellis.read();
    delay(20);
}

void SingleMode::showMine(Player p)
{
    Adafruit_NeoTrellis *_t = this->_trellis;

    // number에서 x, y로 변경
    uint8_t mine_x = p.mine % xdim;
    uint8_t mine_y = p.mine / xdim;
    Serial.println("Boom");

    // 첫번째 영역 on
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) <= DISTANCE1) {
                _t.show();
            }
        }
    }
    delay(500);

    // 두번째 영역 on
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) > DISTANCE1 && distance(mine_x, mine_y, x, y) < DISTANCE2) {
                _t.show();
            }
        }
    }
    delay(500);

    // 세번째 영역 on
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) > DISTANCE2) {
                _t.show();
            }
        }
    }
    delay(500);

    // 지뢰
    uint8_t i = 0;
    while (1) {
        _t.setPixelColor(p.mine, minecolor[i]);
        _t.show();
        delay(500);
        if(i > 4) i = 0;
        else i++;
        // 종료 조건 추가
        // 파이썬에 '게임 종료' 전송?
    }
}

BattleMode::BattleMode()
{
    this->_trellis = (Adafruit_NeoTrellis *)Adafruit_MultiTrellis::_trelli;
}

void BattleMode::setGame(uint16_t redmine, uint16_t bluemine)
{
    // initialize button state
    for(uint8_t i=0; i<ydim*xdim; i++)
        this->ispressed[i] = 0;

    // set Player1
    this->playerRed.ID = "RED";
    this->playerRed.color[0] = RED1;
    this->playerRed.color[1] = RED2;
    this->playerRed.color[2] = RED3;
    this->playerRed.mine = redmine;

    // setPlayer2
    this->playerBlue.ID = "BLUE";
    this->playerBlue.color[0] = BLUE1;
    this->playerBlue.color[1] = BLUE2;
    this->playerBlue.color[2] = BLUE3;
    this->playerBlue.mine = bluemine;

    setColorArray(this->playerRed, this->redcolors);
    setColorArray(this->playerBlue, this->bluecolors);
}

void BattleMode::setColorArray(Player p, uint32_t *arr)
{
    // number에서 x, y로 변경
    uint8_t mine_x = p.mine % xdim;
    uint8_t mine_y = p.mine / xdim;
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            double d = distance(mine_x, mine_y, x, y);
            uint8_t keynum = y * xdim + x;
            if (d <= DISTANCE1) 
                arr[keynum] = p.color[0];
            if (d > DISTANCE1 && DISTANCE2) 
                arr[keynum] = p.color[1];
            if (d >= DISTANCE2) 
                arr[keynum] = p.color[2];
            
        }
    }
}

void setCallback(char turn)
{
    for (int i = 0; i < ydim * xdim; i++) {
        if (turn == 'R')
            this->_trellis.registerCallback(i, red_ON);
        else if (turn == 'B')
            this->_trellis.registerCallback(i, blue_ON);
    }
}

void BattleMode::(TrellisCallback *)red_ON(keyEvent evt)
{
    Adafruit_NeoTrellis *_t = this->_trellis;

    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        Serial.println("Click"); // 버튼 클릭시 Raspberrypi에 전달하기 위해 출력
        if (ispressed[evt.bit.NUM] == 0) {
            if (evt.bit.NUM == playerRed.mine) {
                showMine(playerRed);
                // 파이썬에 'blue 승리' 전송
            } else if (evt.bit.NUM == playerBlue.mine) {
                showMine(playerBlue);
                // 파이썬에 'red 승리' 전송
            } else {
                ispressed[evt.bit.NUM] = 1;
                _t.setPixelColor(evt.bit.NUM, redcolors[evt.bit.NUM]);
                _t.show();
            }
            // 파이썬에 '버튼 클릭 이벤트 발생' 전송
        }
    }
    return 0;
}

void BattleMode::(TrellisCallback *)blue_ON(keyEvent evt)
{
    Adafruit_NeoTrellis *_t = this->_trellis;

    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        Serial.println("Click"); // 버튼 클릭시 Raspberrypi에 전달하기 위해 출력
        if (ispressed[evt.bit.NUM] == 0) {
            if (evt.bit.NUM == playerBlue.mine) {
                showMine(playerBlue); // blue 패배
                // 파이썬에 'red 승리' 전송
            } else if (evt.bit.NUM == playerRed.mine) {
                showMine(playerRed); // red 패배
                // 파이썬에 'blue 승리' 전송
            } else {
                ispressed[evt.bit.NUM] = 1;
                _t.setPixelColor(evt.bit.NUM, bluecolors[evt.bit.NUM]);
                _t.show();
            }
            // 파이썬에 '버튼 클릭 이벤트 발생' 전송
        }
    }
    return 0;
}

void BattleMode::startGame(void)
{
    this->_trellis.read();
    delay(20);
}

void BattleMode::showMine(Player p, uint32_t *arr)
{
    Adafruit_NeoTrellis *_t = this->_trellis;

    // number에서 x, y로 변경
    uint8_t mine_x = p.mine % xdim;
    uint8_t mine_y = p.mine / xdim;
    Serial.println("Boom");

    // 첫번째 영역 on
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) <= DISTANCE1) {
                _t.setPixelColor(x, y, arr[y * xdim + x]);
                _t.show();
            }
        }
    }
    delay(500);

    // 두번째 영역 on
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) > DISTANCE1 && distance(mine_x, mine_y, x, y) < DISTANCE2) {
                _t.setPixelColor(x, y, arr[y * xdim + x]);
                _t.show();
            }
        }
    }
    delay(500);

    // 세번째 영역 on
    for (uint8_t y = 0; y < ydim; y++) {
        for (uint8_t x = 0; x < xdim; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) > DISTANCE2) {
                _t.setPixelColor(x, y, arr[y * xdim + x]);
                _t.show();
            }
        }
    }
    delay(500);

    // 지뢰
    uint8_t i = 0;
    while (1) {
        _t.setPixelColor(p.mine, minecolor[i]);
        _t.show();
        delay(500);
        if(i > 4) i = 0;
        else i++;
        // 종료 조건 추가
        // 파이썬에 '게임 종료' 전송
    }
}
