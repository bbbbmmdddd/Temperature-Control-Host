#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <max6675.h>
#include "color.h"
#include <cmath>
#define EC11_A PB0
#define EC11_B PB1

TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite spr = TFT_eSprite(&tft);
MAX6675 thermocouple(PB13,PA3,PB14);

byte loadNum = 6;
unsigned long previousMillis = 0;//保存上次读取的时间
const long interval = 200;
const int BUZZER_PIN = PB9;
int anjian_readnum= 0;
int ec11_e_num = 0;
int ec11_astate = 0;
int ec11_astate_last = 0;
int stop_time = 0;
int time_h_main = -114514;
int time_m_main = -114514;
float time_h = 0;
float time_m = 0; 
float wd_num = 0;
float ec11 = 30;
float setup_wd = 30;
bool bl = 0;
bool jm = 0;
bool stop = 0;

void times();
void wd_setup();
void wd();
void EC11_READ(){
  ec11_astate = digitalRead(EC11_A);
  if(ec11_astate != ec11_astate_last){
    if(digitalRead(EC11_B) != ec11_astate && ec11 >= -50){
      if(bl==1 && jm == 0){
        ec11 -= 10;
        wd_setup();
      }
      else if(jm == 0){
        ec11 -= 0.5;
        wd_setup();
      }
      else if(jm == 1 && bl == 0){
        time_h -= 0.5;
        times();
      }
      else if(jm == 1 && bl == 1){
        time_m -= 0.5;
        times();
      }
    }
    else if(ec11 <= 600){
      if(bl==1 && jm == 0){
        ec11 += 10;
        wd_setup();
      }
      else if(jm == 0){
        ec11 += 0.5;
        wd_setup();
      }
      else if(jm == 1 && bl == 0){
        time_h += 0.5;
        times();
      }
      else if(jm == 1 && bl == 1){
        time_m += 0.5;
        times();
      }
    }
  ec11_astate_last = ec11_astate;
  }
}

void buzzer_() {
  ec11_e_num = 0;
  if(digitalRead(PA4) == LOW){
    delay(200);
    if(digitalRead(PA4) == LOW){
      bl = 1;
      while(digitalRead(PA4) == LOW){
        EC11_READ();
      }
      bl = 0;
    }
    else{
      tone(BUZZER_PIN,988);
      delay(100);
      noTone(BUZZER_PIN);
      if(jm == 0){
        jm = 1;
        time_h = 0;
        time_m = 0;
        times();
      }
      else{
        jm = 0;
        if(time_h > 0 || time_m > 0){
        	time_h_main = time_h;
        	time_m_main = time_m;
          unsigned long stop_open = millis();
        	stop_time = stop_open + ((time_h_main * 60) + time_m_main) *60*1000;
    	  }
    	  else{
    		  time_h_main = -114514;
    	  	time_m_main = -114514;
    		  stop = 0;
		    }
        wd_setup();
        wd();
      }
    }
  }
}

int Time(char dw){
	if(dw == 'h'){
    unsigned long stop_now = millis();
    if(stop_now < stop_time){
		  return (stop_time - stop_now)/1000/60/60%60;
    }
    else{
      return 0;
    }
	}
  else if(dw == 's'){
    unsigned long stop_now = millis();
    if(stop_now < stop_time){
		  return (stop_time - stop_now)/1000%60;
    }
  }
	else if(dw == 'm'){
    unsigned long stop_now = millis();
    if(stop_now < stop_time){
      if((stop_time - stop_now)/1000 <= 0 && (time_m_main > 0 || time_h_main > 0)){
        stop = 1;
        digitalWrite(PB11, LOW);
    	  digitalWrite(PC13, LOW);
        tone(BUZZER_PIN,988);
        delay(500);
        noTone(BUZZER_PIN);
        delay(500);
        tone(BUZZER_PIN,988);
        delay(500);
        noTone(BUZZER_PIN);
        tone(BUZZER_PIN,988);
        delay(2000);
        noTone(BUZZER_PIN);
      }
		  return (stop_time - stop_now)/1000/60%60;
    }
    else{
      return 0;
    }
	}
  return 0;
}

void wd(){
  spr.setColorDepth(8);
  spr.createSprite(160,64);
  spr.fillSprite(TFT_BLACK);
  spr.setTextSize(3);
  spr.setTextColor(TFT_GREEN);
  spr.setTextDatum(ML_DATUM);
  spr.drawString("Now:",3,15);
  spr.drawString(String(wd_num),5,40);
  spr.drawString(" C",100,40);
  if(time_h_main > 0 || (time_m_main > 0)){
    spr.setTextSize(2);
    spr.setTextColor(TFT_SKYBLUE);
    spr.setTextDatum(ML_DATUM);
    if(Time('h') < 10 && Time('m') < 10){
      spr.drawString(String(Time('h')),90,15);
    }
    else if(Time('h') < 10 && Time('m') > 10 || Time('h') > 10 && Time('m') < 10){
      spr.drawString(String(Time('h')),80,15);
    }
    else{
      spr.drawString(String(Time('h')),70,15);
    }
    spr.setTextDatum(ML_DATUM);
    if(Time('m') < 10){
      spr.drawString(":",101,15);
      spr.drawString(String(Time('m')),110,15);
    }
    else{
      spr.drawString(":",91,15);
      spr.drawString(String(Time('m')),100,15);
    }
    spr.drawString(":",121,15);
    spr.drawString(String(Time('s')),130,15);
  }
  else{
    spr.setTextColor(TFT_SKYBLUE);
    spr.setTextDatum(ML_DATUM);
    spr.setTextSize(2);
    spr.drawString("NULL",90,15);
  }
  spr.pushSprite(0, 0);
  spr.deleteSprite();
}

void wd_setup(){
  spr.createSprite(160,64);
  spr.fillSprite(TFT_BLACK);
  spr.setTextColor(TFT_RED);
  spr.setTextDatum(ML_DATUM);
  spr.setTextSize(3);
  spr.drawString("Setup:",3,15);
  spr.drawString(String(ec11),3,40);
  spr.drawString(" C",100,40);
  spr.pushSprite(0, 64);
  spr.deleteSprite();
}

void times(){
  spr.setColorDepth(8);
  spr.createSprite(160,64);
  spr.fillSprite(TFT_BLACK);
  spr.setTextSize(3);
  spr.setTextColor(TFT_SKYBLUE);
  spr.setTextDatum(ML_DATUM);
  spr.drawString("Time:",3,15);
  spr.setTextColor(TFT_ORANGE);
  spr.drawString(String(time_h),5,40);
  spr.setTextColor(TFT_SKYBLUE);
  spr.drawString(" h",100,40);
  spr.pushSprite(0, 0);
  spr.deleteSprite();

  spr.createSprite(160,64);
  spr.fillSprite(TFT_BLACK);
  spr.setTextColor(TFT_SKYBLUE);
  spr.setTextDatum(ML_DATUM);
  spr.setTextSize(3);
  spr.setTextColor(TFT_ORANGE);
  spr.drawString(String(time_m),3,20);
  spr.setTextColor(TFT_SKYBLUE);
  spr.drawString("min",100,20);
  spr.pushSprite(0, 64);
  spr.deleteSprite();
}

void pid(){
	if(wd_num <= ec11){
    	digitalWrite(PB11, HIGH);
    	digitalWrite(PC13, HIGH);
  	}
  	else{
    	digitalWrite(PB11, LOW);
    	digitalWrite(PC13, LOW);
  	}	
}

void setup() {
  pinMode(PB11, OUTPUT);
  pinMode(PC13, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PA4, INPUT_PULLUP);
  pinMode(EC11_A, INPUT_PULLUP);
  pinMode(EC11_B, INPUT_PULLUP);
  ec11_astate_last = digitalRead(EC11_A);
  tft.begin();
  tft.setRotation(3);//横屏
  tft.fillScreen(TFT_BLACK);//黑色
  tone(BUZZER_PIN,532);
  delay(100);
  tone(BUZZER_PIN,587);
  delay(100);
  tone(BUZZER_PIN,659);
  delay(300);
  noTone(BUZZER_PIN);
  wd();
  wd_setup();
}

void loop() {
unsigned long currentMillis = millis();  // 获取当前时间
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;//保存下一次读取的时间
    wd_num = thermocouple.readCelsius() - 6;
    if(jm == 0){
      wd();
    }
  }
  if(stop == 0){
  	pid();
  }
  EC11_READ();
  buzzer_();
}