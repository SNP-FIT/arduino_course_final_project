#include "Global.h"

void btnHomeScreen(int buttonIndex) {
  g_state = MenuScreen;
}


void displayHomeScreen() {
  char datestring[11];
  sprintf(datestring, "%02u/%02u/%04u", g_now.Day(), g_now.Month(), g_now.Year());
  g_lcd.setCursor(0, 0);
  g_lcd.print(datestring);

  char timestring[16];
  sprintf(timestring, "%02u:%02u:%02u", g_now.Hour(), g_now.Minute(), g_now.Second());
  g_lcd.setCursor(0, 1);
  g_lcd.print(timestring);

  g_lcd.setCursor(12, 0);
  g_lcd.print("PUMP");

  if (g_relayState == 1) {
    g_lcd.setCursor(13, 1);
    g_lcd.print(" ON");
  } else {
    g_lcd.setCursor(13, 1);
    g_lcd.print("OFF");
  }
}