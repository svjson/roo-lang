#ifndef ROO_SERVER_EXPORT_H
#define ROO_SERVER_EXPORT_H

#if defined(_WIN32) && !defined(ROO_SERVER_STATIC_DEFINE)
#ifdef ROO_SERVER_BUILDING
#define ROO_SERVER_API __declspec(dllexport)
#else
#define ROO_SERVER_API __declspec(dllimport)
#endif
#else
#define ROO_SERVER_API
#endif

#endif /* ROO_SERVER_EXPORT_H */
