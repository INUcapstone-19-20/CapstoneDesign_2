#include "MineGame.h"

SingleMode::SingleMode(uint16_t mine)
{
    this->_Y_DIM = _rows * 4; // _Y_DIM = number of rows of keys. _rows = number of rows of boards.
    this->_X_DIM = _cols * 4; // _X_DIM = number of columns of keys. _cols = number of cols of boards.
    
    // initialize button state
    for(uint8_t i=0; i<_Y_DIM*_X_DIM; i++)
        ispressed[i] = 0;

    setPlayer(mine);
}

void SingleMode::setPlayer(uint16_t mine) 
{
    player.ID = "single";
    player.color1 = SINGLE1;
    player.color2 = SINGLE2;
    player.color3 = SINGLE3;
    setColorArray(player);
}

void SingleMode::setColorArray(Player p)
{
    // number에서 x, y로 변경
    uint8_t mine_x = p.mine % _X_DIM;
    uint8_t mine_y = p.mine / _X_DIM;
    for (uint8_t y = 0; y < _Y_DIM; y++) {
        for (uint8_t x = 0; x < _X_DIM; x++) {
            double d = distance(mine_x, mine_y, x, y);
            if (d <= DISTANCE1) 
                p.colors[y * _X_DIM + x] = p.color1;
            if (d > DISTANCE1 && DISTANCE2) 
                p.colors[y * _X_DIM + x] = p.color2;
            if (d >= DISTANCE2) 
                p.colors[y * _X_DIM + x] = p.color3;
        }
    }
}

void SingleMode::setCallback()
{
    for (int i = 0; i < _Y_DIM * _X_DIM; i++)
        _trelli.registerCallback(i, led_ON);
}

TrellisCallback SingleMode::led_ON(keyEvent evt)
{
    if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
        Serial.println("Click"); // 버튼 클릭시 Raspberrypi에 전달하기 위해 출력
        if (ispressed[evt.bit.NUM] == 0) {
            if (evt.bit.NUM == player.mine) {
                showMine(player);
            } else {
                ispressed[evt.bit.NUM] = 1;
                _trelli.setPixelColor(evt.bit.NUM, player.colors[evt.bit.NUM]);
                _trelli.show();
            }
            // 파이썬에 '버튼 클릭 이벤트 발생' 전송
        }
    }
    return 0;
}

void SingleMode::startGame()
{
    _trelli.read();
    delay(20);
}

void SingleMode::showMine(Player p)
{
    // number에서 x, y로 변경
    uint8_t mine_x = p.mine % _X_DIM;
    uint8_t mine_y = p.mine / _X_DIM;
    Serial.println("Boom");

    // 첫번째 영역 on
    for (uint8_t y = 0; y < _Y_DIM; y++) {
        for (uint8_t x = 0; x < _X_DIM; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) <= DISTANCE1) {
                _trelli.setPixelColor(x, y, p.colors[y * _X_DIM + x]);
                _trelli.show();
            }
        }
    }
    delay(500);

    // 두번째 영역 on
    for (uint8_t y = 0; y < _Y_DIM; y++) {
        for (uint8_t x = 0; x < _X_DIM; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) > DISTANCE1 && distance(mine_x, mine_y, x, y) < DISTANCE2) {
                _trelli.setPixelColor(x, y, p.colors[y * _X_DIM + x]);
                _trelli.show();
            }
        }
    }
    delay(500);

    // 세번째 영역 on
    for (uint8_t y = 0; y < _Y_DIM; y++) {
        for (uint8_t x = 0; x < _X_DIM; x++) {
            if (x == mine_x && y == mine_y) continue;
            if (distance(mine_x, mine_y, x, y) > DISTANCE2) {
                _trelli.setPixelColor(x, y, p.colors[y * _X_DIM + x]);
                _trelli.show();
            }
        }
    }
    delay(500);

    // 지뢰
    uint8_t i = 0;
    while (1) {
        _trelli.setPixelColor(p.mine, minecolor[i]);
        _trelli.show();
        delay(500);
        if(i > 4) i = 0;
        else i++;
        // 종료 조건 추가
        // 파이썬에 '게임 종료' 전송?
    }
}

BattleMode::BattleMode(uint16_t redmine, uint16_t bluemine)
{
    this->_Y_DIM = _rows * 4; // _Y_DIM = number of rows of keys. _rows = number of rows of boards.
    this->_X_DIM = _cols * 4; // _X_DIM = number of columns of keys. _cols = number of cols of boards.
    
    // initialize button state
    for(uint8_t i=0; i<_Y_DIM*_X_DIM; i++)
        ispressed[i] = 0;

    setPlayer(redmine, bluemine);
}

void BattleMode::setPlayer(uint16_t redmine, uint16_t bluemine)
{
    // Player1
    playerRed.ID = "RED";
    playerRed.color1 = RED1;
    playerRed.color2 = RED2;
    playerRed.color3 = RED3;
    playerRed.mine = redmine;
    setColorArray(playerRed);

    // Player2
    playerBlue.ID = "BLUE";
    playerBlue.color1 = BLUE1;
    playerBlue.color2 = BLUE2;
    playerBlue.color3 = BLUE3;
    playerBlue.mine = bluemine;
    setColorArray(playerBlue);
}

void BattleMode::setCallback(char turn)
{
    for (int i = 0; i < _Y_DIM * _X_DIM; i++) {
        if (turn == 'R')
            _trelli.registerCallback(i, red_ON);
        else if (turn == 'B')
            _trelli.registerCallback(i, blue_ON);
    }
}

TrellisCallback BattleMode::red_ON(keyEvent evt)
{
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
                _trelli.setPixelColor(evt.bit.NUM, playerRed.colors[evt.bit.NUM]);
                _trelli.show();
            }
            // 파이썬에 '버튼 클릭 이벤트 발생' 전송
        }
    }
    return 0;
}

TrellisCallback blue_ON(keyEvent evt)
{
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
                _trelli.setPixelColor(evt.bit.NUM, playerBlue.colors[evt.bit.NUM]);
                _trelli.show();
            }
            // 파이썬에 '버튼 클릭 이벤트 발생' 전송
        }
    }
    return 0;
}
