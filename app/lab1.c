#include "includes.h"
#include <avr/io.h>	
#define F_CPU	16000000UL	// CPU frequency = 16 Mhz
#include <avr/interrupt.h>
#include <util/delay.h>

#define  TASK_STK_SIZE  OS_TASK_DEF_STK_SIZE            

OS_STK          LedTaskStk[TASK_STK_SIZE];



volatile INT8U count = 0;
volatile INT8U beat = 0;
volatile INT8U state;
volatile int notes=0;

INT8U timer1_key_data_high[] = {
	  0,														//NoSound  //0
//   C    C#   D    D#   E    F    F#   G    G#   A    A#   B
												 113, 121, 129, //0 octave // 1 ~ 3
	136, 143, 149, 155, 161, 166, 171, 176, 180, 184, 188, 192, //1 octave // 4 ~ 15
	196, 199, 202, 205, 208, 211, 213, 216, 218, 220, 222, 224, //2 octave //16 ~ 27
	226, 227, 229, 230, 232, 233, 234, 236, 237, 238, 239, 240, //3 octave //28 ~ 39
	241, 241, 242, 243, 244, 244, 245, 246, 246, 247, 247, 248, //4 octave //40 ~ 51
	248, 248, 249, 249, 250, 250, 250, 251, 251, 251, 251, 252, //5 octave //52 ~ 63
	252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 254, //6 octave //64 ~ 75
	254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, //7 octave //76 ~ 87
	255                                                         //8 octave //88 
	
};
INT8U timer1_key_data_low[]={
	  10,														//NoSound  //0
//   C    C#   D    D#   E    F    F#   G    G#   A    A#   B
												 243, 236, 115, //0 octave //
	141,  65, 149, 142,  49, 131, 137,  71, 192, 249, 246, 185, //1 octave //
	 70, 160, 202, 199, 152,  65, 196,  35,  96, 124, 122,  92, //2 octave //
	 34, 208, 101, 227,  76, 160, 226,  17,  47,  62,  61,  45, //3 octave //
	 17, 231, 178, 113,  37, 208, 112,   8, 151,  30, 158,  22, //4 octave //
	136, 243,  88, 184,  18, 103, 184,   3,  75, 143, 206,  11, //5 octave //
	 67, 121, 172, 219,   9,  51,  91, 129, 165, 199, 231,   5, //6 octave //
	 33,  60,  85, 109, 132, 153, 173, 192, 210, 227, 243,   2, //7 octave //
	 16                                                         //8 octave //

};


INT8U note_key[] = {
	56,	59,
	54,	52,
	54,	56,
	59,	54,
	0,	56,
	59,	66,
	64,	59,
	57,	56,
	54,	0,
	56,	59,
	54,	52,
	54,	56,
	59,	54,
	0,	56,
	59,	66,
	64,	71,
	0,	71,
	69,	68,
	69,	68,
	64,	69,
	68,	66,
	68,	66,
	59,	0,
	71,	69,
	68,	69,
	68,	64,
	69,	73,
};

INT8U note_size[] = {
	4,	2,	4,	1,	1,	4,	2,	4,
	2,	4,	2,	4,	2,	4,	1,	1,
	4,	2,	4,	2,	4,	1,	1,	4,
	2,	4,	2,	4,	2,	4,	2,	10,
	2,	4,	1,	1,	1,	1,	4,	4,
	1,	1,	1,	1,	2,	2,	4,	1,
	1,	1,	1,	2,	2,	12

};

INT8U note_key_[] = {
	30,	35,	38,	37,	35,	42,	40,	37,
	35,	38,	37,	34,	36,	30,	0,	30,
	35,	38,	37,	35,	42,	45,	44,	43,
	39,	43,	42,	41,	30,	38,	35,	38,
	42,	38,	42,	38,	43,	42,	41,	37,
	38,	42,	41,	30,	38,	42,	0,	38,
	42,	38,	42,	38,	45,	44,	43,	39,
	43,	42,	41,	30,	38,	35
};
INT8U note_sizes_[]={
	4,  6,  2,  4,  8,
	4,12,12,6,2,
	4,8,4,16,4,
	4,6,2,4,8,
	4,8,4,8,4,
	6,2,4,8,4,
	20,4,8,4,8,
	4,8,4,8,4,
	6,2,4,8,4,
	16,4,4,8,4,
	8,4,8,4,8,
	4,6,2,4,8,
	4,20
};
INT16U total_song_size;
INT8U default_BPM; //48 for harry potter, 120 for zelda, 


//소리 재생용 인터럽트
ISR(TIMER1_OVF_vect)
{
	if(state==0xff){
		PORTB = 0x00;
		state = ~state;
	}
	else{
		PORTB = 0x10;
		state = ~state;
	}
	TCNT1H = timer1_key_data_high[note_key[notes]];
	TCNT1L = timer1_key_data_low[note_key[notes]];
}


//박자 재생용 인터럽트
ISR(TIMER2_OVF_vect) //0.0005초마다 인터럽트 발생
{                  
	count++;
	if(count==120) //0.0625초 ->1/16초 ==> 16분음마다 이 카운터가 돌아감
	{
		count = 0;
		beat++;
		if(beat == note_size[notes])
		{
			notes++;
			beat = 0;
		}
	}
	TCNT2 = 125;
}



void  LedTask(void *data);


int main (void)
{
  OSInit();

  OS_ENTER_CRITICAL();
  TCCR0=0x07;  
  TIMSK=_BV(TOIE0);                  
  TCNT0=256-(CPU_CLOCK_HZ/OS_TICKS_PER_SEC/ 1024);   

  DDRB = 0x10; //버저 출력 PB4
	TCCR2 = 0x04;//0b00000100 64분주
	TCNT2 = 125;
	TCCR1A = 0x00;
	TCCR1B = 0x02;//0b00000010 8분주
	TCNT1H = timer1_key_data_high[note_key[beat]+1];
	TCNT1L = timer1_key_data_low[note_key[beat]+1];
	
	TIMSK |= 0x44;//0b0100 0100
  OS_EXIT_CRITICAL();
  
  OSTaskCreate(LedTask, (void *)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 0);

  OSStart();                         
  
  return 0;
}


void LedTask (void *data)
{
  data = data;    
  // LED Task

}
