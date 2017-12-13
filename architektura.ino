#include "PCF8574.h"
#include <Wire.h>

const uint32_t neighbors[24] = {
  0x00000032, 0x00000065, 0x000000ca, 0x00000084,
  0x00000121, 0x00000353, 0x000006a6, 0x00000c4c,
  0x00003230, 0x00006560, 0x0000cac0, 0x00008480,
  0x00012100, 0x00035300, 0x0006a600, 0x000c4c00,
  0x00323000, 0x00656000, 0x00cac000, 0x00848000,
  0x00210000, 0x00530000, 0x00a60000, 0x004c0000
};

const uint32_t indexes[24] = {
  0x00000001, 0x00000002, 0x00000004, 0x00000008,
  0x00000010, 0x00000020, 0x00000040, 0x00000080,
  0x00000100, 0x00000200, 0x00000400, 0x00000800,
  0x00001000, 0x00002000, 0x00004000, 0x00008000,
  0x00010000, 0x00020000, 0x00040000, 0x00080000,
  0x00100000, 0x00200000, 0x00400000, 0x00800000,
};

int people_fucking_dying(int min_fields_no, uint32_t grid) {
  if (!grid) {
    return 0;
  }
  int stack[24 * 24];
  int visited[24] = { 0 };
  for (uint32_t i = 0; i < 24; i++) if ((indexes[i] & grid) && !visited[i]) {
    stack[0] = i;
    int stack_len = 1;
    int count = 0;
    while (stack_len--) if (!visited[stack[stack_len]]) {
      int node = stack[stack_len];
      visited[node] = 1;
      uint32_t next = grid & neighbors[stack[stack_len]];
      for (int j = 0; j < 24 && next; j++) {
        if (next & 0x00000001) {
          if (!visited[j]) {
            stack[stack_len++] = j;
          }
        }
        next >>= 1;
      }
      if (++count >= min_fields_no) {
        return 1;
      }
    }
  }

  return 0;
}

PCF8574 buttons[3];
PCF8574 leds[3];
int panic_mode;
int panic_mode_step;
const int panic_mode_tick = 200;
const int tick = 100;

void setup()
{
  Serial.begin(9600);
  // TODO adresy do ustawienia
  buttons[0].begin(0x38);
//  buttons[1].begin(0x00);
//  buttons[2].begin(0x00);
  leds[0].begin(0x39);
//  leds[1].begin(0x00);
//  leds[2].begin(0x00);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i].pinMode(j, OUTPUT);
      buttons[i].pinMode(j, INPUT);
    }
  }
}

void loop()
{
  uint8_t values[3];
  uint32_t grid = 0;
  for (int i = 2; i >= 0; i--) {
    values[i] = buttons[i].read();
//    values[i] &= 0x03;
//    Serial.println(values[i], BIN);
    grid = (grid << 8) | values[i];
  }
  if (people_fucking_dying(5, grid)) {
    if (!panic_mode) {
      panic_mode = 1;
      panic_mode_step = 0;
    }
    for (int i = 2; i >= 0; i--) {
      leds[i].write(panic_mode_step * 2 >= panic_mode_tick ? 0xff : 0x00);
    }
    panic_mode_step = (panic_mode_step + tick) % panic_mode_tick;
  } else {
    panic_mode = 0;
    for (int i = 2; i >= 0; i--) {
      leds[i].write(values[i]);
    }
  }
  delay(tick);
}

