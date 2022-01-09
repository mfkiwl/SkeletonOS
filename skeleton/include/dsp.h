#ifndef DSP_H_
#define DSP_H_

#define ARREYSIZE(x)  (sizeof(x) / sizeof((x)[0]))
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// y = A * sin (2 * pi * f * x) + C
#define SIN_WAVE(AMPLITUDE, FREQUENCY, OFFSET, TIME_VALUE) (AMPLITUDE * sin(2 * PI * FREQUENCY * TIME_VALUE) + OFFSET)

//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))
 
#define PI 3.14159265358979323846	/* pi */

typedef struct {
   double real;
	double imag;
} COMPLEX;

extern double customCABS(COMPLEX x);
extern int fft_real(COMPLEX *x, uint32_t N);
extern int ifft_real(COMPLEX *x, uint32_t N);

/**
 * ds_fft: input - the double sided fft
 * N: input - the size of the double sided fft
 * ss_fft: output - the single sided, amplitude corrected fft
 * size: output - the size of the ss_fft
 */
extern int fftGetSingleSided(COMPLEX *ds_fft, uint32_t N, double *ss_fft, uint32_t *size);

// Statistic Functions

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the standard deviation value
 */
extern double standardDeviation(double *input, uint64_t size);

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the variance value
 */
extern double variance(double *input, uint64_t size);

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the skewness value
 */
extern double skewness(double *input, uint64_t size);

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the kurtosis value
 */
extern double kurtosis(double *input, uint64_t size);

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the maximum value
 */
extern double maxValue(double *input, uint64_t size);

/**
 * input: input - an array of double on which found the min value
 * size: input - the size of the array
 * return: the minimum value
 */
double minValue(double *input, uint64_t size);

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the rms calculated
 */
extern double rms(double *input, uint64_t size);

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the crest factor value
 */
extern double crestFactor(double *input, uint64_t size);

/**
 * input: input - an array of double
 * size: input - the size of the array
 * return: the mean calculated
 */
extern double mean(double *input, uint64_t size);

#endif /* MATHAPI_H_ */
