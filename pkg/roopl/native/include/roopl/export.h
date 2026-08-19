#ifndef ROOPL_EXPORT_H
#define ROOPL_EXPORT_H

#if defined(_WIN32) && !defined(ROOPL_NATIVE_STATIC_DEFINE)
#ifdef ROOPL_NATIVE_BUILDING
#define ROOPL_NATIVE_API __declspec(dllexport)
#else
#define ROOPL_NATIVE_API __declspec(dllimport)
#endif
#else
#define ROOPL_NATIVE_API
#endif

#endif /* ROOPL_EXPORT_H */
