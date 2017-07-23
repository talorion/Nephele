#include "GuiConstantProvider.hpp"

#include <QFontDatabase>


namespace talorion {

  int GuiConstantProvider::font_id = -1;

  GuiConstantProvider::GuiConstantProvider()
  {

  }

  QString GuiConstantProvider::editableBgColor()
  {
    return QString("#FFFFFF"); //white
  }

  QString GuiConstantProvider::fg_color()
  {
    //return QString("#635D58"); //grey
    //return QString("#939393"); //grey
    return QString("#424242"); //grey
  }

  QString GuiConstantProvider::bg_color()
  {
    //return QString("#FFFFFF"); //white
    return QString("#F0F0F0");
    //return QString("#F0F0F0"); //white
  }

  QString GuiConstantProvider::bg_color_warn()
  {
    return QString("lightYellow");
  }

  QString GuiConstantProvider::bg_color_Error()
  {
    return QString("Tomato");
  }

  QString GuiConstantProvider::sel_bg_color()
  {
    //return QString("#eff0f1"); //lightgrey
    return QString("#EFEFEF"); //lightgrey
  }

  QString GuiConstantProvider::baseColor()
  {
    return QString("#0083CB"); //blue
  }

  QString GuiConstantProvider::complementaryColor()
  {
    return QString("#F08A00"); //orange
  }

  QString GuiConstantProvider::analogIndicatorOk()
  {
    return QString("talorion--AnalogIndicator { background-color :    "+bg_color()+";}");
  }

  QString GuiConstantProvider::analogIndicatorWarning()
  {
    return QString("talorion--AnalogIndicator { background-color :  "+bg_color_warn()+";}");
  }

  QString GuiConstantProvider::analogIndicatorError()
  {
    return QString("talorion--AnalogIndicator { background-color : "+bg_color_Error()+";}"); //orange
  }

  QString GuiConstantProvider::neph_font_family()
  {
    //return QString("Futura W01 Book");
    //return QString("Open Sans");
    //return QString("Calibri");
    //return QString("Times New Roman");
    //return QString("Segoe UI");
    //return QString("Cantarell-Regular");


    if(font_id < 0){
        //font_id = QFontDatabase::addApplicationFont(":/fonts/fonts/Cantarell-Regular.ttf");
        //static int id = QFontDatabase::addApplicationFont(":/fonts/fonts/Cantarell-Bold.ttf");
        //static int id = QFontDatabase::addApplicationFont(":/fonts/fonts/Cantarell-BoldOblique.ttf");
        //font_id = QFontDatabase::addApplicationFont(":/fonts/fonts/orbitron-light.otf");
        font_id = QFontDatabase::addApplicationFont(":/fonts/fonts/OpenSans-CondLight.ttf");
      }

    if(font_id >= 0){
        QString family = QFontDatabase::applicationFontFamilies(font_id).at(0);
        //QFont monospace(family);
        return family;
      }else{
        return QString("Segoe UI");
      }
  }

  int GuiConstantProvider::neph_font_size_px()
  {
    return 12;
  }

  QString GuiConstantProvider::neph_font_size()
  {
    return QString::number(neph_font_size_px());
  }

  QString GuiConstantProvider::border()
  {
    return QString("0px solid black");
  }

  QString GuiConstantProvider::buttonBorder()
  {
    return editableBorder();
  }

  QString GuiConstantProvider::editableBorder()
  {
    return QString("0px solid %1").arg(fg_color());
  }

  int GuiConstantProvider::min_col_with()
  {
    return 80;
    //return 180;
  }

  int GuiConstantProvider::horizontal_spaceing()
  {
    return 0;
  }

  int GuiConstantProvider::margin()
  {
    return 0;
  }

  QString GuiConstantProvider::marginStr()
  {
    return QString("%1px").arg(margin());
  }

  int GuiConstantProvider::spacing()
  {
    return 0;
  }

  QString GuiConstantProvider::spacingStr()
  {
    return QString("%1px").arg(spacing());
  }

  QString GuiConstantProvider::stylesheet()
  {
    return "\
            QFrame{ \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+border()+"; \
              background-color: "+bg_color()+" \
            } \
            QMainWindow { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+"; \
              font-family: "+neph_font_family()+"; \
              font-size: "+neph_font_size()+"px; \
              color: "+fg_color()+"; \
              border: "+border()+"; \
            } \
            QWidget { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+"; \
              font-family: "+neph_font_family()+"; \
              font-size: "+neph_font_size()+"px; \
              color: "+fg_color()+"; \
              border: "+border()+"; \
            } \
            QWidget:focus {\
              border: "+border()+"; \
              background-color: "+sel_bg_color()+";\
            } \
            QLabel { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+"; \
              font-family: "+neph_font_family()+"; \
              selection-background-color: "+baseColor()+"; \
              selection-color: "+baseColor()+"; \
              font-size: "+neph_font_size()+"px; \
              color: "+fg_color()+"; \
              border: "+border()+"; \
            } \
            QDockWidget { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+border()+"; \
              text-align: center; \
              background: "+bg_color()+"; \
              color: "+fg_color()+"; \
              font-weight: bold; \
            } \
            QDockWidget::title { \
              text-align: center; \
              background: "+bg_color()+"; \
            } \
            QCustomPlot { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+"; \
              border: "+border()+"; \
            } \
            QDoubleSpinBox { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+border()+"; \
              background-color: "+bg_color()+"; \
              font-family: "+neph_font_family()+"; \
              selection-background-color: "+baseColor()+"; \
              selection-color: white; \
              color: "+fg_color()+"; \
            } \
            QPushButton { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+buttonBorder()+"; \
              background-color: "+bg_color()+"; \
              font-family: "+neph_font_family()+"; \
              selection-background-color: "+baseColor()+"; \
              selection-color: white; \
              color: "+fg_color()+"; \
              border-radius: 2px; \
            } \
            QPushButton:focus { \
              background-color: "+sel_bg_color()+";\
            } \
            QPushButton:pressed  { \
              background-color: "+baseColor()+";\
            } \
            QPushButton:hover { \
              background-color: "+sel_bg_color()+";\
            } \
            QPushButton:disabled { \
              color: "+sel_bg_color()+";\
            } \
            QToolButton { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+border()+"; \
              background-color: "+editableBgColor()+"; \
              font-family: "+neph_font_family()+"; \
              selection-background-color: "+baseColor()+"; \
              selection-color: white; \
              color: "+fg_color()+"; \
              border-radius: 2px; \
            } \
            QToolButton:pressed  { \
              background-color: "+sel_bg_color()+";\
            } \
            QToolButton:hover { \
              background-color: "+sel_bg_color()+";\
            } \
            QToolButton:disabled { \
              color: "+sel_bg_color()+";\
            } \
            QComboBox { \
              background-color: "+editableBgColor()+"; \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+border()+"; \
              border-radius: 2px; \
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
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+editableBgColor()+"; \
              font-family: "+neph_font_family()+"; \
              selection-background-color: "+baseColor()+"; \
              selection-color: white; \
              color: "+fg_color()+"; \
            } \
            QTreeView { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+editableBgColor()+"; \
              font-family: "+neph_font_family()+"; \
              selection-background-color: "+baseColor()+"; \
              selection-color: "+baseColor()+"; \
              font-size: "+neph_font_size()+"px; \
              color: "+fg_color()+"; \
              border: "+border()+"; \
            } \
            QTreeWidget:focus {\
              background-color: "+bg_color()+";\
            } \
            QTreeView::item:hover { \
              background-color: "+sel_bg_color()+";\
            } \
            QTreeView::item { \
              border: "+border()+"; \
              background-color: "+bg_color()+";\
            }\
            QHeaderView { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background: "+bg_color()+"; \
              font-family: "+neph_font_family()+"; \
              text-align: center; \
              font-weight: bold; \
            } \
            QListView { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+editableBgColor()+"; \
              font-family: "+neph_font_family()+"; \
              selection-background-color: "+bg_color()+"; \
              selection-color: "+bg_color()+"; \
              font-size: "+neph_font_size()+"px; \
              color: "+fg_color()+"; \
              border: "+border()+"; \
            } \
            QListWidget:focus {\
              background-color: "+editableBgColor()+"; \
            } \
            QListView::item { \
              border: "+border()+"; \
              background-color: "+editableBgColor()+"; \
            }\
            QListView::item:selected {\
              background-color: "+baseColor()+";\
            }\
            QListView::item:hover { \
              background-color: "+sel_bg_color()+";\
              color: black; \
            }\
            QListView::item:alternate { \
              background: "+baseColor()+"; \
            }\
            talorion--tcp_box_view { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+";\
              border: "+border()+"; \
            }\
            talorion--flowControllerView { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+";\
              border: "+border()+"; \
            }\
            talorion--nephele_main_window { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+";\
              border: "+border()+"; \
            }\
            talorion--AnalogControl { \
              background-color: "+editableBgColor()+"; \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+editableBorder()+"; \
              border-radius: 2px; \
            }\
            talorion--AnalogIndicator { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color :  "+bg_color()+"; \
              border: "+border()+"; \
              border-radius: 2px; \
            }\
            talorion--DigitalControl { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+editableBgColor()+"; \
              border: "+editableBorder()+"; \
              border-radius: 2px; \
              border-style: solid; \
              border-width: 1px; \
              border-color: "+sel_bg_color()+"; \
            }\
            talorion--TimeControl { \
              background-color: "+editableBgColor()+"; \
              border: "+editableBorder()+"; \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              border: "+border()+"; \
              border-radius: 2px; \
            }\
            talorion--NepheleDockWidget { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+";\
              border: "+border()+"; \
            }\
            talorion--TitleLabel { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+";\
              border: "+border()+"; \
            }\
            QGridLayout { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+";\
              border: "+border()+"; \
            }\
            talorion--AnalogIndicator { \
              margin: "+marginStr()+"; \
              spacing: "+spacingStr()+"; \
              background-color: "+bg_color()+";\
              border: "+border()+"; \
              font-size: "+neph_font_size()+"; \
            }\
            talorion--DigitalControlButton { \
              background-color : "+bg_color()+";\
              border-radius: 5px; \
              border-style: solid; \
              border-width: 1px; \
              border-color: "+sel_bg_color()+"; \
            }\
            talorion--DigitalControlLabel { \
              background-color : "+bg_color()+";\
              border-radius: 5px; \
            }\
            ";
   }

} // namespace talorion
