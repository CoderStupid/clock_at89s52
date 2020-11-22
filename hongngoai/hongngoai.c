#include <REGX51.H>

#define led P1_1
#define led7seg1 P2_7
#define led7seg2 P2_6
#define led7seg3 P2_5
#define led7seg4 P2_4
#define btn1 P3_7
#define btn2 P2_0
#define btn3 P2_1
#define buzz P3_6

unsigned int vruiGlbCountCom = 0, vruiGlbCountSecond = 0;
unsigned char led7segNumber[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char led7segDisplay[4];
char vrucGlbLed = 0, vrcGlbHour = 0, vrcGlbMinute = 0, vrcGlbSecond = 0, vrcGlbMode = 0;

void fnDelayms(unsigned vruiTime)
{
	while (vruiTime--);
}

void fnTimer0Isr() interrupt 1
{
	TH0 = 0xfc;
	TL0 = 0x18;
	TR0 = 1;
	
	vruiGlbCountCom++;
	vruiGlbCountSecond++;
	led7seg1 = led7seg2 = led7seg3 = led7seg4 = 1;
	if (vruiGlbCountCom >= 500)
	{
		led =~led;
		vruiGlbCountCom = 0;
	}
	if (vruiGlbCountSecond >= 1000)
	{
		vrcGlbSecond++;
		vruiGlbCountSecond = 0;
	}
	if (vrcGlbSecond > 59)
	{
		vrcGlbSecond = 0;
		vrcGlbMinute++;
	}
	if (vrcGlbMinute > 59)
	{
		vrcGlbMinute = 0;
		vrcGlbHour++;
	}
	if (vrcGlbHour > 23)
	{
		vrcGlbHour = 0;
	}
	if (vrucGlbLed == 0)
	{
		P0 = led7segDisplay[0];
		led7seg1 = 0;
	}
	else if (vrucGlbLed == 1)
	{
		P0 = led7segDisplay[1];
		led7seg2 = 0;
	}
	else if (vrucGlbLed == 2)
	{
		P0 = led7segDisplay[2];
		led7seg3 = 0;
	}
	else if (vrucGlbLed == 3)
	{
		P0 = led7segDisplay[3];
		led7seg4 = 0;
	}
	vrucGlbLed++;
	if (vrucGlbLed > 3)
	{
		vrucGlbLed = 0;
	}
}

void fnTimer1Isr() interrupt 3
{
	TH1 = 0xfc;
	TL1 = 0x18;
	TR1 = 1;
}

void fnSetup()
{
	TMOD = 0x11;
	ET1 = 1;
	TR1 = 1;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
}

void main()
{
	unsigned char vrucCountBtn1 = 0, vrucCountBtn2 = 0, vrucCountBtn3 = 0;
	bit vrbBtn1Flag = 0, vrbBtn2Flag = 0, vrbBtn3Flag = 0;
	fnSetup();
	while(1)
	{
		if (btn1 == 0)
		{
			fnDelayms(10000);
			if (btn1 == 0)
			{
				vrucCountBtn1++;
				if ((vrucCountBtn1 >= 30) && (vrbBtn1Flag == 0))
				{
					buzz = 0;
					fnDelayms(5000);
					buzz = 1;
					vrcGlbMode = 1;
					vrbBtn1Flag = 1;
				}
			}
		}
		else
		{
			if ((vrucCountBtn1 != 0) && (vrucCountBtn1 < 30))
			{
				if (vrcGlbMode == 1)
				{
					vrcGlbMode = 2;
				}
				else if (vrcGlbMode == 2)
				{
					vrcGlbMode = 0;
				}
			}
			vrbBtn1Flag = 0;
			vrucCountBtn1 = 0;
		}
		
		if (btn2 == 0)
		{
			fnDelayms(10000);
			if (btn2 == 0)
			{
				vrucCountBtn2++;
				if ((vrucCountBtn2 >= 30) && (vrbBtn2Flag == 0))
				{
					vrbBtn2Flag = 1;
				}
			}
		}
		else
		{
			if ((vrucCountBtn2 != 0) && (vrucCountBtn2 < 30))
			{
				if (vrcGlbMode == 1)
				{
					vrcGlbHour++;
				}
				else if (vrcGlbMode == 2)
				{
					vrcGlbMinute++;
				}
			}
			vrbBtn2Flag = 0;
			vrucCountBtn2 = 0;
		}
		
		if (btn3 == 0)
		{
			fnDelayms(10000);
			if (btn3 == 0)
			{
				vrucCountBtn3++;
				if ((vrucCountBtn3 >= 30) && (vrbBtn3Flag == 0))
				{
					vrbBtn3Flag = 1;
				}
			}
		}
		else
		{
			if ((vrucCountBtn3 != 0) && (vrucCountBtn3 < 30))
			{
				if (vrcGlbMode == 1)
				{
					vrcGlbHour--;
				}
				else if (vrcGlbMode == 2)
				{
					vrcGlbMinute--;
				}
			}
			vrbBtn3Flag = 0;
			vrucCountBtn3 = 0;
		}
		led7segDisplay[0] = led7segNumber[vrcGlbHour/10];
		led7segDisplay[1] = led7segNumber[vrcGlbHour%10];
		led7segDisplay[2] = led7segNumber[vrcGlbMinute/10];
		led7segDisplay[3] = led7segNumber[vrcGlbMinute%10];
	}
}