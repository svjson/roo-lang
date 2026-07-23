#ifndef PROOF_EXPORT_H
#define PROOF_EXPORT_H

#if defined(_WIN32) && !defined(PROOF_NATIVE_STATIC_DEFINE)
#ifdef PROOF_NATIVE_BUILDING
#define PROOF_NATIVE_API __declspec(dllexport)
#else
#define PROOF_NATIVE_API __declspec(dllimport)
#endif
#else
#define PROOF_NATIVE_API
#endif

#endif /* PROOF_EXPORT_H */
