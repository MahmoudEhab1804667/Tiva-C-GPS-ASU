#include "stdint.h" 
#include "C:/Keil/EE319Kware/inc/tm4c123gh6pm.h"
#include <math.h>

#define redLED 0x02
#define blueLED 0x04
#define greenLED 0x08
/*                                         First Milestone                                        */
/* This Code is to calculate and track the distance made using GPS Tracker on TIVA TM4C123G board */
/* This Code is not yet ready for the second milestone, but it was tested and all works perfectly */
/* Code was integrated and tested  (Software and Hardware) by Mahmoud Fathy Kotb and Mahmoud Ehab */
/* Notes are not totally right due to changes made after detection of some mistakes were existed. */
/*      The notes are right in the meaning, but wrong in its place, we will change them later.    */
/* The code is showing a dummy calculated distance on LCD with LED indicate that you  reached 100m*/


double d = 0; // distance calculated
char num[3] = "0"; // distance calculated as string
int i = 0; // counter
int y = 0; // integer value of d

void SystemInit() {}
void delay_ms(int n);
void delay_us(int n);
void LCD_Command(unsigned char Com);
void LCD_Data(unsigned char data);
void init(void);	
double calculate_Distance(double latHome, double lonHome, double latDest, double lonDest);
void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void distanceEqual(void);
void closeMain(void);

int main(){
	init();
	while(1){
		LCD_Command(0x01);
		LCD_Command(0x80);
		distanceEqual();
		delay_ms(1);
		d = calculate_Distance(30.04644, 31.31281, 30.04639, 31.31139);
		y = d;
		intToStr(y, num, 3);
		for (;i < 3; i++){
			LCD_Data(num[i]);
			delay_ms(1);
		}
		if (i == 2) {
			i = 0;
		}
		if (d >= 100) {
			GPIO_PORTF_DATA_R = redLED;
		}
		else if (d < 100) {
			GPIO_PORTF_DATA_R = greenLED;
		}
		closeMain();
		i = 0;
	}
}

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
	// GPIO_PORTC_AMSEL_R &=~ 0x30;
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

void reverse(char* str, int len){
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int intToStr(int x, char str[], int d){
    int z = 0;
    while (x) {
        str[z++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (z < d)
        str[z++] = '0';

    reverse(str, z);
    str[z] = '\0';
    return z;
}
void distanceEqual(void){
		LCD_Data('D');
		delay_ms(1);
		LCD_Data('i');
		delay_ms(1);
		LCD_Data('s');
		delay_ms(1);
		LCD_Data('t');
		delay_ms(1);
		LCD_Data('a');
		delay_ms(1);
		LCD_Data('n');
		delay_ms(1);
		LCD_Data('c');
		delay_ms(1);
		LCD_Data('e');
		delay_ms(1);
		LCD_Data(' ');
		delay_ms(1);
		LCD_Data('=');
		delay_ms(1);
		LCD_Data(' ');
		delay_ms(1);
}
double calculate_Distance(double latHome, double lonHome, double latDest, double lonDest) {
    double pi = 3.141592653589793;
    double Radius = 6371.0;
    double latHome2 = (pi / 180.0) * (latHome);
    double latDest2 = (pi / 180.0) * (latDest);
    double difference_Lat = (pi / 180.0) * (latDest - latHome);
    double difference_Lon = (pi / 180.0) * (lonDest - lonHome);
    double a = sin(difference_Lat / 2.0) * sin(difference_Lat / 2.0) + cos(latHome2) * cos(latDest2) * sin(difference_Lon / 2.0) * sin(difference_Lon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    double distance = Radius * c * 1000;
    return distance;
}
void closeMain(void){
		LCD_Data(' ');
		delay_ms(1);
		LCD_Data('m');
		delay_ms(1);
		LCD_Data('\0');
		delay_ms(1);
		LCD_Command(0x0C); // turn off cursor
		delay_ms(2000);
}