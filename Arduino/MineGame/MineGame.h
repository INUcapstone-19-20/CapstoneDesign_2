#ifndef MINEGAME_H
#define MINEGAME_H

#define ydim 4
#define xdim 4

#define COLORLEVEL 3

#define SINGLE1 0x00FFFF
#define SINGLE2 0x8aFFFF
#define SINGLE3 0xe0FFFF
#define RED1 0xFF0000
#define RED2 0xFF5A5A
#define RED3 0xFFB4B4
#define BLUE1 0x0000FF
#define BLUE2 0x5A5AFF 
#define BLUE3 0xB4B4FF

#define DISTANCE1 sqrt(2)
#define DISTANCE2 sqrt(8)

typedef struct Player {
    String ID;
    uint32_t color[COLORLEVEL];
    uint16_t mine;
} Player;

double distance(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    return abs(sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}
 
class SingleMode {
public: 
    SingleMode();
    ~SingleMode() {};

    void setGame(uint16_t mine);
    void setColorArray(uint32_t *arr);

    void setCallback(void);
    void (TrellisCallback *)led_ON(keyEvent evt);

    void startGame(void);

    void showMine(Player p);

    friend class BattleMode;

public:
    Adafruit_MultiTrellis *_trellis;
    Player player;
    uint8_t ispressed[ydim*xdim];
    uint32_t singlecolors[ydim*xdim];
    uint32_t minecolor[4] = { 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF };
};

class BattleMode : public SingleMode {
public:
    BattleMode();
    ~BattleMode() {};

    void setGame(uint16_t redmine, uint16_t bluemine);
    void setColorArray(Player p, uint32_t *arr);

    void setCallback(char turn);
    void (TrellisCallback *)red_ON(keyEvent evt);
    void (TrellisCallback *)blue_ON(keyEvent evt);

    void startGame(void);

    void showMine(Player p, uint32_t *arr);

public:
    Adafruit_NeoTrellis *_trellis;
    Player playerRed, playerBlue;
    uint32_t redcolors[ydim * xdim];
    uint32_t bluecolors[ydim * xdim];
};

#endif // !MINEGAME_H
