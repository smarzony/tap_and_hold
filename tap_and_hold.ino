#include <Arduino.h>
#include "timer.h"
#include "pinstate.h"
#include "buttons.h"

#define BTN_MINUS 10
#define BTN_SELECT 9
#define BTN_PLUS 8

#define LED_R 3
#define LED_G 5
#define LED_B 6

Timer t1;
PinState buttons[3];


uint8_t pwm[3];
uint8_t select_state = 0;

void setup()
{
  pinMode(BTN_MINUS, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_PLUS, INPUT_PULLUP);

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  // btn_minus.no = BTN_MINUS;
  buttons[0].no = BTN_MINUS;
  buttons[1].no = BTN_SELECT;
  buttons[2].no = BTN_PLUS;
  Serial.begin(9600);
}

void loop()
{

  buttons[0].actual = digitalRead(buttons[0].no);
  buttons[1].actual = digitalRead(buttons[1].no);
  buttons[2].actual = digitalRead(buttons[2].no);

  button_hold(buttons[0], pwm[select_state], substract);
  button_hold(buttons[2], pwm[select_state], add);
  read_button_inc_switch(BTN_SELECT, 0, 2, select_state, digitalRead);

  analogWrite(LED_R, pwm[0]);
  analogWrite(LED_G, pwm[1]);
  analogWrite(LED_B, pwm[2]);

  buttons[0].last = buttons[0].actual;
  buttons[1].last = buttons[1].actual;
  buttons[2].last = buttons[2].actual;
}

// void button(PinState &pin, uint8_t &input_value, void (*operation)(int, uint8_t&))
// {
//   if (pin.falling() && pin.was_pushed == false)
//   {
//     pin.was_pushed = true;
//     pin.time_save = millis();
//     Serial.println("First push");
//   }
//   if (pin.was_pushed == HIGH && (millis() - pin.time_save > 20))
//   {
//     if (pin.actual == HIGH)
//     {
//       pin.was_pushed = false;
//     }
//     else
//     {
//       pin.time_save = millis();
//       pin.hold_periods++;
//       // Serial.print("Hold: ");
//       // Serial.println(pin.hold_periods);
//       if (pin.hold_periods > 20 && (pin.hold_periods % 10 == 0))
//       {
//         operation(5, input_value);
//         Serial.print("Long hold, output:");        
//         Serial.println(input_value);
//       }
//     }
//   }
//   if (pin.rising() && pin.hold_periods > 0)
//   {
//     if (pin.hold_periods > 0 && pin.hold_periods <= 10)
//     {
//       operation(1, input_value);
//       Serial.print("Tap, output:");  
//       Serial.println(input_value);      
//     }
//     if (pin.hold_periods > 10 && pin.hold_periods <= 20)
//     {
//       operation(2, input_value);
//       Serial.print("Short hold, output:"); 
//       Serial.println(input_value); 
//     }
//     pin.hold_periods = 0;
//     operation(2, input_value);
//   }
// }

void substract(int sub_value, uint8_t &input)
{
  input = input - sub_value;
}

void add(int add_value, uint8_t &input)
{
  input = input + add_value;
}
