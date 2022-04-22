#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

void UART0_Config(void);
void txchar(char c);


void Delay(unsigned long counter);




int main(){

    char data_Chr[10];
    float data;
    UART0_Config();
    for(int i=0;i<417;i++);

    while(1){
        data = 200.0;
        sprintf(data_Chr, "%f", data);
        for(int i=0;i<5;i++){
            txchar(data_Chr[i]);
        }
        txchar('\r');
        txchar('\n');


        Delay(10);

    }

    return 0;
}


void UART0_Config(void){

    SYSCTL_RCGCUART_R |= 0x01;  /* enable clock to UART0 */
    while((SYSCTL_PRUART_R & 0x01) == 0);//Wait for activation
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R0;
    while((SYSCTL_PRGPIO_R & 0x01) == 0);//Wait for activation
    UART0_CTL_R &= ~(0x01); //UART0 deactivated UARTEN = 0
    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PUR_R &= ~(0x03);
    GPIO_PORTA_PDR_R &= ~(0x03);
    GPIO_PORTA_DIR_R &= ~(0x01); //Input PA0
    GPIO_PORTA_DIR_R |= ~(0x02); //Output PA1
    GPIO_PORTA_PCTL_R |= (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011;
    GPIO_PORTA_DEN_R |= 0x03;
    UART0_IBRD_R = (UART0_IBRD_R & ~0xFFFF) | 0x68;
    UART0_FBRD_R = (UART0_FBRD_R & ~0x3F) | 0xB;
    UART0_LCRH_R = (UART0_LCRH_R & ~0xFF) | 0x70;
    UART0_CTL_R &= ~0x20;
    UART0_CTL_R |=0x301;
}

void txchar(char c){

    while((UART0_FR_R & UART_FR_TXFF) != 0);
    UART0_DR_R = c;
}



void Delay(unsigned long counter)
{
    unsigned long i = 0;

    for(i=0; i< counter*10000; i++);
}
