/*
*** INSTRUCTIONS ***
You need to change the pins array and the pinCount int.
Just follow the comments for each of those fields.

Upload this sucker to your teensy, open a text editor, 
and then start pushing buttons.  You need to keep track of 
what row/column combos show up for which button you are currently pressing.
The format is (row#/column#).
*/

// Array of all physical pin numbers on your particular Teensy
int pins[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 
              15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 
              27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 
              39, 40, 41, 42, 43, 44, 45};

// The number of physical pins on your particular Teensy
int pinCount = 45;

void setup() {
  resetPins();
}

void loop() {
  scanMatrix();
}

void resetPins() {
  for (int i = 0; i < pinCount; i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }
}

void activateRow(int row) {
  pinMode(row, OUTPUT);
  digitalWrite(row, LOW);
}

boolean checkColumn(int col) {
  boolean returnValue = false;
  pinMode(col, INPUT_PULLUP);
  
  if (digitalRead(col) == LOW) {
    returnValue = true;
  }
  else {
    returnValue = false;
  }
  
  return returnValue;
}

void scanMatrix() {
  for (int i = 0; i < pinCount; i++) {
    int row = pins[i];
    
    for (int z = 0; z < pinCount; z++) {
      int col = pins[z];
      
      resetPins();
      activateRow(row);
      
      if (row != col && checkColumn(col)) {
        printCombo(row, col);
      }
    }
  }
}

void printCombo(int rowPin, int colPin) {
  Keyboard.print("(");
  Keyboard.print(rowPin);
  Keyboard.print(",");
  Keyboard.print(colPin);
  Keyboard.print(")");
}
