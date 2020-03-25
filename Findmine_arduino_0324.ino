#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)
#define NUM_COLORS (1)


// Global variables
static bool LED_buffer[NUM_LED_COLUMNS][NUM_LED_ROWS];

static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {29, 28, 27, 26};
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {22, 23, 24, 25};
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] = {30, 31, 32, 33};
static const uint8_t colorpins[NUM_LED_ROWS][3]     = {{2, 3, 44}, {5, 6, 7}, {8, 9, 10}, {11, 12, 45}};

static int16_t temp = 0;
static bool br_up = true;

void setup()
{
  uint8_t i;
  Serial.begin(115200);
  
  for(i=0;i<NUM_LED_COLUMNS;i++){
    pinMode(ledcolumnpins[i],OUTPUT);
    pinMode(btncolumnpins[i],OUTPUT);
    pinMode(btnrowpins[i],OUTPUT);
    for(uint8_t j;j<3;j++){
      pinMode(colorpins[i][j],OUTPUT);
    }
    digitalWrite(ledcolumnpins[i], HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
//  digitalWrite(btncolumnpins[0], LOW);
  digitalWrite(ledcolumnpins[0], LOW);
  digitalWrite(ledcolumnpins[1], LOW);
  digitalWrite(ledcolumnpins[2], LOW);
  digitalWrite(ledcolumnpins[3], LOW);
//  digitalWrite(btnrowpins[0], LOW);

//  digitalWrite(colorpins[0],HIGH);
//  digitalWrite(colorpins[1],HIGH);
//  digitalWrite(colorpins[2],LOW);
//  digitalWrite(colorpins[3],HIGH);
//  for(uint8_t i=0;i<NUM_LED_COLUMNS;i++){
////    analogWrite(colorpins[2], val);
//  }
  Serial.println("first");
  for(uint8_t i=0;i<NUM_LED_COLUMNS;i++){
    analogWrite(colorpins[i][0], 255-temp);
    analogWrite(colorpins[i][1], temp);
    
  }
  delay(10);
  if(br_up){
    temp += 1;
    if(temp >= 255)
      br_up = false;
  }
  else{
    temp -= 1;
    if(temp <= 0)
      br_up = true;
  }
  
  Serial.println(temp);
//  analogWrite(colorpins[0], 10);
//  analogWrite(colorpins[1], 90);
//  analogWrite(colorpins[2], 150);
//  analogWrite(colorpins[3], 255);
  
}
