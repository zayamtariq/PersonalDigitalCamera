/**
 * @file KalmanFilter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Median Filter Definitions.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 * @note Source: https://www.kalmanfilter.net/kalman1d.html
 */
#pragma once

/** Device Specific imports. */
#include "./lib/Filter/Filter.h"


/**
 * @brief KalmanFilter_t is a user defined struct that specifies the contents
 *        and operation of a Kalman Filter.
 */
typedef struct KalmanFilter {
    /**
     * @brief A Filter_t struct specifying base parameters of a filter,
     * including its type, location in memory for storing filter values, and the
     * buffer size.
     */
    Filter_t filter;

    /** @brief User defined process variables during initialization. */
    float userSettings[4];

    /** @brief The estimate for the next value in the process. */
    float mEstimate;

    /**
     * @brief Estimate Uncertainty. This typically improves after more samples
     *        are added.
     */
    float mEu;

    /**
     * @brief Measurement Uncertainty. This does not change after
     *        initialization.
     */
    float mMu;

    /**
     * @brief Process Noise Variance. This does not change after
     *        initialization.
     */
    float mQ;
} KalmanFilter_t;

/**
 * @brief KalmanFilterInit initializes a new Kalman filter given a
 *        FilterConfig_t configuration and some initial parameters.
 *
 * @param config The configuration of the Median Filter struct.
 * @param initialEstimate Our starting point. Where we think the true value
 *        should be.
 * @param estimateUncertainty Error variance. How far we predict our initial
 *        estimate could be from the true value. The lower we set this,
 *        potentially the faster the filter converges.
 * @param measurementUncertainty Measurement uncertainty (what we think the
 *        sensor error will be.)
 * @param processNoiseVariance Uncertainty in the model (plant noise). The more
 *        accurate we think our model is, the smaller we can set this.
 * @return KalmanFilter_t A struct instance used for sampling.
 */
KalmanFilter_t KalmanFilterInit(
    const FilterConfig_t config,
    const float initialEstimate,
    const float estimateUncertainty,
    const float measurementUncertainty,
    const float processNoiseVariance);

/**
 * @brief KalmanFilterAddSample Adds a sample to the Kalman filter and updates
 *        the estimated value.
 *
 * @param filter A reference to the KalmanFilter_t object.
 * @param sample The sample to insert.
 * @note The current estimate can be accessed from the filter object mEstimate
 *       field. Do not modify. The recommended usage method is to call
 *       KalmanFilterGetSample.
 */
void KalmanFilterAddSample(KalmanFilter_t * filter, const float sample);

/**
 * @brief KalmanFilterGetSample returns the current estimate from the filter.
 *
 * @param filter A reference to the KalmanFilter_t object.
 * @return float The current estimate from the filter.
 */
float KalmanFilterGetSample(KalmanFilter_t * filter);

/**
 * @brief KalmanFilterClear Clears out the Kalman filter object.
 *
 * @param filter A reference to the KalmanFilter_t object.
 */
void KalmanFilterClear(KalmanFilter_t * filter);
