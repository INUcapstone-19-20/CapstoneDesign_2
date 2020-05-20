#ifndef MINEGAME_H
#define MINEGAME_H

// #define ydim 4
// #define xdim 4

#define COLORLEVEL 3

#define SINGLE1 0x00FFFF
#define SINGLE2 0x8aFFFF
#define SINGLE3 0xe0FFFF
#define RED1 0x990000
#define RED2 0xDD1111
#define RED3 0x772222
#define BLUE1 0x0000FF
#define BLUE2 0x2222DD
#define BLUE3 0x333355

#define DISTANCE1 sqrt(8)
#define DISTANCE2 sqrt(20)

typedef struct Player {
    String ID;
    uint32_t color[COLORLEVEL];
    uint8_t mine;
} Player;

double distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return seesaw_NeoPixel::Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return seesaw_NeoPixel::Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return seesaw_NeoPixel::Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    return 0;
}

class MineGame {
public:
    MineGame(Player p, uint8_t y_dim, uint8_t x_dim);
    MineGame(Player pR, Player pB, uint8_t y_dim, uint8_t x_dim);
    ~MineGame() {};
    
    void setPlayer(Player p);
    void setPlayer(Player pR, Player pB);

    void setPlayerColorArray(Player p, uint32_t *arr);

    void initPlayer(Player p);

protected:
    uint8_t _y_dim, _x_dim;
    String mode;
};
 
// class SingleMode {
// public: 
//     SingleMode();
//     ~SingleMode() {};

//     void setGame(uint16_t mine);
//     void setColorArray(uint32_t *arr);

//     void setCallback(void);
//     void (TrellisCallback *)led_ON(keyEvent evt);

//     void startGame(void);

//     void showMine(Player p);

//     friend class BattleMode;

// public:
//     Adafruit_MultiTrellis *_trellis;
//     Player player;
//     uint8_t ispressed[ydim*xdim];
//     uint32_t singlecolors[ydim*xdim];
//     uint32_t minecolor[4] = { 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF };
// };

// class BattleMode : public SingleMode {
// public:
//     BattleMode();
//     ~BattleMode() {};

//     void setGame(uint16_t redmine, uint16_t bluemine);
//     void setColorArray(Player p, uint32_t *arr);

//     void setCallback(char turn);
//     void (TrellisCallback *)red_ON(keyEvent evt);
//     void (TrellisCallback *)blue_ON(keyEvent evt);

//     void startGame(void);

//     void showMine(Player p, uint32_t *arr);

// public:
//     Adafruit_NeoTrellis *_trellis;
//     Player playerRed, playerBlue;
//     uint32_t redcolors[ydim * xdim];
//     uint32_t bluecolors[ydim * xdim];
// };

#endif // !MINEGAME_H
