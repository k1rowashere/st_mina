#include "headers/main.h"
#include <MCUFRIEND_kbv.h>

// draw action
void draw_action(Actions action, int start_pos = 0)
{
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(start_pos + 18, 24 * 2);
    // clear action area
    tft.fillRect(start_pos + 1, 24 * 2, SCREEN_WIDTH / 2 - 2, 24, TFT_BLACK);
    switch (action)
    {
    case IDLE:
        tft.print("Idle");
        break;
    case FILLING:
        tft.setTextColor(TFT_GREEN);
        tft.print("Filling");
        break;
    case HOMMING:
        tft.setTextColor(TFT_YELLOW);
        tft.print("Homing");
        break;
    case MOVING:
        tft.setTextColor(TFT_YELLOW);
        tft.print("Moving");
        break;
    case STOPPED:
        tft.setTextColor(TFT_RED);
        tft.print("Stopped");
        break;
    }
}

// draw set fill volume
void draw_fill_v(long current_pos, int start_pos = 0)
{
    tft.setCursor(start_pos + 18, 24 * 3);
    tft.setTextColor(TFT_LIGHTGREY);
    tft.print("Fill V: ");
    // redraw the volume if it has changed
    tft.fillRect(tft.getCursorX(), tft.getCursorY(), abs(SCREEN_WIDTH / 2 - tft.getCursorX()), 18, TFT_BLACK);
    tft.print(steps_to_volume(current_pos));
    tft.print(" ml");
}

// draw + / - buttons
void draw_plus_minus_buttons(int start_pos = 0)
{
    int x = start_pos + SCREEN_WIDTH / 4, y = SCREEN_HEIGHT / 2 - 25;

    tft.setTextColor(TFT_WHITE);
    tft.drawRoundRect(x - 75, y, 50, 50, 10, TFT_WHITE);
    tft.drawRoundRect(x + 25, y, 50, 50, 10, TFT_WHITE);
    tft.setCursor(x - 50 - 6, y + 25 - 8);
    tft.print("-");
    tft.setCursor(x + 50 - 6, y + 25 - 8);
    tft.print("+");
}
