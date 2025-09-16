#ifndef GLOBAL_H
#define GLOBAL_H
#include <LiquidCrystal.h>
#include <RtcDS1302.h>

enum UiState {
  HomeScreen,
  MenuScreen,
  SetOnTimeScreen,
  SetOffTimeScreen,
};

extern enum UiState g_state;
extern int g_relayState;
extern LiquidCrystal g_lcd;
extern RtcDateTime g_now;
extern long g_on_time[];
extern long g_off_time[];
extern void refreshScreen();
extern void loadSchedule();
extern void saveSchedule();
extern void bip();
#endif