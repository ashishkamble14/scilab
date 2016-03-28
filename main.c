/*
  Author     : Ashish Kamble
  File name  : main
  Created on : 25/03/2016
  Programmer : Usbasp
  The program is designed to indicate state of the switch.The switch is connected at PIN C0 
  and its state is indicated using an LED, which is connected at PIN B0 of ATmega16 micro-controller.
  While writing this program it was assumed that the switch is an actve Low switch.
  So when switch is not pressed ATmega reads High state at PINC0 and when switch is pressed it gets
  connected to Ground and ATmega read a Low state.
  Because ATmega can sink more and can source only 40mA so the LED is connected in following manner:
  Anode to 5V through a appropriate value of resistor and Cathode to PIN B0.
*/

//Header files
#include<avr/io.h>
#include<stdlib.h>
#include<compat/deprecated.h>
#include<util/delay.h>
#include<avr/eeprom.h>
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif
//BASIC I/O FILE SETTINGS
#define INPUT 0
#define OUTPUT 1


void main()
{
  //Initialisations
  DDRC |= (PINC0 >> 0);          //declares PIN C0 as an input pin without affecting other pin's status.
  DDRB |= (PINB0 >> 1);          //declares PIN B0 as an output pin without affecting other pin's status.
  PORTC |= (PINC0 >> 1);         //PIN C0 is internally pulled high by enabling the internal pull up resistors.
  PORTB |= (PINB0 >> 1);         //PIN B0 is internally pulled high by enabling the internal pull up resistors.
  //In this case LED will be off when switch is not pressed and it will glow as switch is pressed.
  //In this program software debouncing is also included,for this following variables are included
  int switch_pressed_counter = 0;     //sometimes after pressing or releasing the button its analog value fluctuates
  int switch_released_counter = 0;    //counter value confirms whether button was pressed or not.

  //main program
  while (1)
  {
    if (bit_is_clear(PINC, 0))                                //when switch is pressed
    {
      switch_pressed_counter = switch_pressed_counter + 1;    //the confirmation of switch pressed counter is increased
      switch_released_counter = 0;
      if (switch_pressed_counter > 300)                       //once the counter passes certian confidence value it
      {                                                       //indicates the switch was actually pressed
        PORTB |= (PINB0 >> 0);                               //the confidence value 300 can be changed as per sensitivity of switch required.
        switch_pressed_counter = 0;
      }
    }

    else
    {
      switch_released_counter = switch_released_counter + 1;  //the confirmation of switch released counter is increased
      switch_pressed_counter = 0;
      if (switch_released_counter > 300)                      //indicates the switch was actually released.
      {
        PORTB |= (PINB0 >> 1);
        switch_released_counter = 0;
      }
    }
  }
}





