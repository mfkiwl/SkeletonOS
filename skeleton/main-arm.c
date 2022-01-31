#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "main.h"

#define CBUF_HFDATA_SIZE 8192
#define CBUF_LFDATA_SIZE 128

#define DATA_FILE_NAME 		"lfstatistics.data"
#define LOG_FILE_NAME 		"main.log"
#define CONFIG_FILE_NAME 	"common.json"

struct Element
{
	double value;
	uint64_t timestamp;
};

struct LFStatistics
{
	double avg;
	double max;
	double min;
};

struct Config
{
	int LFGeneratorMax;
	int LFGeneratorMin;
	int HFGeneratorFs;
	int HFPopAndComputeStatisticsLowfiltercut;
	int HFPopAndComputeStatisticsNyquistcond;
};
struct Config config;

void internalConfigInit()
{
	CONFIG_READ_INTEGER("LFGeneratorMax", &config.LFGeneratorMax);
	CONFIG_READ_INTEGER("LFGeneratorMin", &config.LFGeneratorMin);
	CONFIG_READ_INTEGER("HFGeneratorFs", &config.HFGeneratorFs);
	CONFIG_READ_INTEGER("HFPopAndComputeStatisticsLowfiltercut", &config.HFPopAndComputeStatisticsLowfiltercut);
	CONFIG_READ_INTEGER("HFPopAndComputeStatisticsNyquistcond", &config.HFPopAndComputeStatisticsNyquistcond);
}

circ_gbuf_t cBufHFData;
circ_gbuf_t cBufLFData;

CIRC_GBUF_DEF(struct Element, cBufHFData, CBUF_HFDATA_SIZE);
CIRC_GBUF_DEF(struct Element, cBufLFData, CBUF_LFDATA_SIZE);

// process test generate data
int LFGenerator(uint8_t pid)
{
	LOGGER_TRACE("LF Data Generator Task");

	struct Element element;
	element.timestamp = getMillisTime();

	const int max = config.LFGeneratorMax;
	const int min = config.LFGeneratorMin;

	float variable = ((max - min) * (float)rnd() / RAND_MAX) + min;

	element.value = variable;

	int slotFull = CBUF_LFDATA_SIZE - CIRC_GBUF_FS(cBufLFData);
	LOGGER_TRACE("LF Data Generator - slot full: %d", slotFull);

	if (CIRC_GBUF_PUSH(cBufLFData, &element) == 0)
	{
		LOGGER_TRACE("LF Data Generator cBuf data push: %" PRId64 " : %f", element.timestamp, element.value);
	}

	return 0;
}

#define SIN_WAVE_SIZE 128

// process test generate data
int HFGenerator(uint8_t pid)
{
	LOGGER_TRACE("HF Data Generator Task");

	struct Element element;
	element.timestamp = getMillisTime();

	const int fs = config.HFGeneratorFs;

	double input[SIN_WAVE_SIZE];

	for (int i = 0; i < SIN_WAVE_SIZE; i += 1)
	{
		//signal test
		input[i] = SIN_WAVE(1, 200, 5, (double)i / fs) + SIN_WAVE(10, 800, 0, (double)i / fs) + SIN_WAVE(1, 1000, 2, (double)i / fs);

		element.value = input[i];

		int slotFull = CBUF_HFDATA_SIZE - CIRC_GBUF_FS(cBufHFData);
		LOGGER_TRACE("HF Data Generator - slot full: %d", slotFull);

		if (CIRC_GBUF_PUSH(cBufHFData, &element) == 0)
		{
			LOGGER_TRACE("HF Data Generator cBuf data push: %" PRId64 " : %f", element.timestamp, element.value);
		}
	}

	return 0;
}

#define HFPOP_WINDOW 4096

int HFPopAndComputeStatistics(uint8_t pid)
{
	LOGGER_TRACE("HF Pop and Compute Statistics Tas");

	struct Element element;
	static COMPLEX HFSignalComplex[HFPOP_WINDOW];

	int slotFull = CBUF_HFDATA_SIZE - CIRC_GBUF_FS(cBufHFData);
	LOGGER_TRACE("HF Pop and Compute Statistics Task - slot full: %d", slotFull);

	if (slotFull == HFPOP_WINDOW)
	{
		for (int i = 0; i < slotFull; i++)
		{
			if (CIRC_GBUF_POP(cBufHFData, &element) == 0)
			{
				LOGGER_TRACE("HF Pop and Compute Statistics Task pop: %" PRIu64 ": %f", element.timestamp, element.value);
				HFSignalComplex[i].real = element.value;
				HFSignalComplex[i].imag = 0.0;
			}
		}
	}
	else
	{
		LOGGER_TRACE("HF Pop and Compute Statistics Task: Waiting Windows Full");
		return 1;
	}

	fft_real(HFSignalComplex, HFPOP_WINDOW);

	const int lowFilterCut = config.HFPopAndComputeStatisticsLowfiltercut;

	// eliminate DC component
	for (int i = 0; i < lowFilterCut; ++i)
	{
		HFSignalComplex[i].real = 0;
		HFSignalComplex[i].imag = 0;
	}

	const int nyquistCondition = config.HFPopAndComputeStatisticsNyquistcond;
	const unsigned long lNyquist = FLOAT_TO_INT(HFPOP_WINDOW / nyquistCondition);

	double singleSidedFFT[HFPOP_WINDOW / 2];
	uint32_t size = 0;

	fftGetSingleSided(HFSignalComplex, HFPOP_WINDOW, singleSidedFFT, &size);

	// plot to singleSidedFFT ! TODO

	ifft_real(HFSignalComplex, HFPOP_WINDOW);

	static double HFSignalFiltered[HFPOP_WINDOW];
	for (int i = 0; i < HFPOP_WINDOW; i++)
	{
		HFSignalFiltered[i] = HFSignalComplex[i].real;
	}

	double std = standardDeviation(HFSignalFiltered, HFPOP_WINDOW);
	double var = variance(HFSignalFiltered, HFPOP_WINDOW);
	double ske = skewness(HFSignalFiltered, HFPOP_WINDOW);
	double kur = kurtosis(HFSignalFiltered, HFPOP_WINDOW);
	double max = maxValue(HFSignalFiltered, HFPOP_WINDOW);
	double sqr = rms(HFSignalFiltered, HFPOP_WINDOW);
	double cfa = crestFactor(HFSignalFiltered, HFPOP_WINDOW);

	LOGGER_DEBUG("\
    HF Pop and Compute Statistics Task: \
    standard deviation:%.3f; \
    variance:%.3f; \
    skewness:%.3f; \
    kurtosis:%.3f; \
    max:%.3f; \
    rms:%.3f; \
    crest factor:%.3f;",
						std, var, ske, kur, max, sqr, cfa);

	return 0;
}

#define LFPOP_WINDOW 16

int LFPopAndComputeStatistics(uint8_t pid)
{

	LOGGER_TRACE("LF Pop and Compute Statistics Task");

	struct Element element;
	static double avg, max;
	static double min = 16384.0; // high number is better

	int slotFull = CBUF_LFDATA_SIZE - CIRC_GBUF_FS(cBufLFData);
	LOGGER_TRACE("LF Pop and Compute Statistics Task - slot full: %d", slotFull);

	if (slotFull == LFPOP_WINDOW)
	{
		for (int i = 0; i < LFPOP_WINDOW; i++)
		{
			if (CIRC_GBUF_POP(cBufLFData, &element) == 0)
			{
				LOGGER_TRACE("LF Pop and Compute Statistics Task pop: %" PRIu64 ": %f", element.timestamp, element.value);
				avg += element.value;
				max = MAX(element.value, max);
				min = MIN(element.value, min);
			}
		}
	}
	else
	{
		LOGGER_TRACE("LF Pop and Compute Statistics Task: Waiting Windows Full");
		return 1;
	}

	avg = avg / LFPOP_WINDOW;

	static struct LFStatistics lfstatistics;

	int ret = FS_READ(DATA_FILE_NAME, &lfstatistics, sizeof(struct LFStatistics));

	if (ret < 0)
		lfstatistics.avg = avg;
	lfstatistics.avg = (avg + lfstatistics.avg) / 2;

	FS_WRITE(DATA_FILE_NAME, &lfstatistics, sizeof(struct LFStatistics));

	LOGGER_DEBUG("\
    LF Pop and Compute Statistics Task: \
    avg:%.3f; \
    avgHistory:%.3f; \
    max:%.3f; \
    min:%.3f;",
							avg, lfstatistics.avg, max, min);

	return 0;
}

// define Tasks period (us)
#define LF_GENERATOR_PERIOD 1 * 1000 * 1000
#define HF_GENERATOR_PERIOD 1 * 1000 * 1000
#define HF_POP_PERIOD 100 * 1000
#define LF_POP_PERIOD 100 * 1000

void dspRun()
{
    RUN(1);
}

int main(int argc, char **argv)
{
	initDrivers();

	// init logger, specific the path of file, the level where start log and if you want write log in STDOUT or not.
	LOGGER(LOG_FILE_NAME, LOGGER_LEVEL_DEBUG, LOGGER_SERIAL_PRINT);
	//LOGGER(LOG_FILE_NAME, LOGGER_LEVEL_DEBUG, LOGGER_STDOUT_ON);

	// init config JSON file, specific the path of JSON file.
	CONFIG_INIT(CONFIG_FILE_NAME);
	internalConfigInit();

	// start application
	LOGGER_INFO("Skeleton %s starting...", SKELETON_VERSION);

	// reset circular buffer.
	CIRC_GBUF_FLUSH(cBufHFData); //init cBufHF
	CIRC_GBUF_FLUSH(cBufLFData); //init cBufLF
	
	PROCESS_ATTACH(0, LF_GENERATOR_PERIOD, LFGenerator);
	PROCESS_ATTACH(0, HF_GENERATOR_PERIOD, HFGenerator);
	PROCESS_ATTACH(0, HF_POP_PERIOD, HFPopAndComputeStatistics);
	PROCESS_ATTACH(0, LF_POP_PERIOD, LFPopAndComputeStatistics);

	PROCESS_ATTACH(1, 25 * 1000, SquareGenerator);

	// scheduler, while(1) handle the tasks
	RUN(0);

	return 0;
}
