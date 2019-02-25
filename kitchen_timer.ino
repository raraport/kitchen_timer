#include <M5Stack.h>

uint32_t perSecond = 0;
boolean Abtnpressed = M5.BtnA.isPressed(), Bbtnpressed = M5.BtnB.isPressed(), Cbtnpressed = M5.BtnC.isPressed();
boolean stopping = true, beeping = false;

byte Maxmm =99, Maxss=59;
byte mm =0, ss =0;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.fillScreen(0xffffff);
  perSecond = millis() + 1000;

  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setCursor(60, 200);
  M5.Lcd.print("M");
  M5.Lcd.setCursor(150, 200);
  M5.Lcd.print("S");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(240, 200);
  M5.Lcd.print("START");
  M5.Lcd.setCursor(240, 220);
  M5.Lcd.print("/STOP");

}

void draw_minutes(byte mm){
   M5.Lcd.setTextFont(7);
   M5.Lcd.setTextSize(2);
   M5.Lcd.setTextColor(BLACK, WHITE);
   M5.Lcd.setCursor( 0, 40);
   M5.Lcd.printf("[%02d]", mm);  
}


void draw_seconds(byte ss){
  M5.Lcd.setTextFont(7);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setCursor(180, 40);
  M5.Lcd.printf("[%02d]", ss);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(stopping){
    if(Abtnpressed){
      if(!(mm ==99)){
        mm++;
      }
      else{
        mm = 0;
      }
      draw_minutes(mm);
      M5.update();
    }
    if(Bbtnpressed){
      if(!(ss == 59)){
        ss++;
      }
      else if(ss ==60){
        mm +=1;
        ss = 0;
      }
      draw_minutes(mm);
      draw_seconds(ss);
      M5.update();
    }
    if(Abtnpressed && Bbtnpressed){
      mm =0;
      ss =0;
      draw_minutes(mm);
      draw_seconds(ss);
      M5.update();
    }
    if(Cbtnpressed){
      stopping = false;
      M5.update();
    }
  }

  if(perSecond <millis()){
    perSecond = millis() + 1000;
    
    if(!stopping){
      if(Cbtnpressed){
        stopping =true;
        M5.update();
     }
     ss--;
     if(ss ==0){
      if(mm == 0){
        stopping = true;
        beeping = true;
        if(beeping){
          M5.Speaker.beep();
          if(Cbtnpressed){
            beeping = false;
            M5.update();
          }
        }
      }
      mm--;
      ss =59;
      draw_minutes(mm);
      draw_seconds(ss);
     }
       // インジケーターの点滅
        if (ss % 2 && !hasStop) {
          M5.Lcd.setCursor( 150, 40);
          M5.Lcd.setTextColor(TFT_LIGHTGREY, TFT_LIGHTGREY);
          M5.Lcd.print(":");
          M5.Lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
        }
        else {
          M5.Lcd.setCursor( 150, 40);
          M5.Lcd.print(":");
        }
    }
  }
} 
