#ifndef PROOFREAD_EXPORT_H
#define PROOFREAD_EXPORT_H

#if defined(_WIN32) && !defined(PROOFREAD_NATIVE_STATIC_DEFINE)
#ifdef PROOFREAD_NATIVE_BUILDING
#define PROOFREAD_NATIVE_API __declspec(dllexport)
#else
#define PROOFREAD_NATIVE_API __declspec(dllimport)
#endif
#else
#define PROOFREAD_NATIVE_API
#endif

#endif /* PROOFREAD_EXPORT_H */
