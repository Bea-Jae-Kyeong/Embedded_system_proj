#define __AVR_ATmega128__
#include "includes.h"
#include <avr/io.h>	
#define F_CPU	16000000UL	// CPU frequency = 16 Mhz
#include <avr/interrupt.h>
#include <util/delay.h>
#include "flashmem.h"
#define NULL 0x00
#define  TASK_STK_SIZE  OS_TASK_DEF_STK_SIZE            
#define QUEUE_SIZE 10
OS_STK          LedTaskStk[TASK_STK_SIZE];


OS_EVENT* MusicSem;
OS_EVENT* FNDMbox;
OS_EVENT* PlayMbox;
OS_EVENT* PlayQueue;
OS_FLAG_GRP* ProgressFlag;//진행도 플래그
volatile INT8U count = 0;
volatile INT8U beat = 0;
volatile INT8U state;
volatile int notes=0;
volatile INT8U playButton_Press;
volatile INT8U nextButton_Press;
volatile INT8U isPlaying;
volatile INT8U TrackNumber = 1;

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
	TCNT1H = timer1_key_data_high[track1_note_key[notes]];
	TCNT1L = timer1_key_data_low[track1_note_key[notes]];
}


//박자 재생용 인터럽트
ISR(TIMER2_OVF_vect) //0.0005초마다 인터럽트 발생
{                  
	count++;
	if(count==125) //0.0625초 ->1/16초 ==> 16분음마다 이 카운터가 돌아감
	{
		count = 0;
		beat++;
		if(beat == track1_note_key[notes])
		{
			notes++;
			beat = 0;
		}
	}
	TCNT2 = 125;
}


//재생버튼을 누르는 경우
ISR(INT4_vect)
{
	playButton_Press = TRUE;
	_delay_ms(30);
}
//다음곡 버튼을 누르는 경우
ISR(INT5_vect)
{
	nextButton_Press = TRUE;
	TrackNumber++;
	if(TrackNumber>4){
		TrackNumber=1;
	}
	_delay_ms(30);
}

void LedTask(void *data);
void FNDTask (void* data);
void MusicTask (void* data);
void MainTask (void* data);
void display_FND(int count);

int main (void)
{
	INT8U err;

  	OSInit();

  	OS_ENTER_CRITICAL();
  	TCCR0=0x07;  
  	TIMSK=_BV(TOIE0);                  
  	TCNT0=256-(CPU_CLOCK_HZ/OS_TICKS_PER_SEC/ 1024);   
	DDRA = 0xff; //LED 출력 설정
  	DDRB = 0x10; //버저 출력(PB4) 설정
	DDRC = 0xff; //FND Data 출력 설정
	DDRG = 0x0f; //FND Select 출력 설정
	DDRE = 0xcf; //PE4,PE5 입력 설정(button press)
	EICRB = 0x0A; //INT4, INT5 falling edge 인터럽트 설정
	EIMSK = 0x30; //INT4, INT5 외부 인터럽트 Enable
	TCCR2 = 0x04; //0b00000100 64분주
	TCNT2 = 125;  //0.5ms 단위 overflow interrupt
	TCCR1A = 0x00;
	TCCR1B = 0x02;//0b00000010 8분주
	TCNT1H = timer1_key_data_high[track1_note_key[beat]+1];
	TCNT1L = timer1_key_data_low[track1_note_key[beat]+1];
	
	TIMSK |= 0x44;//0b0100 0100	
	SREG |= 1<<7; //전역 인터럽트 Enable 설정
  	OS_EXIT_CRITICAL();
	playButton_Press = FALSE;
	isPlaying = FALSE;

	
	MusicSem = OSSemCreate(1);
	PlayMbox = OSMboxCreate(NULL);
	FNDMbox = OSMboxCreate(NULL);
	PlayQueue = OSQCreate(NULL, QUEUE_SIZE);
	ProgressFlag = OSFlagCreate((OS_FLAGS)0x00,&err);

	OSTaskCreate(MainTask,(void*)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 0);
	OSTaskCreate(MusicTask,(void*)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 1);
  	OSTaskCreate(LedTask, (void *)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 2);
	OSTaskCreate(FNDTask,(void*)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 3);
	

  	OSStart();                         

  	return 0;
}

void display_FND(int count)
{ 
	int i;
	if(count==1){  
		for(i=0; i<4; i++) { 
			PORTC = stop[i];
			PORTG = fnd_sel[i];
			_delay_ms(2.5);
		}
	}
	else if(count==0){

		for(i=0; i<4; i++) { 
			PORTC = play[i];
			PORTG = fnd_sel[i];
 
			_delay_ms(2.5);
		}
	}
	else if(count==3){
		for(i=0; i<4; i++) {
			if(i==0){
				PORTC = digit[TrackNumber];
				PORTG = fnd_sel[i];
 
				_delay_ms(2.5);
			}
			else if(i==1){

				PORTC = digit[0];
				PORTG = fnd_sel[i];
 
				_delay_ms(2.5);
			}
			else {
				PORTC = no[i-2];
				PORTG = fnd_sel[i];
 
				_delay_ms(2.5);
			}
		  
		}
	}

}

//button control 
void MainTask (void* data)
{
	data = data;
	char ping;
	while (TRUE)
	{
		if (playButton_Press)
		{	
			playButton_Press = FALSE;

			if (isPlaying == TRUE)
				ping = 'P';	//play ping
			else
				ping = 'S';//stop ping

			OSMboxPost(FNDMbox, &ping);
		}
		if (nextButton_Press)
		{
			nextButton_Press = FALSE;
			ping = 'N';
			OSMboxPost(FNDMbox, &ping);
		}
		OSTimeDly(3);
	}

}

void MusicTask (void* data)
{
	INT8U err;
	data = data;
	INT8U progress;
	OSSemPend(MusicSem, 0, &err);
		isPlaying = FALSE;
	OSSemPost(MusicSem);


	OSFlagPost(ProgressFlag,0x01<<(8-progress),OS_FLAG_SET,&err);
}


void FNDTask (void* data)
{
	INT8U err;
	data = data;
	char command;
	while (1) {
		command = *((char*)OSMboxPend(FNDMbox, 0, &err));
		switch (command)
		{
		case 'P':
			display_FND(1);
			break;
		case 'S':
			display_FND(0);
			break;
		case 'N':
			display_FND(3);
			break;
		}
	}
  

}

// LED Task(progress)
void LedTask (void *data)
{
	INT8U err;
  	data = data;   
	INT8U progress; 
  	

	while(TRUE)
	{  
		//진행도 갖고오기
		OSFlagPend(ProgressFlag,0xff,OS_FLAG_WAIT_SET_ANY,0,&err);
		OSSemPend(MusicSem,0,&err);
			progress = (INT8U)ProgressFlag->OSFlagFlags;
		OSSemPost(MusicSem);
		OSFlagPost(ProgressFlag,0xff,OS_FLAG_CLR,&err);

		if(progress == 0x80)
		{
			PORTA = 0x00;
		}
		PORTA |= progress;
	}
	return;
}