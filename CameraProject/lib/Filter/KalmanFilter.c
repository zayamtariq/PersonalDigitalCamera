/**
 * @file KalmanFilter.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Median Filter Definitions.
 * @version 0.1
 * @date 2022-02-06
 * @copyright Copyright (c) 2021
 */

/** General imports. */
#include <assert.h>
#include <stdlib.h>
#include <math.h>

/** Device Specific imports. */
#include "./lib/Filter/KalmanFilter.h"


KalmanFilter_t KalmanFilterInit(const FilterConfig_t config, const float initialEstimate, const float estimateUncertainty, const float measurementUncertainty, const float processNoiseVariance) {
    /* Initialization asserts. */
    assert(config.type == FILTER_KALMAN);
    assert(config.buffer != NULL);
    assert(config.bufferSize > 1);

    Filter_t filter = {
        .type=config.type,
        .buffer=config.buffer,
        .bufferSize=config.bufferSize,
    };

    KalmanFilter_t kalmanFilter = {
        .filter=filter,
        .userSettings={initialEstimate, estimateUncertainty, measurementUncertainty, processNoiseVariance},
        .mEstimate=initialEstimate,
        .mEu=estimateUncertainty,
        .mMu=measurementUncertainty,
        .mQ=processNoiseVariance
    };

    return kalmanFilter;
}

void KalmanFilterAddSample(KalmanFilter_t * filter, const float sample) {
    /* Kalman Gain. */
    double K = filter->mEu / (filter->mEu + filter->mMu);
    /* Estimate update (state update). */
    filter->mEstimate = filter->mEstimate + K * (sample - filter->mEstimate);
    /* Estimate uncertainty. */
    filter->mEu = (1-K) * filter->mEu;
    /* Predict estimate. */
    // mEstimate = mEstimate;
    /* Predict estimate uncertainty. */
    filter->mEu = filter->mEu + filter->mQ;
}

float KalmanFilterGetSample(KalmanFilter_t * filter) {
    return filter->mEstimate;
}

void KalmanFilterClear(KalmanFilter_t * filter) {
    filter->mEstimate = filter->userSettings[0];
    filter->mEu = filter->userSettings[1];
    filter->mMu = filter->userSettings[2];
    filter->mQ = filter->userSettings[3];
}
