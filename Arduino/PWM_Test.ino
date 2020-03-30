#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)


// 전역변수
static bool LED_buffer[NUM_LED_COLUMNS][NUM_LED_ROWS];

static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {29, 28, 27, 26};
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {22, 23, 24, 25};
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] = {30, 31, 32, 33};
static const uint8_t ledrowpins[NUM_LED_ROWS][3]     = {{2, 3, 44}, {5, 6, 7}, {8, 9, 10}, {11, 12, 45}};

void setup()
{
  uint8_t i;
  Serial.begin(115200);
  
  for(i=0;i<NUM_LED_COLUMNS;i++){
    pinMode(ledcolumnpins[i],OUTPUT);
    pinMode(btncolumnpins[i],OUTPUT);
    pinMode(btnrowpins[i],OUTPUT);
    digitalWrite(ledcolumnpins[i], LOW);
  }
}

void loop() {
  Serial.println("start");
  
  analogWrite(ledrowpins[0][0], 10);
  analogWrite(ledrowpins[0][1], 10);
  analogWrite(ledrowpins[0][2], 10);
  
  analogWrite(ledrowpins[1][0], 10);
  analogWrite(ledrowpins[1][1], 10);
  analogWrite(ledrowpins[1][2], 10);
  
  analogWrite(ledrowpins[2][0], 10);
  analogWrite(ledrowpins[2][1], 10);
  analogWrite(ledrowpins[2][2], 10);
  
  analogWrite(ledrowpins[3][0], 10);
  analogWrite(ledrowpins[3][1], 10);
  analogWrite(ledrowpins[3][2], 10);

  Serial.println("end");
}
