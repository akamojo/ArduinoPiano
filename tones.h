#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
struct musicNote {
  unsigned int color;
  unsigned int freq;
};

enum noteNames {
  C0, Cs0, D0, Ds0, E0, F0, Fs0, G0, Gs0, A_0, As0, H0,
  C1, Cs1, D1, Ds1, E1, F1, Fs1, G1, Gs1, A_1, As1, H1,
  C2, Cs2, D2, Ds2, E2, F2, Fs2, G2, Gs2, A_2, As2, H2,
  C3, Cs3, D3, Ds3, E3, F3, Fs3, G3, Gs3, A_3, As3, H3,
  C4, Cs4, D4, Ds4, E4, F4, Fs4, G4, Gs4, A_4, As4, H4,
  C5, Cs5, D5, Ds5, E5, F5, Fs5, G5, Gs5, A_5, As5, H5,
  C6, Cs6, D6, Ds6, E6, F6, Fs6, G6, Gs6, A_6, As6, H6,
  C7, Cs7, D7, Ds7, E7, F7, Fs7, G7, Gs7, A_7, As7, H7,
  C8, Cs8, D8, Ds8
};

unsigned int octaveLen = 12;
unsigned int blackIdx[] = {
  1, 3, 6, 8, 10
};

char noteChars[][4] = {
  "C\0", "C#\0", "D\0", "D#\0", "E\0", "F\0", "F#\0", "G\0", "G#\0", "A\0", "A#\0", "H\0"
};

unsigned int freqs[] = {
  16.35,
  17.32,
  18.35,
  19.45,
  20.60,
  21.83,
  23.12,
  24.50,
  25.96,
  27.50,
  29.14,
  30.87,
  32.70,
  34.65,
  36.71,
  38.89,
  41.20,
  43.65,
  46.25,49.00,
  51.91,55.00,
  58.27,61.74,
  65.41,69.30,
  73.42,77.78,
  82.41,87.31,
  92.50,
  98.00,
  103.83,
  110.00,
  116.54,
  123.47,
  130.81,
  138.59,
  146.83,
  155.56,
  164.81,
  174.61,
  185.00,
  196.00,
  207.65,
  220.00,
  233.08,
  246.94,
  261.63,
  277.18,
  293.66,
  311.13,
  329.63,
  349.23,
  369.99,
  392.00,
  415.30,
  440.00,
  466.16,
  493.88,
  523.25,
  554.37,
  587.33,
  622.25,
  659.26,
  698.46,
  739.99,
  783.99,
  830.61,
  880.00,
  932.33,
  987.77,
  1046.50,
  1108.73,
  1174.66,
  1244.51,
  1318.51,
  1396.91,
  1479.98,
  1567.98,
  1661.22,
  1760.00,
  1864.66,
  1975.53,
  2093.00,
  2217.46,
  2349.32,
  2489.02,
  2637.02,
  2793.83,
  2959.96,
  3135.96,
  3322.44,
  3520.01,
  3729.31,
  3951.07,
  4186.01,
  4434.92,
  4698.64,
  4978.03
};

