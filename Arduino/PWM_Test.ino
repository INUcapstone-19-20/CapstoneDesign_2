//config variables
#define NUM_BTN_COLUMNS (4)
#define NUM_BTN_ROWS (4)
#define NUM_LED_COLUMNS (4)
#define NUM_LED_ROWS (4)
#define NUM_COLORS (3)

#define MAX_DEBOUNCE (3)

// Global variables
static bool LED_buffer[NUM_LED_COLUMNS][NUM_LED_ROWS];

static const uint8_t btncolumnpins[NUM_BTN_COLUMNS] = {29, 28, 27, 26};   // SWT-GND 1,2,3,4
static const uint8_t btnrowpins[NUM_BTN_ROWS]       = {22, 23, 24, 25};   // SWTICH 1,2,3,4
static const uint8_t ledcolumnpins[NUM_LED_COLUMNS] = {30, 31, 32, 33};   // LED-GND 1,2,3,4
//pin number 2~13 : PWM
static const uint8_t ledrowpins[NUM_LED_ROWS][NUM_COLORS] = { {2, 3, 4},      // RED1, GREEN1, BLUE1
                                                              {5, 6, 7},      // RED2, GREEN2, BLUE2
                                                              {8, 9, 10},     // RED3, GREEN3, BLUE3
                                                              {11, 12, 13} };  // RED4, GREEN4, BLUE4

static int8_t debounce_count[NUM_BTN_COLUMNS][NUM_BTN_ROWS];

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

  // LED drive lines
  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    for(uint8_t j = 0; j < NUM_COLORS; j++)
    {
      pinMode(ledrowpins[i][j], OUTPUT);
      digitalWrite(ledrowpins[i][j], LOW);
    }
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
    if (LED_buffer[current][i])
    {
      analogWrite(ledrowpins[i][0], 255);
      analogWrite(ledrowpins[i][1], 0);
      analogWrite(ledrowpins[i][2], 0);
    }
  }

  // pause a moment
  delay(1);

  // Read the button inputs
  for ( j = 0; j < NUM_BTN_ROWS; j++)
  {
    val = digitalRead(btnrowpins[j]);

    if (val == LOW)
    {
      // active low: val is low when btn is pressed
      if ( debounce_count[current][j] < MAX_DEBOUNCE)
      {
        debounce_count[current][j]++;
        if ( debounce_count[current][j] == MAX_DEBOUNCE )
        {
          Serial.print("Key Down ");
          Serial.println((current * NUM_BTN_ROWS) + j);

          // toggle the current LED state
          LED_buffer[current][j] = !LED_buffer[current][j];
        }
      }
    }
    else
    {
      // otherwise, button is released
      if ( debounce_count[current][j] > 0)
      {
        debounce_count[current][j]--;
        if ( debounce_count[current][j] == 0 )
        {
          Serial.print("Key Up ");
          Serial.println((current * NUM_BTN_ROWS) + j);
        }
      }
    }
  }

  delay(1);

  digitalWrite(btncolumnpins[current], HIGH);
  digitalWrite(ledcolumnpins[current], HIGH);

  for(i = 0; i < NUM_LED_ROWS; i++)
  {
    for(j = 0; j < NUM_COLORS; j++)
    {
      digitalWrite(ledrowpins[i][j], LOW);
    }
  }

  current++;
  if (current >= NUM_LED_COLUMNS)
  {
    current = 0;
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
      LED_buffer[i][j] = 0;
    }
  }

  Serial.println("Setup Complete.");

}

void loop() {
  
  scan();

}
