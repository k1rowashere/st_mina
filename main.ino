#include "src/headers/main.h"

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
    current_action[0] = HOMING;
    current_action[1] = HOMING;
#endif

    // lcd setup
    Draw::init(current_status, pos_unlock);

    // read from eeprom
    vis_set_pos[0] = eeprom_read<uint16_t>(0);
    vis_set_pos[1] = eeprom_read<uint16_t>(2);

    steppers[0].set_pos = vis_set_pos[0];
    steppers[1].set_pos = vis_set_pos[1];
}

void loop()
{
    // check switches ----------------------------------------------------------
#ifndef DISABLE_SWITCHES
    // detect falling edge on ON/OFF switch and selector switches
    // if ON/OFF & selector switch are on, init steppers
    // if ON/OFF switch is off, stop steppers and stop fillers
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
            fillers[i].stop();
        }
    }

    prev_on_off = curr_on_off;
#endif

    // // check if error is cleared ----------------------------------------------
    // // if (clear_error)
    // // {
    // //     Draw::clear_error();
    // //     clear_error = false;
    // //     for (uint8_t i = 0; i < 2; i++)
    // //         if (current_status[i] == Status::ERROR)
    // //             current_status[i] = Status::STOPPED;
    // // }
    // // if (current_status[0] == Status::ERROR || current_status[1] == Status::ERROR)
    // //     ack_flag = true;
    // // else
    // //     ack_flag = false;

    // update steppers and fillers ---------------------------------------------
    for (uint8_t i = 0; i < 2; i++)
    {
        // update steppers (move to set_pos || home on startup)
        current_status[i] = steppers[i].update(current_status[i]);

        // update fillers
        current_status[i] = fillers[i].update(current_status[i]);
    }

    // cosmetic led ------------------------------------------------------------
#ifndef DISABLE_LED
    static uint16_t hue = 0;
    static uint32_t last_led_time = millis();

    if (hue == 360)
        hue = 0;

    if (millis() - last_led_time > 50)
    {
        last_led_time = millis();
        rgb(hue_to_rgb(hue++));
    }
#endif

    // touch screen handling ---------------------------------------------
    Touch::run_handles(handles, sizeof(handles) / sizeof(handles[0]), touch_args);

    // draw on lcd -------------------------------------------------------------

    // if either stepper is moving, limit drawing (to reduce lag)
    static uint32_t last_draw_time = millis();
    if (current_status[0] == Status::MOVING || current_status[1] == Status::MOVING)
        if (millis() - last_draw_time < 1000)
            return;

    last_draw_time = millis();

    Draw::status(current_status);
    const uint16_t act_val[2] = {steppers[0].actual_pos, steppers[1].actual_pos};
    Draw::volume_indicator(act_val, vis_set_pos);

    Draw::btn_plus_minus(pos_unlock ? TFT_WHITE : TFT_DARKGREY);
    Draw::btn_lock(pos_unlock ? TFT_WHITE : TFT_RED);

    // if set_pos is not equal to the value in EEPROM, show save and cancel buttons
    if (eeprom_read<uint32_t>(0) != *(uint32_t*)vis_set_pos)
    {
        // if the buttons are not already drawn, draw them
        if (!not_saved)
        {
            not_saved = true;
            Draw::btn_apply_cancel();
        }
    }
    else if (not_saved)
    {
        // set stepper set_pos to the value vis_set_pos
        steppers[0].set_pos = vis_set_pos[0];
        steppers[1].set_pos = vis_set_pos[1];

        Draw::btn_clear();
        not_saved = false;
    }
}
