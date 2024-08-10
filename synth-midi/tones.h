#include <math.h>
#include <stdint.h>

#define A4 440
#define A4_MIDI 69
#define C2_MIDI 48 // First key of M-VAVE SMK-25 Mini

const uint8_t sine_wave[16] = { 177, 219, 246, 255, 246, 219, 177, 128, 79, 37, 10, 1, 10, 37, 79, 128 };
const uint8_t square_wave[16] = { 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static float semitone_midi(int const note) {
  return A4 * pow(2.0, (note-A4_MIDI)/12.0);
}
