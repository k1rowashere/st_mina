#include "headers/main.h"

constexpr Touch::Handle handles[] = {
    Touch::handle<SCREEN_WIDTH / 4 - 75, SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, inc_dec_0_0>,
    Touch::handle<SCREEN_WIDTH / 4 + 25, SCREEN_WIDTH / 4 + 75, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, inc_dec_0_1>,
    Touch::handle<SCREEN_WIDTH / 4 - 75, SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 2 + 25, SCREEN_HEIGHT / 2 + 75, inc_dec_1_0>,
    Touch::handle<SCREEN_WIDTH / 4 + 25, SCREEN_WIDTH / 4 + 75, SCREEN_HEIGHT / 2 + 25, SCREEN_HEIGHT / 2 + 75, inc_dec_1_1>,
    Touch::handle<SCREEN_WIDTH / 2 - 75, SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT - 50, SCREEN_HEIGHT, apply>,
    Touch::handle<SCREEN_WIDTH / 2 + 25, SCREEN_WIDTH / 2 + 75, SCREEN_HEIGHT - 50, SCREEN_HEIGHT, cancel>};

void fill()
{
}

void setup()
{
    Serial.begin(9600);

    // lcd setup
    Draw::init();
    Draw::title();
    Draw::action(HOMMING, 0);
    Draw::action(IDLE, SCREEN_WIDTH / 2);

    // stepper setup
    stepper_0.home();
    Draw::action(IDLE, 0);
    Draw::action(HOMMING, SCREEN_WIDTH / 2);
    stepper_1.home();

    // read from eeprom
    stepper_0.set_pos = EEPROM.get(0, stepper_0.set_pos);
    stepper_1.set_pos = EEPROM.get(4, stepper_1.set_pos);
}

void loop()
{
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
    {
        not_saved = false;
        Draw::clear_buttons();
    }

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
    Touch::run_handles(handles, sizeof(handles) / sizeof(handles[0]));
}