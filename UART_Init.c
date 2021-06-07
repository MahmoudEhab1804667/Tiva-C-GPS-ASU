void UART_Init(void) {
	SYSCTL_RCGCUART_R |= 0x02; // Activate UART1
	SYSCTL_RCGCGPIO_R |= 0x04; // Activate Port C
	UART1_CTL_R &=~ 0x01; // disable UART
	UART1_IBRD_R = 43; // IBRD = int(80,000,000/(16*115,200))
	UART1_FBRD_R = 26; // FBRD = int(0.40278 * 64 + 0.5)
	UART1_LCRH_R = 0x70; // 8 bits, no barity bits, one stop, FIFOs
	UART1_CTL_R |= 0x01; // Enable UART
	GPIO_PORTC_AFSEL_R |= 0x30; // Enable Alternative functions on PC4-5
	GPIO_PORTC_DEN_R |= 0x30; // Digital pins to be configured as UART1
	GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R & 0xFF00FFFF)+0x00220000;
	GPIO_PORTC_AMSEL_R &= ~0x30; // disable analog on PC4-5
}