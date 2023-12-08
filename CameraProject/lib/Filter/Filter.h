/**
 * @file Filter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Generic filter driver data structures.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 */
#pragma once

/** General imports. */
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief FilterType is an enumeration specifying the type of filter this base
 *        filter should be used for.
 */
enum FilterType {
    FILTER_MEDIAN,
    FILTER_EMA,
    FILTER_SMA,
    FILTER_KALMAN,
    NUM_FILTERS
};

/**
 * @brief FilterConfig_t is a user defined struct that specifies a base filter
 *        configuration.
 */
typedef struct FilterConfig {
    /** @brief The filter type. */
    enum FilterType type;

    /**
     * @brief Reference to an allocated array of memory to be used for the
     *        filter.
     */
    float * buffer;

    /** @brief The discrete size of the buffer field reference. */
    uint16_t bufferSize;
} FilterConfig_t;

/**
 * @brief Filter_t is a user defined struct that specifies the contents
 *        and operation of any filter.
 */
typedef struct Filter {
    /** @brief The filter type. */
    enum FilterType type;

    /**
     * @brief Reference to an allocated array of memory to be used for the
     *        filter.
     */
    float * buffer;

    /** @brief The discrete size of the buffer field reference. */
    uint16_t bufferSize;
} Filter_t;
