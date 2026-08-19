#ifndef INPOOTS_EXPORT_H
#define INPOOTS_EXPORT_H

#if defined(_WIN32) && !defined(INPOOTS_NATIVE_STATIC_DEFINE)
#ifdef INPOOTS_NATIVE_BUILDING
#define INPOOTS_NATIVE_API __declspec(dllexport)
#else
#define INPOOTS_NATIVE_API __declspec(dllimport)
#endif
#else
#define INPOOTS_NATIVE_API
#endif

#endif /* INPOOTS_EXPORT_H */
