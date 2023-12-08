/**
 * @file SMAFilter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Simple Moving Average filter.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 */
#pragma once

/** Device Specific imports. */
#include "./lib/Filter/Filter.h"


/**
 * @brief SMAFilter_t is a user defined struct that specifies the contents
 *        and operation of a SMA (Simple Moving Average) Filter.
 */
typedef struct SMAFilter {
    /**
     * @brief A Filter_t struct specifying base parameters of a filter,
     *        including its type, location in memory for storing filter values,
     *        and the buffer size.
     */
    Filter_t filter;

    /* @brief the current filter sum. */
    float sum;

    /* @brief The current index in the filter. */
    uint16_t curIdx;

    /* @brief The number of samples currently in the filter. */
    uint16_t numSamples;
} SMAFilter_t;

/**
 * @brief SMAFilterInit initializes a new SMA filter given a
 *        FilterConfig_t configuration.
 *
 * @param config The configuration of the SMA Filter struct.
 * @return SMAFilter_t A struct instance used for sampling.
 */
SMAFilter_t SMAFilterInit(const FilterConfig_t config);

/**
 * @brief SMAFilterAddSample Adds a sample to the SMA filter.
 *
 * @param filter A reference to the SMAFilter_t object.
 * @param sample The sample to insert.
 */
void SMAFilterAddSample(SMAFilter_t * filter, const float sample);

/**
 * @brief SMAFilterGetSample returns the current rolling mean from the filter.
 *
 * @param filter A reference to the SMAFilter_t object.
 * @return float The current mean output from the filter.
 */
float SMAFilterGetSample(const SMAFilter_t * filter);

/**
 * @brief SMAFilterClear Clears out the SMA filter object.
 *
 * @param filter A reference to the SMAFilter_t object.
 */
void SMAFilterClear(SMAFilter_t * filter);
