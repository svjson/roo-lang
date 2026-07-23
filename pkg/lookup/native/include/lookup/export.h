#ifndef LOOKUP_EXPORT_H
#define LOOKUP_EXPORT_H

#if defined(_WIN32) && !defined(LOOKUP_NATIVE_STATIC_DEFINE)
#ifdef LOOKUP_NATIVE_BUILDING
#define LOOKUP_NATIVE_API __declspec(dllexport)
#else
#define LOOKUP_NATIVE_API __declspec(dllimport)
#endif
#else
#define LOOKUP_NATIVE_API
#endif

#endif /* LOOKUP_EXPORT_H */
