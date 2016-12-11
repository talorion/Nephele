#include "Style.hpp"

#include "guisystem_global.hpp"

namespace talorion {

  Style::Style()
  {

  }

  QString Style::getFlatStyle()
  {

    return "\
        QFrame{ \
          background-color: "+BG_COLOR+" \
        } \
        QMainWindow { \
          background-color: "+BG_COLOR+"; \
          font-family: "+IC_FONT_FAMILY+"; \
          font-size: "+FONT_SIZE_STR+"px; \
          color: "+IC_COLOR_GREY+"; \
          border: "+BORDER+"; \
        } \
        QWidget { \
          background-color: "+BG_COLOR+"; \
          font-family: "+IC_FONT_FAMILY+"; \
          font-size: "+FONT_SIZE_STR+"px; \
          color: "+IC_COLOR_GREY+"; \
          border: "+BORDER+"; \
        } \
        QWidget:focus {\
          background-color: "+SEL_BG_COLOR+";\
        } \
        QLabel { \
          background-color: "+BG_COLOR+"; \
          font-family: "+IC_FONT_FAMILY+"; \
          selection-background-color: "+IC_COLOR_BLUE+"; \
          selection-color: "+IC_COLOR_BLUE+"; \
          font-size: "+FONT_SIZE_STR+"px; \
          color: "+IC_COLOR_GREY+"; \
          border: "+BORDER+"; \
        } \
        QDockWidget { \
          text-align: center; \
          background: "+BG_COLOR+"; \
          color: "+IC_COLOR_GREY+"; \
          font-weight: bold; \
        } \
        QDockWidget::title { \
          text-align: center; \
          background: "+BG_COLOR+"; \
        } \
        QCustomPlot { \
          background-color: "+BG_COLOR+"; \
          border: "+BORDER+"; \
        } \
        QDoubleSpinBox { \
          background-color: "+BG_COLOR+"; \
          font-family: "+IC_FONT_FAMILY+"; \
          selection-background-color: "+IC_COLOR_BLUE+"; \
          selection-color: white; \
          color: "+IC_COLOR_GREY+"; \
        } \
        QPushButton { \
          background-color: "+BG_COLOR+"; \
          font-family: "+IC_FONT_FAMILY+"; \
          selection-background-color: "+IC_COLOR_BLUE+"; \
          selection-color: white; \
          color: "+IC_COLOR_GREY+"; \
        } \
        QPushButton:focus { \
          background-color: "+SEL_BG_COLOR+";\
        } \
        QPushButton:pressed  { \
          background-color: "+SEL_BG_COLOR+";\
        } \
        QPushButton:hover { \
          background-color: "+SEL_BG_COLOR+";\
        } \
        QToolButton { \
          background-color: "+BG_COLOR+"; \
          font-family: "+IC_FONT_FAMILY+"; \
          selection-background-color: "+IC_COLOR_BLUE+"; \
          selection-color: white; \
          color: "+IC_COLOR_GREY+"; \
          border-radius: 5px; \
        } \
        QToolButton:pressed  { \
          background-color: "+SEL_BG_COLOR+";\
        } \
        QToolButton:hover { \
          background-color: "+SEL_BG_COLOR+";\
        } \
        QComboBox { \
          border-radius: 5px; \
          border: "+BORDER+"; \
        } \
        QComboBox::drop-down { \
          subcontrol-origin: padding; \
          subcontrol-position: top right; \
          width: 0px;  \
          border-left-width: 0px; \
          border-left-color: darkgray; \
          border-left-style: solid; \
          border-top-right-radius: 0px; \
          border-bottom-right-radius: 0px; \
        } \
        QTimeEditQTimeEdit { \
          background-color: "+BG_COLOR+"; \
          font-family: "+IC_FONT_FAMILY+"; \
          selection-background-color: "+IC_COLOR_BLUE+"; \
          selection-color: white; \
          color: "+IC_COLOR_GREY+"; \
        } \
        QCustomPlot { \
          background-color: "+SEL_BG_COLOR+";\
        } \
        ";
  }

} // namespace talorion
