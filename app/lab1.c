#define __AVR_ATmega128__
#define F_CPU	16000000UL	// CPU frequency = 16 Mhz
#define NULL 0x00
#define  TASK_STK_SIZE  OS_TASK_DEF_STK_SIZE
#define QUEUE_SIZE 10
#define N_TASK 5
#include "includes.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "flashmem.h"
OS_STK          TaskStk[N_TASK][TASK_STK_SIZE];
OS_EVENT* MusicSem;
OS_EVENT* FNDMbox;
OS_EVENT* PlayMbox;
OS_EVENT* PlayQueue;
OS_FLAG_GRP* ProgressFlag;//진행도 플래그

volatile INT8U count = 0;
volatile INT8U digit = 0;
volatile INT8U beat = 0;
volatile INT8U state;
volatile INT8U note=0;
volatile INT16U notes=1;
volatile INT8U playButton_Press;
volatile INT8U nextButton_Press;
volatile INT8U isPlaying;
volatile INT8U TrackNumber = 1;
volatile INT8U fnd_out[4] = { 0x54, 0xDC, 0x3f, 0x06 };
volatile INT8U prog=0;
volatile INT8U First;
volatile INT8U Next;
INT16U kick;





//소리 재생용 인터럽트
ISR(TIMER1_OVF_vect)
{
	kick = track_note_key[notes];
	if(state == 0xff||!isPlaying||kick==0){
		PORTB = 0x00;
	}
	else{
		PORTB = 0x10;
	}
	state = ~state;

	TCNT1H = timer1_key_data_high[kick];
	TCNT1L = timer1_key_data_low[kick];
}


//박자 재생용&FND 표시용 인터럽트
ISR(TIMER2_OVF_vect) //0.0005초마다 인터럽트 발생
{
	PORTC = fnd_out[digit];
	PORTG = FND_DIGIT[digit++];
	if(digit == 4)
	{
		digit = 0;
	}
	beat++;
	if(beat == 55) //27.5ms
	{
		beat = 0;
		if(isPlaying)
		note++;
		if(track_note_size[notes] == note)
		{
			note = 0;
			notes++;
		}


	}
	TCNT2 = 125;
}


//재생버튼(SW1)을 누를 때의 인터럽트
ISR(INT4_vect)
{
	playButton_Press = TRUE;
	_delay_ms(15);
}
//다음곡 버튼(SW2)을 누를 때의 인터럽트
ISR(INT5_vect)
{
	nextButton_Press = TRUE;
	isPlaying = TRUE;
	prog=1;
	Next = TRUE;
	_delay_ms(15);
	if (TrackNumber >= 4) {
		First = TRUE;
		TrackNumber = 1;
		notes = total_song_notes[0];

		_delay_ms(300);
	}
	else {
		notes = total_song_notes[TrackNumber];
	}
}

void LedTask(void *data);
void FNDTask (void* data);
void MusicTask (void* data);
void MainTask (void* data);
void display_FND(int count);
void playControlTask(void* data);
int main (void)
{
	INT8U err;

  	OSInit();

  	OS_ENTER_CRITICAL();
  	TCCR0 = 0x07;  //0b00000111 => 1024분주

  	TCNT0 = 256 - (CPU_CLOCK_HZ/OS_TICKS_PER_SEC/ 1024);  //timer0 is used on uCOS-II
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
	TCNT1H = 241;
	TCNT1L = 17;

	TIMSK = 0x45; //0b0100 0101
	kick = 15;
	sei(); //전역 인터럽트 Enable 설정
  	OS_EXIT_CRITICAL();
	playButton_Press = FALSE;
	nextButton_Press = FALSE;
	isPlaying = FALSE;	// 재생버튼 누르기 전 초기 상태
	First = TRUE;		// 초기 상태

	MusicSem = OSSemCreate(1);		//Semaphore 생성
	PlayMbox = OSMboxCreate(NULL);	//Mailbox 생성
	FNDMbox = OSMboxCreate(NULL);
	PlayQueue = OSQCreate(NULL, QUEUE_SIZE);		//Message Queue 생성
	ProgressFlag = OSFlagCreate((OS_FLAGS)0x00, &err);	//Event flag 생성
	//4개의 Task
	OSTaskCreate(MainTask, (void*)0, (void *)&TaskStk[0][TASK_STK_SIZE - 1], 0);
	OSTaskCreate(MusicTask, (void*)0, (void *)&TaskStk[1][TASK_STK_SIZE - 1], 1);
  	OSTaskCreate(LedTask, (void *)0, (void *)&TaskStk[2][TASK_STK_SIZE - 1], 2);
	OSTaskCreate(FNDTask, (void*)0, (void *)&TaskStk[3][TASK_STK_SIZE - 1], 3);
	OSTaskCreate(playControlTask, (void*)0, (void *)&TaskStk[4][TASK_STK_SIZE - 1], 4);

  	OSStart();

  	return 0;
}

/*
	FND 표시 부분을 Timer2에 넣어주는게 좋을 것 같음. => Volatile INT8U 변수 배열로 넣어주고 그에 엑세스 하도록
	Timer2 인터럽트 발생 빈도인 0.5ms에 따라 카운터 5개짜리 하나 만들어서 FND값을 조정
*/
void display_FND(int command)
{
	int i;

	switch (command) {
	case 1:
		for (i = 0; i < 4; i++)
			fnd_out[i] = stop[i];

		break;

	case 0:
		for (i = 0; i < 4; i++)
			fnd_out[i] = play[i];

		break;

	case 3:
		for (i = 0; i < 2; i++)
			fnd_out[i] = no[i];

		fnd_out[i++] = FND_NUMBERS[0];
		fnd_out[i] = FND_NUMBERS[TrackNumber];
		
		break;
	}


}

//button control
void MainTask (void* data)
{
	INT8U err;
	data = data;
	char ping;
	while (TRUE)
	{
		if (playButton_Press)
		{
			playButton_Press = FALSE;

			if (isPlaying == TRUE){
				OSSemPend(MusicSem,0,&err);
				isPlaying = FALSE;
				OSSemPost(MusicSem);
				ping = 'S';	//stop ping
			}

			else
			{
				OSSemPend(MusicSem,0,&err);
				isPlaying = TRUE;
				OSSemPost(MusicSem);
				ping = 'P';//play ping
			}


			OSMboxPost(FNDMbox, &ping);
			//OSTimeDlyHMSM(0,0,1,500);
		}
		if (nextButton_Press)
		{
			nextButton_Press = FALSE;
			ping = 'N';
			OSQPost(PlayQueue, &ping);
		}
		OSTimeDlyHMSM(0, 0, 0, 500);
	}

}
//음악 재생 태스크
void MusicTask (void* data)
{
	INT8U err;
	data = data;
	INT8U progress;

	/*
	OSSemPend(MusicSem, 0, &err);
		isPlaying = FALSE;
	OSSemPost(MusicSem);
	*/
	while (TRUE)
	{
		if (isPlaying == TRUE) {
			OSFlagPost(ProgressFlag, (OS_FLAGS)(0x01 << (7 - progress)), OS_FLAG_SET, &err);
			progress = (progress + 1) % 8;
			
			if (prog == 1) {
				progress = 0;
			}
			if (Next == TRUE) {
				Next = FALSE;
				OSTimeDlyHMSM(0, 0, 0, 500);
			}
			else {
				OSTimeDlyHMSM(0, 0, total_song_time[TrackNumber], 0);
			}
		}
		else
		{
			OSTimeDlyHMSM(0, 0, 1, 0);
		}


	}
}
void playControlTask(void* data)
{
	INT8U err;
	data = data;
	char command;
	while(TRUE){
		command = *(char*)OSQPend(PlayQueue,0,&err);

		
		if(command == 'N')
		{
			OSSemPend(MusicSem,0,&err);
			isPlaying = TRUE;
			TrackNumber++;
			display_FND(3);
			prog = 1;
			OSSemPost(MusicSem);
		}
		if(command =='P')
		{
			OSSemPend(MusicSem,0,&err);
			isPlaying = TRUE;
			OSSemPost(MusicSem);
		}
		else
		{
			OSSemPend(MusicSem,0,&err);
			isPlaying = FALSE;
			OSSemPost(MusicSem);
		}


		OSTimeDlyHMSM(0,0,0,300);
	}



}

//FND 출력 태스크
void FNDTask (void* data)
{
	INT8U err;
	data = data;
	char command;
	while (TRUE) {
		command = *((char*)OSMboxPend(FNDMbox, 0, &err));	// Mailbox에서 받아옴

		if(command =='P'){
			display_FND(0);
			OSQPost(PlayQueue,&command);
			OSTimeDlyHMSM(0, 0, 0, 500);// 0.5초 뒤 트랙 번호 띄워줌
		}
		else
		{
			OSQPost(PlayQueue,&command);
			display_FND(1);
			OSTimeDlyHMSM(0, 0, 1, 500);// 1.5초 뒤 트랙 번호 띄워줌
		}
		display_FND(3);

	}
}


// LED로 진행도 표시해주는 태스크
void LedTask (void *data)
{
	INT8U err;
	INT8U progress;
  	data = data;


	while (TRUE)
	{
		//진행도 갖고오기
		OSFlagPend(ProgressFlag, (OS_FLAGS)0xff, OS_FLAG_WAIT_SET_ANY, 0, &err);
		OSSemPend(MusicSem, 0, &err);
			progress = (INT8U)ProgressFlag->OSFlagFlags;
		OSSemPost(MusicSem);
		OSFlagPost(ProgressFlag, (OS_FLAGS)0xff, OS_FLAG_CLR, &err);

		if (progress == 0x80)
		{
			
			if (TrackNumber == 4) {
				TrackNumber = 0;
			}
			if (First == FALSE) {// 첫 시작에 대한 예외 처리
				fnd_out[3] = FND_NUMBERS[++TrackNumber];
			}
			else {
				First = FALSE;
				fnd_out[3] = FND_NUMBERS[TrackNumber];
			}
			
			PORTA = 0x00;
		}
		if (prog == 1) {
			prog = 0;
			PORTA = 0x00;
		}
		else {
			PORTA |= progress;
		}
		OSTimeDlyHMSM(0, 0, 0, 30);
	}
	return;
}
