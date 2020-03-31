#include "mbed.h"

Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int sample = 1000;
int i;

float ADCdata[1000];
float j;

int main(){
    int freq = 0;
    redLED = 0;
    greenLED = 1;
    for (i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        wait(1./sample);
        if(i>=1){
            if(ADCdata[i-1] <= 0.2){
                if(ADCdata[i] > 0.2){
                    freq += 1 ;                    
                }
            }            
        }
    }

    for (i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.1);
    }

    freq = freq*97/100;    
    while(1){
        if( Switch == 0 ){
            redLED = 0;
            greenLED = 1;
            display = table[(freq / 100) % 10];
            wait(1.0);
            display = table[(freq / 10) % 10];
            wait(1.0);
            display = int(table[freq % 10])+0x80;
            wait(1.0);
        }
        if( Switch == 1 ){
            display  = 0x00;
            redLED = 1;
            greenLED = 0;
        }
        for( j=0; j<2; j+=0.05 ){
            Aout = 0.5 + 0.5*sin(j*3.14159);
            wait(1./freq/40);
        }
    }
}