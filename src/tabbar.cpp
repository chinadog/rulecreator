#include "tabbar.h"

TabBar::TabBar(QWidget* parent) :
    QTabBar(parent)
{
    setDrawBase(false);
}


QSize TabBar::tabSizeHint(int index) const
{
    // Вычисление размеров кнопки "Х" относительно шрифта
    int width = fontMetrics().width("W");
    QWidget *w = tabButton(index,QTabBar::RightSide);
    if(w != 0)
    {
        w->resize(width*1.3, width/1.3);
    }
    return QTabBar::tabSizeHint(index);
}
