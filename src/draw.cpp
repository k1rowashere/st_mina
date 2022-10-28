#include "headers/draw.h"

MCUFRIEND_kbv tft;

void Draw::init(const Actions (&current_action)[2], bool pos_unlock)
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
    tft.setCursor(SCREEN_WIDTH / 4 - 45, 0);
    tft.print("(LHS)");
    tft.setCursor(SCREEN_WIDTH / 4 * 3 - 45, 0);
    tft.print("(RHS)");

    tft.setTextSize(2);
    Draw::action(current_action);
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
    tft.fillRect(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 40, 50, 80, TFT_BLACK);
    tft.drawRect(10, SCREEN_HEIGHT / 2 - 40, SCREEN_WIDTH - 20, 80, TFT_DARKGREY);
    Draw::lock_button(pos_unlock ? TFT_WHITE : TFT_RED);
    Draw::plus_minus_buttons();
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2);
}

void Draw::action(const Actions (&actions)[2])
{
    static Actions prev_action[2] = {READY, READY};

    auto draw = [](Actions action, uint16_t x_offset)
    {
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(x_offset + 18, 24 * 2);
        // clear action area
        tft.fillRect(x_offset + 1, 24 * 2, SCREEN_WIDTH / 2 - 2, 24, TFT_BLACK);
        switch (action)
        {
        case READY:
            tft.print("READY");
            break;
        case FILLING:
            tft.setTextColor(TFT_GREEN);
            tft.print("FILLING");
            break;
        case EMPTYING:
            tft.setTextColor(TFT_GREEN);
            tft.print("EMPTYING");
            break;
        case HOMING:
            tft.setTextColor(TFT_YELLOW);
            tft.print("HOMING");
            break;
        case MOVING:
            tft.setTextColor(TFT_YELLOW);
            tft.print("MOVING");
            break;
        case STOPPED:
            tft.setTextColor(TFT_RED);
            tft.print("STOPPED");
            break;
        }
    };

    // draw action if changed
    for (uint8_t i = 0; i < 2; i++)
    {
        if (actions[i] != prev_action[i])
        {
            draw(actions[i], i * SCREEN_WIDTH / 2);
            prev_action[i] = actions[i];
        }
    }
}

void Draw::volume_indicator(uint32_t curr_vol_pos, uint32_t set_vol_pos, uint16_t x_offset)
{

    // TODO: remove repitition

    // draw current volume indicator, only draw if changed
    static uint32_t prev_curr_vol_pos[2] = {UINT32_MAX, UINT32_MAX};
    if (curr_vol_pos != prev_curr_vol_pos[x_offset / (SCREEN_WIDTH / 2)])
    {

        // clear previous text (faster than tft.fillRect)
        tft.setTextColor(TFT_BLACK);
        tft.setCursor(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 3);
        tft.print(steps_to_volume(prev_curr_vol_pos[x_offset / (SCREEN_WIDTH / 2)]));

        tft.setTextColor(TFT_LIGHTGREY);
        tft.setCursor(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 3);
        tft.print(steps_to_volume(curr_vol_pos));

        prev_curr_vol_pos[x_offset / (SCREEN_WIDTH / 2)] = curr_vol_pos;
    }

    // draw set volume indicator, only draw if changed
    static uint32_t prev_set_vol_pos[2] = {UINT32_MAX, UINT32_MAX};
    if (set_vol_pos != prev_set_vol_pos[x_offset / (SCREEN_WIDTH / 2)])
    {

        // clear previous text (faster than tft.fillRect)
        tft.setTextColor(TFT_BLACK);
        tft.setCursor(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 4);
        tft.print(steps_to_volume(prev_set_vol_pos[x_offset / (SCREEN_WIDTH / 2)]));

        tft.setTextColor(TFT_LIGHTGREY);
        tft.setCursor(x_offset + SCREEN_WIDTH / 2 - 50, 24 * 4);
        tft.print(steps_to_volume(set_vol_pos));

        prev_set_vol_pos[x_offset / (SCREEN_WIDTH / 2)] = set_vol_pos;
    }
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
    tft.drawRoundRect(x - 70, y, 50, 50, 10, color);
    tft.drawRoundRect(x + 20, y, 50, 50, 10, color);
    tft.setCursor(x - 45 - 6, y + 25 - 8);
    tft.print("-");
    tft.setCursor(x + 45 - 6, y + 25 - 8);
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
    tft.drawRoundRect(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50, 10, color);

    // clear lock text
    tft.fillRect(SCREEN_WIDTH / 2 - 25 + 13, SCREEN_HEIGHT / 2 - 25 + 10, 24, 30, TFT_BLACK);

    // bitmap of a lock
    if (color == TFT_WHITE)
        tft.drawBitmap(SCREEN_WIDTH / 2 - 25 + 13, SCREEN_HEIGHT / 2 - 25 + 10, open_lock_bmp, 24, 30, color);
    else
        tft.drawBitmap(SCREEN_WIDTH / 2 - 25 + 13, SCREEN_HEIGHT / 2 - 25 + 10, lock_bmp, 24, 30, color);

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