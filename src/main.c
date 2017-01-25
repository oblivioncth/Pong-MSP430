#include <msp430.h>
#include <stdio.h>
#include <math.h>
#include "sndmodule.h"
#include "dispmodule.h"

#define padlocres 16  //paddle location resolution
#define padwidth 4
unsigned int p1pad,p2pad,iBallX,iBallY,balld,iP1Score,iP2Score,fc,spd;
unsigned int iPad1Y = 0 ;
unsigned int iPad2Y = 0 ;

unsigned int iPad1YL = 0;
unsigned int iPad2YL = 0;
unsigned int iBallXL = 0;
unsigned int iBallYL = 0;
volatile float ballsub,ballud;
const float abspad = 64/(float)padlocres;
void score(int player);
void reset(void);


void main(void) {

WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

//CHANGE CLOCK FREQUENCY TO 2MHz, and restore SMCLK to 1MHz
  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_2;                      // Select DCO range 24MHz operation
  UCSCTL2 = FLLD_1 + 60;                // Set DCO Multiplier for 2MHz
											// (N + 1) * FLLRef = Fdco
											// (60 + 1) * 32768 = 2MHz
											// Set FLL Div = fDCOCLK/2
  UCSCTL5 = DIVS_1;
  __bic_SR_register(SCG0);                  // Enable the FLL control loop
  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 2 MHz / 32,768 Hz = 62500 = MCLK cycles for DCO to settle
  __delay_cycles(62500);//
  // Loop until XT1,XT2 & DCO fault flag is cleared
  do
  {
	UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
											// Clear XT2,XT1,DCO fault flags
	SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
// END FREQUENCY CHANGE

P1DIR = 0x00; // Set to input direction
P2DIR = 0x01;
P3DIR = 0xFF; //set output dir


P2IE =BIT3+BIT2;// start inturrpt management
P1IE = BIT1+BIT2+BIT3;
P2IES = BIT3+BIT2;
P1IES = BIT3+BIT2;
P1REN = BIT1+BIT2+BIT3;
P2REN = BIT3+BIT2;
P1OUT= BIT1+BIT2+BIT3;
P2OUT = BIT3+BIT2;

P2IFG =0;
P1IFG =0;
fnInitializeSound();		//initialize sounds
fnInitializeDisplay();



p1pad=padlocres >> 1;
p2pad=padlocres >> 1;
iBallX=64;
iBallY=32;
balld=0;
ballud=0;
iP1Score=0;
iP2Score=0;
P1IV=0;
P2IV=0;


TA0CCR0 =10000; //start clock management
TA0CTL=TASSEL_2+MC_1;
TA0CCTL0 |= CCIE;//add ccie if start on boot
P2SEL |= 0x01; //workaround
TB0CTL=TBSSEL_1+MC_1;
TB0CCR0=1092; 		// 30fps  32768/fps
TB0CCTL0 |= CCIE;
_BIS_SR(LPM4_bits + GIE); //Debug LPM0
}

void reset(){
	p1pad=padlocres >> 1;
	p2pad=padlocres >> 1;
	iBallX=64;
	iBallY=32;
	balld=0;
	ballud=0;
	iP1Score=0;
	iP2Score=0;
	P1IV=0;
	P2IV=0;
	TA0CCR0 =10000;
	TA0CTL|=CCIE;
	fnInitializeDisplay();
}


#pragma vector=PORT1_VECTOR;
__interrupt void PORT_1(void)
{
	switch(P1IFG) //cascading button press for player 1 and reset
	{
	case 0b00001100 :
	case 0b00000100 : //Player 1 Up
		if(p1pad<padlocres){
			__delay_cycles(2000); //1ms delay at 2MHz
			if(!(P1IN & BIT2))
			p1pad++;		//up
		}
			P1IFG &=~(BIT2);
			break;
	case 0b00001000 :
		if(p1pad>0){
			__delay_cycles(2000); //1ms delay at 2MHz
			if(!(P1IN & BIT3))
			p1pad--;		//down

		}
		P1IFG &=~(BIT3);
		break;
	case 0b00000010 :
	case 0b00000110 :
	case 0b00001010 :
	case 0b00001110 :			//reset game
		reset();
		P1IFG &=~(BIT1);
		break;
	default:
		break;
	}
}
#pragma vector=PORT2_VECTOR;
__interrupt void PORT_2(void)
{
switch(P2IFG) //cascading button press for player 2
{
case 0b00001100 :
case 0b00000100 :
	if(p2pad<padlocres){
		__delay_cycles(2000); //1ms delay at 2MHz
		if(!(P2IN & BIT2))
		p2pad++;		//up

	}
		P2IFG &=~(BIT2);
		break;
case 0b00001000 :
	if(p2pad>0){
		__delay_cycles(2000); //1ms delay at 2MHz
		if(!(P2IN & BIT3))
		p2pad--;		//down

	}
	P2IFG &=~(BIT3);
	break;
default: break;
}
}
#pragma vector=TIMER0_A0_VECTOR;
__interrupt void Timer_A0(void)  //ball movements
{
	switch(iBallX)		//start x movement interrupt
	{
	case 4:
		if(abs(iBallY-(p1pad*abspad))<=(padwidth+1))
		{														//collison detection player 1 side
			balld=1;
			ballud=.15*(iBallY-(p1pad*abspad))+ballud;
			fnPlaySound(1,spd);
			iBallX++;
		}
		else
			score(1);
		break;
	case 124:
		if(abs(iBallY-p2pad*abspad)<=(padwidth+1))
		{														//collison detection player 1 side
			balld=0;
			ballud=.15*(iBallY-(p2pad*abspad));
			fnPlaySound(1,spd);
			iBallX--;
		}
		else
			score(0);
		break;
	default:				// not on screen edge
		if (balld)
			iBallX++;
		else
			iBallX--;
		break;
	}
	switch(iBallY)
	{
	case 0:
		if (ballud<0)
		{
			ballud= -ballud;
			fnPlaySound(0,spd);
		}
		ballsub+=ballud;
		if (ballsub>1||ballsub<-1){			//ball y axis subpixel presicion
			while(ballsub > 1 && ~(iBallY==63)){
				iBallY++;
				ballsub = ballsub - 1;
			}
			while(ballsub < -1 && ~(iBallY==0)){
				iBallY--;
				ballsub = ballsub + 1;
			}

		}
		break;
	case 63:
		if (ballud>0)
		{
			ballud= -ballud;
			fnPlaySound(0,spd);

		}
		ballsub+=ballud;
		if (ballsub>1||ballsub<-1){			//ball y axis subpixel presicion
			while(ballsub > 1 && ~(iBallY==63)){
				iBallY++;
				ballsub = ballsub - 1;
			}
			while(ballsub < -1 && ~(iBallY==0)){
				iBallY--;
				ballsub = ballsub + 1;
			}

		}
		break;
	default:
		ballsub+=ballud;
		if (ballsub>1||ballsub<-1){			//ball y axis subpixel presicion
			while(ballsub > 1 && ~(iBallY==63)){
				iBallY++;
				ballsub = ballsub - 1;
			}
			while(ballsub < -1 && ~(iBallY==0)){
				iBallY--;
				ballsub = ballsub + 1;
			}

		}
		break;
	}

}
void score(player){
if (player)
	iP2Score++;
else
	iP1Score++;
	fnPlaySound(2,spd);

	p1pad=padlocres>>1;
	p2pad=padlocres>>1;
	iBallX=64;
	iBallY=32;
	balld=0;
	ballud=0;
	P1IV=0;
	P2IV=0;
	TA0CCR0 =10000;
	spd=0;
	if(iP2Score==5||iP1Score==5)
		TA0CTL &= ~(CCIE);
		fnScoreDisplay();

}

#pragma vector=TIMER0_B0_VECTOR;
__interrupt void Timer_B0(void)  //game acceleration
{
	fc++;
	if(p1pad==padlocres)
		iPad1Y=63;
	else
		iPad1Y=p1pad*abspad;
	if(p2pad==padlocres)
		iPad2Y=63;
	else
		iPad2Y=p2pad*abspad;
	fnRedrawScreen();
	if (fc>200)	//number of frames before acceleration
	{
		TA0CCR0=TA0CCR0*.96;
		fc=0;
		spd++;
	}
}
