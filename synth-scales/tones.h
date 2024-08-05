#include <math.h>

#define A4 440
#define C4_OFFSET -9
#define OCT 8

static float semitone(int const smt) {
	return A4 * pow(2.0, (smt+C4_OFFSET)/12.0);
}
