#include "stdint.h" 
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "C:/Keil/EE319Kware/inc/tm4c123gh6pm.h"
#include <math.h>
#include "string.h"
#include <time.h>

#define redLED 0x02
#define blueLED 0x04
#define greenLED 0x08

void SystemInit() {}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* This Code is to calculate and track the distance made using GPS Tracker on TIVA TM4C123G board */
/* This Code is not yet ready for the second milestone, but it was tested and all works perfectly */
/* Code was integrated and tested  (Software and Hardware) by Mahmoud Fathy Kotb and Mahmoud Ehab */
/* Notes are not totally right due to changes made after detection of some mistakes were existed. */
/*      The notes are right in the meaning, but wrong in its place, we will change them later.    */
/* The code is showing a dummy calculated distance on LCD with LED indicate that you  reached 100m*/


double d = 0; // distance calculated
char num[3] = "0"; // distance calculated as string
int i = 0; // counter
//int j = 0; // counter
//int k = 0; // counter
//int x = 0; //
//int z = 0; //
int y = 0; // integer value of d
//int l = 0;
double mVelocity = 0;
char sTime[10];
int cc = 0;
int flag = 0;
//int tempY = 0;
//int iDistance = 0;
char tempData[55];
double finalTime = 0;
//int Integer = 0;
//double Fraction = 0;

//char GPS_getData[55];
//char stringOfData[55];
char Longitude[12];
char Latitude[12];
//char ccDistance[12];
char sVelocity[6];
double doldLatitude = 0;
double dnewLatitude = 0;
double doldLongitude = 0;
double dnewLongitude = 0;
double tempLat = 0;
double tempLon = 0;
double dTime = 0;
double calcDist = 0;
double fiveCyclesError = 0;
clock_t start;
clock_t end;
// double dDeltaDistance = 0;
char* ptr1;
char* ptr2;
char* cMessage;
char* cTime;
char* cDataValid;
char* newLatitude;
char* NS;
char* newLongitude;
char* EW;
// char* cSpeed;


void LCD_Command(unsigned char Com);
void init(void);
void delay_ms(int n);
void delay_us(int n);
void LCD_Data(unsigned char data);
double calculate_Distance(double latHome, double lonHome, double latDest, double lonDest);
void reverse(char* str, int len);
int intToStr(int x, char str[], int d);
void doubleToString(double n, char* res, int afterpoint);
char readChar(void);
bool bGetData(void);
void printString(char* string);
void clearLCD(void);
void displayLCD(char* string1,char* string2);

int main(){
	init();
	printString("Initializing..");
	LCD_Data('\0');
	delay_ms(10000);
	clearLCD();
	while(1){
		clearLCD();
		bGetData(); // gives back a string (tempData) contains GPS GPGGA line.
		cMessage = strtok(tempData,","); // cuts $GPGGA to before the first comma
		newLatitude = strtok(NULL,","); // Cuts Latitude as string
		NS = strtok(NULL,","); // cuts S or N as string
		newLongitude = strtok(NULL,","); // Cuts Longitude as string
		EW = strtok(NULL,","); // cuts E or W as string
		cTime = strtok(NULL,","); // cuts time (connected to PC is a must here.
		cDataValid = strtok(NULL,","); // Cuts Validation Check (A/V)

		// Check validation of data
		if (strcmp(cDataValid,"V") == 0){
			printString("Initializing GPS");
			delay_ms(1000);
		}
		else {
			start = clock();
			tempLat = strtod(newLatitude,&ptr1);
			tempLon = strtod(newLongitude, &ptr2);
			dnewLatitude = (int)(tempLat/100)+((tempLat-((int)(tempLat/100)*100))/60);
			dnewLongitude = (int)(tempLon/100)+((tempLon-((int)(tempLon/100)*100))/60);
			if ((int)doldLatitude == 0) {
				doldLatitude = dnewLatitude;
			}
			if ((int)doldLongitude == 0) {
				doldLongitude = dnewLongitude;
			} // for initializing
			
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			calcDist = calculate_Distance(doldLatitude, doldLongitude, dnewLatitude, dnewLongitude);
			d = d + calculate_Distance(doldLatitude, doldLongitude, dnewLatitude, dnewLongitude);
			y = d; // integer
			
			doubleToString(dnewLatitude, Latitude, 8);
			doubleToString(dnewLongitude, Longitude, 8);
			
			end = clock();
			dTime = ((double)(end-start))/CLOCKS_PER_SEC;
			finalTime = 1 + dTime * 2;
			mVelocity = calculate_Distance(doldLatitude, doldLongitude, dnewLatitude, dnewLongitude)/finalTime;
			doubleToString(mVelocity,sVelocity,2);
			// to indicate if we reached the distance or not.
			if (d >= 100) {
				GPIO_PORTF_DATA_R &=~ greenLED;
				GPIO_PORTF_DATA_R |= redLED;
			}
			else if (d < 100) {
				GPIO_PORTF_DATA_R &=~ redLED;
				GPIO_PORTF_DATA_R |= greenLED;
			}
			
			// Using switches to control the data shown on display.
			if ((GPIO_PORTF_DATA_R & 0x11) == 0x10) { // check if switch 2 on pin 0 is on - Show speed
				displayLCD("Speed(m/s)~=",sVelocity);
				delay_ms(1000);
				clearLCD();
				delay_ms(1);
			}
			else if ((GPIO_PORTF_DATA_R & 0x11) == 0x01) { // check if switch 4 on pin 0 is on - Show Longitude and Latitude
				displayLCD("Lat:",Latitude);
				delay_ms(1000);
				clearLCD();
				delay_ms(1);
				displayLCD("Lon:",Longitude);
				delay_ms(1000);
				clearLCD();
			}
			else { // Show distance
				intToStr(y, num, 3);  /////// e7na hnaaaaaaaaaaaaaaaaaaaaaaaaaaa
				printString("Distance = ");
				for (;i < 3; i++){
				LCD_Data(num[i]);
				delay_ms(1);
				}
				printString(" m");
				LCD_Data('\0');
				i = 0;
				//if (i == 2) {
				//i = 0;
				//}
				}
		}
		if (flag%7 == 0 && y > 0 && calcDist > 0.5){
			d = d + 1;
		}
		LCD_Command(0x0C); // turn off cursor
		delay_ms(500);
		i = 0;
		doldLatitude = dnewLatitude;
		doldLongitude = dnewLongitude;
		flag++;
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

void clearLCD(void){
		LCD_Command(0x01);
		LCD_Command(0x80);
}

void init(void) {
					// PC4: U1Rx, PC5: U1Tx, PB0~7: LCD Data wires, PA5~7: LCD Command pins, PF0~4: 2 switches and 3 leds.
	SYSCTL_RCGCGPIO_R |= 0x27; // enable clock for Port A, B, C and F
	SYSCTL_RCGCUART_R |= 0x02; // Activate UART1
	while((SYSCTL_RCGCGPIO_R0 & 0x27) == 0) {} // Peripheral Ready Register, to check if the clock is active or not yet.
	delay_us(10);
					// UART Initialization
	UART1_CTL_R &=~ 0x01; // disable UART
	UART1_IBRD_R = 104; // IBRD = int(80,000,000/(16*115,200))
	UART1_FBRD_R = 11; // FBRD = int(0.40278 * 64 + 0.5)
	UART1_LCRH_R = 0x70; // 8 bits, no barity bits, one stop, FIFOs
	UART1_CTL_R |= 0x301; // Enable UART
	GPIO_PORTC_LOCK_R = 0x4C4F434B; // Why do we use it?
	GPIO_PORTC_CR_R |= 0x30; // uncommmiting the 2 (4, 5) pins
	GPIO_PORTC_AFSEL_R |= 0x30; // Enable Alternative functions on PC4-5
	GPIO_PORTC_DEN_R |= 0x30; // Digital pins to be configured as UART1
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF)+0x00220000;
	GPIO_PORTC_AMSEL_R &= ~0x30; // disable analog on PC4-5
	////////////////////////////////////////////////////////////////////
					// Ports Lock
	GPIO_PORTA_LOCK_R = 0x4C4F434B; // To open the lock of the ports
	GPIO_PORTB_LOCK_R = 0x4C4F434B; // To open the lock of the ports
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // Why do we use it?
	GPIO_PORTA_CR_R = 0xE0; // uncommmiting the 3 (5,6,7) pins we use.
	GPIO_PORTB_CR_R = 0xFF; // uncommmiting the 8 pins we use. 
	GPIO_PORTF_CR_R = 0x1F; // uncommmiting the 5 pins we use.
	GPIO_PORTA_DIR_R = 0xE0; // we want 3 (5,6,7) pins to be OUTPUT.
	GPIO_PORTB_DIR_R = 0xFF; // we want 8 pins to be OUTPUT. 
	GPIO_PORTF_DIR_R = 0x0E; // we want pins 1,2,3 to be OUTPUT.
	GPIO_PORTA_DEN_R = 0xE0; // we want 3 (5,6,7) pins to be Digital.
	GPIO_PORTB_DEN_R = 0xFF; // we want the first 8 pins to be Digital. 
	GPIO_PORTF_DEN_R = 0x1F; // we want the first 5 pins to be Digital.
	GPIO_PORTA_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTA_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTA_AMSEL_R = 0; // we don't want it to be analog.
	GPIO_PORTB_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTB_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTB_AMSEL_R = 0; // we don't want it to be analog.
	GPIO_PORTF_AFSEL_R = 0; // Alternative Function select - we don't want it.
	GPIO_PORTF_PCTL_R = 0; // Port Control - we don't need it.
	GPIO_PORTF_AMSEL_R = 0; // we don't want it to be analog. 
	GPIO_PORTF_PUR_R = 0x11; // Pull Up Resistor for the two switches.
					// LCD Command Initialization
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
void doubleToString(double n, char* res, int afterpoint){
    // Extract integer part
    int ipart = (int)n;
  
    // Extract floating part
    double fpart = n - (double)ipart;
  
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
  
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
  
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
  
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
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

bool bGetData(void){
	while(1){
 		int Commas = 0;
	  tempData[0] = readChar();	
		if(tempData[0] == '$'){
				tempData[1] = readChar();
			if(tempData[1] == 'G'){
						tempData[2] = readChar();	
				if(tempData[2] == 'P'){
								tempData[3] = readChar();
					if(tempData[3] == 'G'){
										tempData[4] = readChar();
						if(tempData[4] == 'L'){
												tempData[5] = readChar();
							if(tempData[5] == 'L'){
								for ( cc = 6; cc < 55; cc++)
								{
									if (tempData[cc] == ',') Commas++;
									tempData[cc]= readChar();
									if (Commas == 6){
										if( readChar()=='A'){
											Commas=0;
											cc = 0;
											return 1;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
char readChar(void){
	char c;
	while ((UART1_FR_R & 0x10) != 0){};
	c = UART1_DR_R;
	return c;
}
void printString(char* string) {
	while(*string){
		LCD_Data(*(string++));
	}
}
void displayLCD(char* string1,char* string2){
    clearLCD();
    delay_ms(1);
    printString(string1);
    LCD_Data('\0');
    delay_ms(2);
    LCD_Command(0xC0);
    printString(string2);
    LCD_Data('\0');
    delay_ms(2);
    LCD_Command(0x0C); // turn off cursor
    delay_ms(1);
}
