#include "stdint.h" 
#include "C:/Keil/EE319Kware/inc/tm4c123gh6pm.h" // Kotb's PC's Library address to collect and test the code 
/* Mili seconds delay function */
void delay_ms(int n) // Fathy or Rady's part
{
 int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3180;j++)
 {}
}

/* Micro seconds delay function */
void delay_us(int n) // Fathy or Rady's part
{
 int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3;j++)
 {}
}
void LCD_Command(unsigned char Com) {
	GPIO_PORTA_DATA_R = 0;
	GPIO_PORTB_DATA_R = Com;	
	GPIO_PORTA_DATA_R |= 0x80; // 5: RS(Write or Command), 6: Read/Write, 7: Enable
	delay_ms(1);
	GPIO_PORTA_DATA_R = 0;
	delay_ms(1);
}
void LCD_Data(unsigned char data){
	GPIO_PORTA_DATA_R = 0x20;
	GPIO_PORTB_DATA_R = data;
	GPIO_PORTA_DATA_R |= 0x80;
	delay_ms(1);
	// Delay
	
}
void SystemInit() {}
// Test Part
void init(void) {
	//uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x03; // enable clock for Port A
	// delay = 1; // dummy variable.
	while((SYSCTL_RCGCGPIO_R0 & 0x03) == 0) {} // Peripheral Ready Register, to check if the clock is active or not yet.
	// Ports Lock
	GPIO_PORTA_LOCK_R = 0x4C4F434B; // To open the lock of the ports
	GPIO_PORTB_LOCK_R = 0x4C4F434B; // To open the lock of the ports
	GPIO_PORTA_CR_R = 0xE0; // uncommmiting the 8 pins we use.
	GPIO_PORTB_CR_R = 0xFF; // uncommmiting the 3 (5,6,7) pins we use.
	GPIO_PORTA_DIR_R = 0xE0; // we want 8 pins to be OUTPUT.
	GPIO_PORTB_DIR_R = 0xFF; // we want 3 (5,6,7) pins to be OUTPUT.
	GPIO_PORTA_DEN_R = 0xE0; // we want the first 8 pins to be Digital.
	GPIO_PORTB_DEN_R = 0xFF; // we want 3 (5,6,7) pins to be Digital.
	GPIO_PORTA_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTA_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTA_AMSEL_R = 0; // we don't want it to be analog.
	GPIO_PORTB_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTB_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTB_AMSEL_R = 0; // we don't want it to be analog.
	// GPIO_PORTF_PUR_R = 0x11; // Pull Up Resistor for the two switches.
}
int main(){
	init();
	LCD_Command(0x01); // CLEAR
	LCD_Command(0x30); // set to 8 bits / 1 row
	LCD_Command(0x38); // set to 8 bits - 2 rows
	LCD_Command(0x06); // Entry Mode
	LCD_Command(0x0F); // Blinking
	LCD_Command(0x80); // First row
	while(1){
		LCD_Command(0x01);
		LCD_Command(0x80);
		LCD_Data('T');
		LCD_Data('E');
		LCD_Data('S');
		LCD_Data('T');
		LCD_Data('\0');
		LCD_Command(0x0C);
		delay_ms(50);
	}
}