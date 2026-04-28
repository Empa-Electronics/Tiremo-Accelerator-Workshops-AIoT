#ifndef TIREMO_CONFIG_H
#define TIREMO_CONFIG_H

/* Backend identifiers (numeric so #if comparisons work). */
#define TIREMO_BACKEND_INLINE  1
#define TIREMO_BACKEND_GNB     2
#define TIREMO_BACKEND_KNN     3

/* Active configuration. */
#define TIREMO_BACKEND      TIREMO_BACKEND_INLINE
#define TIREMO_N_FEATURES   768

#endif /* TIREMO_CONFIG_H */
