#include "tiremo_api.h"
#include "model.h"

#include <stddef.h>
#include <stdint.h>


extern int32_t model_predict(const int *features,
                             int32_t        features_length);


static int _run_single(const int16_t *input,
                       size_t         input_len,
                       int32_t       *label_out)
{
    int32_t label = -1;
    size_t  i;

    /* Model expects integer features; prepare contiguous input buffer. */
    static int x_int[TIREMO_N_FEATURES];

    if (input == NULL || label_out == NULL) {
        return TIREMO_ERR_NULL_PTR;
    }
    if (input_len < TIREMO_N_FEATURES) {
        return TIREMO_ERR_INPUT_LEN;
    }

    for (i = 0; i < TIREMO_N_FEATURES; i++) {
        x_int[i] = (int)input[i];
    }

    label = model_predict(x_int, TIREMO_N_FEATURES);
    if (label < 0 || label >= (int32_t)TIREMO_N_CLASSES) {
        return TIREMO_ERR_BACKEND;
    }
    *label_out = label;

    return TIREMO_OK;
}


int tiremo_ai_classify_index(const int16_t *input,
                             size_t         input_len,
                             int32_t       *class_index_out)
{
    return _run_single(input, input_len, class_index_out);
}


int tiremo_ai_classify_label(const int16_t *input,
                             size_t         input_len,
                             const char   **label_out)
{
    int32_t label = -1;
    int     rc;

    if (label_out == NULL) {
        return TIREMO_ERR_NULL_PTR;
    }

    rc = _run_single(input, input_len, &label);
    if (rc != TIREMO_OK) {
        return rc;
    }

    *label_out = TIREMO_CLASS_NAMES[label];
    return TIREMO_OK;
}
