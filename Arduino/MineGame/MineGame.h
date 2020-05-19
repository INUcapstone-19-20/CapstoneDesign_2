#ifndef MINEGAME_H
#define MINEGAME_H

#include "Adafruit_NeoTrellis.h"

#define COLORLEVEL 3

enum COLOR {
    SINGLE1 = 0x00FFFF,
    SINGLE2 = 0x8aFFFF,
    SINGLE3 = 0xe0FFFF,
    RED1 = 0xFF0000,
    RED2 = 0xFF5A5A,
    RED3 = 0xFFB4B4,
    BLUE1 = 0x0000FF,
    BLUE2 = 0x5A5AFF, 
    BLUE3 = 0xB4B4FF
};

enum DISTANCE {
    DISTANCE1 = sqrt(2),
    DISTANCE2 = sqrt(8)
}

typedef struct Player {
    String ID;
    COLOR color1, color2, color3;
    uint16_t mine;
    uint32_t colors[(NEO_TRELLIS_NUM_ROWS * _rows) * (NEO_TRELLIS_NUM_ROWS * _cols)];
} Player;

double distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

class SingleMode : public Adafruit_MultiTrellis {
public:
    SingleMode(uint16_t mine);
    ~SingleMode() {};

    void setPlayer(uint16_t mine);
    void setColorArray(Player p);

    void setCallback();
    TrellisCallback led_ON(keyEvent evt);

    void startGame();

    void showMine(Player p);

protected:
    uint8_t _Y_DIM, _X_DIM;
    Player player;
    uint8_t ispressed[(NEO_TRELLIS_NUM_ROWS * _rows) * (NEO_TRELLIS_NUM_ROWS * _cols)]; // button state. 1 is pressed, 0 is not pressed
    uint32_t minecolor[4] = { 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF };
}

class BattleMode : public SingleMode {
public:
    BattleMode(uint16_t redmine, uint16_t bluemine);
    ~BattleMode() {};

    void setPlayer(uint16_t redmine, uint16_t bluemine);

    void setCallback(char turn);
    TrellisCallback red_ON(keyEvent evt);
    TrellisCallback blue_ON(keyEvent evt);

protected:
    Player playerRed, playerBlue;
};

#endif // !MINEGAME_H
