#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>

class TabBar : public QTabBar
{
public:
    TabBar(QWidget* parent = 0);
private:
    QSize tabSizeHint(int index) const override;
};

#endif // TABBAR_H
