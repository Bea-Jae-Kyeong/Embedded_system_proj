#include <avr/io.h>
#include <avr/pgmspace.h>



const unsigned char timer1_key_data_high[] PROGMEM = {
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
const unsigned char timer1_key_data_low[] PROGMEM ={
	  10,														//NoSound  //0
//   C    C#   D    D#   E    F    F#   G    G#   A    A#   B
												 243, 236, 115, //0 octave // 1 ~ 3
	141,  65, 149, 142,  49, 131, 137,  71, 192, 249, 246, 185, //1 octave // 4 ~ 15
	 70, 160, 202, 199, 152,  65, 196,  35,  96, 124, 122,  92, //2 octave //16 ~ 27
	 34, 208, 101, 227,  76, 160, 226,  17,  47,  62,  61,  45, //3 octave //28 ~ 39
	 17, 231, 178, 113,  37, 208, 112,   8, 151,  30, 158,  22, //4 octave //40 ~ 51
	136, 243,  88, 184,  18, 103, 184,   3,  75, 143, 206,  11, //5 octave //52 ~ 63
	 67, 121, 172, 219,   9,  51,  91, 129, 165, 199, 231,   5, //6 octave //64 ~ 75
	 33,  60,  85, 109, 132, 153, 173, 192, 210, 227, 243,   2, //7 octave //76 ~ 87
	 16                                                         //8 octave //88 

};

const unsigned char track2_note_key[] PROGMEM = {
	56,	59,	54,	52,	54,	56,	59,	54,
	0,	56,	59,	66,	64,	59,	57,	56,
	54,	 0,	56,	59,	54,	52,	54,	56,
	59,	54,	0,	56,	59,	66,	64,	71,
	 0,	71,	69,	68,	69,	68,	64,	69,
	68,	66,	68,	66,	59,	 0,	71,	69,
	68,	69,	68,	64,	69,	73,
};

const unsigned char track2_note_size[] PROGMEM = {
	4,	2,	4,	1,	1,	4,	2,	4,
	2,	4,	2,	4,	2,	4,	1,	1,
	4,	2,	4,	2,	4,	1,	1,	4,
	2,	4,	2,	4,	2,	4,	2, 10,
	2,	4,	1,	1,	1,	1,	4,	4,
	1,	1,	1,	1,	2,	2,	4,	1,
	1,	1,	1,	2,	2,	12
};

const unsigned char track1_note_key[] PROGMEM = {
	30,	35,	38,	37,	35,	42,	40,	37,
	35,	38,	37,	34,	36,	30,	0,	30,
	35,	38,	37,	35,	42,	45,	44,	43,
	39,	43,	42,	41,	30,	38,	35,	38,
	42,	38,	42,	38,	43,	42,	41,	37,
	38,	42,	41,	30,	38,	42,	0,	38,
	42,	38,	42,	38,	45,	44,	43,	39,
	43,	42,	41,	30,	38,	35
};
const unsigned char track1_note_sizes[] PROGMEM ={
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

const unsigned short total_song_size PROGMEM;