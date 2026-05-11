#ifndef TIREMO_API_H
#define TIREMO_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* int16_t, int32_t */

#include "tiremo_classes.h"   /* TIREMO_N_CLASSES, TIREMO_CLASS_NAMES */
#include "tiremo_config.h"    /* TIREMO_BACKEND, TIREMO_N_FEATURES   */

typedef enum {
    TIREMO_OK              =  0, /**< Call succeeded.                            */
    TIREMO_ERR_NULL_PTR    = -1, /**< A required pointer argument was NULL.      */
    TIREMO_ERR_INPUT_LEN   = -2, /**< `input_len`  < `expected_input_len`.       */
    TIREMO_ERR_BACKEND     = -4  /**< Underlying backend (emlearn) failed.       */
} TiremoStatus;


/**
 * @brief Run integer-only classification and return class index.
 *
 * @param input            Input features as integer values.
 * @param input_len        Number of input features in input.
 * @param class_index_out  Receives predicted class index.
 */
int tiremo_ai_classify_index(const int16_t *input,
                             size_t         input_len,
                             int32_t       *class_index_out);


/**
 * @brief Run integer-only classification and return class label.
 *
 * @param input      Input features as integer values.
 * @param input_len  Number of input features in input.
 * @param label_out  Receives pointer to class label from TIREMO_CLASS_NAMES.
 */
int tiremo_ai_classify_label(const int16_t *input,
                             size_t         input_len,
                             const char   **label_out);


/****************** Internal implementation details ******************/
#ifdef __cplusplus
}
#endif

#endif /* TIREMO_API_H */
