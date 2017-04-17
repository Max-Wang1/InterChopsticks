/*
Columbia University Art of Engineering Poject
Coding part done by Zichuan Wang and Lillian Zha
Other team member: Ye Shao and Dongyi Wang


Based on Sample code provided by Limor Fried, Adafruit Industries
GitHub link: https://github.com/adafruit/ST7565-LCD
*/
#include "ST7565.h"
const int ledPin =  13;    // LED connected to digital pin 13
const int buttonPinR = 3;     // the number of the pushbutton pin
const int buttonPinL = 4;     // the number of the pushbutton pin
int buttonStateR = 0;  
int buttonStateL = 0;  

// the LCD backlight is connected up to a pin so you can turn it on & off
#define BACKLIGHT_LED 4

// pin 9 - Serial data out (SID)
// pin 8 - Serial clock out (SCLK)
// pin 7 - Data/Command select (RS or A0)
// pin 6 - LCD reset (RST)
// pin 5 - LCD chip select (CS)
ST7565 glcd(9, 8, 7, 6, 5);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

// a bitmap of a 16x16 fruit icon
/*
const static unsigned char __attribute__ ((progmem)) logo16_glcd_bmp[]={
0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 
0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };
*/
const static unsigned char __attribute__ ((progmem)) logo16_glcd_bmp[]={
0xf0, 0xf0, };
// The setup() method runs once, when the sketch starts
void setup()   {                
  Serial.begin(9600);

#ifdef __AVR__
  Serial.print(freeRam());
#endif

  // turn on backlight
  pinMode(BACKLIGHT_LED, OUTPUT);
  digitalWrite(BACKLIGHT_LED, LOW);

  // initialize and set the contrast to 0x18
  glcd.begin(0x18);
  glcd.clear();

  // draw a string at location (0,0)
  glcd.drawstring(0, 0, "Chopsticks is No.1 !!Welcome to           InterChopsticks!!!");
  glcd.display();
  delay(5000);
  glcd.clear();
  pinMode(buttonPinL, INPUT_PULLUP);
  pinMode(buttonPinR, INPUT_PULLUP);
  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define DELTAX 2
#define DELTAX2 3

void loop()                     
{
  uint8_t w=16;
  uint8_t h=16;
  uint8_t icons[NUMFLAKES][3];
  uint8_t box[4];
  uint8_t wid =10;
  uint8_t he =4; 
  uint8_t loopCount=0;
  uint8_t screw =0;
  String a = "";
  randomSeed(666);     // whatever seed

  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(128);
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(2) + 1;
  }
  
  box[XPOS]=64;
  box[YPOS]=60;
  box[DELTAX]=0;
  box[DELTAX2]=0;
  while (1) {
    
    
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
   
      glcd.drawrect(icons[f][XPOS], icons[f][YPOS], 2, h, BLACK);
    }
    //Serial.println("1");
    glcd.drawrect( box[XPOS], box[YPOS],  wid,  he,BLACK);
    glcd.display();
    //delay(10);
    buttonStateR = digitalRead(buttonPinR);
    delay(10);
    buttonStateL = digitalRead(buttonPinL);
    
    if(buttonStateR == HIGH){
        box[DELTAX]=10;
        Serial.println("R");
    }
    else{
      box[DELTAX]=0;
    }
    delay(10);
    if(buttonStateL == HIGH){
        box[DELTAX2]=10;
        Serial.println("L");
    }
    else{
      box[DELTAX2]=0;
    }
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      glcd.drawrect(icons[f][XPOS], icons[f][YPOS], 2, h, 0);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      
      // if its gone, reinit
      if (icons[f][YPOS] > 64) {
        icons[f][XPOS] = random(128);
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
    
    glcd.drawrect( box[XPOS], box[YPOS],  wid,  he,0);

    box[XPOS] += box[DELTAX];
    box[XPOS] -= box[DELTAX2];
    if (box[XPOS] > 117) {
        box[XPOS] = 1;
      }
      else if (box[XPOS] <2) {
        box[XPOS] = 117;
      }
    
    //Serial.println(freeRam());
    loopCount=loopCount+1;
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      if(icons[f][YPOS]>44){
        if(icons[f][XPOS]>box[XPOS]-2&&icons[f][XPOS]<box[XPOS]+10){
          screw =1 ;
          break;
        }
      }
      
     
    }
    if(screw==1){
      break;
    }
  }
   glcd.clear();

  glcd.drawstring(0, 0, "                                               You lose!");
  glcd.display();
   delay(3000);
  glcd.clear();
  glcd.drawstring(0, 0, "    The score is...");
  glcd.display();
  delay(3000);
  glcd.clear();
  int aaa = loopCount/5;

  char aaaa[200];
  String(aaa, DEC).toCharArray(aaaa, 200);
  glcd.drawstring(0,0,aaaa);
  glcd.display();

  delay(3000);
  glcd.clear();
  glcd.drawstring(0, 0, "Chopsticks is No.1 !!Welcome to           InterChopsticks!!!");
  glcd.display();
  delay(5000);
  glcd.clear();
  
  
 }

#ifdef __AVR__
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 
#endif



void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {

}


void testdrawchar(void) {
  for (uint8_t i=0; i < 168; i++) {
    glcd.drawchar((i % 21) * 6, i/21, i);
  }    
}




