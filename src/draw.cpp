#include "headers/draw.h"

MCUFRIEND_kbv tft;

void Draw::init()
{
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(-45);
    tft.fillScreen(TFT_BLACK);
    tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, TFT_WHITE);
}

void Draw::title()
{
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(SCREEN_WIDTH / 4 - 19, 0);
    tft.print("(1)");
    tft.setCursor(SCREEN_WIDTH / 4 * 3 - 19, 0);
    tft.print("(2)");
}

void Draw::action(Actions action, uint16_t x_offset)
{
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(x_offset + 18, 24 * 2);
    // clear action area
    tft.fillRect(x_offset + 1, 24 * 2, SCREEN_WIDTH / 2 - 2, 24, TFT_BLACK);
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

void Draw::volume_indicator(uint32_t curr_vol_pos, uint32_t set_vol_pos, uint16_t x_offset)
{
    // draw current volume indicator
    tft.setCursor(x_offset + 18, 24 * 3);
    tft.setTextColor(TFT_LIGHTGREY);
    tft.print("Curr vol: ");
    tft.fillRect(tft.getCursorX(), tft.getCursorY(), abs(SCREEN_WIDTH / 2 - tft.getCursorX()), 18, TFT_BLACK);
    tft.print(steps_to_volume(curr_vol_pos));
    tft.print(" ml");

    // draw set volume indicator
    tft.setCursor(x_offset + 18, 24 * 4);
    tft.print("Set vol: ");
    tft.fillRect(tft.getCursorX(), tft.getCursorY(), abs(SCREEN_WIDTH / 2 - tft.getCursorX()), 18, TFT_BLACK);
    tft.print(steps_to_volume(set_vol_pos));
    tft.print(" ml");
}

void Draw::plus_minus_buttons(uint16_t x_offset)
{
    int x = x_offset + SCREEN_WIDTH / 4, y = SCREEN_HEIGHT / 2 - 25;

    tft.setTextColor(TFT_WHITE);
    tft.drawRoundRect(x - 75, y, 50, 50, 10, TFT_WHITE);
    tft.drawRoundRect(x + 25, y, 50, 50, 10, TFT_WHITE);
    tft.setCursor(x - 50 - 6, y + 25 - 8);
    tft.print("-");
    tft.setCursor(x + 50 - 6, y + 25 - 8);
    tft.print("+");
}

void Draw::apply_cancel_buttons()
{
    int x = SCREEN_WIDTH / 4, y = SCREEN_HEIGHT - 50;

    tft.setTextColor(TFT_WHITE);
    tft.drawRoundRect(x - 50, y, 100, 50, 10, TFT_WHITE);
    tft.drawRoundRect(3 * x - 50, y, 100, 50, 10, TFT_WHITE);
    tft.setCursor(x - 30, y + 25 - 8);
    tft.print("Apply");
    tft.setCursor(3 * x - 30, y + 25 - 8);
    tft.print("Cancel");
}

void Draw::apply_success()
{
    int x = SCREEN_WIDTH / 4, y = SCREEN_HEIGHT - 50;

    // clear apply button
    tft.fillRect(0, SCREEN_HEIGHT - 50, SCREEN_WIDTH / 2 - 1, 50, TFT_BLACK);

    // draw success
    tft.setTextColor(TFT_GREEN);
    tft.drawRoundRect(x - 50, y, 100, 50, 10, TFT_GREEN);
    tft.setCursor(x - 42, y + 25 - 8);
    tft.print("Applied");
}

void Draw::clear_buttons()
{
    tft.fillRect(0, SCREEN_HEIGHT - 50, SCREEN_WIDTH / 2 - 1, 50, TFT_BLACK);
    tft.fillRect(SCREEN_WIDTH / 2 + 1, SCREEN_HEIGHT - 50, SCREEN_WIDTH / 2, 50, TFT_BLACK);
}