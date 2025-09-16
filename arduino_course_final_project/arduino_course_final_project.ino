#include <RtcDS1302.h>
#include <TimerOne.h>


#include "Global.h"
#include "Pins.h"

#include "HomeScreen.h"
#include "MenuScreen.h"
#include "SetTimeScreen.h"

ThreeWire myWire(RTC_DAT, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> Rtc(myWire);


int timer1Flag = false;
int buttonPins[] = { BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3 };
bool lastButtonStates[] = { 1, 1, 1, 1 };
unsigned long lastButtonPressed[] = { 0, 0, 0, 0 };
bool isSleep = 0;
unsigned long lastInteract = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  loadSchedule();
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(timerISR);
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  g_now = Rtc.GetDateTime();
  if (g_now < compiled) {
    Rtc.SetDateTime(compiled);
  }
  Rtc.SetDateTime(compiled);
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(LCD_BL, OUTPUT);
  pinMode(RELAY_OUT, OUTPUT);
  pinMode(BUZZER_OUT, OUTPUT);

  digitalWrite(LCD_BL, HIGH);
  g_lcd.begin(16, 2);
  refreshScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (timer1Flag) {
    g_now = Rtc.GetDateTime();
    if (g_state == HomeScreen) {
      refreshScreen();
    }

    long currentSod = (long)g_now.Hour() * 3600 + (long)g_now.Minute() * 60 + (long)g_now.Second();
    for (int i = 0; i < 5; i++) {
      if (currentSod == g_on_time[i]) {
        g_relayState = 1;
        break;
      }
    }

    for (int i = 0; i < 5; i++) {
      if (currentSod == g_off_time[i]) {
        g_relayState = 0;
        break;
      }
    }

    if (millis() - lastInteract >= 5000) {
      isSleep = 1;
    }

    timer1Flag = false;
  }


  for (int i = 0; i < 4; i++) {
    int currentButtonState = digitalRead(buttonPins[i]);
    unsigned long currentTime = millis();
    if (currentButtonState == 0 && lastButtonStates[i] == 1 && currentTime - lastButtonPressed[i] > 200) {
      bip();
      if (isSleep) {
        isSleep = 0;
      } else {
        g_lcd.clear();
        if (g_state == HomeScreen) {
          btnHomeScreen(i);
        } else if (g_state == MenuScreen) {
          btnMenuScreen(i);
        } else if (g_state == SetOnTimeScreen || g_state == SetOffTimeScreen) {
          btnSetTimeScreen(i);
        }
        refreshScreen();
        lastButtonPressed[i] = currentTime;
      }
      lastInteract = currentTime;
    }
    lastButtonStates[i] = currentButtonState;
  }
  digitalWrite(RELAY_OUT, g_relayState);
  digitalWrite(LCD_BL, !isSleep);
}

void timerISR() {
  timer1Flag = true;
}
