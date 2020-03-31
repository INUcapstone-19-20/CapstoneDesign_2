#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)
#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)

// 전역변수
//pin number 2~13 : PWM
static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {29, 28, 27, 26};   // SWT-GND 1,2,3,4
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {22, 23, 24, 25};   // SWTICH 1,2,3,4
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] = {30, 31, 32, 33};   // LED-GND 1,2,3,4
static const uint8_t ledrowpins[NUM_LED_ROWS][3]     = {{2, 3, 44}, {5, 6, 7}, {8, 9, 10}, {11, 12, 45}};
                                                //{RED1,GREEN1,BLUE1}, {RED2,GREEN2,BLUE2}, {RED3,GREEN3,BLUE3}, {RED4,GREEN4,BLUE4}
void setup()
{
  uint8_t i;
  Serial.begin(115200);
  
 for(i=0;i<NUM_LED_COLUMNS;i++){
//    pinMode(btncolumnpins[i],OUTPUT);
//    pinMode(btnrowpins[i],OUTPUT);
    pinMode(ledcolumnpins[i],OUTPUT);
    digitalWrite(ledcolumnpins[i], HIGH);
  }
}

void loop() {
  Serial.println("start");
  
  // 각 열의 신호 ON
//  digitalWrite(btncolumnpins[0], LOW);
//  digitalWrite(btncolumnpins[1], LOW);
//  digitalWrite(btncolumnpins[2], LOW);
//  digitalWrite(btncolumnpins[3], LOW);
//
//  digitalWrite(btnrowpins[0], LOW);
//  digitalWrite(btnrowpins[1], LOW);
//  digitalWrite(btnrowpins[2], LOW);
//  digitalWrite(btnrowpins[3], LOW);
  
  digitalWrite(ledcolumnpins[0], LOW);
  digitalWrite(ledcolumnpins[1], LOW);
  digitalWrite(ledcolumnpins[2], LOW);
  digitalWrite(ledcolumnpins[3], LOW);

  // 각 행의 RGB 값 변경
  // 1행
  analogWrite(ledrowpins[0][0], 10);      // RED
  analogWrite(ledrowpins[0][1], 10);      // BLUE
  analogWrite(ledrowpins[0][2], 10);      // GREEN
   // 2행
  analogWrite(ledrowpins[1][0], 10);
  analogWrite(ledrowpins[1][1], 10);
  analogWrite(ledrowpins[1][2], 10);
   // 3행
  analogWrite(ledrowpins[2][0], 10);
  analogWrite(ledrowpins[2][1], 10);
  analogWrite(ledrowpins[2][2], 10);
   // 4행
  analogWrite(ledrowpins[3][0], 10);
  analogWrite(ledrowpins[3][1], 10);
  analogWrite(ledrowpins[3][2], 10);

  Serial.println("end");
}
