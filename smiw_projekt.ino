//Patryk Heisig
//informatyka niestacjonarna

#include "U8glib.h"
#include "pitches.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

uint8_t uiKeyA = 7;
uint8_t uiKeyB = 6;
uint8_t uiKeyLeft = 2;
uint8_t uiKeyRight = 5;
uint8_t uiKeyUp = 3;
uint8_t uiKeyDown = 4;
uint8_t uiBuzzer = 12;
uint8_t uiVibMotor = 9;

uint8_t cycles = 0; //vibration cycle
bool isVibrating = 0; //motor is vibrating

uint8_t workingTrybe = 0; //0-menu; 1-snake; 2-button test
uint8_t menuSelect = 1; //which menu position is currently selected
const uint8_t menuCount = 2; //menu count

boolean isDrawed = true; //true if frame is drawed

int snakeLength = 8;  //initial lenght of snake = 8
//current position of snake head
int currentPosition[] {
  9, 23
};

//position of last snake module
int lastPosition[] {
  9, 15
};
//position of food
int foodPosition[] {
  2, 23
};

///// IMAGES ///////
const uint8_t snakeModuleBitmap[] PROGMEM = {
  0x2,          // 00000010
  0x5,          // 00000101
  0x2           // 00000010
};

const uint8_t snakeHeadDownBitmap[] PROGMEM = {
  0x1f,          // 00011111
  0x0e,          // 00001110
  0x04           // 00000100
};

const uint8_t snakeHeadOpenDownBitmap[] PROGMEM = {
  0x1f,          // 00011111
  0x09,          // 00001001
  0x05           // 00000101
};

const uint8_t snakeHeadRightBitmap[] PROGMEM = {
  0x04,          // 00000100
  0x06,          // 00000110
  0x07,          // 00000111
  0x06,          // 00000110
  0x04           // 00000100
};

const uint8_t snakeHeadOpenRightBitmap[] PROGMEM = {
  0x04,          // 00000100
  0x07,          // 00000111
  0x04,          // 00000100
  0x04,          // 00000100
  0x07,          // 00000111
  0x04           // 00000100
};

const uint8_t snakeHeadLeftBitmap[] PROGMEM = {
  0x01,          // 00000001
  0x03,          // 00000011
  0x07,          // 00000111
  0x03,          // 00000011
  0x01           // 00000001
};

const uint8_t snakeHeadOpenLeftBitmap[] PROGMEM = {
  0x01,          // 00000001
  0x07,          // 00000111
  0x01,          // 00000001
  0x07,          // 00000111
  0x01           // 00000001
};

const uint8_t snakeHeadUpBitmap[] PROGMEM = {
  0x04,          // 00000100
  0x0e,          // 00001110
  0x1f           // 00011111
};

const uint8_t snakeHeadOpenUpBitmap[] PROGMEM = {
  0x05,          // 00000101
  0x09,          // 00001001
  0x1f           // 00011111
};

const uint8_t food01Bitmap[] PROGMEM = {
  0x06,          // 00000110
  0x08,          // 00001000
  0x3e,          // 00111110
  0x7f,          // 01111111
  0x7f,          // 01111111
  0x7f,          // 01111111
  0x3e           // 00111110
};

const uint8_t food02Bitmap[] PROGMEM = {
  0x36,          // 00110110
  0x7f,          // 01111111
  0x7f,          // 01111111
  0x7f,          // 01111111
  0x3e,          // 00111110
  0x1c,          // 00011100
  0x08           // 00001000
};

//map 21 x 42
//y x
uint8_t snakePositions[][42] {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


// configure input keys
void uiSetup(void) {
  pinMode(uiKeyA, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeyB, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeyLeft, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeyRight, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeyUp, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiKeyDown, INPUT_PULLUP);           // set pin to input with pullup
  pinMode(uiBuzzer, OUTPUT);
  pinMode(uiVibMotor, OUTPUT);
}

//print text at oled at definied location
void drawText(int x, int y, const char* text) {
  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( x, y, text);
}


//init vibrations
void vibrate() {
  analogWrite( uiVibMotor , 153 );
  cycles = 0;
  isVibrating = true;
}

//check if vibrator should still vibrate and shut it down
void vibrateCheck(int maxCycles) {
  if (isVibrating) {
    if (cycles > maxCycles) {
      analogWrite( uiVibMotor , 0 );
    }
    else
      cycles++;
  }
}

//take info from buttons, return number which correspondes with buttons
uint8_t uiInput() {
  if ( digitalRead(uiKeyA) == LOW ) {
    return 1;
  }

  else if ( digitalRead(uiKeyB) == LOW ) {
    return 2;
  }

  else if ( digitalRead(uiKeyLeft) == LOW ) {
    return 3;
  }

  else if ( digitalRead(uiKeyRight) == LOW ) {
    return 4;
  }

  else if ( digitalRead(uiKeyUp) == LOW ) {
    return 5;
  }

  else if ( digitalRead(uiKeyDown) == LOW ) {
    return 6;
  }
  else
    return -1;
}

//play a tone, and start vibrations
void buttonPushed(int soundHz) {
  tone(uiBuzzer, soundHz, 200);
  vibrate();
  delay(50);
}

//simple menu, user can choose one option from listed using arrows button, and confirm by A button
void menu() {
  drawText(10, 22, "Snake game");
  drawText(10, 44, "Button test");

  if (menuSelect == 1)
    drawText(0, 22, "*");
  if (menuSelect == 2)
    drawText(0, 44, "*");

  switch (uiInput()) {
    case 1: //A
      workingTrybe = menuSelect;
      playStartMelody();
      break;

    case 2: //B
      buttonPushed(1000);
      break;

    case 3: //Left
      buttonPushed(1000);
      break;

    case 4: //Right
      buttonPushed(1000);
      break;

    case 5: //Up
      //move selected menu up
      if (menuSelect == 1) menuSelect = menuCount;
      else menuSelect--;
      buttonPushed(1000);
      break;

    case 6: //Down
      buttonPushed(1000);
      //move selected menu down
      if (menuSelect == menuCount) menuSelect = 1;
      else menuSelect++;
      break;
  }
  vibrateCheck(15);
}

//display at screen currently presed button
void buttonTest() {
  drawText(0, 22, "Button test:");
  switch (uiInput()) {
    case 1: //A
      drawText(0, 44, "A pressed");
      buttonPushed(1000);
      break;

    case 2: //B
      drawText(0, 44, "B pressed");
      buttonPushed(1000);
      break;

    case 3: //Left
      drawText(0, 44, "LEFT pressed");
      buttonPushed(1000);
      break;

    case 4: //Right
      drawText(0, 44, "RIGHT pressed");
      buttonPushed(1000);
      break;

    case 5: //Up
      drawText(0, 44, "UP pressed");
      buttonPushed(1000);
      break;

    case 6: //Down
      drawText(0, 44, "DOWN pressed");
      buttonPushed(1000);
      break;
  }
  vibrateCheck(15);
}

//set random position of next food
void randFood() {
  int temp1 = 0;
  int temp2 = 0;
  do {
    temp1 = random(21);
    temp2 = random(42);
  } while (snakePositions[temp1][temp2] > 0); //it is important to not set food at snake
  foodPosition[0] = temp1;
  foodPosition[1] = temp2;
}

//deals with snake lenght
void addRange() {
  for (int x = 0; x < 42; x++) {
    for (int y = 0; y < 21; y++) {
      if (snakePositions[y][x] > 0) {
        snakePositions[y][x]++;
        if (snakePositions[y][x] > snakeLength) {
          snakePositions[y][x] = 0; //remove last
        }
      }
    }
  }
}

//snake moving functions, change values at snake map
void moveRight() {
  addRange();
  if (currentPosition[1] == 41) currentPosition[1] = 0;
  else currentPosition[1]++;
}
void moveLeft() {
  addRange();
  if (currentPosition[1] == 0) currentPosition[1] = 41;
  else currentPosition[1]--;
}
void moveUp() {
  addRange();
  if (currentPosition[0] == 0) currentPosition[0] = 20;
  else currentPosition[0]--;
}
void moveDown() {
  addRange();
  if (currentPosition[0] == 20) currentPosition[0] = 0;
  else currentPosition[0]++;
}

// read snake map, andd draw snake on the screen
void drawSnake() {
  for (int x = 0; x < 42; x++) {
    for (int y = 0; y < 21; y++) {
      if (snakePositions[y][x])
        u8g.drawBitmapP( x * 3, y * 3, 1, 3, snakeModuleBitmap);
    }
  }
  u8g.drawBitmapP( foodPosition[1] * 3, foodPosition[0] * 3, 1, 7, food01Bitmap);
}

//melody code from https://www.arduino.cc/en/Tutorial/ToneMelody
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

//play a song
void playStartMelody() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(uiBuzzer, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(uiBuzzer);
  }
}
//END of copied code

//snake game function, take info abut user input, manage scene drawing
uint8_t snakeDirection = 0;
void snake() {
  drawSnake();
  switch (uiInput()) {
    case 1: //A
      buttonPushed(1000);
      break;

    case 2: //B exit from game
      workingTrybe = 0;
      buttonPushed(1000);
      break;

    case 3: //Left
      if (snakeDirection != 4)
        snakeDirection = 3;
      buttonPushed(1000);
      break;

    case 4: //Right
      if (snakeDirection != 3)
        snakeDirection = 4;
      buttonPushed(1000);
      break;

    case 5: //Up
      if (snakeDirection != 6)
        snakeDirection = 5;
      buttonPushed(1000);
      break;

    case 6: //Down
      if (snakeDirection != 5)
        snakeDirection = 6;
      buttonPushed(1000);
      break;
  }

  //move snake only when last frame is properly drawed
  if (isDrawed) {
    switch (snakeDirection) {
      case 1: //A
        break;

      case 2: //B
        break;

      case 3: //Left
        moveLeft();
        break;

      case 4: //Right
        moveRight();
        break;

      case 5: //Up
        moveUp();
        break;

      case 6: //Down
        moveDown();
        break;
    }

    //add new snake head to snake-map
    snakePositions[currentPosition[0]][ currentPosition[1]] = 1;
    //when snake eat food
    if (currentPosition[0] == foodPosition[0] && currentPosition[1] == foodPosition[1]) {
      snakeLength++;
      randFood();
      buttonPushed(4000);
    }
  }
  vibrateCheck(15); //turn off vibrations after a while
  isDrawed = 0; //wait for frame to be drawed
}

void setup() {
  // put your setup code here, to run once:
  u8g.setColorIndex(1); // pixel on
  uiSetup(); //init IO at Arduino
  randomSeed(analogRead(0));//shuffle the random generator
  randFood(); //generate initial random position of snake food
}

void loop(void) {
  // picture loop
  u8g.firstPage();
  do {
    switch (workingTrybe) {
      case 0:
        menu();
        break;
      case 1:
        snake();
        break;
      case 2:
        buttonTest();
        break;
    }
    // buttonTest();
  } while ( u8g.nextPage() );
  isDrawed = true;
  delay(50);
}
