//////////////////////////////////////////////
//     2.8" TOUCH SCREEN TIC TAC TOE        //
//                                          //
//           http://www.educ8s.tv           //
/////////////////////////////////////////////

#include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>
#include <stdint.h>

const int speaker=19; //Pin to which the speaker is connected -- use 19 (this is the A5 pin, as a digital output) for Uno or 53 for Mega

//Define frequencies for each of the possible notes
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587

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

#define TS_MINX 200
#define TS_MINY 190
#define TS_MAXX 970
#define TS_MAXY 915

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

void setup() {
    Serial.begin(9600);
    Serial.print("Starting...\n");
    randomSeed(analogRead(0));

    initDisplay();
    drawStartScreen();
}

void loop()
{
    TSPoint p = ts.getPoint();  //Get touch point

    if (p.z > ts.pressureThreshhold) {

        p.x = map(p.x, TS_MAXX, TS_MINX, 0, 300);
        p.y = map(p.y, TS_MAXY, TS_MINY, 0, 250);

        /*Serial.print("X = "); Serial.print(p.x);
        Serial.print("\tY = "); Serial.print(p.y);
        Serial.print("\n");*/
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);

        if(p.y > 120)
        {
            tft.fillCircle(250-p.y,p.x,5,MAGENTA);
            //Key might be a sharp -- split Y axis into black and white keys
            if (p.x>=0 && p.x<=16) Serial.print("D5\n");//tone(speaker,NOTE_D5, duration);
            if (p.x>=16 && p.x<=38) Serial.print("CS5\n");//tone(speaker,NOTE_CS5, duration);
            if (p.x>=38 && p.x<=62) Serial.print("C5\n");//tone(speaker,NOTE_C5, duration);
            if (p.x>=62 && p.x<=80) Serial.print("B4\n");//tone(speaker,NOTE_B4, duration);
            if (p.x>=80 && p.x<=102) Serial.print("AS4\n");//tone(speaker,NOTE_AS4, duration);
            if (p.x>=102 && p.x<=112) Serial.print("A4\n");//tone(speaker,NOTE_A4, duration);
            if (p.x>=112 && p.x<=134) Serial.print("GS4\n");//tone(speaker,NOTE_GS4, duration);
            if (p.x>=134 && p.x<=144) Serial.print("G4\n");//tone(speaker,NOTE_G4, duration);
            if (p.x>=144 && p.x<=166) Serial.print("FS4\n");//tone(speaker,NOTE_FS4, duration);
            if (p.x>=166 && p.x<=190) Serial.print("F4\n");//tone(speaker,NOTE_F4, duration);
            if (p.x>=192 && p.x<=210) Serial.print("E4\n");//tone(speaker,NOTE_E4, duration);
            if (p.x>=210 && p.x<=232) Serial.print("DS4\n");//tone(speaker,NOTE_DS4, duration);
            if (p.x>=232 && p.x<=241) Serial.print("D4\n");//tone(speaker,NOTE_D4, duration);
            if (p.x>=241 && p.x<=263) Serial.print("CS4\n");//tone(speaker,NOTE_CS4, duration);
            if (p.x>=263 && p.x<=286) Serial.print("C4\n");//tone(speaker,NOTE_C4, duration);
            if (p.x>=288 && p.x<=305) Serial.print("B3\n");//tone(speaker,NOTE_B3, duration);
            if (p.x>=305 && p.x<=327) Serial.print("AS3\n");//tone(speaker,NOTE_AS3, duration);
        }
        else
        {
            //Key is too far along X axis to be sharp -- split Y axis into white keys only
            tft.fillCircle(250-p.y,p.x,5,MAGENTA);
            if (p.x>=0 && p.x<=30) Serial.print("D5\n");//tone(speaker,NOTE_D5, duration);
            if (p.x>=32 && p.x<=62) Serial.print("C5\n");//tone(speaker,NOTE_C5, duration);
            if (p.x>=64 && p.x<=94) Serial.print("B4\n");//tone(speaker,NOTE_B4, duration);
            if (p.x>=96 && p.x<=126) Serial.print("A4\n");//tone(speaker,NOTE_A4, duration);
            if (p.x>=128 && p.x<=158) Serial.print("G4\n");//tone(speaker,NOTE_G4, duration);
            if (p.x>=160 && p.x<=190) Serial.print("F4\n");//tone(speaker,NOTE_F4, duration);
            if (p.x>=192 && p.x<=222) Serial.print("E4\n");//tone(speaker,NOTE_E4, duration);
            if (p.x>=224 && p.x<=254) Serial.print("D4\n");//tone(speaker,NOTE_D4, duration);
            if (p.x>=256 && p.x<=286) Serial.print("C4\n");//tone(speaker,NOTE_C4, duration);
            if (p.x>=288 && p.x<=318) Serial.print("B3\n");//tone(speaker,NOTE_B3, duration);
        }
        //Wait for the tone to play -- you can adjust the scalar to make the gap between tones more pronounced -- and then kill the tone output to prepare for the next cycle
        //int wait=duration*1;
        //delay(wait);
        //noTone(speaker);
    }
}

void drawStartScreen()
{
    tft.fillScreen(BLACK);

    for (int i=0; i<10; i++) {
        tft.fillRect(0,i*32,240,30,WHITE);
    }
    //Draw the black keys on the keyboard
    tft.fillRect(0,16,140,22,BLACK);
    tft.fillRect(0,80,140,22,BLACK);
    tft.fillRect(0,112,140,22,BLACK);
    tft.fillRect(0,144,140,22,BLACK);
    tft.fillRect(0,210,140,22,BLACK);
    tft.fillRect(0,241,140,22,BLACK);
    tft.fillRect(0,305,140,22,BLACK);

}

void initDisplay()
{
    tft.reset();
    tft.begin(0x9341);
    tft.setRotation(2);
}

