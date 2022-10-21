#include "headers/main.h"

// create touch handlers list

constexpr Touch::Handle handles[] = {
    Touch::handle<SCREEN_WIDTH / 4 - 75, SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, Handlers::inc_dec<current_pos, DECREMENT>>,
    Touch::handle<SCREEN_WIDTH / 4 + 25, SCREEN_WIDTH / 4 + 75, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, Handlers::inc_dec<current_pos, INCREMENT>>,
    Touch::handle<SCREEN_WIDTH / 4 * 3 - 75, SCREEN_WIDTH / 4 * 3 - 25, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, Handlers::inc_dec<current_pos + 1, DECREMENT>>,
    Touch::handle<SCREEN_WIDTH / 4 * 3 + 25, SCREEN_WIDTH / 4 * 3 + 75, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, Handlers::inc_dec<current_pos + 1, INCREMENT>>,
    Touch::handle<SCREEN_WIDTH / 2 - 75, SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT - 50, SCREEN_HEIGHT, Handlers::apply>,
    Touch::handle<SCREEN_WIDTH / 2 + 25, SCREEN_WIDTH / 2 + 75, SCREEN_HEIGHT - 50, SCREEN_HEIGHT, Handlers::cancel>};

constexpr uint8_t handle_count = sizeof(handles) / sizeof(handles[0]);

void fill()
{
}

void setup()
{
    // lcd setup
    Serial.begin(9600);
    uint16_t ID = Draw::tft.readID();
    Draw::tft.begin(ID);
    Draw::tft.setRotation(-45);
    Draw::tft.fillScreen(TFT_BLACK);
    Draw::tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, TFT_WHITE);

    Draw::title();
    Draw::action(HOMMING, 0);
    Draw::action(HOMMING, SCREEN_WIDTH / 2);
}

void loop()
{
    // static bool redraw_set_vol = true; // Flag for redrawing the volume
    // static bool redraw_action = true;  // Flag for redrawing the action
    // static bool not_saved = false;     // Flag for saving the volume to EEPROM

    // update steppers
    current_pos[0] = stepper_0.update();
    current_pos[1] = stepper_1.update();

    // TODO: remove jank
    long temp;
    // if current_pos is not equal to the value in EEPROM, show save and cancel buttons
    if (current_pos[0] != EEPROM.get(0, temp) || current_pos[1] != EEPROM.get(4, temp))
    {
        if (!not_saved)
        {
            not_saved = true;
            Draw::apply_cancel_buttons();
        }
    }
    else
        not_saved = false;

    if (redraw_action)
    {
        redraw_action = false;
        Draw::action(current_action[0]);
        Draw::action(current_action[1], SCREEN_WIDTH / 2);
    }

    if (redraw_set_vol)
    {
        redraw_set_vol = false;
        Draw::fill_v(current_pos[0]);
        Draw::fill_v(current_pos[1], SCREEN_WIDTH / 2);
    }

    Draw::plus_minus_buttons();
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2);

    // touch screen handling
    Touch::run_handles(handles, handle_count);
}