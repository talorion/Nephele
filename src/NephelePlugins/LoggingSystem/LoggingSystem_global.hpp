#ifndef LOGGING_SYSTEM_GLOBAL_HPP
#define LOGGING_SYSTEM_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(LOGGING_SYSTEM_LIBRARY)
#  define DLL_TESTSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DLL_TESTSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LOGGING_SYSTEM_GLOBAL_HPP