#include "headers/main.h"

void setup()
{
    // setup pins
    pinMode(ON_OFF_PIN, INPUT_PULLUP);
    pinMode(SELECTOR_PIN_0, INPUT_PULLUP);
    pinMode(SELECTOR_PIN_1, INPUT_PULLUP);

#ifndef DISABLE_SWITCHES
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

        G::filler_0.empty();
        G::filler_1.empty();
    }
#else
    G::current_action[0] = HOMING;
    G::current_action[1] = HOMING;
#endif
    Serial.begin(9600);

    // lcd setup
    Draw::init(G::current_action, G::pos_unlock);

    // read from eeprom
    G::vis_set_pos[0] = EEPROM.get(0, G::stepper_0.set_pos);
    G::vis_set_pos[1] = EEPROM.get(4, G::stepper_1.set_pos);
}

void loop()
{

#ifndef DISABLE_SWITCHES
    // detect rising edge on ON/OFF switch and selector switches
    // if ON/OFF & selector switch are on, init steppers
    // if ON/OFF switch is off, stop steppers and empty fillers
    uint8_t curr_on_off = 0;
    bitWrite(curr_on_off, 0, digitalRead(ON_OFF_PIN) && digitalRead(SELECTOR_PIN_0));
    bitWrite(curr_on_off, 1, digitalRead(ON_OFF_PIN) && digitalRead(SELECTOR_PIN_1));

    static uint8_t prev_on_off = curr_on_off;

    if (bitRead(~prev_on_off & curr_on_off, 0))
        G::current_action[0] = HOMING;
    else if (!curr_on_off)
    {
        G::filler_0.empty();
        G::current_action[0] = STOPPED;
    }

    if (bitRead(~prev_on_off & curr_on_off, 1))
        G::current_action[1] = HOMING;
    else if (!curr_on_off)
    {
        G::filler_1.empty();
        G::current_action[1] = STOPPED;
    }

    prev_on_off = curr_on_off;
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
