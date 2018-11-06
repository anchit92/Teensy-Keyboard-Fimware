#include <usb_private.h>
#include <Bounce2.h>

//function lock stuff
//fn_key row=2 col=41
#define FN_LK  1 //just some function lock value
//bool fnLk = false;
bool fnMode = false;
bool fnPressed = false;
int fnLkCount=0;//count for what fnnlk state will only swithc if last 50 have been the same

//led pins
const int capsLockPin = 26;
const int numLockPin = 25;
const int scrollLockPin = 24;

const int rowCount = 18;
const int colCount = 8;
const int rowPins[18] = {22, 2, 4, 7, 9, 10, 13, 15, 42, 44, 8, 5, 3, 1, 14, 39, 43, 45};
const int colPins[9] = {11, 17, 38, 40, 12, 16, 18, 41};

//debounce for column setup
//Bounce * columnPins = new Bounce[colCount];
//const int debounceInterval = 1000000;
//Lookup Tables

//Main lookup table
const int keys[rowCount][colCount] = {
  {KEY_F6,        KEY_K,           KEY_I,           KEY_COMMA,      KEY_8,           KEY_EQUAL,     KEY_RIGHT_BRACE, -1           },
  { -1,            KEY_L,           KEY_O,           KEY_PERIOD,     KEY_9,           KEY_F8,        KEY_F7,          KEY_MENU     },
  {KEY_G,         KEY_F,           KEY_R,           KEY_V,          KEY_4,           KEY_5,         KEY_T,           KEY_B        },
  { -1,            KEY_S,           KEY_W,           KEY_X,          KEY_2,           KEY_F1,        KEY_CAPS_LOCK,   -1           },
  { -1,            -1,              KEY_PAUSE,       KEY_RIGHT_CTRL, KEY_F5,          KEY_LEFT_CTRL, -1,              -1           },
  {KEY_F11,       KEY_BACKSLASH,   -1,              KEY_ENTER,      KEY_F10,         KEY_F9,        KEY_BACKSPACE,   KEY_F12           },
  {KEYPAD_0,      KEYPAD_2,        KEYPAD_8,        KEYPAD_SLASH,   -1,              KEY_INSERT,    KEYPAD_5,        KEY_RIGHT    },
  {KEY_SPACE,     KEYPAD_1,        KEYPAD_7,        KEY_NUM_LOCK,   -1,              KEY_DELETE,    KEYPAD_4,        KEY_DOWN     },
  { -1,            KEY_RIGHT_GUI,   -1,              -1,             -1,              -1,            -1,              -1           },
  { -1,            KEY_RIGHT_SHIFT, -1,              -1,             -1,              -1,            KEY_LEFT_SHIFT,  -1           },
  {KEY_ESC,       KEY_A,           KEY_Q,           KEY_Z,          KEY_1,           KEY_TILDE,     KEY_TAB,         -1           },
  {KEY_F4,        KEY_D,           KEY_E,           KEY_C,          KEY_3,           KEY_F2,        KEY_F3,          -1           },
  {KEY_H,         KEY_J,           KEY_U,           KEY_M,          KEY_7,           KEY_6,         KEY_Y,           KEY_N        },
  {KEY_LEFT_ALT,  -1,              KEY_SCROLL_LOCK, -1,             KEY_PRINTSCREEN, -1,            -1,              KEY_RIGHT_ALT},
  {KEY_QUOTE,     KEY_SEMICOLON,   KEY_P,           -1,             KEY_0,           KEY_MINUS,     KEY_LEFT_BRACE,  KEY_SLASH    },
  {KEYPAD_PERIOD, KEYPAD_3,        KEYPAD_9,        KEYPAD_ASTERIX, KEY_PAGE_DOWN,   KEY_PAGE_UP,   KEYPAD_6,        KEYPAD_MINUS },
  { -1,            -1,              -1,              -1,             -1,              -1,            KEY_LEFT_GUI,    -1,          },
  {KEY_UP,        KEYPAD_ENTER,    KEYPAD_PLUS,     -1,             KEY_END,         KEY_HOME,      -1,              KEY_LEFT     }
};
//Function lookup Keys

const int fnkeys[rowCount][colCount] = {
  
  {KEY_MEDIA_STOP,        KEY_K,           KEY_I,           KEY_COMMA,      KEY_8,           KEY_EQUAL,     KEY_RIGHT_BRACE, -1           },
  { -1,            KEY_L,           KEY_O,           KEY_PERIOD,     KEY_9,          KEY_MEDIA_NEXT_TRACK,        KEY_MEDIA_PREV_TRACK,          KEY_MENU     },
  {KEY_G,         KEY_F,           KEY_R,           KEY_V,          KEY_4,           KEY_5,         KEY_T,           KEY_B        },
  { -1,            KEY_S,           KEY_W,           KEY_X,          KEY_2,           KEY_F1,        KEY_CAPS_LOCK,   -1           },
  { -1,            -1,              KEY_PAUSE,       KEY_RIGHT_CTRL, KEY_MEDIA_PLAY_PAUSE,          KEY_LEFT_CTRL, -1,              -1           },
  {KEY_MEDIA_VOLUME_INC,       KEY_BACKSLASH,   -1,              KEY_ENTER,      KEY_MEDIA_VOLUME_DEC,         KEY_MEDIA_MUTE,        KEY_BACKSPACE,   KEY_F12},
  {KEYPAD_0,      KEYPAD_2,        KEYPAD_8,        KEYPAD_SLASH,   -1,              KEY_INSERT,    KEYPAD_5,        KEY_RIGHT    },
  {KEY_SPACE,     KEYPAD_1,        KEYPAD_7,        KEY_NUM_LOCK,   -1,              KEY_DELETE,    KEYPAD_4,        KEY_DOWN     },
  { -1,            KEY_RIGHT_GUI,   -1,              -1,             -1,              -1,            -1,              -1           },
  { -1,            KEY_RIGHT_SHIFT, -1,              -1,             -1,              -1,            KEY_LEFT_SHIFT,  -1           },
  {KEY_ESC,       KEY_A,           KEY_Q,           KEY_Z,          KEY_1,           KEY_TILDE,     KEY_TAB,         -1           },
  {KEY_F4,        KEY_D,           KEY_E,           KEY_C,          KEY_3,           KEY_F2,        KEY_F3,          -1           },
  {KEY_H,         KEY_J,           KEY_U,           KEY_M,          KEY_7,           KEY_6,         KEY_Y,           KEY_N        },
  {KEY_LEFT_ALT,  -1,              KEY_SCROLL_LOCK, -1,             KEY_PRINTSCREEN, -1,            -1,              KEY_RIGHT_ALT},
  {KEY_QUOTE,     KEY_SEMICOLON,   KEY_P,           -1,             KEY_0,           KEY_MINUS,     KEY_LEFT_BRACE,  KEY_SLASH    },
  {KEYPAD_PERIOD, KEYPAD_3,        KEYPAD_9,        KEYPAD_ASTERIX, KEY_PAGE_DOWN,   KEY_PAGE_UP,   KEYPAD_6,        KEYPAD_MINUS },
  { -1,            -1,              -1,              -1,             -1,              -1,            KEY_LEFT_GUI,    -1,          },
  {KEY_UP,        KEYPAD_ENTER,    KEYPAD_PLUS,     -1,             KEY_END,         KEY_HOME,      -1,              KEY_LEFT     }
};
void setup() {
  Keyboard.begin();
  fnMode = false;
  for (int ri = 0; ri < rowCount; ri++) {
    int row = rowPins[ri];
    pinMode(row, OUTPUT);
    digitalWrite(row, HIGH);
  }
  for (int i = 0; i < colCount; i++) {
    int col = colPins[i];
    pinMode(col, INPUT_PULLUP);
    //columnPins[i].attach(col , INPUT_PULLUP);
    //columnPins[i].interval(debounceInterval);


  }

  pinMode(numLockPin, OUTPUT);
  pinMode(capsLockPin, OUTPUT);
  pinMode(scrollLockPin, OUTPUT);
}

void loop() {
  scanMatrix();
  digitalWrite(numLockPin, !isNumOn());
  digitalWrite(capsLockPin, !isCapsOn());
  digitalWrite(scrollLockPin, !isScrollOn());
}

void scanMatrix() {
  bool fnLk =false;
  for (int i = 0; i < rowCount; i++) {
    int row = rowPins[i];
    digitalWrite(row, LOW);

    for (int j = 0; j < colCount; j++) {
      int col = colPins[j];
      int key=0;
      if (fnMode){
        key = fnkeys[i][j];
      }
      else{
        key = keys[i][j];
      }
      if (digitalRead(col) == LOW) {
        //if ( columnPins[j].fell() ==true ){

        if ( key == KEY_MENU && !fnPressed) {     //check for function key
            fnMode = !fnMode;
            fnPressed=true;
            fnLkCount=0;
       
        }
        else if (key== KEY_F12 && fnPressed){     //lock function mode
          if (fnLkCount ==10){  //only update if the last 10 have been the same to debounce..dont update if more than 10 have been the same...that just puts it in a toggle loop
            fnMode = !fnMode;
            fnLk = true;
            }
            fnLkCount++;
            
      } 
        else if (key!= KEY_MENU && fnLk == false){                         //else do normal keys
           Keyboard.press(key);
         }
      }
      else {
        if (key==KEY_MENU && fnPressed){
          fnMode=!fnMode;
          fnPressed=false;
          fnLkCount=0;
          }
        else if(key !=KEY_MENU && fnLk == false){
          Keyboard.release(key);
          }
        if (key== KEY_F12)
          fnLkCount=0;
        }
      }

    digitalWrite(row, HIGH);
  }
}

boolean isScrollOn(void) {
  return (keyboard_leds & 4) == 4;
}

boolean isCapsOn(void) {
  return (keyboard_leds & 2) == 2;
}

boolean isNumOn(void) {
  return (keyboard_leds & 1) == 1;
}

