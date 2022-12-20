#include "headers/main.h"

void setup()
{
    // setup pins
    pinMode(ON_OFF_PIN, INPUT_PULLUP);
    pinMode(SELECTOR_PIN_0, INPUT_PULLUP);
    pinMode(SELECTOR_PIN_1, INPUT_PULLUP);

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    Serial.begin(9600);

    // init steppers
    // steppers[0].init();
    // steppers[1].init();

    // init fillers
    // fillers[0].init();
    // fillers[1].init();
#ifndef DISABLE_SWITCHES
    // init steppers if ON/OFF switch is on
    if (digitalRead(ON_OFF_PIN) == LOW)
    {
        current_status[0] = !digitalRead(SELECTOR_PIN_0) ? HOMING : STOPPED;
        current_status[1] = !digitalRead(SELECTOR_PIN_1) ? HOMING : STOPPED;
    }
    else
    {
        current_status[0] = STOPPED;
        current_status[1] = STOPPED;
    }
#else
    G::current_action[0] = HOMING;
    G::current_action[1] = HOMING;
#endif

    // lcd setup
    Draw::init(current_status, G::pos_unlock);

    // read from eeprom
    G::vis_set_pos[0] = eeprom_read<uint16_t>(0);
    G::vis_set_pos[1] = eeprom_read<uint16_t>(2);

    steppers[0].set_pos = G::vis_set_pos[0];
    steppers[1].set_pos = G::vis_set_pos[1];
}

void loop()
{
    // check switches ----------------------------------------------------------    
#ifndef DISABLE_SWITCHES
    // detect falling edge on ON/OFF switch and selector switches
    // if ON/OFF & selector switch are on, init steppers
    // if ON/OFF switch is off, stop steppers and empty fillers
    uint8_t curr_on_off = 0;
    bitWrite(curr_on_off, 0, !digitalRead(ON_OFF_PIN) && !digitalRead(SELECTOR_PIN_0));
    bitWrite(curr_on_off, 1, !digitalRead(ON_OFF_PIN) && !digitalRead(SELECTOR_PIN_1));

    static uint8_t prev_on_off = curr_on_off;

    for (uint8_t i = 0; i < 2; i++)
    {
        if (bitRead(~prev_on_off & curr_on_off, i))
            current_status[i] = HOMING;
        else if (!bitRead(curr_on_off, i))
        {
            current_status[i] = STOPPED;
            fillers[i].empty();
        }
    }

    prev_on_off = curr_on_off;
#endif

    // check ultrasonic sensor -------------------------------------------------
#ifndef DISABLE_ULTRA_SONIC_CHECK
    static Ultrasonic ultrasonic(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);

    // check if product tank is not empty
    if (ultrasonic.get_avg_dist() > MAX_DISTANCE)
    {
        current_status[0] = STOPPED;
        current_status[1] = STOPPED;
        Draw::error("Err: Tank Empty.", Side::LHS);
    }
#endif

    // check if error is cleared ----------------------------------------------
    if (G::clear_error)
    {
        Draw::clear_error();
        G::clear_error = false;
        for (uint8_t i = 0; i < 2; i++)
            current_status[i] = (current_status[i] == Status::ERROR) ? fillers[i].empty() : current_status[i];
    }

    if (current_status[0] == Status::ERROR || current_status[1] == Status::ERROR)
        ack_flag = true;
    else
        ack_flag = false;

    // update steppers and fillers ---------------------------------------------
    for (uint8_t i = 0; i < 2; i++)
    {
        // update steppers (move to set_pos || home on startup)
        current_status[i] = steppers[i].update(current_status[i]);

        // if the stepper is done homing + moving back to set_pos, empty the filler
        if (current_status[i] == Status::DONE)
            current_status[i] = fillers[i].empty();

        // update fillers
        current_status[i] = fillers[i].update(current_status[i]);
    }

    // cosmetic led ------------------------------------------------------------
#ifndef DISABLE_LED
    static uint16_t hue = 0;
    static uint32_t last_time = millis();

    if (hue == 360)
        hue = 0;

    if (millis() - last_time > 10)
    {
        last_time = millis();
        rgb(hue_to_rgb(hue++));
    }
#endif

    // draw on lcd -------------------------------------------------------------
    Draw::status(current_status);
    // if either stepper is moving, skip drawing the volume indicator / touch buttons (to prevent lag)
    // if (current_status[0] != Status::MOVING || current_status[1] != Status::MOVING)
    //     return;

    const uint16_t act_val[2] = { steppers[0].actual_pos, steppers[1].actual_pos };
    Draw::volume_indicator(act_val, G::vis_set_pos);
    Draw::plus_minus_buttons(G::pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::lock_button(G::pos_unlock ? TFT_WHITE : TFT_RED);

    // if set_pos is not equal to the value in EEPROM, show save and cancel buttons
    if (eeprom_read<uint32_t>(0) != *(uint32_t*)G::vis_set_pos)
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
