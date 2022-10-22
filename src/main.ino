#include "headers/main.h"

constexpr Touch::Handle handles[] = {
    {SCREEN_WIDTH / 4 - 75, SCREEN_WIDTH / 4 - 25, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, INC_DEC__(0, NEGATIVE)},
    {SCREEN_WIDTH / 4 + 25, SCREEN_WIDTH / 4 + 75, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, INC_DEC__(0, POSITIVE)},
    {SCREEN_WIDTH / 4 * 3 - 75, SCREEN_WIDTH / 4 * 3 - 25, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, INC_DEC__(1, NEGATIVE)},
    {SCREEN_WIDTH / 4 * 3 + 25, SCREEN_WIDTH / 4 * 3 + 75, SCREEN_HEIGHT / 2 - 25, SCREEN_HEIGHT / 2 + 25, INC_DEC__(1, POSITIVE)},
    {SCREEN_WIDTH / 4 - 50, SCREEN_WIDTH / 4 + 50, SCREEN_HEIGHT - 50, SCREEN_HEIGHT, apply},
    {SCREEN_WIDTH / 4 * 3 - 50, SCREEN_WIDTH / 4 * 3 + 50, SCREEN_HEIGHT - 50, SCREEN_HEIGHT, cancel}};

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
    G::stepper_0.home();
    Draw::action(IDLE, 0);
    Draw::action(HOMMING, SCREEN_WIDTH / 2);
    G::stepper_1.home();

    // read from eeprom
    G::set_pos[0] = EEPROM.get(0, G::stepper_0.set_pos);
    G::set_pos[1] = EEPROM.get(4, G::stepper_0.set_pos);
}

void loop()
{
    // update steppers
    // G::stepper_0.update();
    // G::stepper_1.update();

    // TODO: remove jank
    long temp;
    // if current_pos is not equal to the value in EEPROM, show save and cancel buttons
    if (G::set_pos[0] != EEPROM.get(0, temp) || G::set_pos[1] != EEPROM.get(4, temp))
    {
        if (!G::not_saved)
        {
            G::not_saved = true;
            Draw::apply_cancel_buttons();
        }
    }
    else
    {
        G::not_saved = false;
        Draw::clear_buttons();
    }

    if (G::redraw_action)
    {
        G::redraw_action = false;
        Draw::action(G::current_action[0]);
        Draw::action(G::current_action[1], SCREEN_WIDTH / 2);
    }

    if (G::redraw_set_vol)
    {
        G::redraw_set_vol = false;
        Draw::volume_indicator(G::stepper_0.current_pos, G::set_pos[0]);
        Draw::volume_indicator(G::stepper_1.current_pos, G::set_pos[1], SCREEN_WIDTH / 2);
    }

    Draw::plus_minus_buttons();
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2);

    // touch screen handling
    Touch::run_handles(handles, sizeof(handles) / sizeof(handles[0]));
}