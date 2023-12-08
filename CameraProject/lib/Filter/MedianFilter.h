/**
 * @file MedianFilter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Median Filter Definitions.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 * @note Utilizes an amazing rolling median algorithm written by
 *       Ollie Niemitalo, who licensed his code under CC0 1.0. The
 *       original source can be found here:
 *       https://dsp.stackexchange.com/a/36886
 */
#pragma once

/** Device Specific imports. */
#include "./lib/Filter/Filter.h"


/**
 * @brief MedianFilter_t is a user defined struct that specifies the contents
 *        and operation of a Median Filter.
 */
typedef struct MedianFilter {
    /**
     * @brief A Filter_t struct specifying base parameters of a filter,
     * including its type, location in memory for storing filter values, and the
     * buffer size.
     */
    Filter_t filter;

    /* @brief The current index in the filter. */
    uint16_t curIdx;

    /* @brief The maximum sample window for determining the median. Think of it
       as the buffer size. */
    uint16_t maxSampleWindow;

    /* @brief The current filter median. */
    float median;
} MedianFilter_t;

/**
 * @brief MedianFilterInit initializes a new median filter given a
 *        FilterConfig_t configuration.
 *
 * @param config The configuration of the Median Filter struct.
 * @return MedianFilter_t A struct instance used for sampling.
 */
MedianFilter_t MedianFilterInit(const FilterConfig_t config);

/**
 * @brief MedianFilterAddSample Adds a sample to the median filter and updates
 *        the output median value.
 *
 * @param filter A reference to the MedianFilter_t object.
 * @param sample The sample to insert.
 * @note The current median output can be accessed from the filter object median
 *       field. Do not modify. The recommended usage method is to call
 *       MedianFilterGetSample.
 */
void MedianFilterAddSample(MedianFilter_t * filter, const float sample);

/**
 * @brief MedianFilterGetSample returns the current rolling median from the filter.
 *
 * @param filter A reference to the MedianFilter_t object.
 * @return float The current median output from the filter.
 */
float MedianFilterGetSample(MedianFilter_t * filter);

/**
 * @brief MedianFilterClear Clears out the Median filter object.
 *
 * @param filter A reference to the MedianFilter_t object.
 */
void MedianFilterClear(MedianFilter_t * filter);
