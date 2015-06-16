#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif
void dbg_printf(const char *fmt, ...);
#ifdef __cplusplus
}
#endif

#ifdef DEBUG
# define TRACE(x) dbg_printf x
#else
# define TRACE(x) do {} while (0)
#endif

#endif // DEBUG_H

