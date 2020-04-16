
#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)
#define NUM_COLORS (3)


static uint8_t LED_outputs[NUM_LED_COLUMNS][NUM_LED_ROWS] =
{
  {0,1,2,3},
  {3,0,1,2},
  {2,3,0,1},
  {1,2,3,0}
};

static const uint8_t ledselpins[4]   = {30,31,32,33};
static const uint8_t colorpins[4][3] = {{2,3,44}, {5,6,7},{8,9,10},{11,12,45}};

void setup() {
  // put your setup code here, to run once:
    uint8_t i;

    for(i = 0; i < NUM_LED_COLUMNS; i++)
    {
        pinMode(ledselpins[i], OUTPUT);
        digitalWrite(ledselpins[i], HIGH);
    }


}

void scan() {
  // put your main code here, to run repeatedly:

  static uint8_t current = 0;
  uint8_t val=1;
  uint8_t i, j;

    //run
    digitalWrite(ledselpins[current], LOW);

    for(i = 0; i < NUM_LED_ROWS; i++)
    {
        uint8_t val = (LED_outputs[current][i] & 0x03);


        if(val)
        {
//            digitalWrite(colorpins[i][val-1], HIGH);
            analogWrite(colorpins[i][val-1], 200);
            delayMicroseconds(1);
//            delay(1);
            analogWrite(colorpins[i][val-1], 0);
        }
    }
  
  digitalWrite(ledselpins[current], HIGH);

  current++;
  if (current >= NUM_BTN_COLUMNS)
  {
    current = 0;
  }

}

void loop() {

  scan();
  
}
