#include "headers/main.h"

// init touchscreen event handlers
bool always_true = true;
// x0, x1, y0, y1, callback, condition
constexpr Touch::Handle handles[] = {
    {SCREEN_WIDTH / 4 - 75, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(0, NEGATIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 + 25, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(0, POSITIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 * 3 - 75, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(1, NEGATIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 * 3 + 25, SCREEN_HEIGHT / 2 - 25, 50, 50, INC_DEC__(1, POSITIVE), G::pos_unlock},
    {SCREEN_WIDTH / 4 - 50, SCREEN_HEIGHT - 50, 100, 50, apply, G::not_saved},
    {SCREEN_WIDTH / 4 * 3 - 50, SCREEN_HEIGHT - 50, 100, 50, cancel, G::not_saved},
    {0, SCREEN_HEIGHT - 100, 50, 50, lock, always_true},
};

void fill()
{
}

void setup()
{
    Serial.begin(9600);

    // lcd setup
    Draw::init();
    Draw::lock_button(G::pos_unlock ? TFT_GREEN : TFT_RED);

    // stepper setup
    G::stepper_0.home();
    G::stepper_1.home();

    // read from eeprom
    G::vis_set_pos[0] = EEPROM.get(0, G::stepper_0.set_pos);
    G::vis_set_pos[1] = EEPROM.get(4, G::stepper_1.set_pos);
}

void loop()
{
    // update steppers
    G::stepper_0.update();
    G::stepper_1.update();

    Draw::action();
    Draw::plus_minus_buttons(0, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::lock_button(G::pos_unlock ? TFT_GREEN : TFT_RED);

    // TODO: remove temp variable
    long temp;
    // if current_pos is not equal to the value in EEPROM, show save and cancel buttons
    if (G::vis_set_pos[0] != EEPROM.get(0, temp) || G::vis_set_pos[1] != EEPROM.get(4, temp))
    {
        // if the buttons are not already drawn, draw them
        if (!G::not_saved)
        {
            G::not_saved = true;
            Draw::apply_cancel_buttons();
        }
    }
    else if (G::not_saved)
    {
        // if the buttons are drawn, clear them
        G::not_saved = false;
        Draw::clear_buttons();
    }

    if (G::redraw_set_vol)
    {
        G::redraw_set_vol = false;
        Draw::volume_indicator(G::stepper_0.current_pos, G::vis_set_pos[0]);
        Draw::volume_indicator(G::stepper_1.current_pos, G::vis_set_pos[1], SCREEN_WIDTH / 2);
    }

    // touch screen handling
    Touch::run_handles(handles, sizeof(handles) / sizeof(handles[0]));
}
