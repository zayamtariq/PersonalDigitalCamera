/**
 * @file MedianFilter.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Median Filter implementation.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 */

/** General imports. */
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

/** Device Specific imports. */
#include "./lib/Filter/MedianFilter.h"


MedianFilter_t MedianFilterInit(const FilterConfig_t config) {
    /* Initialization asserts. */
    assert(config.type == FILTER_MEDIAN);
    assert(config.buffer != NULL);
    assert(config.bufferSize > 1);

    Filter_t filter = {
        .type=config.type,
        .buffer=config.buffer,
        .bufferSize=config.bufferSize,
    };

    MedianFilter_t medianFilter = {
        .filter=filter,
        .curIdx=0,
        .maxSampleWindow=filter.bufferSize % 2 ? filter.bufferSize : filter.bufferSize - 1,
        .median=0
    };

    return medianFilter;
}

void MedianFilterAddSample(MedianFilter_t * filter, const float sample) {
    /* Initialization asserts. */
    assert(filter != NULL);
    assert(filter->filter.type == FILTER_MEDIAN);
    assert(filter->filter.buffer != NULL);
    assert(filter->filter.bufferSize > 0);
    assert(filter->maxSampleWindow % 2 && filter->maxSampleWindow <= filter->filter.bufferSize);
    assert(filter->curIdx < filter->filter.bufferSize);

    /* Remove last value. */
    float removeValue = filter->filter.buffer[filter->curIdx];

    /* Add new value. */
    filter->filter.buffer[filter->curIdx] = sample;

    /* Median adjust. */
    if (sample > filter->median && removeValue <= filter->median) {
        float lowestAboveMedian = FLT_MAX;
        uint16_t numAboveMedian = 0;

        uint16_t i;
        for (i = 0; i < filter->maxSampleWindow; ++i) {
            if (filter->filter.buffer[i] > filter->median) {
                ++numAboveMedian;
                if (filter->filter.buffer[i] < lowestAboveMedian) {
                    lowestAboveMedian = filter->filter.buffer[i];
                }
            }
        }
        if (numAboveMedian > (filter->maxSampleWindow - 1)/2) {
            filter->median = lowestAboveMedian;
        }
    } else if (sample < filter->median && removeValue >= filter->median) {
        float highestBelowMedian = FLT_MIN;
        uint16_t numBelowMedian = 0;
        uint16_t i;
        for (i = 0; i < filter->maxSampleWindow; ++i) {
            if (filter->filter.buffer[i] < filter->median) {
                ++numBelowMedian;
                if (filter->filter.buffer[i] > highestBelowMedian) {
                    highestBelowMedian = filter->filter.buffer[i];
                }
            }
        }
        if (numBelowMedian > (filter->maxSampleWindow - 1)/2) {
            filter->median = highestBelowMedian;
        }
    }

    filter->curIdx = (filter->curIdx + 1) % filter->maxSampleWindow;
}

float MedianFilterGetSample(MedianFilter_t * filter) {
    return filter->median;
}

void MedianFilterClear(MedianFilter_t * filter) {
    /* Initialization asserts. */
    assert(filter != NULL);
    assert(filter->filter.type == FILTER_MEDIAN);
    assert(filter->filter.buffer != NULL);
    assert(filter->maxSampleWindow % 2 && filter->maxSampleWindow <= filter->filter.bufferSize);

    filter->curIdx = 0;
    filter->median = 0;
    uint16_t i;
    for (i = 0; i < filter->maxSampleWindow; ++i) {
        filter->filter.buffer[i] = 0;
    }
}
