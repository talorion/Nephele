#ifndef GUISYSTEM_GLOBAL_HPP
#define GUISYSTEM_GLOBAL_HPP

#include <QtCore/qglobal.h>
#include <QString>

#if defined(GUISYSTEM_LIBRARY)
#  define GUISYSTEMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GUISYSTEMSHARED_EXPORT Q_DECL_IMPORT
#endif

#define FONT_SIZE_PX 12
#define FONT_SIZE_STR QString::number(FONT_SIZE_PX) //white
#define BG_COLOR QString("#FFFFFF") //white
#define SEL_BG_COLOR QString("#eff0f1") //lightgrey
#define IC_FONT_FAMILY QString("Futura W01 Book")
#define IC_COLOR_GREY QString("#635D58") //grey
#define IC_COLOR_ORANGE QString("#F08A00") //orange
#define IC_COLOR_BLUE QString("#0083CB") //blue
#define MIN_COL_WIDTH 80
#define HORIZONTAL_SPACEING 0
#define MARGIN 5
#define BORDER QString("0px solid black") //white

#endif // GUISYSTEM_GLOBAL_HPP
