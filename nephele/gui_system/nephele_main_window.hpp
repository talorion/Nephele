#ifndef NEPHELE_MAIN_WINDOW_HPP
#define NEPHELE_MAIN_WINDOW_HPP

#include <QMainWindow>

class nephele_main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit nephele_main_window(QWidget *par = 0);
    ~nephele_main_window();

private:
};

#endif // NEPHELE_MAIN_WINDOW_HPP
