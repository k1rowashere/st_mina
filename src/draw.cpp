#include "headers/draw.h"

MCUFRIEND_kbv tft;

void Draw::init()
{
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(-45);
    tft.fillScreen(TFT_BLACK);
    tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, TFT_WHITE);

    // draw static parts:

    // draw title
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(SCREEN_WIDTH / 4 - 19, 0);
    tft.print("(1)");
    tft.setCursor(SCREEN_WIDTH / 4 * 3 - 19, 0);
    tft.print("(2)");

    tft.setTextSize(2);
    // draw volume indicator (static parts)
    auto volume_indicator_static = [](uint16_t x_offset = 0)
    {
        tft.setTextColor(TFT_LIGHTGREY);
        tft.setCursor(x_offset + 18, 24 * 3);
        tft.print("Cur vol (ml): ");

        tft.setCursor(x_offset + 18, 24 * 4);
        tft.print("Set vol (ml): ");
    };
    volume_indicator_static();
    volume_indicator_static(SCREEN_WIDTH / 2);

    // draw buttons
    Draw::plus_minus_buttons();
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2);
}

void Draw::action()
{
    static Actions prev_action[2] = {IDLE, IDLE};

    auto draw = [](Actions action, uint16_t x_offset)
    {
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
        case HOMING:
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
    };

    // draw action if changed
    if (G::current_action[0] != prev_action[0])
    {
        draw(G::current_action[0], 0);
        prev_action[0] = G::current_action[0];
    }
    if (G::current_action[1] != prev_action[1])
    {
        draw(G::current_action[1], SCREEN_WIDTH / 2);
        prev_action[1] = G::current_action[1];
    }
}

void Draw::volume_indicator(uint32_t curr_vol_pos, uint32_t set_vol_pos, uint16_t x_offset)
{
    tft.setTextColor(TFT_LIGHTGREY);
    // draw current volume indicator
    tft.setCursor(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 3);
    tft.fillRect(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 3, 50, 18, TFT_BLACK);
    tft.print(steps_to_volume(curr_vol_pos));

    // draw set volume indicator
    tft.setCursor(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 4);
    tft.fillRect(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 4, 50, 18, TFT_BLACK);
    tft.print(steps_to_volume(set_vol_pos));
}

void Draw::plus_minus_buttons(uint16_t x_offset, uint16_t color)
{
    // draw only if color changed
    static uint16_t prev_color[2] = {TFT_BLACK, TFT_BLACK};
    if (color == prev_color[x_offset / (SCREEN_WIDTH / 2)])
        return;
    prev_color[x_offset / (SCREEN_WIDTH / 2)] = color;

    int x = x_offset + SCREEN_WIDTH / 4, y = SCREEN_HEIGHT / 2 - 25;

    tft.setTextColor(color);
    tft.drawRoundRect(x - 75, y, 50, 50, 10, color);
    tft.drawRoundRect(x + 25, y, 50, 50, 10, color);
    tft.setCursor(x - 50 - 6, y + 25 - 8);
    tft.print("-");
    tft.setCursor(x + 50 - 6, y + 25 - 8);
    tft.print("+");
}

void Draw::lock_button(uint16_t color)
{
    static uint16_t prev_color = 0;
    // drawn only if color changed
    if (color == prev_color)
        return;

    // bottom left of the screen
    tft.setTextColor(color);
    tft.drawRoundRect(0, SCREEN_HEIGHT - 100, 50, 50, 10, color);
    tft.setCursor(25 - 6, SCREEN_HEIGHT - 75 - 8);
    tft.print("L");

    prev_color = color;
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