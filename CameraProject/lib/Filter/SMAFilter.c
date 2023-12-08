/**
 * @file SMAFilter.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Simple Moving Average filter.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 */

/** General imports. */
#include <assert.h>
#include <stdlib.h>
#include <math.h>

/** Device Specific imports. */
#include "./lib/Filter/SMAFilter.h"


SMAFilter_t SMAFilterInit(const FilterConfig_t config) {
    /* Initialization asserts. */
    assert(config.type == FILTER_SMA);
    assert(config.buffer != NULL);
    assert(config.bufferSize > 0);

    Filter_t filter = {
        .type=FILTER_SMA,
        .buffer=config.buffer,
        .bufferSize=config.bufferSize
    };

    SMAFilter_t smaFilter = {
        .filter=filter,
        .sum=0,
        .curIdx=0,
        .numSamples=0
    };

    return smaFilter;
}

void SMAFilterAddSample(SMAFilter_t * filter, const float sample) {
    /* Initialization asserts. */
    assert(filter != NULL);
    assert(filter->filter.type == FILTER_SMA);
    assert(filter->filter.buffer != NULL);
    assert(filter->filter.bufferSize > 0);
    assert(filter->numSamples <= filter->filter.bufferSize);
    assert(filter->curIdx < filter->filter.bufferSize);

    /* Add sample. */
    if (filter->numSamples < filter->filter.bufferSize) {
        filter->numSamples++;
        filter->sum += sample;
    } else {
        filter->sum += sample - filter->filter.buffer[filter->curIdx];
    }

    filter->filter.buffer[filter->curIdx] = sample;
    filter->curIdx = (filter->curIdx + 1) % filter->filter.bufferSize;
}

float SMAFilterGetSample(const SMAFilter_t * filter) {
    /* Initialization asserts. */
    assert(filter != NULL);
    assert(filter->filter.type == FILTER_SMA);
    assert(filter->numSamples <= filter->filter.bufferSize);

    if (filter->numSamples == 0) return 0.0;

    return filter->sum / filter->numSamples;
}

void SMAFilterClear(SMAFilter_t * filter) {
    /* Initialization asserts. */
    assert(filter != NULL);
    assert(filter->filter.type == FILTER_SMA);

    filter->sum = 0;
    filter->curIdx = 0;
    filter->numSamples = 0;
}
