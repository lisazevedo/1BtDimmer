#include "mbed.h"

// GLOBAL VALUES
InterruptIn button(p5);
DigitalOut red_led(p6);
DigitalOut blue_led(p7);
PwmOut intensity(p8);
Timeout timeout;
Ticker ticker;
int curr_state;
bool state_button;
float freq;
enum STATE {DOWN, UP, FALL, MIN, RISE, MAX};

// FUNCTIONS DECLARATION
void init();
void press_button();
void release_button();
void transition_state_press();
void led_blink();

// MAIN
int main() {
    init();
    button.rise(&press_button);
    button.fall(&release_button);
}

//FUNCTIONS
void init(){
    blue_led = 1.0;
    red_led = 0.0;
    intensity = 1.0;
    curr_state = UP;
}
void transition_state_press(){
    timeout.attach(&led_blink, 0.1);
    if(state_button){
        if (curr_state == UP) curr_state = RISE;
        else if (curr_state == DOWN) curr_state = FALL;
        else if (curr_state == FALL){
            if(intensity == 0.0) curr_state = MIN;
            else intensity = intensity - 0.05;
        }
        else if (curr_state == RISE){
            if(intensity == 1.0) curr_state = MAX;
            else intensity = intensity + 0.05;
        }
        else{
            if (intensity >= 1.0) curr_state = MAX;
            else if (intensity <= 0.0) curr_state = MIN;
        }
    }
}
void press_button(){
    state_button = true;
    ticker.attach(&transition_state_press, 1.0);
}
void release_button(){
    state_button = false;
    ticker.detach();
    if(curr_state == MAX || curr_state == UP || curr_state == FALL){
        if(intensity > 0.0){
            blue_led = 0.0;
            red_led = 1.0;
            curr_state = DOWN;
        }
    }
    else if(curr_state == MIN || curr_state == DOWN || curr_state == RISE){
        if(intensity < 1.0){
            blue_led = 1.0;
            red_led = 0.0;
            curr_state = UP;
        }
    }
}
void led_blink(){
    if(curr_state == FALL){
        freq = 1;
        red_led = !red_led;
    }
    else if(curr_state == RISE){
        freq = 1;
        blue_led = !blue_led;
    }
    else if(curr_state == MIN){
        freq = 0.1;
        red_led = !red_led;
    }
    else if(curr_state == MAX){
        freq = 0.1;
        blue_led = !blue_led;
    }
    timeout.attach(&led_blink, freq);
}






