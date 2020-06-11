#include "Keyboard.h"

#define KEY_1 0x1e // Keyboard 1 and !
#define KEY_2 0x1f // Keyboard 2 and " and @
#define KEY_3 0x20 // Keyboard 3 and #
#define KEY_4 0x21 // Keyboard 4 and ¤
#define KEY_5 0x22 // Keyboard 5 and %
#define KEY_6 0x23 // Keyboard 6 and & 
#define KEY_7 0x24 // Keyboard 7 and / and {
#define KEY_8 0x25 // Keyboard 8 and (
#define KEY_9 0x26 // Keyboard 9 and )
#define KEY_0 0x27 // Keyboard 0 and = and }
#define KEY_PLUS 0x2d //Keyboard + and ? and backslash
#define KEY_ACCENT 0x2e //Keyboard ´ and `
#define KEY_HIGHDOTS 0x30 //Keyboard ¨ and ^
#define KEY_APOSTROPHE 0x31 //Keyboard ' and *
#define KEY_COMMA 0x36 //Keyboard , and ;
#define KEY_DOT 0x37 //Keyboard . and :
#define KEY_DASH 0x38 //Keyboard - and _

typedef struct Pair {
  char c;
  int sc;
} Pair;

int btnPin = 7;

Pair ordinary[] = {(Pair){.c = '+', .sc = KEY_PLUS}, (Pair){.c = '´', .sc = KEY_ACCENT}, (Pair){.c = '¨', .sc = KEY_HIGHDOTS}, 
  (Pair){.c = '\'', .sc = KEY_APOSTROPHE}, (Pair){.c = ',', .sc = KEY_COMMA}, (Pair){.c = '.', .sc = KEY_DOT}, (Pair) {.c = '-', .sc = KEY_DASH}};

Pair shifted[] = {(Pair){.c = '!', .sc = KEY_1}, (Pair){.c = '"', .sc = KEY_2}, (Pair){.c = '#', .sc = KEY_3}, (Pair){.c = '¤', .sc = KEY_4}, 
  (Pair){.c = '%', .sc = KEY_5}, (Pair){.c = '&', .sc = KEY_6}, (Pair){.c = '/', .sc = KEY_7}, (Pair){.c = '(', .sc = KEY_8}, (Pair){.c = ')', .sc = KEY_9}, 
  (Pair) {.c = '=', .sc = KEY_0}, (Pair){.c = '?', .sc = KEY_PLUS}, (Pair){.c = '`', .sc = KEY_ACCENT}, (Pair){.c = '^', .sc = KEY_HIGHDOTS}, 
  (Pair) {.c = '*', .sc = KEY_APOSTROPHE}, (Pair) {.c = ';', .sc = KEY_COMMA}, (Pair) {.c = ':', .sc = KEY_DOT}, (Pair) {.c = '_', .sc = KEY_DASH}};

Pair altGr[] = {(Pair){.c = '{', .sc = KEY_7}, (Pair){.c = '}', .sc = KEY_0}, (Pair) {.c = '\\', .sc = KEY_PLUS}, (Pair) {.c = '@', .sc = KEY_2}};

void setup() {
  pinMode(btnPin, INPUT_PULLUP);
  Keyboard.begin();
  delay(2000); //  Give the keyboard driver time to boot
}

void loop() {
  while (digitalRead(btnPin) == LOW) {
    delay(1000);
  }
  //Enter the string(s) that should be injected here.
  //Example:
  //printString("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890+´¨'-.,!\"#¤%&/()?`^*_:;{}\\@");
  //qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890+´¨'-.,!"#¤%&/()?`^*_:;{}\@

  winR("cmd");
  enter();
  
  //NOP indefinately
  while (true);
}

void winR(String s){
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(300);
  Keyboard.releaseAll();
  printString(s);
}

//Exit arbitrary terminal
void done() {
  Keyboard.println("exit");
}

void enter(){
  Keyboard.println();
}

void printShifted(int scUnaltered) {
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.write(scUnaltered + 0x88);
  Keyboard.releaseAll();
}
void printAltGr(int scUnaltered) {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.write(scUnaltered + 0x88);
  Keyboard.releaseAll();
}
void printOrdinary(int scUnaltered) {
  Keyboard.write(scUnaltered + 0x88);
}

int lookupOrdinary(char c) {
  for (int i = 0; i < sizeof(ordinary) / sizeof(Pair); i++) {
    if (c == ordinary[i].c) {
      return ordinary[i].sc;
    }
  }
  return 0; // Not found
}

int lookupShifted(char c) {
  for (int i = 0; i < sizeof(shifted) / sizeof(Pair); i++) {
    if (c == shifted[i].c) {
      return shifted[i].sc;
    }
  }
  return 0; // Not found
}

int lookupAltGr(char c) {
  for (int i = 0; i < sizeof(altGr) / sizeof(Pair); i++) {
    if (c == altGr[i].c) {
      return altGr[i].sc;
    }
  }
  return 0; // Not found
}

void printString(String s) {
  for (int i = 0; i < s.length(); i++) {
    char c = s.charAt(i);
    //Is c in a-Z or a digit?
    if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || ( c >= 48 && c <= 57)||(c == ' ')) {
      Keyboard.print(c);
    }
    //Is c a special character?
    else {
      //Is c without modifiers?
      int sc = lookupOrdinary(c);
      if (sc != 0) {
        printOrdinary(sc);
      }
      else {
        //Is c modified by shift?
        sc = lookupShifted(c);
        if (sc != 0) {
          printShifted(sc);
        }
        else {
          //Is c modified by AltGr (Ctrl+Alt)?
          sc = lookupAltGr(c);
          if (sc != 0) {
            printAltGr(sc);
          }
        }
      }
    }
  }
}
