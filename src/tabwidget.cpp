#include "tabwidget.h"

TabWidget::TabWidget(QWidget* parent) :
    QTabWidget(parent)
{
    setTabBar(new TabBar(this));
}

