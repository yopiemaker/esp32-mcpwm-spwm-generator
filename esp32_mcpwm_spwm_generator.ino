/*
esp32_mcpwm_spwm_generator.ino

Created : 14/02/2023
Author  : yopie DIY


SPWM generation using ESP32 MCPWM peripheral

Left bridge used for fundamental signal (50Hz), Right bridge for SPWM (20kHz carrier freq.)
Using real time sine calculation instead of Look-Up Table

Copyright (C) 2023 yopie DIY

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*------------------------------------------------------------------------*/  

/*  LIBRARY : esp32 by Espressif Systems ver 2.0.14  */

#include <driver/mcpwm.h>

const int LO1 = 12;                        // PWM output pin Low Side
const int HO1 = 13;                        // PWM output pin High Side
const int LO2 = 14;                        // output pin Low Side
const int HO2 = 27;                        //  output pin High Side
const float freqCarr = 20000.0;            // Carrier freq. Hz
float freqMod = 50.0;                      // Modulation freq. Hz
int amplitude = 300;                       // Amplitude of Sine Wave Output
int sampleNum = int(freqCarr/freqMod);     // samples per cycle sine wave
int i;                                     // PWM index number


/*-------------------------- Define the ISR function ---------------------*/
void IRAM_ATTR MCPWM_ISR(void*) { 
  float sineValue;
  int sineVal;

  WRITE_PERI_REG(0x3FF5E11C, BIT(3));            // INT_CLR_PWM_REG 
                                                 // clear interrupt   
// digitalWrite(23, HIGH);                       
  sineValue=  amplitude * sin (2 * PI * freqMod * (i / freqCarr));
  sineVal = int (sineValue + 0.5);               // integer rounding 
  if (sineVal>0)  {                              // Pos half of sine wave
    WRITE_PERI_REG(0x3FF5E040, sineVal);         // PWM_GEN0_TSTMP_A_REG 
    digitalWrite(LO2,1);                         // sine value->register
  }
  if (sineVal<0) {                               // Neg half of sine wave 
    WRITE_PERI_REG(0x3FF5E040, 400 + sineVal);       
    digitalWrite(HO2,1);
  } 
  if (i==0 ) {                                   // Zero cross (0 deg)
    WRITE_PERI_REG(0x3FF5E040,0);                     
    digitalWrite(HO2,0);
  }
  if (i==(sampleNum/2 )) {                      // Zero Cross (180 deg)
    WRITE_PERI_REG(0x3FF5E040,400);                 
    digitalWrite(LO2,0);
  } 
  i++;
  if (i > sampleNum ) i=0;                      // reset i if > 1 cycle
//  digitalWrite(23, LOW);                      
}

/*------------------------------------------------------------------------*/
void setup() {

  pinMode(23, OUTPUT);
  pinMode(LO2, OUTPUT);
  pinMode(HO2, OUTPUT);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, LO1);   // Use GPIO 12 for MCPWM0A
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, HO1);   // Use GPIO 13 for MCPWM0B

  mcpwm_config_t pwm_config;
  pwm_config.frequency = freqCarr * 2 ;           // up-down ctr->freq*2
  pwm_config.cmpr_a = 0;                          // duty cycle to 0%
  pwm_config.cmpr_b = 0;                          // duty cycle to 0%
  pwm_config.counter_mode = MCPWM_UP_DOWN_COUNTER;// phase freq correct 
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;       // active high PWM
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); 

  WRITE_PERI_REG(0x3FF5E000, 0);                  // PWM_CLK_CFG_REG 
                                                  // pwm_clock no prescaler 
  float tmrRegVal = (((1/freqCarr)/2)/62.5E-9)-1; // timer period calc 
  WRITE_PERI_REG(0x3FF5E004, int(tmrRegVal)<<8);  // PWM_TIMER0_CFG0_REG 
  WRITE_PERI_REG(0x3FF5E03C, 0);                  // PWM_GEN0_STMP_CFG_REG 
                                                  // time update immediately    
   
  // Attach ISR function to MCPWM interrupt
  mcpwm_isr_register(MCPWM_UNIT_0, MCPWM_ISR, NULL, ESP_INTR_FLAG_IRAM, NULL);
  // Deadtime i.e (31 + 1) * 6.25 nS   
  mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_ACTIVE_HIGH_COMPLIMENT_MODE , 31, 31);
  // INT_ENA_PWM_REG enable interrupt on TEZ
  WRITE_PERI_REG(0x3FF5E110, BIT(3));                   
//  Serial.begin(115200);
}

/*-----------------------------------------------------------------------*/
void loop() {
  // nothing to do right now.....
}