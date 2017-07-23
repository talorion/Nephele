#include "TitleLabel.hpp"

#include <gui_system/GuiConstantProvider.hpp>


namespace talorion {

  TitleLabel::TitleLabel(QWidget *par, Qt::WindowFlags f):
    QLabel(par, f)
  {
    init();
  }

  TitleLabel::TitleLabel(const QString &text, QWidget *par, Qt::WindowFlags f):
    QLabel(text, par, f)
  {
    init();
  }

  void TitleLabel::init()
  {
    setAlignment(Qt::AlignHCenter);
    QFont ionicFont = font();  // start out with MainWindow's font..
    ionicFont.setPointSize(GuiConstantProvider::neph_font_size_px()); // and make a bit smaller for legend
    ionicFont.setBold(true);
    ionicFont.setFamily(GuiConstantProvider::neph_font_family());
    setFont(ionicFont);

  }

} // namespace talorion
