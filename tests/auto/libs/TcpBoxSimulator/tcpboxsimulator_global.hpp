#ifndef TCPBOXSIMULATOR_GLOBAL_HPP
#define TCPBOXSIMULATOR_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(TCPBOXSIMULATOR_LIBRARY)
#  define TCPBOXSIMULATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TCPBOXSIMULATORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TCPBOXSIMULATOR_GLOBAL_HPP
