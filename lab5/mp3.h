// NOTES FREQUENCIES

// note freqs busclock = 80MHz and n = 32
#define C1	1194	 // 2093 Hz
#define B1	1265	 // 1975.5 Hz
#define BF1	1341	 // 1864.7 Hz
#define A1	1420	 // 1760 Hz
#define AF1	1505	 // 1661.2 Hz
#define G1	1594	 // 1568 Hz
#define GF1	1689	 // 1480 Hz
#define F1	1790	 // 1396.9 Hz
#define E1	1896	 // 1318.5 Hz
#define EF1	2009	 // 1244.5 Hz
#define D1	2128	 // 1174.7 Hz
#define DF1	2255	 // 1108.7 Hz
#define C 	2389	 // 1046.5 Hz
#define B 	2531	 // 987.8 Hz
#define BF	2681	 // 932.3 Hz
#define A 	2841	 // 880 Hz
#define AF	3010	 // 830.6 Hz
#define G 	3189	 // 784 Hz
#define GF	3378	 // 740 Hz
#define F 	3579	 // 698.5 Hz
#define E 	3792	 // 659.3 Hz
#define EF	4018	 // 622.3 Hz
#define D 	4257	 // 587.3 Hz
#define DF	4510	 // 554.4 Hz
#define C0	4778	 // 523.3 Hz
#define B0	5062	 // 493.9 Hz
#define BF0	5363	 // 466.2 Hz
#define A0	5682	 // 440 Hz
#define AF0	6020	 // 415.3 Hz
#define G0	6378	 // 392 Hz
#define GF0	6757	 // 370 Hz
#define F0	7159	 // 349.2 Hz
#define E0	7584	 // 329.6 Hz
#define EF0	8035	 // 311.1 Hz
#define D0	8513	 // 293.7 Hz
#define DF0	9019	 // 277.2 Hz
#define C7	9556	 // 261.6 Hz
#define B7	10200	 // 246.9 Hz (Sounds a little sharp
#define BF7	10726	 // 233.1 Hz	(BE CAREFUL WITH THESE 2
#define A7	11364	 // 220 Hz
#define AF7	12039	 // 207.7 Hz
#define G7	12755	 // 196 Hz
#define GF7	13514	 // 185 Hz
#define F7	14317	 // 174.6 Hz
#define E7	15169	 // 164.8 Hz
#define EF7	16071	 // 155.6 Hz
#define D7	17026	 // 146.8 Hz
#define DF7	18039	 // 138.6 Hz
#define C6	19111	 // 130.8 Hz

// periods calculated for tempo at 144 bpm
// beat period = (60s/tempo)*bus speed 
#define sixteenth_notes 6750000
#define eigth_notes 13500000
#define quarter_notes 27000000
#define half_notes 54000000

// slower 120bpm
//#define sixteenth_note 10000000
//#define eigth_note 20000000
//#define quarter_note 40000000
//#define half_note 80000000


/** mp3 **
 * Stores the meta data for a song to play
 * length: number of notes / beats in a song
 * tempo: recommended tempo of the song
 * bass, harmony, melody: pointers to the note arrays
 */
#ifndef MP3_H
#define MP3_H
struct instrument {
	const unsigned short *melody;
	const unsigned short *harmony;
	const unsigned short *bass;
};
typedef const struct instrument instrument;

struct mp3 {
	int length; 
	int tempo;
	char name[20];
	struct instrument sound;
	const unsigned long *melody;
	const unsigned long *harmony;
	const unsigned long *bass;
	//const unsigned long *harmony;
};
typedef const struct mp3 mp3;

#endif // MP3_H

/** Song Data **/

//LEGEND OF ZELDA File Select
extern const mp3 ZELDA;

//Pokemon Center
extern const mp3 POKEMON;

// cerulean city theme
extern const mp3 CERULEAN;




