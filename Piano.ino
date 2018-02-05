#include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <stdint.h>

#include "tones.h"

const int speaker = 19;

const int whiteWidth = 30;
const int whiteGap = 2;

const int blackWidth = 22;
const int blackHeight = 155;
const int blackOff = 20;

int startKey = C4;
int octaveNumber = startKey / octaveLen;

//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1
static unsigned int TS_MINX, TS_MAXX, TS_MINY, TS_MAXY;
//Touch Screen Co-ordinate mapping register
static unsigned int MapX1, MapX2, MapY1, MapY2;

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define TS_MINX 152
#define TS_MINY 149
#define TS_MAXX 947
#define TS_MAXY 908
int WIDTH = 320, HEIGHT = 240;

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET); 
TouchScreen ts(XP, YP, XM, YM, 300);


//int minX = 9999, minY = 9999, maxX = -9999, maxY = -9999;
int currentFreq = -1;
int currentIdx = -1;

musicNote musicNotesArray[sizeof(freqs) / sizeof(unsigned int)];
void setup() {
    
    Serial.begin(9600);
    Serial.print("Starting...\n");
    randomSeed(analogRead(0));

    int notesSize = sizeof(freqs) / sizeof(unsigned int);
    int colorCheck;
    for (int i = 0; i < notesSize; i++) {
      musicNotesArray[i].freq = freqs[i];
      musicNotesArray[i].color = WHITE;
      
      colorCheck = i % octaveLen;
      for (int c = 0; c < sizeof(blackIdx)/sizeof(unsigned int); c++) {
        if (colorCheck == blackIdx[c]) {
          musicNotesArray[i].color = BLACK;
        }
      }
      // Serial.print(i); Serial.print(" "); Serial.print(musicNotesArray[i].freq); Serial.print(" "); Serial.print(musicNotesArray[i].color); Serial.println();
    }
    initDisplay();
    drawStartScreen();
}

enum {
  FREQ,
  GLOBAL_IDX
};

const int whiteKeysCount = 10;
int whiteKeysMap[whiteKeysCount][2];

int currentPainted = -1;
int currentColor = 0;
void paintKey(int idx, int keyColor, int paintColor) {
  if (idx != currentPainted) {
    if (currentPainted != -1) {
      processPaint(currentPainted, currentColor, currentColor);
    }
    processPaint(idx, keyColor, paintColor);
    currentPainted = idx;
    currentColor = keyColor;
  }
}

void processPaint(int idx, int keyColor, int paintColor) {
  if (keyColor == WHITE) {
    tft.fillRect(0, idx*(whiteWidth + whiteGap), HEIGHT, whiteWidth, paintColor);
    musicNote mN = musicNotesArray[whiteKeysMap[idx][GLOBAL_IDX] - 1];
    if (mN.color == BLACK) {
      tft.fillRect(HEIGHT-blackHeight, (idx-1)*(whiteWidth+whiteGap) + blackOff, blackHeight, blackWidth, BLACK);
    }
    mN = musicNotesArray[whiteKeysMap[idx][GLOBAL_IDX] + 1];
    if (mN.color == BLACK) {
      tft.fillRect(HEIGHT-blackHeight, idx*(whiteWidth+whiteGap) + blackOff, blackHeight, blackWidth, BLACK);
    }
  }
  else if (keyColor == BLACK) {
    tft.fillRect(HEIGHT-blackHeight, idx*(whiteWidth+whiteGap) + blackOff, blackHeight, blackWidth, paintColor);
  }
}

void updateOctaveInfo() {
  tft.fillRect(205, 30, 25, 50, BLACK);
  tft.setRotation(1);
  tft.setCursor(37, 13);
  tft.setTextColor(GREEN);
  tft.setTextSize(3);
  tft.print("C");
  tft.print(octaveNumber);
  tft.setRotation(0);
}

void updateNoteInfo(int idx) {
  if (idx != currentIdx) {
    currentIdx = idx;
    tft.fillRect(205, 150, 25, 50, BLACK);
    tft.setRotation(1);
    tft.setCursor(150, 13);
    tft.setTextColor(YELLOW);
    tft.setTextSize(3);
    tft.print(noteChars[currentIdx % octaveLen]);
    tft.setRotation(0);
  }
}

void drawStartScreen()
{
    tft.fillScreen(BLACK);
    int keysCount = whiteKeysCount;

    //WHITE KEYS
    int whiteIdx = startKey;
    for (int i=0; i<keysCount; i++) {

        while (musicNotesArray[whiteIdx].color != WHITE) {
          whiteIdx++;
        }
        whiteKeysMap[i][GLOBAL_IDX] = whiteIdx;
        whiteKeysMap[i][FREQ] = musicNotesArray[whiteIdx++].freq;
        tft.fillRect(0, i*(whiteWidth + whiteGap), HEIGHT * 0.8, whiteWidth, WHITE);
    }

    //BLACK KEYS
    for (int i = 0; i < sizeof(blackIdx) / sizeof(unsigned int); i++) {
      whiteIdx = (int) ( (float)(blackIdx[i]-1)/2.0 + 0.5);
      tft.fillRect(HEIGHT-blackHeight, whiteIdx * (whiteWidth+whiteGap) + blackOff, blackHeight, blackWidth, BLACK);
    }
    keysCount -= 7;

    int off = 7 * (whiteWidth+whiteGap);
    for (int i = 0; i < sizeof(blackIdx) / sizeof(unsigned int); i++) {
      whiteIdx = (int) ( (float)(blackIdx[i]-1)/2.0 + 0.5 );
      if (whiteIdx >= keysCount)
        break;
      tft.fillRect(HEIGHT-blackHeight, whiteIdx * (whiteWidth+whiteGap) + blackOff + off, blackHeight, blackWidth, BLACK);
    }
    updateOctaveInfo();
   tft.fillTriangle(205, 25, 215, 10, 225, 25, MAGENTA);
   tft.fillTriangle(205, 80, 215, 95, 225, 80, MAGENTA);

}

void loop()
{
    TSPoint p = ts.getPoint();  //Get touch point

    int duration = 50;
    
    if (p.z > ts.pressureThreshhold) {
  
        //FINDING BOUNDARIES
        /*if (p.x < minX)
          minX = p.x;
        if (p.x > maxX)
          maxX = p.x;
        if (p.y < minY)
          minY = p.y;
        if (p.y > maxY)
          maxY = p.y;
          
        Serial.print("X: "); Serial.print(minX); Serial.print(" - "); Serial.print(maxX); Serial.print("\n");
        Serial.print("Y: "); Serial.print(minY); Serial.print(" - "); Serial.print(maxY); Serial.print("\n");
          
        return;*/
        
        p.x = map(p.x, TS_MINX, TS_MAXX, 0, WIDTH);
        p.y = map(p.y, TS_MAXY, TS_MINY, 0, HEIGHT);
         
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        // Serial.print("X = "); Serial.print(p.x); Serial.print("\tY = "); Serial.print(p.y); Serial.print("\n");       

        if (p.y > 190) {
          if (p.y >= 200 && p.y <= 230) {
            if (p.x >= 10 && p.x <= 35) {
              if (startKey > C1) {
                startKey -= octaveLen;
                tft.fillTriangle(205, 25, 215, 10, 225, 25, GREEN);
              }
            }
            else if (p.x >= 80 && p.x <= 105) {
              if (startKey < C6) {
                startKey += octaveLen;  
                tft.fillTriangle(205, 80, 215, 95, 225, 80, GREEN);
              }
            }
            int whiteIdx = startKey;
            for (int i=0; i<whiteKeysCount; i++) {
                while (musicNotesArray[whiteIdx].color != WHITE) {
                  whiteIdx++;
                }
                whiteKeysMap[i][GLOBAL_IDX] = whiteIdx;
                whiteKeysMap[i][FREQ] = musicNotesArray[whiteIdx++].freq;
            }
          octaveNumber = startKey / octaveLen;
  
            updateOctaveInfo();
            delay(500);
            
          tft.fillTriangle(205, 25, 215, 10, 225, 25, MAGENTA);
          tft.fillTriangle(205, 80, 215, 95, 225, 80, MAGENTA);
          }
        }

        else if (p.y > 80)
        {            
            //Key might be a sharp -- split Y axis into black and white keys
            int idx = p.x / (whiteWidth + whiteGap);

            int bLeft = idx * (whiteWidth+whiteGap) + blackOff/2;
            int bRight = idx * (whiteWidth+whiteGap) + blackOff;
            
            if (p.x < bLeft - 2) {
              // czarny na lewo
              musicNote mN = musicNotesArray[whiteKeysMap[idx][GLOBAL_IDX] - 1];
              if (mN.color == BLACK) {
                currentFreq = mN.freq; 
                updateNoteInfo(whiteKeysMap[idx][GLOBAL_IDX] - 1);
                tone(speaker, mN.freq, duration);
              }
              else {
                 if ((p.x % 32 >= 30 || p.x % 32 <= 2) 
                      && currentFreq != -1) {
                    tone(speaker, currentFreq, duration);
                }
                else {
                  // jednak white              
                  currentFreq = whiteKeysMap[idx][FREQ];  
                  updateNoteInfo(whiteKeysMap[idx][GLOBAL_IDX]);
                  tone(speaker, whiteKeysMap[idx][FREQ], duration);
                } 
              }
            }
            
            else if (p.x > bRight + 2) {
              // czarny na prawo
              musicNote mN = musicNotesArray[whiteKeysMap[idx][GLOBAL_IDX] + 1];
              if (mN.color == BLACK) {
                currentFreq = mN.freq;  
                updateNoteInfo(whiteKeysMap[idx][GLOBAL_IDX] + 1);
                tone(speaker, mN.freq, duration);  
              }
              else {
                if ((p.x % 32 >= 30 || p.x % 32 <= 2) 
                      && currentFreq != -1) {
                    tone(speaker, currentFreq, duration);
                }
                else {
                  // jednak white
                  currentFreq = whiteKeysMap[idx][FREQ];  
                  updateNoteInfo(whiteKeysMap[idx][GLOBAL_IDX]);
                  tone(speaker, whiteKeysMap[idx][FREQ], duration);
                }    
              }
            }
            
            else if (p.x > bLeft + 2 && p.x < bRight - 2) {
              // white
              currentFreq = whiteKeysMap[idx][FREQ];
              updateNoteInfo(whiteKeysMap[idx][GLOBAL_IDX]);
              tone(speaker, whiteKeysMap[idx][FREQ], duration);
            }
            else {
              tone(speaker, currentFreq, duration);
            }
           
        }
        else
        {
            if ((p.x % 32 >= 30 || p.x % 32 <= 2) 
                  && currentFreq != -1) {
                tone(speaker, currentFreq, duration);
            }
            else {
              int idx = p.x / (whiteWidth + whiteGap);
              //paintKey(idx, WHITE, MAGENTA);              
              currentFreq =  whiteKeysMap[idx][FREQ];
              updateNoteInfo(whiteKeysMap[idx][GLOBAL_IDX]);
              tone(speaker, whiteKeysMap[idx][FREQ], duration);
             }
        }
        
    }
}

void initDisplay()
{
    tft.reset();
    tft.begin(0x9341);
    //tft.setRotation(2);
}

