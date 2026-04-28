#include "tiremo_api.h"
#include "model.h"

#include <stddef.h>
#include <stdint.h>


extern int32_t model_predict(const int *features,
                             int32_t        features_length);


extern int     model_predict_proba(const int *features,
                                   int32_t        features_length,
                                   float         *out,
                                   int            out_length);


/* Cast feature from double to int expected by generated model. */
static inline int _to_model_feature(double v) {
    return (int)v;
}


static int _run_single(const double *input,
                       int32_t      *label_out,
                       double       *probs_out,
                       size_t        n_classes)
{
    int32_t label = -1;
    size_t  i;

    /* Integer feature buffer + native float proba buffer. */
    static int     x_int  [TIREMO_N_FEATURES];
    static float   probs_f[TIREMO_N_CLASSES];

    for (i = 0; i < TIREMO_N_FEATURES; i++) {
        x_int[i] = _to_model_feature(input[i]);
    }

    if (probs_out != NULL) {
        if (model_predict_proba(x_int, TIREMO_N_FEATURES,
                                probs_f, (int)TIREMO_N_CLASSES) != 0) {
            return TIREMO_ERR_BACKEND;
        }
        for (i = 0; i < n_classes && i < TIREMO_N_CLASSES; i++) {
            probs_out[i] = (double)probs_f[i];
        }
    }
    label = model_predict(x_int, TIREMO_N_FEATURES);

    if (label_out != NULL) {
        *label_out = label;
    }

    return TIREMO_OK;
}


int tiremo_ai_classify_label(const double  *input,
                             size_t         n_classes,
                             double        *probabilities_out,
                             const char   **label_out)
{
    int32_t label;
    int     rc;

    if (input == NULL || probabilities_out == NULL) return TIREMO_ERR_NULL_PTR;
    if (n_classes == 0)                              return TIREMO_ERR_INPUT_LEN;

    rc = _run_single(input, &label, probabilities_out, n_classes);
    if (rc != TIREMO_OK) return rc;

    if (label_out != NULL) {
        *label_out = (label >= 0 && label < (int32_t)TIREMO_N_CLASSES)
                     ? TIREMO_CLASS_NAMES[label]
                     : NULL;
    }
    return TIREMO_OK;
}
