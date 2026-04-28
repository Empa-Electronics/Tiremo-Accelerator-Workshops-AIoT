#ifndef TIREMO_CLASSES_H
#define TIREMO_CLASSES_H

/* ---------------------------------------------------------------
 * Number of output classes
 * --------------------------------------------------------------- */
#define TIREMO_N_CLASSES  5

/* ---------------------------------------------------------------
 * Class-index enumeration
 * --------------------------------------------------------------- */
typedef enum {
    CIRCLE               = 0,
    HORIZONTAL           = 1,
    STANDBY              = 2,
    TRIANGLE             = 3,
    VERTICAL             = 4,
} TiremoClass;

/* ---------------------------------------------------------------
 * Human-readable label for each class index.
 * static const so this header is self-contained and safe to
 * include from multiple translation units.
 * --------------------------------------------------------------- */
static const char * const TIREMO_CLASS_NAMES[TIREMO_N_CLASSES] = {
    "CIRCLE",
    "HORIZONTAL",
    "STANDBY",
    "TRIANGLE",
    "VERTICAL",
};

#endif /* TIREMO_CLASSES_H */
