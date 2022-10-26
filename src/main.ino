#include "headers/main.h"

void setup()
{
    Serial.begin(9600);

    // lcd setup
    Draw::init();

    // init steppers
    G::current_action[0] = HOMING;
    G::current_action[1] = HOMING;

    // attach interrupts
    constexpr uint8_t pins[] = {PEDAL_PIN,
                                LOW_LIMIT_SWITCH_PIN_0,
                                HIGH_LIMIT_SWITCH_PIN_0,
                                LOW_LIMIT_SWITCH_PIN_1,
                                HIGH_LIMIT_SWITCH_PIN_1};
    for (uint8_t i : pins)
        attachInterrupt(digitalPinToInterrupt(i), Filler::handle_interrupt, RISING);

    // read from eeprom
    G::vis_set_pos[0] = EEPROM.get(0, G::stepper_0.set_pos);
    G::vis_set_pos[1] = EEPROM.get(4, G::stepper_1.set_pos);
}

void loop()
{
    // update steppers (move to set_pos || home on startup)
    noInterrupts();
    G::stepper_0.update();
    G::stepper_1.update();
    interrupts();

    // Filler::error()
    G::filler_0.error();
    G::filler_1.error();

    // draw on lcd
    Draw::action();
    Draw::plus_minus_buttons(0, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::lock_button(G::pos_unlock ? TFT_WHITE : TFT_RED);

    // TODO: remove temp variable
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

    if (G::redraw_set_vol)
    {
        G::redraw_set_vol = false;
        Draw::volume_indicator(G::stepper_0.current_pos, G::vis_set_pos[0]);
        Draw::volume_indicator(G::stepper_1.current_pos, G::vis_set_pos[1], SCREEN_WIDTH / 2);
    }

    // touch screen handling
    Touch::run_handles(handles, sizeof(handles) / sizeof(handles[0]));
}
