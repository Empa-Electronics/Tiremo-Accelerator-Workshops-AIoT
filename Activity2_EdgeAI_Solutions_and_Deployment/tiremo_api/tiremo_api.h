#ifndef TIREMO_API_H
#define TIREMO_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   /* size_t */

#include "tiremo_classes.h"   /* TIREMO_N_CLASSES, TIREMO_CLASS_NAMES */
#include "tiremo_config.h"    /* TIREMO_BACKEND, TIREMO_N_FEATURES   */

typedef enum {
    TIREMO_OK              =  0, /**< Call succeeded.                            */
    TIREMO_ERR_NULL_PTR    = -1, /**< A required pointer argument was NULL.      */
    TIREMO_ERR_INPUT_LEN   = -2, /**< `input_len`  < `expected_input_len`.       */
    TIREMO_ERR_BACKEND     = -4  /**< Underlying backend (emlearn) failed.       */
} TiremoStatus;


/**
 * @brief Run classification and write the winning class label string.
 *
 * Identical to tiremo_ai_classify() but writes the human-readable
 * label from tiremo_classes.h instead of the integer index.
 *
 * @param label_out  Receives a pointer to the winning class label
 *                   string (from TIREMO_CLASS_NAMES).  Pass NULL if
 *                   you only need probabilities.
 *
 * Example:
 * @code
 *   double probs[TIREMO_N_CLASSES] = { 0 };
 *   const char *label = NULL;
 *
 *   if (tiremo_ai_classify_label(input, TIREMO_N_CLASSES,
 *                                probs, &label) == TIREMO_OK)
 *       printf("Gesture: %s\n", label);
 * @endcode
 */
int tiremo_ai_classify_label(const double  *input,
                             size_t         n_classes,
                             double        *probabilities_out,
                             const char   **label_out);


/****************** Internal implementation details ******************/
#ifdef __cplusplus
}
#endif

#endif /* TIREMO_API_H */
