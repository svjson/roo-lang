
#ifndef LISPLE__MACRO_SUPPORT_H
#define LISPLE__MACRO_SUPPORT_H

/*
 * Helper macros for selecting a specific macro implementation depending on the
 * size of varargs.
 */
#define __SELECT_MACRO__2(_1, _2, MACRO_NAME, ...) MACRO_NAME
#define __SELECT_MACRO__3(_1, _2, _3, MACRO_NAME, ...) MACRO_NAME
#define __SELECT_MACRO__4(_1, _2, _3, _4, MACRO_NAME, ...) MACRO_NAME
#define __SELECT_MACRO__5(_1, _2, _3, _4, _5, MACRO_NAME, ...) MACRO_NAME

/*
 * Helper macro for expanding values eliminating containing parentheses.
 */
#define ESC(...) __VA_ARGS__

/*
 * Helper macros for defining a dynamic number of adapter properties.
 */
#define __VA_REP_1(M_NAME, PROP)                \
  M_NAME(PROP)

#define __VA_REP_2(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_1(M_NAME,__VA_ARGS__)

#define __VA_REP_3(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_2(M_NAME,__VA_ARGS__)

#define __VA_REP_4(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_3(M_NAME,__VA_ARGS__)

#define __VA_REP_5(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_4(M_NAME,__VA_ARGS__)

#define __VA_REP_6(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_5(M_NAME,__VA_ARGS__)

#define __VA_REP_7(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_6(M_NAME,__VA_ARGS__)

#define __VA_REP_8(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_7(M_NAME,__VA_ARGS__)

#define __VA_REP_9(M_NAME, PROP, ...)           \
  M_NAME(PROP)                                  \
  __VA_REP_8(M_NAME,__VA_ARGS__)

#define __VA_REP_10(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_9(M_NAME,__VA_ARGS__)

#define __VA_REP_11(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_10(M_NAME,__VA_ARGS__)

#define __VA_REP_12(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_11(M_NAME,__VA_ARGS__)

#define __VA_REP_13(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_12(M_NAME,__VA_ARGS__)

#define __VA_REP_14(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_13(M_NAME,__VA_ARGS__)

#define __VA_REP_15(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_14(M_NAME,__VA_ARGS__)

#define __VA_REP_16(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_15(M_NAME,__VA_ARGS__)

#define __VA_REP_17(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_16(M_NAME,__VA_ARGS__)

#define __VA_REP_18(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_17(M_NAME,__VA_ARGS__)

#define __VA_REP_19(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_18(M_NAME,__VA_ARGS__)

#define __VA_REP_20(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_19(M_NAME,__VA_ARGS__)

#define __VA_REP_21(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_20(M_NAME,__VA_ARGS__)

#define __VA_REP_22(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_21(M_NAME,__VA_ARGS__)

#define __VA_REP_23(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_22(M_NAME,__VA_ARGS__)

#define __VA_REP_24(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_23(M_NAME,__VA_ARGS__)

#define __VA_REP_25(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_24(M_NAME,__VA_ARGS__)

#define __VA_REP_26(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_25(M_NAME,__VA_ARGS__)

#define __VA_REP_27(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_26(M_NAME,__VA_ARGS__)

#define __VA_REP_28(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_27(M_NAME,__VA_ARGS__)

#define __VA_REP_29(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_28(M_NAME,__VA_ARGS__)

#define __VA_REP_30(M_NAME, PROP, ...)          \
  M_NAME(PROP)                                  \
  __VA_REP_29(M_NAME,__VA_ARGS__)

#define __VA_SELECT_MACRO(_1,                                                               \
                          _2,                                                               \
                          _3,                                                               \
                          _4,                                                               \
                          _5,                                                               \
                          _6,                                                               \
                          _7,                                                               \
                          _8,                                                               \
                          _9,                                                               \
                          _10,                                                              \
                          _11,                                                              \
                          _12,                                                              \
                          _13,                                                              \
                          _14,                                                              \
                          _15,                                                              \
                          _16,                                                              \
                          _17,                                                              \
                          _18,                                                              \
                          _19,                                                              \
                          _20,                                                              \
                          _21,                                                              \
                          _22,                                                              \
                          _23,                                                              \
                          _24,                                                              \
                          _25,                                                              \
                          _26,                                                              \
                          _27,                                                              \
                          _28,                                                              \
                          _29,                                                              \
                          _30,                                                              \
                          MACRO_NAME,                                                       \
                          ...) MACRO_NAME

#define __VA_MACRO(M_NAME, ...) __VA_SELECT_MACRO(__VA_ARGS__,  \
                                                  __VA_REP_30,  \
                                                  __VA_REP_29,  \
                                                  __VA_REP_28,  \
                                                  __VA_REP_27,  \
                                                  __VA_REP_26,  \
                                                  __VA_REP_25,  \
                                                  __VA_REP_24,  \
                                                  __VA_REP_23,  \
                                                  __VA_REP_22,  \
                                                  __VA_REP_21,  \
                                                  __VA_REP_20,  \
                                                  __VA_REP_19,  \
                                                  __VA_REP_18,  \
                                                  __VA_REP_17,  \
                                                  __VA_REP_16,  \
                                                  __VA_REP_15,  \
                                                  __VA_REP_14,  \
                                                  __VA_REP_13,  \
                                                  __VA_REP_12,  \
                                                  __VA_REP_11,  \
                                                  __VA_REP_10,  \
                                                  __VA_REP_9,   \
                                                  __VA_REP_8,   \
                                                  __VA_REP_7,   \
                                                  __VA_REP_6,   \
                                                  __VA_REP_5,   \
                                                  __VA_REP_4,   \
                                                  __VA_REP_3,   \
                                                  __VA_REP_2,   \
                                                  __VA_REP_1)   \
  (M_NAME, __VA_ARGS__)

#endif /* LISPLE__MACRO_SUPPORT_H */
