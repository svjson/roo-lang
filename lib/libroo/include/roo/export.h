#ifndef ROO_EXPORT_H
#define ROO_EXPORT_H

#if defined(_WIN32) && !defined(ROO_STATIC_DEFINE)
#ifdef ROO_BUILDING_ROO
#define ROO_API __declspec(dllexport)
#else
#define ROO_API __declspec(dllimport)
#endif
#else
#define ROO_API
#endif

#endif /* ROO_EXPORT_H */
