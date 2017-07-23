#ifndef TALORION_GUICONSTANTPROVIDER_HPP
#define TALORION_GUICONSTANTPROVIDER_HPP

#include <QString>

namespace talorion {

class GuiConstantProvider
{
private:
    GuiConstantProvider();
    ~GuiConstantProvider();

public:
    static QString editableBgColor();
    static QString fg_color() ;
    static QString bg_color() ;
    static QString bg_color_warn() ;
    static QString bg_color_Error() ;
    static QString sel_bg_color();
    static QString baseColor();
    static QString complementaryColor();

    static QString analogIndicatorOk();
    static QString analogIndicatorWarning();
    static QString analogIndicatorError();

    static QString neph_font_family();
    static int neph_font_size_px() ;
    static QString neph_font_size() ;

    static QString border() ;
    static QString buttonBorder() ;
    static QString editableBorder() ;
    static int min_col_with() ;
    static int horizontal_spaceing() ;
    static int margin() ;
    static QString marginStr() ;
    static int spacing() ;
    static QString spacingStr() ;


    static QString stylesheet() ;

private:
    static int font_id;
};

} // namespace talorion

#endif // TALORION_GUICONSTANTPROVIDER_HPP
