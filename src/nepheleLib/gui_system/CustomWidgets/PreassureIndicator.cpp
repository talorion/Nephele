#include "PreassureIndicator.hpp"

#include <gui_system/GuiConstantProvider.hpp>


namespace talorion {

PreassureIndicator::PreassureIndicator(QWidget *par, Qt::WindowFlags f):
    QLabel(par, f)
{
    init();
}

PreassureIndicator::PreassureIndicator(const QString &text, QWidget *par, Qt::WindowFlags f):
    QLabel(text, par, f)
{
    init();
}

void PreassureIndicator::init()
{
    setText("low sen.err.");
    setAlignment(Qt::AlignHCenter);
    QFont ionicFont = font();  // start out with MainWindow's font..
    ionicFont.setPointSize(GuiConstantProvider::neph_font_size_px()); // and make a bit smaller for legend
    ionicFont.setFamily(GuiConstantProvider::neph_font_family());
    setFont(ionicFont);
}



} // namespace talorion
