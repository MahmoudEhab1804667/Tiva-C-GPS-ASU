#include "stdint.h" 
#include "C:/Keil/EE319Kware/inc/tm4c123gh6pm.h" // Code will be collected on Qotp's PC, address is related to his PC.
#include <math.h>

#define redLED 0x02
#define blueLED 0x04
#define greenLED 0x08

double d = 0;
char num[3] = "0";
int i = 0;
int y = 0;

void SystemInit() {}

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

int main(){
	init();
	while(1){
		LCD_Command(0x01); // Kotb and Ehab's part, to be collected later
		LCD_Command(0x80); // Kotb and Ehab's part, to be collected later
		distanceEqual();
		delay_ms(1);
		d = calculate_Distance(30.04644, 31.31281, 30.04639, 31.31139); // Kemi's part, to be collected later .. dummy test variables.
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