#include <msp430f5529.h>
#include <stdio.h>
#include <stdlib.h>
#include "sndmodule.h"

void fnPlaySound(int iSound, int iGmeSpd) //Input desired sound and the current scaled game speed to play
{
	TA2R = 0;             // Reset Timer_A2
	switch (iSound)
	{
		case 0: //Ball Collide
			TA1CCR0 = 1000000/(400+10*iGmeSpd);
			TA1CCR1 = TA1CCR0/2; //Set duty cycle percent to 50% based on sound freq
			TA1CTL |= MC_1;               //Start sound driver timer
			TA2CCR0 = 553;              // Set Timer1 A0's max count to 64,000 cycles (64 milisec sound length)
			TA2CTL |= MC_1;               // Start Timer1 A0 (Play time counter)
		break;
		case 1: //paddle collide
			TA1CCR0 = 1000000/(10*iGmeSpd);
			TA1CCR1 = TA1CCR0/2; //Set duty cycle percent to 50% based on sound freq
			TA1CTL |= MC_1;               //Start sound driver timer
			TA2CCR0 = 1000;              // Set Timer1 A0's max count to 64,000 cycles (64 milisec sound length)
			TA2CTL |= MC_1;               // Start Timer1 A0 (Play time counter)
		break;
		case 2: //score
			TA1CCR0 = 1000000/(200);
			TA1CCR1 = TA1CCR0/2; //Set duty cycle percent to 50% based on sound freq
			TA1CTL |= MC_1;               //Start sound driver timer
			TA2CCR0 = 30000;              // Set Timer1 A0's max count to 64,000 cycles (64 milisec sound length)
			TA2CTL |= MC_1;               // Start Timer1 A0 (Play time counter)
		break;

	}
}

void fnInitializeSound() //Run function before using speaker
{
	TA1CCTL1 = OUTMOD_7;                // Set TimerA_1.1 to reset/set mode
	TA1CTL = TASSEL_2 + MC_0;           // Set TimerA_1 source to SMCLK and count mode to up
    TA2CTL = TASSEL_1 + MC_0+ID_3;      // Set TimerA_2 source to SMCLK and to Halt
    TA2CCTL0 |= CCIE;					// Set TimerA_2.0 to Compare mode
}

#pragma vector = TIMER2_A0_VECTOR       //Timer A0 interrupt service routine
__interrupt void Timer_A2(void) //Sound stop ISR
{
	TA1CTL &=~(BIT4+BIT5);            // Stop sound driving timer
	TA2CTL &=~(BIT4+BIT5);            // Stop sound duration timer
	TA2R = 0;             // Reset Timer_A2
}
