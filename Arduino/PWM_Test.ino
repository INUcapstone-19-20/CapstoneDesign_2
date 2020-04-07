//config variables
#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)
#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_COLORS (3)

#define MAX_DEBOUNCE (3)

// Global variables
static bool LED_pressed[NUM_LED_ROWS][NUM_BTN_COLUMNS];

static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {29, 28, 27, 26};   // SWT-GND 1,2,3,4
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {22, 23, 24, 25};   // SWTICH 1,2,3,4
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] = {30, 31, 32, 33};   // LED-GND 1,2,3,4
//pin number 2~13 : PWM
static const uint8_t ledrowpins[NUM_LED_ROWS][NUM_COLORS] = { {2, 3, 4},      // RED1, GREEN1, BLUE1
                                                              {5, 6, 7},      // RED2, GREEN2, BLUE2
                                                              {8, 9, 10},     // RED3, GREEN3, BLUE3
                                                              {11, 12, 13} };  // RED4, GREEN4, BLUE4
                                                              
static const uint8_t rgbvalues[NUM_LED_ROWS][NUM_COLORS] = {{255, 0, 0},  // row1
                                                                 {0, 255, 0},  // row2
                                                                 {0, 0, 255},  // row3
                                                                 {255 ,255, 255}}; // row4
static int8_t debounce_count[NUM_LED_ROWS][NUM_BTN_COLUMNS];

static void setuppins()
{
  uint8_t i;

  // initialize
  // select lines
  
  // LED columns
  for (i = 0; i < NUM_LED_COLUMNS; i++)
  {
    pinMode(ledcolumnpins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(ledcolumnpins[i], HIGH);
  }

  // button columns
  for (i = 0; i < NUM_BTN_COLUMNS; i++)
  {
    pinMode(btncolumnpins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(btncolumnpins[i], HIGH);
  }

  // button row input lines
  for (i = 0; i < NUM_BTN_ROWS; i++)
  {
    pinMode(btnrowpins[i], INPUT_PULLUP);
  }

  // Initialize the debounce counter array
  for (uint8_t i = 0; i < NUM_BTN_COLUMNS; i++)
  {
    for (uint8_t j = 0; j < NUM_BTN_ROWS; j++)
    {
      debounce_count[i][j] = 0;
    }
  }
}

void setup()
{
  Serial.begin(115200);

  Serial.print("Starting Setup...");

  // setup hardware
  setuppins();

  // init global variables
  for (uint8_t i = 0; i < NUM_LED_COLUMNS; i++)
  {
    for (uint8_t j = 0; j < NUM_LED_ROWS; j++)
    {
      LED_pressed[i][j] = 0;
    }
  }
  Serial.println("Setup Complete.");
}

static void scan()
{
  static uint8_t current = 0;
  uint8_t val;
  uint8_t i, j;

  // Select current columns
  digitalWrite(btncolumnpins[current], LOW);
  digitalWrite(ledcolumnpins[current], LOW);
  
  // output LED row values
  for (i = 0; i < NUM_LED_ROWS; i++)
  {
    for (j = 0; j < NUM_COLORS; j++)
    {
      if (LED_pressed[i][current])
      {
        analogWrite(ledrowpins[i][j], rgbvalues[i][j]);
      }
    }
  }
  
  // pause a moment
  delay(10);
  
  // Read the button inputs
  for (j = 0; j < NUM_BTN_ROWS; j++)
  {
    val = digitalRead(btnrowpins[j]);

    if (val == LOW)
    {
      // active low: val is low when btn is pressed
      if (debounce_count[j][current] < MAX_DEBOUNCE)
      {
        debounce_count[j][current]++;
        if (debounce_count[j][current] == MAX_DEBOUNCE )
        {
          Serial.print("Key Down ");
          Serial.println((current * NUM_BTN_ROWS) + j);

          // toggle the current LED state
          LED_pressed[j][current] = !LED_pressed[j][current];
        }
      }
    }
    else
    {
      // otherwise, button is released
      if (debounce_count[j][current] > 0)
      {
        debounce_count[j][current]--;
        if (debounce_count[j][current] == 0 )
        {
          Serial.print("Key Up ");
          Serial.println((current * NUM_BTN_ROWS) + j);
        }
      }
    }
  }

  delay(10);

  digitalWrite(btncolumnpins[current], HIGH);
  digitalWrite(ledcolumnpins[current], HIGH);

  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    for(j = 0; j < NUM_COLORS; j++)
    {
      analogWrite(ledrowpins[i][j], 0);
    }
  }
  
  current++;
  
  if (current >= NUM_LED_COLUMNS)
  {
    current = 0;
  }
}

void loop() {
  scan();
}
