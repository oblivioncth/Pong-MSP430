//DUE TO MSP430's METHOD OF PORT REFERRAL, A PORT CANNOT SIMPLY BE DEFINED.
//THERFORE KEEP IN MIND THE FOLLOWING WHEN DOING PORT I/O:

//SCLK - P6.3 which is the seriel CLK output UCA0CLK (P2.7 is actual SCLK)
//SI - P6.4 which is also UCA0TXBUF (Serial output buffer) (P3.3 is actual SI)
//A0 - P6.0
//RESET - P6.1
//CS - P6.2

#include <msp430f5529.h>
#include <stdio.h>
#include <stdlib.h>
#include "dispmodule.h"

void comm_out(char c)  //Command out for LCD - DO NOT CALL
{
  int i;
  P6OUT &= ~(BIT2 + BIT0); //Set CS low, Set A0 low
  for(i=0;i<8;i++)
  {
    if((c & 0x80) == 0x80) //Checks if first bit is a 1
    {
      P6OUT |= BIT4; //Set
      P6OUT &= ~(BIT3);
      P6OUT |= BIT3;
    }
    else
    {
	  P6OUT &= ~(BIT4);
	  P6OUT &= ~(BIT3);
	  P6OUT |= BIT3;
    }
    c = c<<1;
  }
  P6OUT |= BIT2 + BIT0; //Set CS high, Set A0 high
}

void data_out(unsigned char d) //Data out for LCD - DO NOT CALL
{
  int i;
  P6OUT &= ~BIT2; //Set CS low
  for(i=0;i<8;i++)
  {
    if((d & 0x80) == 0x80) // Checks to see if first bit is a 1
    {
      P6OUT |= BIT4;
      P6OUT &= ~(BIT3);
      P6OUT |= BIT3;
    }
    else
    {
      P6OUT &= ~(BIT4);
      P6OUT &= ~(BIT3);
      P6OUT |= BIT3;
    }
    d = d<<1;
  }
  P6OUT |= BIT2;  //Set CS high
}

void clearA1Z(void) //Clear for LCD
{
    unsigned int a, i;
	unsigned char page = 0xB0;
    unsigned int x = 0;

	for(i=0;i<8;i++)
	{
		comm_out(page);
		comm_out(0x10);
		comm_out(0x00);
		for (a=0;a<128;a++)
		{
		 	data_out(0x00);
		}
                 x+=128;
		 page++;
	}
}

void fnClearElements(void) //Clear elements for LCD - DO NOT CALL
{
	unsigned int iPage;
	float fTemp;
	unsigned int iCompNum;
	unsigned int iPix;

/////////////////////Clear Ball//////////////////////////////
	fTemp = iBallYL/(float)8;
	iPage = (int)fTemp;
	iCompNum = (int)((fTemp-iPage)*8);

	if(iCompNum == 0)
	{
		comm_out(iPage+0xB0);
		comm_out(0x10+((iBallXL-1) >> 4));
		comm_out(0x00+((iBallXL-1) & 0x0F));
		for(iPix = 0; iPix < 3; iPix++)
		{
			data_out(0x00); //2 lines of ball at top of page
		}
		if(iBallYL != 0)
		{
			comm_out(iPage-1+0xB0);
			comm_out(0x10+((iBallXL-1) >> 4));
			comm_out(0x00+((iBallXL-1) & 0x0F));
			for(iPix = 0; iPix < 3; iPix++)
			{
				data_out(0x00); //1 lines of ball at bottom of page
			}
		}
	}
	else if(iCompNum == 7)
	{
		comm_out(iPage+0xB0);
		comm_out(0x10+((iBallXL-1) >> 4));
		comm_out(0x00+((iBallXL-1) & 0x0F));
		for(iPix = 0; iPix < 3; iPix++)
		{
			data_out(0x00); //2 lines of ball at bottom of page
		}
		if(iBallYL != 63)
		{
			comm_out(iPage+1+0xB0);
			comm_out(0x10+((iBallXL-1) >> 4));
			comm_out(0x00+((iBallXL-1) & 0x0F));
			for(iPix = 0; iPix < 3; iPix++)
			{
				data_out(0x00); //1 lines of ball at top
			}
		}
	}
	else
	{
		comm_out(iPage+0xB0);
		comm_out(0x10+((iBallXL-1) >> 4));
		comm_out(0x00+((iBallXL-1) & 0x0F));
		for(iPix = 0; iPix < 3; iPix++)
		{
			data_out(0x00); //3 lines of ball in middle of page
		}
	}

/////////////////////Clear P1 Paddle//////////////////////////////
	fTemp = iPad1YL/(float)8;
	iPage = (int)fTemp;
	iCompNum = (int)((fTemp-iPage)*8);

	if(iCompNum < 4)
	{
		comm_out(iPage+0xB0);
		comm_out(0x10);
		comm_out(0x02);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0x00);
		}
		if(iPad1YL > 3)
		{
			comm_out(iPage-1+0xB0);
			comm_out(0x10);
			comm_out(0x02);
			for(iPix = 0; iPix < 2; iPix++)
			{
				data_out(0x00);
			}
		}
	}
	else
	{
		comm_out(iPage+0xB0);
		comm_out(0x10);
		comm_out(0x02);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0x00);
		}
		comm_out(iPage+1+0xB0);
		comm_out(0x10);
		comm_out(0x02);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0x00);
		}
	}

///////////////////////Clear P2 Paddle//////////////////////////////
	fTemp = iPad2YL/(float)8;
	iPage = (int)fTemp;
	iCompNum = (int)((fTemp-iPage)*8);

	if(iCompNum < 4)
	{
		comm_out(iPage+0xB0);
		comm_out(0x17);
		comm_out(0x0C);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0x00);
		}
		if(iPad1YL > 3)
		{
			comm_out(iPage-1+0xB0);
			comm_out(0x17);
			comm_out(0x0C);
			for(iPix = 0; iPix < 2; iPix++)
			{
				data_out(0x00);
			}
		}
	}
	else
	{
		comm_out(iPage+0xB0);
		comm_out(0x17);
		comm_out(0x0C);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0x00);
		}
		comm_out(iPage+1+0xB0);
		comm_out(0x17);
		comm_out(0x0C);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0x00);
		}
	}
}

void fnRedrawScreen(void) //Redraws screen
{
	unsigned int iPage;
	float fTemp;
	unsigned int iCompNum;
	unsigned int iPix;
	unsigned int iST;
	unsigned int iPadOut;

	fnClearElements();

/////////////////////Draw Ball//////////////////////////////
	fTemp = iBallY/(float)8;
	iPage = (int)fTemp;
	iCompNum = (int)((fTemp-iPage)*8);

	if(iCompNum == 0)
	{
		comm_out(iPage+0xB0);
		comm_out(0x10+((iBallX-1) >> 4));
		comm_out(0x00+((iBallX-1) & 0x0F));
		for(iPix = 0; iPix < 3; iPix++)
		{
			data_out(0x03); //2 lines of ball at top of page
		}
		if(iBallY != 0)
		{
			comm_out(iPage-1+0xB0);
			comm_out(0x10+((iBallX-1) >> 4));
			comm_out(0x00+((iBallX-1) & 0x0F));
			for(iPix = 0; iPix < 3; iPix++)
			{
				data_out(0x80); //1 lines of ball at bottom of page
			}
		}
	}
	else if(iCompNum == 7)
	{
		comm_out(iPage+0xB0);
		comm_out(0x10+((iBallX-1) >> 4));
		comm_out(0x00+((iBallX-1) & 0x0F));
		for(iPix = 0; iPix < 3; iPix++)
		{
			data_out(0xC0); //2 lines of ball at bottom of page
		}
		if(iBallY != 63)
		{
			comm_out(iPage+1+0xB0);
			comm_out(0x10+((iBallX-1) >> 4));
			comm_out(0x00+((iBallX-1) & 0x0F));
			for(iPix = 0; iPix < 3; iPix++)
			{
				data_out(0x01); //1 lines of ball at top
			}
		}
	}
	else
	{
		comm_out(iPage+0xB0);
		comm_out(0x10+((iBallX-1) >> 4));
		comm_out(0x00+((iBallX-1) & 0x0F));
		for(iPix = 0; iPix < 3; iPix++)
		{
			data_out(0x07 << (iCompNum -1)); //3 lines of ball in middle of page
		}
	}
/////////////////////Draw P1 Paddle//////////////////////////////
	fTemp = iPad1Y/(float)8;
	iPage = (int)fTemp;
	iCompNum = (int)((fTemp-iPage)*8);

	if(iCompNum < 4)
	{
		comm_out(iPage+0xB0);
		comm_out(0x10);
		comm_out(0x02);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0xFF >> (3-iCompNum));
		}
		if(iPad1Y > 3)
		{
			comm_out(iPage-1+0xB0);
			comm_out(0x10);
			comm_out(0x02);
			iPadOut = 0x80;
			for(iST = 0; iST < (3-iCompNum); iST++)
			{
				iPadOut>>= 1;
				iPadOut+=0x80;
			}
			for(iPix = 0; iPix < 2; iPix++)
			{
				data_out(iPadOut);
			}
		}
	}
	else
	{
		comm_out(iPage+0xB0);
		comm_out(0x10);
		comm_out(0x02);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0xFF << (iCompNum-4));
		}
		comm_out(iPage+1+0xB0);
		comm_out(0x10);
		comm_out(0x02);
		iPadOut = 0x01;
		for(iST = 0; iST < (iCompNum-4); iST++)
		{
			iPadOut<<= 1;
			iPadOut+=0x01;
		}
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(iPadOut);
		}
	}


///////////////////////Draw P2 Paddle//////////////////////////////
	fTemp = iPad2Y/(float)8;
	iPage = (int)fTemp;
	iCompNum = (int)((fTemp-iPage)*8);

	if(iCompNum < 4)
	{
		comm_out(iPage+0xB0);
		comm_out(0x17);
		comm_out(0x0C);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0xFF >> (3-iCompNum));
		}
		if(iPad1Y > 3)
		{
			comm_out(iPage-1+0xB0);
			comm_out(0x17);
			comm_out(0x0C);
			iPadOut = 0x80;
			for(iST = 0; iST < (3-iCompNum); iST++)
			{
				iPadOut>>= 1;
				iPadOut+=0x80;
			}
			for(iPix = 0; iPix < 2; iPix++)
			{
				data_out(iPadOut);
			}
		}
	}
	else
	{
		comm_out(iPage+0xB0);
		comm_out(0x17);
		comm_out(0x0C);
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(0xFF << (iCompNum-4));
		}
		comm_out(iPage+1+0xB0);
		comm_out(0x17);
		comm_out(0x0C);
		iPadOut = 0x01;
		for(iST = 0; iST < (iCompNum-4); iST++)
		{
			iPadOut<<= 1;
			iPadOut+=0x01;
		}
		for(iPix = 0; iPix < 2; iPix++)
		{
			data_out(iPadOut);
		}
	}

//////////////////Setup variables for next clear////////////////
	iPad1YL = iPad1Y;
	iPad2YL = iPad2Y;
	iBallXL = iBallX;
	iBallYL = iBallY;
}

void fnScoreDisplay(void)  // Shows score for 4 seconds
{
	unsigned int iPage;
	unsigned int iPlayer;
	unsigned int iClm;
	unsigned int iCompare;

	clearA1Z();

/////////////////////////Draw P1 Label/////////////////////////
	comm_out(0xB7); comm_out(0x11); comm_out(0x0A);
	data_out(0x3F); data_out(0x3F);
	data_out(0x33); data_out(0x33); data_out(0x33);
	data_out(0x3F); data_out(0x3F);
	data_out(0x00); data_out(0x00); data_out(0x00);
	data_out(0x3F); data_out(0x3F);
	comm_out(0xB6); comm_out(0x11); comm_out(0x0A);
	data_out(0xFC); data_out(0xFC);
	data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00);
	data_out(0xFC); data_out(0xFC);

/////////////////////////Draw Divider/////////////////////////
	comm_out(0xB7); comm_out(0x13); comm_out(0x0F);
	data_out(0x3F); data_out(0x3F);
	for(iPage = 0xB6; iPage > 0xB0; iPage--)
	{
		comm_out(iPage); comm_out(0x13); comm_out(0x0F);
		data_out(0xFF); data_out(0xFF);
	}
	comm_out(0xB0); comm_out(0x13); comm_out(0x0F);
	data_out(0xFC); data_out(0xFC);

/////////////////////////Draw P2 Label/////////////////////////
	comm_out(0xB7); comm_out(0x15); comm_out(0x0A);
	data_out(0x3F); data_out(0x3F);
	data_out(0x33); data_out(0x33); data_out(0x33);
	data_out(0x3F); data_out(0x3F);
	data_out(0x00); data_out(0x00); data_out(0x00);
	data_out(0x31); data_out(0x31); data_out(0x31); data_out(0x31); data_out(0x31);
	data_out(0x3F); data_out(0x3F);
	comm_out(0xB6); comm_out(0x15); comm_out(0x0A);
	data_out(0xFC); data_out(0xFC);
	data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00); data_out(0x00);
	data_out(0xFC); data_out(0xFC);
	data_out(0x8C); data_out(0x8C); data_out(0x8C); data_out(0x8C); data_out(0x8C);

/////////////////////////Draw Scores/////////////////////////
	for(iPlayer = 1; iPlayer < 3; iPlayer ++)
	{
		iClm = 4*iPlayer + 13;
		switch(iPlayer)
		{
			case (1):
			iCompare = iP1Score;
			break;

			case (2):
			iCompare = iP2Score;
			break;
		}
		comm_out(0xB4); comm_out(iClm); comm_out(0x0A);
		switch(iCompare)
		{
			case (0):
			{
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
				data_out(0xC0); data_out(0xC0); data_out(0xC0); data_out(0xC0); data_out(0xC0);
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
				comm_out(0xB3); comm_out(iClm); comm_out(0x0A);
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
				data_out(0x03); data_out(0x03); data_out(0x03); data_out(0x03); data_out(0x03);
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
			}
			break;
			case (1):
			{
				data_out(0x30);
				data_out(0x70); data_out(0x70);
				data_out(0xF0);
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
				comm_out(0xB3); comm_out(iClm); comm_out(0x0A);
				data_out(0x03); data_out(0x03); data_out(0x03); data_out(0x03);
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
				data_out(0x03); data_out(0x03); data_out(0x03); data_out(0x03);
			}
			break;
			case (2):
			{
				data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3);
				data_out(0xFF); data_out(0xFF); data_out(0xFF);
				comm_out(0xB3); comm_out(iClm); comm_out(0x0A);
				data_out(0xFF); data_out(0xFF); data_out(0xFF);
				data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F);
			}
			break;
			case (3):
			{
				data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3);
				data_out(0xFF); data_out(0xFF); data_out(0xFF);
				comm_out(0xB3); comm_out(iClm); comm_out(0x0A);
				data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F);
				data_out(0xFF); data_out(0xFF); data_out(0xFF);
			}
			break;
			case (4):
			{
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
				data_out(0x07); data_out(0x07); data_out(0x07); data_out(0x07); data_out(0x07);
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
				comm_out(0xB3); comm_out(iClm); comm_out(0x0A);
				data_out(0x80); data_out(0x80); data_out(0x80); data_out(0x80); data_out(0x80); data_out(0x80); data_out(0x80); data_out(0x80); data_out(0x80);
				data_out(0xFF); data_out(0xFF); data_out(0xFF); data_out(0xFF);
			}
			break;
			case (5):
			{
				data_out(0xFF); data_out(0xFF); data_out(0xFF);
				data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3); data_out(0xE3);
				comm_out(0xB3); comm_out(iClm); comm_out(0x0A);
				data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F); data_out(0x8F);
				data_out(0xFF); data_out(0xFF); data_out(0xFF);
			}
			break;
		}
	}


	/////////////////////////Book Keeping/////////////////////////
	__delay_cycles(6000000); // Delay for 4 seconds at 2MHz
	clearA1Z();
}


void fnInitializeDisplay(void) //Run before using display
{
	P6DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4;   // Set SI, SCLK, A0, RESET, and CS to output
	P6REN |= BIT1 + BIT1 + BIT2 + BIT3 + BIT4;   // Set SI, SCLK, A0, RESET, and CS to pull-up
	P6OUT &= ~BIT1;                // Set RESET low
	__delay_cycles(240000);   // Delay for 120ms at 2MHz
	P6OUT |= BIT1;         // Set RESET high
	P6OUT |= BIT2 + BIT0; //Set CS high, Set A0 high

	comm_out(0xA2);  //added 1/9 bias
	comm_out(0xA0);   //ADC segment driver direction (A0=Normal)
	comm_out(0xC8);   //added
	comm_out(0xC0);   //COM output scan direction (C0=Normal)
	comm_out(0x26);   //resistor ratio
	comm_out(0x81);   //electronic volume mode set
	comm_out(0x19);   //electronic volume registerset
	comm_out(0x2F);   //power control set
	comm_out(0xAF);   //displayON/OFF - set to ON
	//comm_out(0xA7);  //displayMode - set to reverse
	clearA1Z();
}
