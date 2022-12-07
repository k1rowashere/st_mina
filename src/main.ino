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
        current_status[0] = digitalRead(SELECTOR_PIN_0) ? HOMING : STOPPED;
        current_status[1] = digitalRead(SELECTOR_PIN_1) ? HOMING : STOPPED;
    }
    else
    {
        current_status[0] = STOPPED;
        current_status[1] = STOPPED;

        fillers[0].empty();
        fillers[1].empty();
    }
#else
    G::current_action[0] = HOMING;
    G::current_action[1] = HOMING;
#endif
    Serial.begin(9600);

    // lcd setup
    Draw::init(current_status, G::pos_unlock);

    // read from eeprom
    G::vis_set_pos[0] = EEPROM.get(0, steppers[0].set_pos);
    G::vis_set_pos[1] = EEPROM.get(2, steppers[1].set_pos);
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
        current_status[0] = HOMING;
    else if (!curr_on_off)
    {
        fillers[0].empty();
        current_status[0] = STOPPED;
    }

    if (bitRead(~prev_on_off & curr_on_off, 1))
        current_status[1] = HOMING;
    else if (!curr_on_off)
    {
        fillers[1].empty();
        current_status[1] = STOPPED;
    }

    prev_on_off = curr_on_off;
#endif

#ifndef DISABLE_ULTRA_SONIC_CHECK
    static Ultrasonic ultrasonic(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);

    // check if product tank is not empty
    if (ultrasonic.get_avg_dist() > MAX_DISTANCE)
    {
        current_status[0] = STOPPED;
        current_status[1] = STOPPED;
        Draw::print_error("Err: Tank Empty.");
    }
#endif

    // for each stepper and filler ---------------------------------------------
    for (uint8_t i = 0; i < 2; i++)
    {
        // update steppers (move to set_pos || home on startup)
        if ((current_status[i] = steppers[i].update(current_status[i])) == DONE)
        {
            // if the stepper is done moving, cycle the filler
            current_status[i] = fillers[i].fill_cycle(current_status[i]);
        }

        // update fillers
        current_status[i] = fillers[i].update(current_status[i]);
    }

    // draw on lcd -------------------------------------------------------------
    Draw::status(current_status);

    uint16_t act_val[2] = {steppers[0].actual_pos, steppers[1].actual_pos};
    Draw::volume_indicator(act_val, G::vis_set_pos);

    Draw::plus_minus_buttons(0, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::plus_minus_buttons(SCREEN_WIDTH / 2, G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::lock_button(G::pos_unlock ? TFT_WHITE : TFT_RED);

    uint16_t temp;
    // if set_pos is not equal to the value in EEPROM, show save and cancel buttons
    if (G::vis_set_pos[0] != EEPROM.get(0, temp) || G::vis_set_pos[1] != EEPROM.get(2, temp))
    {
        // if the buttons are not already drawn, draw them
        if (!not_saved)
        {
            not_saved = true;
            Draw::apply_cancel_buttons();
        }
    }
    else if (not_saved)
    {
        // set stepper set_pos to the value vis_set_pos
        steppers[0].set_pos = G::vis_set_pos[0];
        steppers[1].set_pos = G::vis_set_pos[1];

        // if the buttons are drawn, clear them
        Draw::clear_buttons();

        not_saved = false;
    }

    // touch screen handling ---------------------------------------------
    Touch::run_handles(handles, sizeof(handles) / sizeof(handles[0]));
}
