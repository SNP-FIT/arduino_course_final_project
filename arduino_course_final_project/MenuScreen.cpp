#include "Global.h"

char menu[3][20] = { " SET ON TIME", " SET OFF TIME", "     MANUAL" };
unsigned int currentMenuIndex = 0;
void btnMenuScreen(int buttonIndex) {
  if (buttonIndex == 0) {
    if (currentMenuIndex == 0) {
      currentMenuIndex = 2;
    } else {
      currentMenuIndex--;
    }
  } else if (buttonIndex == 1) {
    if (currentMenuIndex == 2) {
      currentMenuIndex = 0;
    } else {
      currentMenuIndex++;
    }
  } else if (buttonIndex == 2) {
    g_state = HomeScreen;
  } else {
    if (currentMenuIndex == 0) {
      g_state = SetOnTimeScreen;
    } else if (currentMenuIndex == 1) {
      g_state = SetOffTimeScreen;
    } else {
      g_relayState = !g_relayState;
    }
  }
}


void displayMenuScreen() {
  g_lcd.setCursor(0, 0);
  g_lcd.write(0b00111100);
  g_lcd.setCursor(15, 0);
  g_lcd.write(0b00111110);
  g_lcd.setCursor(1, 0);
  g_lcd.print(menu[currentMenuIndex]);
}