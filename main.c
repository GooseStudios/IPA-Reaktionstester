#include <u8g2.h>
#include <util/delay.h>
#include <u8x8_avr.h>
#include <avr/power.h>

//Pinbelegung CS
#define CS_DDR DDRB
#define CS_PORT PORTB //Port B
#define CS_BIT 2 //PIn 2

//Pinbelegung DC
#define DC_DDR DDRB 
#define DC_PORT PORTB //Port B
#define DC_BIT 1 //Pin 1

//Pinbelegung Reset
#define RESET_DDR DDRC 
#define RESET_PORT PORTC //Port C
#define RESET_BIT 6 // Pin 6

u8g2_t u8g2; //u8g2 wird defniniert

uint8_t
u8x8_gpio_and_delay (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) { //Funkion wird definiert
 
  if (u8x8_avr_delay(u8x8, msg, arg_int, arg_ptr))
    return 1;

  switch (msg) { //
    // called once during init phase of u8g2/u8x8
    // can be used to setup pins
    case U8X8_MSG_GPIO_AND_DELAY_INIT: //Pin CS/DC/Reset werden initialisiert
      CS_DDR |= _BV(CS_BIT); //Pin CS wird als Ausgang definiert
      DC_DDR |= _BV(DC_BIT); //Pin DC wird als Ausgang definiert
      RESET_DDR |= _BV(RESET_BIT);
      break;
    // CS (chip select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_CS:
      if (arg_int)//Wenn arg_int nicht null ist
        CS_PORT |= _BV(CS_BIT);
      else
        CS_PORT &= ~_BV(CS_BIT);
      break;
    // DC (data/cmd, A0, register select) pin: Output level in arg_int
    case U8X8_MSG_GPIO_DC:
      if (arg_int)//Wenn arg_int nicht null ist
        DC_PORT |= _BV(DC_BIT); //Pin CS wird auf high gesetzt
      else
        DC_PORT &= ~_BV(DC_BIT); //Pin CS wird geloescht
      break;
    // Reset pin: Output level in arg_int
    case U8X8_MSG_GPIO_RESET:
      if (arg_int) //Wenn arg_int nicht null ist
        RESET_PORT |= _BV(RESET_BIT); //Reset wird high gesetzt
      else
        RESET_PORT &= ~_BV(RESET_BIT); //Reset wird geloescht
      break;
    default:
      u8x8_SetGPIOResult(u8x8, 1); //Wird zur ueberpruefung von fehlern genutzt
      break;
  }
  return 1;
}

int main (void) {
  u8g2_Setup_sh1106_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_spi,u8x8_gpio_and_delay);//Kommunikation ueber Hardware SPI
  //Displaytyp mit gegebenen Pixel( keine Rotation, Hardware SPI als Kommunikation)
  
  u8g2_InitDisplay(&u8g2); //Initialisiert das display fertig
  u8g2_SetPowerSave(&u8g2, 0); //Weckt das Display auf

  while (1) {
    u8g2_ClearBuffer(&u8g2);// Loescht den Buffer
    u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr); //Schriftart wird definiert
    u8g2_DrawStr(&u8g2, 0, 15, "Hello World!!"); //string wird auf zeile 0, 15 geschrieben
    u8g2_SendBuffer(&u8g2);// Sendet daten an den Buffer
  }
}
