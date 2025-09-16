#include <LiquidCrystal.h>
#include <RtcDS1302.h>
#include <EEPROM.h>
#include "Pins.h"
#include "HomeScreen.h"
#include "MenuScreen.h"
#include "SetTimeScreen.h"
#include "Global.h"

enum UiState g_state = HomeScreen;
int g_relayState = 0;
LiquidCrystal g_lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
RtcDateTime g_now;
long g_on_time[] = { -1, -1, -1, -1, -1 };
long g_off_time[] = { -1, -1, -1, -1, -1 };

void bip(){
  digitalWrite(BUZZER_OUT, HIGH);
  delay(50);
  digitalWrite(BUZZER_OUT, LOW);
}

void loadSchedule() {
  int magicNumber = EEPROM.read(77);
  Serial.println(EEPROM.read(3));
  if(magicNumber != 99){
    saveSchedule();
    EEPROM.write(77, 99);
  }
  int address = 0;
  for (int i = 0; i < 5; i++) {
    EEPROM.get(address, g_on_time[i]);
    address = address + sizeof(long);
  }

  for (int i = 0; i < 5; i++) {
    EEPROM.get(address, g_off_time[i]);
    address = address + sizeof(long);
  }
}

void saveSchedule() {
  int address = 0;
  for (int i = 0; i < 5; i++) {
    EEPROM.put(address, g_on_time[i]);
    address = address + sizeof(long);
  }

  for (int i = 0; i < 5; i++) {
    EEPROM.put(address, g_off_time[i]);
    address = address + sizeof(long);
  }
}

void refreshScreen() {
  if (g_state == HomeScreen) {
    displayHomeScreen();
  } else if (g_state == MenuScreen) {
    displayMenuScreen();
  } else if (g_state == SetOnTimeScreen || g_state == SetOffTimeScreen) {
    displaySetTimeScreen();
  }
}