#include "headers/main.h"

void setup()
{
    // setup pins
    pinMode(ON_OFF_PIN, INPUT_PULLUP);
    pinMode(SELECTOR_PIN_0, INPUT_PULLUP);
    pinMode(SELECTOR_PIN_1, INPUT_PULLUP);

#ifndef DEBUG
    // init steppers if ON/OFF switch is on
    if (digitalRead(ON_OFF_PIN) == HIGH)
    {
        G::current_action[0] = digitalRead(SELECTOR_PIN_0) ? HOMING : STOPPED;
        G::current_action[1] = digitalRead(SELECTOR_PIN_1) ? HOMING : STOPPED;
    }
    else
    {
        G::current_action[0] = STOPPED;
        G::current_action[1] = STOPPED;
    }
#else
    Serial.begin(9600);
    G::current_action[0] = READY;
    G::current_action[1] = READY;
#endif

    // lcd setup
    Draw::init(G::current_action, G::pos_unlock);

    // read from eeprom
    G::vis_set_pos[0] = EEPROM.get(0, G::stepper_0.set_pos);
    G::vis_set_pos[1] = EEPROM.get(4, G::stepper_1.set_pos);
}

void loop()
{
#ifndef DEBUG
    // check on/off switch status
    if (digitalRead(ON_OFF_PIN) == LOW)
    {
        G::current_action[0] = STOPPED;
        G::current_action[1] = STOPPED;
    }
    else
    {
        // check 3 way selector status
        if (digitalRead(SELECTOR_PIN_0) == LOW)
            G::current_action[0] = STOPPED;
        else if (G::current_action[0] == STOPPED)
            G::current_action[0] = READY;

        if (digitalRead(SELECTOR_PIN_1) == LOW)
            G::current_action[1] = STOPPED;
        else if (G::current_action[1] == STOPPED)
            G::current_action[1] = READY;
    }
#endif

    // update steppers (move to set_pos || home on startup)
    G::stepper_0.update();
    G::stepper_1.update();

    // update fillers
    G::filler_0.update();
    G::filler_1.update();

    // draw on lcd -------------------------------------------------------------
    Draw::action(G::current_action);
    Draw::plus_minus_buttons(0, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::lock_button(G::pos_unlock ? TFT_WHITE : TFT_RED);
    long temp;
    // if set_pos is not equal to the value in EEPROM, show save and cancel buttons
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
    // TODO: move to Draw::volume_indicator()
    if (G::redraw_set_vol)
    {
        G::redraw_set_vol = false;
        Draw::volume_indicator(G::stepper_0.current_pos, G::vis_set_pos[0]);
        Draw::volume_indicator(G::stepper_1.current_pos, G::vis_set_pos[1], SCREEN_WIDTH / 2);
    }

    // touch screen handling ---------------------------------------------
    Touch::run_handles(handles, sizeof(handles) / sizeof(handles[0]));
}
