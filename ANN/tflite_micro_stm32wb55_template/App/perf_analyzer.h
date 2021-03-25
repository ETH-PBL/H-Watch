/*
 * perf_analyzer.h
 *
 */

#ifndef PERF_ANALYZER_H_
#define PERF_ANALYZER_H_

#include "stdint.h"

struct dwtTime {
    uint32_t fcpu;
    int s;
    int ms;
    int us;
};

void logDeviceConf(void);
uint32_t getFlashCacheConf(void);

void perfAnalyzerInit(void);
void perfAnalyzerStart(void);
float perfAnalyzerGetLatency(void);
float dwtCyclesToFloatMs(uint64_t clks);


#endif /* PERF_ANALYZER_H_ */
