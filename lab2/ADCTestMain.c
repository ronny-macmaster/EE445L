/** ADcTestMain.c **
 * Authors: Ronald Macmaster and Parth Adhia
 * Created: September 6th 2016
 * Description: Sample the ADC, and build a pmf / observe time-jitter measurements.
 * utitilizes various timer interrupts
 * Lab: 1
 * TA: Dylan Zika
 * Date: September 9th 2016
 *********************************************************************************/

/** hardware connections **
 * Backlight (pin 10) connected to +3.3 V
 * MISO (pin 9) unconnected 
 * SCK (pin 8) connected to PA2 (SSI0Clk)
 * MOSI (pin 7) connected to PA5 (SSI0Tx)
 * TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 * CARD_CS (pin 5) unconnected
 * Data/Command (pin 4) connected to PA6 (GPIO)
 * RESET (pin 3) connected to PA7 (GPIO)
 * VCC (pin 2) connected to +3.3 V
 * Gnd (pin 1) connected to ground
 * center of X-ohm potentiometer connected to PE3/AIN0
 * bottom of X-ohm potentiometer connected to ground
 * top of X-ohm potentiometer connected to +3.3V 
 **************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "ADCSWTrigger.h"
#include "ST7735.h"
#include "PLL.h"
#include "Timer.h"

#define DEBUG

// process managment
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// port initializations
void PortF_Init(void);
#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))

// data profiling
volatile uint32_t ADCvalue;
volatile static uint8_t recordFlag;
volatile static uint32_t time[1000];
volatile static uint32_t data[1000];
	
// pmf function (size 16 buckets)
#define PMF_SIZE 4096
static uint16_t pmf[PMF_SIZE];
void CalculatePmf(void);
uint32_t CalculateJitter(void);

int main(void){
	// hardware initialization
  PortF_Init();
	PLL_Init(Bus80MHz);
	ST7735_InitR(INITR_REDTAB);
  ADC0_InitSWTriggerSeq3_Ch9(); 
	Timer1A_Init(TIMER_MAXHz);	// Timer1A has Max countdown (53s)
  Timer0A_Init(TIMER_1000Hz); // Timer0A has 100 Hz interrupts
	
	// sampling profile
	recordFlag = true;
	PF2 = 0; // turn off ISR LED
	printf("Started sampling...\n");
  while(recordFlag){
    PF1 = PF1 ^ 0x02;  // toggles while profiling
  } printf("Finished Sampling :)\n\n");
	
	// calculate jitter
	uint32_t jitter = CalculateJitter();

	// build pmf
	CalculatePmf();
	
	#ifdef DEBUG // feedback
	printf("time[0] %u\n", time[0]);
	printf("data[0] %u\n", data[0]);
	printf("time[999] %u\n", time[999]);
	printf("data[999] %u\n", data[999]);
	printf("jitter: %u\n", jitter); 
	#endif
	
}

uint32_t CalculateJitter(){
	// calculate time jitter
	uint32_t minDiff = time[0] - time[1];
	uint32_t maxDiff = time[0] - time[1];
	for(int idx = 1; idx < 1000; idx++){
		// find min and max time diff
		uint32_t diff = time[idx - 1] - time[idx];
		if(diff < minDiff){
			minDiff = diff;
		}
		else if(diff > maxDiff){
			maxDiff = diff;
		}
	}
	uint32_t jitter = maxDiff - minDiff;
	return jitter;
}

void CalculatePmf(){
	/** algorithm: build an ADC frequency table
	 * find x and y ranges
	 * plot the function with ST7735 plot functions
	 */
	for(int idx = 0; idx < PMF_SIZE; idx++){
		pmf[idx] = 0;
	}
	for(int idx = 0; idx < 1000; idx++){
		uint32_t x = data[idx];
		pmf[x] = pmf[x] + 1;
	}
}


void Timer0A_Handler(void){
	// acknowledge timer0A timeout
  TIMER0_ICR_R = TIMER_ICR_TATOCINT; 
	static uint16_t idx = 0;
  PF2 ^= 0x04; // profile
  PF2 ^= 0x04; // profile
  
	// record ADC value
	ADCvalue = ADC0_InSeq3();
	if(idx < 1000){
		time[idx] = TIMER1_TAR_R;
		data[idx] = ADCvalue;
		idx = idx + 1;
	}
	else{ // turn off sampling
		recordFlag = false;
		Timer0A_Disarm();
		Timer0A_Stop();
	}
  PF2 ^= 0x04; // profile
}


// ***************** PortF_Init ****************
// Activate Port F for LED profiling
// Inputs:  none
// Outputs: none
void PortF_Init(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
	delay = SYSCTL_RCGCTIMER_R;      			// allow time to finish activating
	GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000; // configure PF2 as GPIO
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
}

