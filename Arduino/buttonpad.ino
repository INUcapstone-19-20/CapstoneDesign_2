#include "Adafruit_NeoTrellis.h"

#define Y_DIM 4 //number of rows of keys
#define X_DIM 4 //number of columns of keys

//create a matrix of neotrellis boards
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
  
  { Adafruit_NeoTrellis(0x2E) }
  
};

/*
If you were using a 2x2 array of NeoTrellis boards, the above lines would be:

#define Y_DIM 8 //number of rows of key
#define X_DIM 8 //number of columns of keys

//create a matrix of trellis panels
Adafruit_NeoTrellis t_array[Y_DIM/4][X_DIM/4] = {
  
  { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F) },

  { Adafruit_NeoTrellis(0x30), Adafruit_NeoTrellis(0x31) }
  
};
*/

// pass this matrix to the multitrellis constructor with matrix's size.
Adafruit_MultiTrellis trellis((Adafruit_NeoTrellis *)t_array, Y_DIM/4, X_DIM/4);

// players' colors.
static uint32_t blue_colors[16] = { 0x6363FF, 0x0000FF, 0x0000FF, 0x0000FF,     // - - - -
                                    0x6363FF, 0x0000FF, 0xFFFFFF, 0x0000FF,     // - - * -
                                    0x6363FF, 0x0000FF, 0x0000FF, 0x0000FF,     // - - - -
                                    0xC6C6FF, 0x6363FF, 0x6363FF, 0x6363FF }    // - - - -    ( * is mine) 
static uint32_t red_colors[16];

static uint8_t red_turn, blue turn;

// Input a value 0 to 255 to get a 32-bit color value.
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

//define a callback for key presses
//TrellisCallback blink(keyEvent evt){
//  
//  // Check is the pad pressed?
//  if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING)
//    trellis.setPixelColor(evt.bit.NUM, blue_colors[evt.bit.NUM]); //on rising. evt.bit.NUM is key number.
//    
////  // or is the pad released?
////  else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING)
////    trellis.setPixelColor(evt.bit.NUM, 0); //off falling
//    
//  trellis.show(); 
//  return 0;
//}

//define a callback for player1's key presses
TrellisCallback red_ON(keyEvent evt){
  if(red_turn > 0) {
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
      trellis.setPixelColor(evt.bit.NUM, red_colors[evt.bit.NUM]); //on rising. evt.bit.NUM is key number.
      trellis.show();
      red_turn--;
      if(red_turn == 0){ // 턴 == 0 되면 루프 중단하고 라즈베리or파이썬에 신호 보내는 코드?
        firsttime = 1;
        // 잠시 루프 중단하고 신호 보내기
      }
    }
  }
  return 0;
}

//define a callback for player2's key presses
TrellisCallback blue_ON(keyEvent evt){
  if (blue_turn > 0) {
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
      trellis.setPixelColor(evt.bit.NUM, blue_colors[evt.bit.NUM]); //on rising. evt.bit.NUM is key number.
      trellis.show();
      blue_turn--;
      if(blue_turn == 0){ // 턴 == 0 되면 루프 중단하고 라즈베리or파이썬에 신호 보내는 코드?
        firsttime = 1;
        // 잠시 루프 중단하고 신호 보내기
      }
    }
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  //while(!Serial);

  /* begin(): begin communication with the matrix of neotrellis boards. return true is success. */
  if(!trellis.begin()){ 
    Serial.println("failed to begin trellis");
    while(1);
  }

  // set players' color array. values get from RasberryPi
  for(int i=0; i<Y_DIM*X_DIM; i++){
    blue_colors[i] = 전달값
    red_colors[i] = 전달값
  }

  /* the array can be addressed as x,y or with the key number */
  for(int i=0; i<Y_DIM*X_DIM; i++){
      trellis.setPixelColor(i, Wheel(map(i, 0, X_DIM*Y_DIM, 0, 255))); // Wheel() is return 32-bit RGB color.
      trellis.show();
      delay(50);
  }

  for(int y=0; y<Y_DIM; y++){
    for(int x=0; x<X_DIM; x++){      
      //activate rising and falling edges on all keys
      /* 
       activateKey(x, y, edge, true) : Activate a key by index or number.
       x: column index of the key. column 0 is on the lefthand side of the matrix.
       y: row index of the key. row 0 is at the top of the matrix and the numbers increase downwards.
       edge: the edge that the key triggers an event on.
       true: to enable the key on the passed edge, false: to disable it.
      */
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
      trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);


      /*
       setPixelColor(x, y, color) : set the color of a neopixel at a key by index or number.
       color: the color to set the pixel. This is a 24 bit RGB value.
      */
      trellis.setPixelColor(x, y, 0x000000); //addressed with x,y


      // call show for all connected neotrellis boards to show all neopixels.
      trellis.show(); //show all LEDs
      
      delay(50);
    }
  }

}

void loop() {
  if (firsttime){ // 방금 막 턴이 바뀌었으면 턴 횟수 재설정 필요
    red전달값 > blue전달값 ? red_turn = red전달값 : blue_turn = blue전달값  ;
    firsttime = 0;
  }
  
  // register a callback for all keys
  /* 
   registerCallback(x, y, cb*) : register a callback for a key by index or number.
   cb: the function to be called when an event is detected from the specified key.
  */
  for(int y=0; y<Y_DIM; y++){
    for(int x=0; x<X_DIM; x++){
      if (red_turn > 0) { // 차례가 아니면 -1, 파이썬으로부터 해당 턴의 횟수를 전달 받음.
        trellis.registerCallback(x, y, red_ON);
      }
      else if (blue_turn > 0) {
        trellis.registerCallback(x, y, blue_ON);
      }
    }
  }
      
  /* read(): read all events currently stored in the seesaw fifo and call any callbacks.
   1. Get the number of events currently in the fifo
   2. Read all keyEvents into the passed buffer
   3. call any callbacks associated  with the key. (In this code, blink associated with keys.)
  */
  trellis.read();
  delay(20);
}
