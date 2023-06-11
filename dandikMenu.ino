#include <Tiny4kOLED.h>
  uint8_t width = 128;
  uint8_t height = 64;

#define encoderPinA 2
#define encoderPinB 3
#define encoderButtonPin 4
  #include "5x5_font.h"
  const DCfont *currentFont = FONT5X5;
  

const int numTitles = 3;
const int numSubtitles = 2;
const int numTextLines = 4;

const int lineSpacing = 6;
const int textMargin = 5;

const char* titleList[] = {
  "Title 1",
  "Title 2",
  "Title 3"
};

const char* subtitleList[][numSubtitles] = {
  {"Subtitle 1-1", "Subtitle 1-2"},
  {"Subtitle 2-1", "Subtitle 2-2"},
  {"Subtitle 3-1", "Subtitle 3-2"}
};

const char* textList[][numSubtitles][numTextLines] = {
  {
    {"Text 1-1-1", "Text 1-1-2", "Text 1-1-3", "Text 1-1-4"},
    {"Text 1-2-1", "Text 1-2-2", "Text 1-2-3", "Text 1-2-4"}
  },
  {
    {"Text 2-1-1", "Text 2-1-2", "Text 2-1-3", "Text 2-1-4"},
    {"Text 2-2-1", "Text 2-2-2", "Text 2-2-3", "Text 2-2-4"}
  },
  {
    {"Text 3-1-1", "Text 3-1-2", "Text 3-1-3", "Text 3-1-4"},
    {"Text 3-2-1", "Text 3-2-2", "Text 3-2-3", "Text 3-2-4"}
  }
};

int selectedTitle = 0;
int selectedSubtitle = 0;

void setup() {
  oled.begin(width, height, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.on();
  oled.clear();
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(encoderButtonPin, INPUT_PULLUP);
}

void loop() {
  // Main menu
  clearOLED();
  printTitleList();
  selectTitle();
  
  // Submenu
  clearOLED();
  printSubtitleList();
  selectSubtitle();
  
  // Text lines
  clearOLED();
  printTextLines();
  waitForLongPress();
}

void clearOLED() {
  oled.clear();
}

void printTitleList() {
  oled.setFont(currentFont);
  
  for (int i = 0; i < numTitles; i++) {
    oled.setCursor(textMargin, i * lineSpacing);
    oled.print(titleList[i]);
  }
}

void selectTitle() {
  delay(350);
  while (true) {
    int encoderVal = readEncoder();

    if (encoderVal != 0) {
      selectedTitle += encoderVal;
      if (selectedTitle < 0) {
        selectedTitle = numTitles - 1;
      } else if (selectedTitle >= numTitles) {
        selectedTitle = 0;
      }
      clearOLED();
      printTitleList();
      delay(525);
    }

    if (digitalRead(encoderButtonPin) == LOW) {
      break;
    }
  }
  delay(350);
}

void printSubtitleList() {
  
  oled.setFont(currentFont);
  for (int i = 0; i < numSubtitles; i++) {
    oled.setCursor(textMargin, i * lineSpacing);
    
    if (i == selectedSubtitle) {
      oled.print(">");
    }
    
    oled.setCursor(textMargin + 6, i * lineSpacing);
    oled.print(subtitleList[selectedTitle][i]);
  }
}

void selectSubtitle() {
  while (true) {
    int encoderVal = readEncoder();

    if (encoderVal != 0) {
      selectedSubtitle += encoderVal;
      if (selectedSubtitle < 0) {
        selectedSubtitle = numSubtitles - 1;
      } else if (selectedSubtitle >= numSubtitles) {
        selectedSubtitle = 0;
      }
      clearOLED();
      printSubtitleList();
      delay(525);
    }

    if (digitalRead(encoderButtonPin) == LOW) {
      break;
    }
  }
}

void printTextLines() {
  oled.setFont(currentFont);
  
  for (int i = 0; i < numTextLines; i++) {
    oled.setCursor(textMargin, i * lineSpacing);
    
    if (i == 0) {
      oled.print(">");
    }
    
    oled.setCursor(textMargin + 6, i * lineSpacing);
    oled.print(textList[selectedTitle][selectedSubtitle][i]);
  }
}

void waitForLongPress() {
  delay(350);
  while (true) {
    if (digitalRead(encoderButtonPin) == LOW) {
      return;
    } 
  }
}

int readEncoder() {
  static int oldA = HIGH;
  static int oldB = HIGH;
  int newA = digitalRead(encoderPinA);
  int newB = digitalRead(encoderPinB);
  
  if (newA != oldA || newB != oldB) {
    oldA = newA;
    oldB = newB;
    if (newA == HIGH && newB == HIGH) {
      return 1;
    }
    if (newA == LOW && newB == HIGH) {
      return -1;
    }
  }
  
  return 0;
}
