#ifndef __UTIL_H
#define __UTIL_H

/**
 * Return -1 if VAL evaluates as `-1`.
 * Used to propagate errors across the program.
 * `errsrc` is set with VAL, so it can be displayed with `geterr`
 */
#define TRY(VAL) \
  if ((VAL) == -1) { \
    if (!errsrc) errsrc = #VAL; \
    return -1; \
  }

/**
 * Same as TRY, but for allocations.
 * If VAL evaluates as NULL, it return NULL.
 */
#define TRY_ALLOC(VAL) \
  if ((VAL) == NULL) { \
    if (!errsrc) errsrc = #VAL; \
    return NULL; \
  }

/**
 * Size of the buffer used to print time.
 */
#define MAX_DATE_SIZE 255

/**
 * The origin of the last error is stored in there.
 */
extern char *errsrc;

/**
 * @brief Print the current time using the given format
 *
 * @param[in] format Time format. See STRFTIME(3)
 * @return -1 if something failed
 */
int print_time(const char *format);

/**
 * @brief Get the TRY error origin
 *
 * It also resets `errsrc`'s content
 *
 * @return A string containing the error
 */
char *geterr(void);

#endif
