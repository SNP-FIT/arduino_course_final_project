#include "Arduino.h"
#include "Global.h"
#include "Pins.h"

int currentSlotIndex = 0;
int isEditing = 0;
int editingTime[] = { 0, 0, 0 };  //h,m,s
int currentEditingIndex = 0;

int decrementDigit(int value, int maxValue, int digit);
int incrementDigit(int value, int maxValue, int digit);

void btnSetTimeScreen(int buttonIndex) {
  if (buttonIndex == 0) {
    if (isEditing) {
      if (currentEditingIndex == 0) {
        editingTime[2] = decrementDigit(editingTime[2], 9, 1);
      } else if (currentEditingIndex == 1) {
        editingTime[2] = decrementDigit(editingTime[2], 5, 10);
      } else if (currentEditingIndex == 2) {
        editingTime[1] = decrementDigit(editingTime[1], 9, 1);
      } else if (currentEditingIndex == 3) {
        editingTime[1] = decrementDigit(editingTime[1], 5, 10);
      } else if (currentEditingIndex == 4) {
        editingTime[0] = decrementDigit(editingTime[0], 9, 1);
      } else if (currentEditingIndex == 5) {
        if (editingTime[0] % 10 > 3) {
          editingTime[0] = decrementDigit(editingTime[0], 1, 10);
        } else {
          editingTime[0] = decrementDigit(editingTime[0], 2, 10);
        }
      }
    } else {
      if (currentSlotIndex == 0) {
        currentSlotIndex = 4;
      } else {
        currentSlotIndex--;
      }
    }
  } else if (buttonIndex == 1) {
    if (isEditing) {
      if (currentEditingIndex == 0) {
        editingTime[2] = incrementDigit(editingTime[2], 9, 1);
      } else if (currentEditingIndex == 1) {
        editingTime[2] = incrementDigit(editingTime[2], 5, 10);
      } else if (currentEditingIndex == 2) {
        editingTime[1] = incrementDigit(editingTime[1], 9, 1);
      } else if (currentEditingIndex == 3) {
        editingTime[1] = incrementDigit(editingTime[1], 5, 10);
      } else if (currentEditingIndex == 4) {
        editingTime[0] = incrementDigit(editingTime[0], 9, 1);
      } else if (currentEditingIndex == 5) {
        if (editingTime[0] % 10 > 3) {
          editingTime[0] = incrementDigit(editingTime[0], 1, 10);
        } else {
          editingTime[0] = incrementDigit(editingTime[0], 2, 10);
        }
      }
    } else {
      if (currentSlotIndex == 4) {
        currentSlotIndex = 0;
      } else {
        currentSlotIndex++;
      }
    }
  } else if (buttonIndex == 2) {
    if (isEditing) {
      isEditing = 0;
    } else {
      g_state = MenuScreen;
      currentSlotIndex = 0;
      g_lcd.noBlink();
    }

  } else {
    if (!isEditing) {
      unsigned long startTimestamp = millis();
      unsigned long lastTimestamp = 0;
      bool isLongPressed = 0;
      g_lcd.clear();
      g_lcd.noBlink();
      while (digitalRead(BUTTON_3) == 0) {
        unsigned long currentTimestamp = millis();
        if (currentTimestamp - startTimestamp > 300) {
          g_lcd.setCursor(0, 0);
          g_lcd.print("Deleting");
          g_lcd.setCursor(0, 1);
          g_lcd.print("[");
          g_lcd.setCursor(15, 1);
          g_lcd.print("]");
          int cursorPos = (currentTimestamp - (startTimestamp + 300)) / (2000 / 14);
          g_lcd.setCursor(cursorPos + 1, 1);
          g_lcd.print("=");
        }
        if (currentTimestamp - startTimestamp > 2300) {
          isLongPressed = 1;
          break;
        }
        lastTimestamp = currentTimestamp;
      }
      g_lcd.clear();
      g_lcd.blink();

      if (isLongPressed) {
        if (g_state == SetOnTimeScreen) {
          g_on_time[currentSlotIndex] = -1;
        } else {
          g_off_time[currentSlotIndex] = -1;
        }
      }

      if (lastTimestamp - startTimestamp < 300) {
        isEditing = 1;
        currentEditingIndex = 0;
        editingTime[0] = 0;
        editingTime[1] = 0;
        editingTime[2] = 0;
      }

    } else {
      if (currentEditingIndex == 5) {
        isEditing = 0;
        if (g_state == SetOnTimeScreen) {
          g_on_time[currentSlotIndex] = (long)editingTime[0] * 3600 + (long)editingTime[1] * 60 + (long)editingTime[2];
        } else {
          g_off_time[currentSlotIndex] = (long)editingTime[0] * 3600 + (long)editingTime[1] * 60 + (long)editingTime[2];
        }
        saveSchedule();
      }
      currentEditingIndex++;
    }
  }
};


void displaySetTimeScreen() {
  Serial.println(currentEditingIndex);
  g_lcd.setCursor(0, 0);
  if (g_state == SetOnTimeScreen) {
    g_lcd.print("ON");
  } else {
    g_lcd.print("OFF");
  }

  g_lcd.setCursor(7, 0);
  g_lcd.print("1");
  g_lcd.setCursor(9, 0);
  g_lcd.print("2");
  g_lcd.setCursor(11, 0);
  g_lcd.print("3");
  g_lcd.setCursor(13, 0);
  g_lcd.print("4");
  g_lcd.setCursor(15, 0);
  g_lcd.print("5");

  if (isEditing) {
    char timestring[16];
    sprintf(timestring, "%02u:%02u:%02u", editingTime[0], editingTime[1], editingTime[2]);
    g_lcd.setCursor(8, 1);
    g_lcd.print(timestring);
  } else {

    long currentTime;
    if (g_state == SetOnTimeScreen) {
      currentTime = g_on_time[currentSlotIndex];
    } else {
      currentTime = g_off_time[currentSlotIndex];
    }
    if (currentTime < 0) {
      g_lcd.setCursor(9, 1);
      g_lcd.print("NO DATA");
    } else {
      int hour = currentTime / 3600;
      int minute = (currentTime / 60) % 60;
      int second = currentTime % 60;
      char timestring[16];
      sprintf(timestring, "%02u:%02u:%02u", hour, minute, second);
      g_lcd.setCursor(8, 1);
      g_lcd.print(timestring);
    }
  }

  if (isEditing) {
    if (currentEditingIndex == 0) {
      g_lcd.setCursor(15, 1);
    } else if (currentEditingIndex == 1) {
      g_lcd.setCursor(14, 1);
    } else if (currentEditingIndex == 2) {
      g_lcd.setCursor(12, 1);
    } else if (currentEditingIndex == 3) {
      g_lcd.setCursor(11, 1);
    } else if (currentEditingIndex == 4) {
      g_lcd.setCursor(9, 1);
    } else if (currentEditingIndex == 5) {
      g_lcd.setCursor(8, 1);
    }
  } else {
    g_lcd.setCursor(currentSlotIndex * 2 + 7, 0);  // 0-4
    g_lcd.blink();
  }
};

int decrementDigit(int value, int maxValue, int digit) {
  if (value <= digit - 1) {
    value = value + maxValue * digit;
  } else {
    value = value - digit;
  }
  return value;
}

int incrementDigit(int value, int maxValue, int digit) {
  if (value >= maxValue * digit) {
    value = value - maxValue * digit;
  } else {
    value = value + digit;
  }
  return value;
}
