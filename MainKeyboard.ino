/* ################################################################################
 * Keypad methods
 * The following file defines methods that run and control the LED patterns.
 */

// Define constant characters in matrix
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Map Arduino pin numbers to the rows and columns
// Remember left to right, first 4 pins are rows, next 3 are cols
byte rowPins[ROWS] = {12, 14, 27, 26};
byte colPins[COLS] = {25, 33, 32};

// Final creation of keypad based on pins, rows, and cols
//Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/*
 * Store Keys
 * Here we cycle what's been entered into a 2 character string
 */
char holdKeys[] = "00";
void addKey(char val)
{
  Serial.print("keypress: ");
  Serial.println(val);
  // cycle all characters one position to the left
  int len = strlen(holdKeys);
  for( int i = 0; i < len-1; i++ )
    {
      holdKeys[i] = holdKeys[i+1];
    }
  // Add the new character to the last position
  holdKeys[len-1] = val;
}

void setPattern()
{
  PATTERN_NUMBER=String(holdKeys).toInt();
  holdKeys[0] = holdKeys[1] = '0';
  // alert new pattern change
  alertPattern();
}

void clearKeys()
{
    int len = strlen(holdKeys);
    for( int i = 0; i < len-1; i++ )
    {
      holdKeys[i] = '0';
    }
  PATTERN_NUMBER=0;
  Serial.println("Clear Pattern: [" + String(PATTERN_NUMBER) + "]" );
  // alert new pattern change
  alertPattern();
}

void keyboardCycleLoop() {
  
 //2020-04-13 - commenting out keyboard logic
  //char key = keypad.getKey();
  char key = 'R';
  if (key)
  {
  if ( key == '1' ) 
    {
      prevPattern();
    }
  if ( key == '2' ) 
    {
      nextPattern();
    }
  if ( key == '*' ) 
    {
      clearKeys();
    }
  }
}

void keyLogic( char key ) {
    // Depending on which key we press, we will change the LED pattern
    switch(key)
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      addKey(key); break;
      case '#': setPattern(); break;
      case '*': clearKeys(); break;
      default:
      break;
    }
}

/*
 * Keyboard loop logic
 * Here we specify what we detect from the keypad and act upon it
 */
void keyboardLoop() {
 //2020-04-13 - commenting out keyboard logic
 // char key = keypad.getKey();
 // if (key)
 // {
 //   keyLogic(key);
 // }
}
