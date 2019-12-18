
#include <avr/pgmspace.h>

const unsigned char play[] PROGMEM = { 0x73, 0x38, 0x77, 0x6e };
const unsigned char stop[] PROGMEM = { 0x6d, 0x78, 0x3f, 0x73 };
const unsigned char no[]   PROGMEM = { 0x54, 0xDC };


const unsigned char FND_NUMBERS[]  = { 0x3f, 0x06, 0x5b, 0x4F, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f };
const unsigned char FND_DIGIT[]   = { 0x08, 0x04, 0x02, 0x01 };


 unsigned char timer1_key_data_high[]  = {
	0x00,																	//NoSound  //0
//   C     C#     D    D#    E     F     F#    G     G#    A     A#    B
														  0x71, 0x79, 0x81, //0 octave // 1 ~ 3
	0x88, 0x8F, 0x95, 0x9B, 0xA1, 0xA6, 0xAB, 0xB0, 0xB4, 0xB8, 0xBC, 0xC0, //1 octave // 4 ~ 15
	0xC4, 0xC7, 0xCA, 0xCD, 0xD0, 0xD3, 0xD5, 0xD8, 0xDA, 0xDC, 0xDE, 0xE0, //2 octave //16 ~ 27
	0xE2, 0xE3, 0xE5, 0xE6, 0xE8, 0xE9, 0xEA, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, //3 octave //28 ~ 39
	0xF1, 0xF1, 0xF2, 0xF3, 0xF4, 0xF4, 0xF5, 0xF6, 0xF6, 0xF7, 0xF7, 0xF8, //4 octave //40 ~ 51
	0xF8, 0xF8, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA, 0xFB, 0xFB, 0xFB, 0xFB, 0xFC, //5 octave //52 ~ 63
	0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, //6 octave //64 ~ 75
	0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, //7 octave //76 ~ 87
	0xFF, 																	//8 octave //88
};

 unsigned char timer1_key_data_low[]  ={
    0x00, 																	//NoSound  // 0
//   C     C#     D    D#    E     F     F#    G     G#    A     A#    B
													      0xF3, 0xEC, 0x73, //0 octave // 1 ~ 3
	0x8D, 0x41, 0x95, 0x8E, 0x31, 0x83, 0x89, 0x47, 0xC0, 0xF9, 0xF6, 0xB9, //1 octave // 4 ~ 15
	0x46, 0xA0, 0xCA, 0xC7, 0x98, 0x41, 0xC4, 0x23, 0x60, 0x7C, 0x7A, 0x5C, //2 octave //16 ~ 27
	0x22, 0xD0, 0x65, 0xE3, 0x4C, 0xA0, 0xE2, 0x11, 0x2F, 0x3E, 0x3D, 0x2D, //3 octave //28 ~ 39
	0x11, 0xE7, 0xB2, 0x71, 0x25, 0xD0, 0x70, 0x08, 0x97, 0x1E, 0x9E, 0x16, //4 octave //40 ~ 51
	0x88, 0xF3, 0x58, 0xB8, 0x12, 0x67, 0xB8, 0x03, 0x4B, 0x8F, 0xCE, 0x0B, //5 octave //52 ~ 63
	0x43, 0x79, 0xAC, 0xDB, 0x09, 0x33, 0x5B, 0x81, 0xA5, 0xC7, 0xE7, 0x05, //6 octave //64 ~ 75
	0x21, 0x3C, 0x55, 0x6D, 0x84, 0x99, 0xAD, 0xC0, 0xD2, 0xE3, 0xF3, 0x02, //7 octave //76 ~ 87
	0x10, 																	//8 octave //88
};




INT8U track_note_key[]  = {
	30,	35,	38,	37,	35,	42,	40,	37,
	35,	38,	37,	34,	36,	30,	0,	30,
	35,	38,	37,	35,	42,	45,	44,	43,
	39,	43,	42,	41,	30,	38,	35,	38,
	42,	38,	42,	38,	43,	42,	41,	37,
	38,	42,	41,	30,	38,	42,	0,	38,
	42,	38,	42,	38,	45,	44,	43,	39,
	43,	42,	41,	30,	38,	35, //track 1 end
	0,
	56,	59,	54,	52,	54,	56,	59,	54,
	0,	56,	59,	66,	64,	59,	57,	56,
	54,	 0,	56,	59,	54,	52,	54,	56,
	59,	54,	0,	56,	59,	66,	64,	71,
	 0,	71,	69,	68,	69,	68,	64,	69,
	68,	66,	68,	66,	59,	 0,	71,	69,
	68,	69,	68,	64,	69,	73, //track 2 end
	0,
	52, 64, 59, 0, 57, 64, 52, 0,
	52, 57, 64, 66, 64, 59, 57, 0,
	52, 64, 59, 0, 57, 64, 52, 0,
	52, 57, 64, 66, 64, 59, 57, 0,
	49, 50, 52, 0, 64, 61, 59, 57,
	56, 57, 59, 68, 67, 66, 65, 64,
	0, 56, 64, 61, 59, 57, 59, 61,
	59, 61, 59, 57, 56, 57, 52, 64,
	61, 59, 57, 56, 57, 59, 68, 67,
	66, 65, 64, 0, 56, 64, 61, 59,
	57, 59, 61, 59, 61, 59, 57, 56,
	57, 64, 57, 59, 57, 51, 57, 59,
	57, 50, 57, 59, 62, 61, 59, 57,
	52, 64, 57, 59, 57, 51, 57, 59,
	57, 50, 57, 59, 62, 61, 59, 57,
	52, 64, 57, 59, 57, 51, 57, 59,
	57, 50, 57, 59, 62, 61, 59, 57,
	52, 64, 57, 59, 57, 51, 57, 59,
	57, 50, 57, 59, 62, 61, 59, 57,
	52,	//track 3 end
	0,
	40, 45, 40,  0, 45,  0, 45, 47, 
	49, 50, 52,  0, 52, 53, 55, 57, 
	 0, 57, 55, 53, 55, 53, 52,  0, 
	52, 50,  0, 50, 52, 53,  0, 52, 
	50, 48,  0, 48, 50, 52,  0, 50, 
	48, 47,  0, 47, 49, 51,  0, 54, 
	52,  0, 45, 40,  0, 45,  0, 45, 
	47, 49, 50, 52,  0, 52, 53, 55, 
	57,  0, 60, 59, 56,  0, 52, 53, 
	 0, 57, 56, 52,  0, 52, 53,  0, 
	57, 56, 52,  0, 48, 50,  0, 53, 
	52,  0, 48,  0, 45, 47,  0, 47, 
	49, 51,  0, 54, 52,  0, 45, 40, 
	 0, 45,  0, 45, 47, 49, 50, 52, 
	 0, 52, 53, 55, 57,  0, 57, 55, 
	53, 55, 53, 52, 0, 52, 50,  0, 
	50, 52, 53,  0, 52, 50, 48,  0, 
	48, 50, 52,  0, 50, 48, 47,  0, 
	47, 49, 51,  0, 54, 52,  0, 45, 
	40,  0, 45,  0, 45, 47, 49, 50, 
	52,  0, 52, 53, 55, 57,  0, 60, 
	59, 56,  0, 52, 53,  0, 57, 56, 
	52,  0, 52, 53,  0, 57, 56, 52, 
	 0, 48, 50,  0, 53, 52,  0, 48, 
	 0, 45, 47,  0, 47, 49, 51,  0, 
	54, 52,  0, 54,  0, 54,  0, 54, 
	 0, 52, 54,  0, 54,  0, 54,  0, 
	57,  0, 57,  0, 54,  0, 54,  0, 
	54,  0, 52, 54,  0, 54,  0, 54, 
	 0, 54,  0, //track 4 end
	0,
};
INT8U track_note_size[]  ={
	 4,  6,  2,  4,  8,  4, 12, 12,
	 6,  2,  4,  8,  4, 16,  4,  4,
	 6,  2,  4,  8,  4,  8,  4,  8,
	 4,  6,  2,  4,  8,  4, 20,  4,
	 8,  4,  8,  4,  8,  4,  8,  4,
	 6,  2,  4,  8,  4, 16,  4,  4,
	 8,  4,  8,  4,  8,  4,  8,  4,
	 6,  2,  4,  8,  4, 20, //track 1 end
	50,
	 8, 4, 8, 2, 2, 8, 4, 8, 
	 4, 8, 4, 8, 4, 8, 2, 2, 
	 8, 4, 8, 4, 8, 2, 2, 8, 
	 4, 8, 4, 8, 4, 8, 4, 20, 
	 4, 8, 2, 2, 2, 2, 8, 8, 
	 2, 2, 2, 2, 4, 4, 8, 2, 
	 2, 2, 2, 4, 4, 24,  //track 2 end
	50,
	 8, 8, 8, 8, 8, 8, 8, 8,
	 8, 8, 8, 8, 8, 8, 8, 8,
	 8, 8, 8, 8, 8, 8, 8, 8,
	 8, 8, 8, 8, 8, 8, 8, 8,
	 1, 1, 4, 4, 4, 4, 4, 4,
	 4, 4, 8, 4, 1, 1, 2, 4,
	 12, 8, 4, 4, 4, 4, 4, 4,
	 8, 8, 4, 4, 4, 4, 8, 4,
	 4, 4, 4, 4, 4, 8, 4, 1,
	 1, 2, 4, 12, 8, 4, 4, 4,
	 4, 4, 4, 8, 8, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, 4, 4, 4, 4, 4, 4, 4,
	 4, //track 3 end
	50,
	2, 4, 4, 2, 1, 1, 1, 1, 
	1, 1, 4, 6, 1, 2, 1, 4, 
	5, 2, 2, 2, 4, 1, 2, 6, 
	3, 1, 1, 1, 1, 3, 4, 2, 
	2, 1, 1, 1, 1, 3, 4, 2,
	2, 1, 1, 1, 1, 3, 4, 4, 
	4, 13,4, 4, 2, 1, 1, 1, 
	1, 1, 1, 4, 6, 1, 2, 1, 
	4, 6, 4, 4, 4, 4, 4, 4, 
	8, 4, 4, 4, 4, 4, 4, 8, 
	4, 4, 4, 4, 4, 4, 8, 4, 
	2, 2, 4, 4, 4, 1, 1, 1, 
	1, 3, 5, 4, 8, 12,4, 4, 
	2, 1, 1, 1, 1, 1, 1, 4, 
	6, 1, 2, 1, 4, 5, 2, 2, 
	2, 4, 1, 2, 6, 3, 1, 1, 
	1, 1, 3, 4, 2, 2, 1, 1, 
	1, 1, 3, 4, 2, 2, 1, 1, 
	1, 1, 3, 4, 4, 4, 13,4, 
	4, 2, 1, 1, 1, 1, 1, 1, 
	4, 6, 1, 2, 1, 4, 6, 4, 
	4, 4, 4, 4, 4, 8, 4, 4, 
	4, 4, 4, 4, 8, 4, 4, 4, 
	4, 4, 4, 8, 4, 2, 2, 4, 
	4, 4, 1, 1, 1, 1, 3, 5, 
	4, 8, 10,4, 4, 4, 4, 4, 
	8, 4, 4, 4, 4, 4, 4, 4, 
	2, 2, 2, 2, 4, 4, 4, 4, 
	4, 8, 4, 4, 4, 4, 4, 2, 
	2, 2, 2, //track 4 end
	50,
};

INT16U total_song_time[] = {
	0, 6, 4, 13, 13
};

INT16U total_song_notes[] = {
	0,62,117,271,507
};