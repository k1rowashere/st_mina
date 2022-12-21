#include "headers/draw.h"

MCUFRIEND_kbv tft;

void Draw::init(const Status(&current_status)[2], bool pos_unlock)
{
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT / 2, TFT_WHITE);

    // draw static parts:

    // draw title
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(SCREEN_WIDTH / 4 - 45, 0);
    tft.print("(LHS)");
    tft.setCursor(SCREEN_WIDTH / 4 * 3 - 45, 0);
    tft.print("(RHS)");

    tft.setTextSize(2);
    Draw::status(current_status);
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

void Draw::status(const Status(&status)[2])
{
    static Status prev_status[2] = { READY, READY };

    auto draw = [](Status status, uint16_t x_offset)
    {
        tft.setTextColor(TFT_WHITE);
        tft.setCursor(x_offset + 18, 24 * 2);
        // clear status area
        tft.fillRect(x_offset + 1, 24 * 2, SCREEN_WIDTH / 2 - 2, 24, TFT_BLACK);
        switch (status)
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

        case ERROR:
            tft.setTextColor(TFT_RED);
            tft.print("ERROR");
            break;

        default:
            tft.setTextColor(TFT_WHITE);
            tft.print(status);
            break;
        }
    };

    // draw status if changed
    for (uint8_t i = 0; i < 2; i++)
    {
        if (status[i] != prev_status[i])
        {
            draw(status[i], i * SCREEN_WIDTH / 2);
            prev_status[i] = status[i];
        }
    }
}

void Draw::volume_indicator(const uint16_t act_set_pos[2], const uint16_t vis_set_pos[2])
{
    static uint16_t prev_act_pos[2] = { UINT16_MAX, UINT16_MAX };
    static uint16_t prev_set_pos[2] = { UINT16_MAX, UINT16_MAX };
    static uint32_t prev_time = 0;

    // rate limit
    if (millis() - prev_time < 100)
        return;
    prev_time = millis();

    auto draw = [](uint16_t pos, uint8_t y_offset = 0, uint16_t x_offset = 0)
    {
        tft.setCursor(x_offset + SCREEN_WIDTH / 2 - 50, 24 * (3 + y_offset));
        // clear previous text (the width of the text is 12 * 4 = 48)
        tft.fillRect(tft.getCursorX(), tft.getCursorY(), 48, 24, TFT_BLACK);

        // draw new text
        tft.setTextColor(TFT_LIGHTGREY);
        tft.print(steps_to_volume(pos));
    };

    // draw act_pos volume indicator, only draw if changed
    for (uint8_t i = 0; i < 2; i++)
    {
        if (act_set_pos[i] != prev_act_pos[i])
        {
            draw(act_set_pos[i], 0, i * SCREEN_WIDTH / 2);
            prev_act_pos[i] = act_set_pos[i];
        }
    }

    // draw set_pos volume indicator, only draw if changed
    for (uint8_t i = 0; i < 2; i++)
    {
        if (vis_set_pos[i] != prev_set_pos[i])
        {
            draw(vis_set_pos[i], 1, i * SCREEN_WIDTH / 2);
            prev_set_pos[i] = vis_set_pos[i];
        }
    }
}

void Draw::plus_minus_buttons(uint16_t color)
{
    static uint16_t prev_color[2] = { TFT_BLACK, TFT_BLACK };

    for (uint8_t i = 0; i < 2; i++)
    {
        // draw only if color changed
        if (color == prev_color[i])
            continue;
        prev_color[i] = color;

        int x = (i * SCREEN_WIDTH / 2) + SCREEN_WIDTH / 4, y = SCREEN_HEIGHT / 2 - 25;

        tft.setTextColor(color);
        tft.drawRoundRect(x - 70, y, 50, 50, 10, color);
        tft.drawRoundRect(x + 20, y, 50, 50, 10, color);
        tft.setCursor(x - 45 - 6, y + 25 - 8);
        tft.print("-");
        tft.setCursor(x + 45 - 6, y + 25 - 8);
        tft.print("+");
    }
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
    tft.setCursor(3 * x - 36, y + 25 - 8);
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

void Draw::error(const char* error, Side side)
{
    tft.setTextColor(TFT_RED);
    tft.setCursor(10 + side, SCREEN_HEIGHT - 50 - 24);
    tft.print(error);
    acknowledge_error();
}

void Draw::clear_error(Side side)
{
    tft.fillRect(side, SCREEN_HEIGHT - 50 - 24, SCREEN_WIDTH, 24, TFT_BLACK);
    tft.fillRect(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 50, 100, 50, TFT_BLACK);
}

void Draw::acknowledge_error()
{
    tft.setTextColor(TFT_ORANGE);
    tft.fillRect(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 50, 100, 50, TFT_BLACK);
    tft.drawRoundRect(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 50, 100, 50, 10, TFT_WHITE);
    tft.setCursor(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT - 50 + 25 - 8);
    tft.print("ACK");
}