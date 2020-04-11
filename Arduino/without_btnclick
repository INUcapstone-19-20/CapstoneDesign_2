// config variables
#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_COLORS (3)

// Global variables
static uint8_t LED_outputsR[NUM_LED_COLUMNS][NUM_LED_ROWS], LED_outputsB[NUM_LED_COLUMNS][NUM_LED_ROWS], LED_outputsG[NUM_LED_COLUMNS][NUM_LED_ROWS];
static int32_t next_scanR, next_scanB, next_scanG;
static uint8_t led_indexR, led_indexB, led_indexG;

static const uint8_t ledcolumnpins[4] = {30, 31, 32, 33};

// RGB pins for each of 4 rows
static const uint8_t colorpins[4][3] = {{2, 3, 4}, {5, 6, 7}, {8, 9, 10}, {11, 12, 13}};

static uint8_t flag = 0;

static void setuppins()
{
  uint8_t i, j;

  // initialize output pins
  
  // led column lines
  for(i = 0; i < NUM_LED_COLUMNS; i++)
  {
    pinMode(ledcolumnpins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(ledcolumnpins[i], HIGH);
  }
}

static void scan()
{
  static uint8_t currentR = 0, currentB = 0, currentG = 0;
  uint8_t val;
  uint8_t i, j;
  
  // color1
  digitalWrite(ledcolumnpins[currentR], LOW);
  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    if (LED_outputsR[i][currentR])
    {
      analogWrite(colorpins[i][0], 50);
      analogWrite(colorpins[i][1], 0);
      analogWrite(colorpins[i][2], 0);
    }
  }
  delay(1);
  digitalWrite(ledcolumnpins[currentR], HIGH);
  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    analogWrite(colorpins[i][0], 0);
  }
  currentR++;
  if (currentR >= NUM_LED_COLUMNS)
  {
    currentR = 0;
  }

  

  // color2
  digitalWrite(ledcolumnpins[currentB], LOW);
  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    if (LED_outputsB[i][currentB])
    {
      analogWrite(colorpins[i][0], 0);
      analogWrite(colorpins[i][1], 50);
      analogWrite(colorpins[i][2], 0);
    }
  }
  delay(1);
  digitalWrite(ledcolumnpins[currentB], HIGH);
  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    analogWrite(colorpins[i][1], 0);
  }
  currentB++;
  if (currentB >= NUM_LED_COLUMNS)
  {
    currentB = 0;
  }



  // color3
  digitalWrite(ledcolumnpins[currentG], LOW);
  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    if (LED_outputsG[i][currentG])
    {
      analogWrite(colorpins[i][0], 0);
      analogWrite(colorpins[i][1], 0);
      analogWrite(colorpins[i][2], 50);
    }
  }
  delay(1);
  digitalWrite(ledcolumnpins[currentG], HIGH);
  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    analogWrite(colorpins[i][2], 0);
  }
  currentG++;
  if (currentG >= NUM_LED_COLUMNS)
  {
    currentG = 0;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting Setup...");

  // setup hardware
  setuppins();

  // init global variables
  next_scanR = millis() + 1000;
  next_scanB = millis() + 2000;
  next_scanG = millis() + 3000;
  led_indexR = 0;
  led_indexB = 0;
  led_indexG = 0;

  for(uint8_t i = 0; i < NUM_LED_COLUMNS; i++)
  {
    for(uint8_t j = 0; j < NUM_LED_ROWS; j++)
    {
      LED_outputsR[i][j] = 0;
      LED_outputsB[i][j] = 0;
      LED_outputsG[i][j] = 0;
    }
  }
  // Set the first led in the outputs on
  LED_outputsR[0][0] = true;
  LED_outputsB[0][0] = true;
  LED_outputsG[0][0] = true;

  Serial.println("Setup Complete.");
}

void loop()
{
  scan();
  
  if(millis() >= next_scanR)
  {
    next_scanR = millis() + 1000;

    LED_outputsR[led_indexR/NUM_LED_COLUMNS][led_indexR%NUM_LED_COLUMNS] = 0;
    led_indexR++;
    led_indexR %= (NUM_LED_COLUMNS * NUM_LED_ROWS);
    LED_outputsR[led_indexR/NUM_LED_COLUMNS][led_indexR%NUM_LED_COLUMNS] = 1;
  }

  if(millis() >= next_scanB)
  {
    next_scanB = millis() + 1000;
    
    LED_outputsB[led_indexB/NUM_LED_COLUMNS][led_indexB%NUM_LED_COLUMNS] = 0;
    led_indexB++;
    led_indexB %= (NUM_LED_COLUMNS * NUM_LED_ROWS);
    LED_outputsB[led_indexB/NUM_LED_COLUMNS][led_indexB%NUM_LED_COLUMNS] = 1;
  }
  
  if(millis() >= next_scanG)
  {
    next_scanG = millis() + 1000;

    LED_outputsG[led_indexG/NUM_LED_COLUMNS][led_indexG%NUM_LED_COLUMNS] = 0;
    led_indexG++;
    led_indexG %= (NUM_LED_COLUMNS * NUM_LED_ROWS);
    LED_outputsG[led_indexG/NUM_LED_COLUMNS][led_indexG%NUM_LED_COLUMNS] = 1;
  }
}
