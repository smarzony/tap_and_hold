#include "buttons.h"
#include "Arduino.h"

void read_button_neg_switch(uint8_t button, bool &state, int (*read_button_callback)(uint8_t))
{
	static unsigned long lastTime;
	unsigned long timeNow = millis();

	if (read_button_callback(button) == 0) {
		if (timeNow - lastTime < BUTTON_DELAY)
			return;
		if (read_button_callback(button) == 0)
		{
			state = !state;
		}
		lastTime = timeNow;
	}
}

void read_button_inc_switch(uint8_t button, uint8_t limit_min, uint8_t limit_max, uint8_t& state, int (*read_button_callback)(uint8_t))
{
  static unsigned long lastTime;
  unsigned long timeNow = millis();
  if (read_button_callback(button) == 0) {
    if (timeNow - lastTime < BUTTON_DELAY)
      return;
    if (read_button_callback(button) == 0)
    {
      state += 1;
      if (state > limit_max)
      {
        state = limit_min;
      } 
    }
    lastTime = timeNow;
  }
}

void button_hold(PinState &pin, uint8_t &input_value, void (*operation)(int, uint8_t&))
{
  if (pin.falling() && pin.was_pushed == false)
  {
    pin.was_pushed = true;
    pin.time_save = millis();
  }
  if ((pin.was_pushed == true) && (millis() - pin.time_save > 20))
  {
    if (pin.actual == HIGH)
    {
      pin.was_pushed = false;
    }
    else
    {
      pin.time_save = millis();
      pin.hold_periods++;
      if (pin.hold_periods > 20 && (pin.hold_periods % 10 == 0))
      {
        operation(5, input_value);
        Serial.println(input_value);
      }
    }
  }
  if (pin.rising() && pin.hold_periods > 0 && pin.was_pushed == true)
  {
    pin.was_pushed = false;
    if (pin.hold_periods > 0 && pin.hold_periods <= 10)
    {
      operation(1, input_value); 
    }
    if (pin.hold_periods > 10 && pin.hold_periods <= 20)
    {
      operation(2, input_value);
    }
    pin.hold_periods = 0;

  }
}