#include "stdint.h" 
#include "C:/Keil/EE319Kware/inc/tm4c123gh6pm.h" // Code will be collected on Qotp's PC, address is related to his PC.
#include <math.h>

#define redLED 0x02
#define blueLED 0x04
#define greenLED 0x08

// Don't use PORTC until update, it's not ready yet. //
double d = 0;
char num[3] = "0";
int i = 0;
int y = 0;

void SystemInit() {}

void delay_ms(int n){
 int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3180;j++)
 {}
}

void delay_us(int n){
 int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3;j++)
 {}
}
void init(void) {
	SYSCTL_RCGCGPIO_R |= 0x23; // enable clock for Port A, B and F
	// Ports Lock
	GPIO_PORTA_LOCK_R = 0x4C4F434B; // To open the lock of the ports
	GPIO_PORTB_LOCK_R = 0x4C4F434B; // To open the lock of the ports
	// GPIO_PORTC_LOCK_R = 0x4C4F434B;// To open the lock of the ports
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // Why do we use it?
	GPIO_PORTA_CR_R = 0xE0; // uncommmiting the 8 pins we use.
	GPIO_PORTB_CR_R = 0xFF; // uncommmiting the 3 (5,6,7) pins we use.
	// GPIO_PORTC_CR_R = 0x30; // uncommmiting the 2 (4,5) pins we use.
	GPIO_PORTF_CR_R = 0x1F; // uncommmiting the 5 pins we use.
	while((SYSCTL_RCGCGPIO_R0 & 0x27) == 0) {} // Peripheral Ready Register, to check if the clock is active or not yet.
	GPIO_PORTA_DIR_R = 0xE0; // we want 8 pins to be OUTPUT.
	GPIO_PORTB_DIR_R = 0xFF; // we want 3 (5,6,7) pins to be OUTPUT.
	// GPIO_PORTC_DIR_R = 0x30; is it will be I/O?
	GPIO_PORTF_DIR_R = 0x0E; // we want pins 1,2,3 to be OUTPUT.
	GPIO_PORTA_DEN_R = 0xE0; // we want the first 8 pins to be Digital.
	GPIO_PORTB_DEN_R = 0xFF; // we want 3 (5,6,7) pins to be Digital.
	// GPIO_PORTC_DEN_R |= 0x30;
	GPIO_PORTF_DEN_R = 0x1F; // we want the first 5 pins to be Digital.
	GPIO_PORTA_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTA_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTA_AMSEL_R = 0; // we don't want it to be analog.
	GPIO_PORTB_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTB_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTB_AMSEL_R = 0; // we don't want it to be analog.
	// GPIO_PORTC_AFSEL_R |= 0x30;
	// GPIO_PORTC_PCTL_R = ?;
	// GPIO_PORTC_AMSEL_R &=~ 0x30; PORTC is still not ready to be used.
	GPIO_PORTF_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTF_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTF_AMSEL_R = 0; // we don't want it to be analog.
	GPIO_PORTF_PUR_R = 0x11; // Pull Up Resistor for the two switches.
	LCD_Command(0x01); // CLEAR
	LCD_Command(0x30); // set to 8 bits / 1 row
	LCD_Command(0x38); // set to 8 bits - 2 rows
	LCD_Command(0x06); // Entry Mode
	LCD_Command(0x0F); // Blinking
	LCD_Command(0x80); // First row
}
int main () {
	// Main Code to be here

)