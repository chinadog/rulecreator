#ifndef MENU_H
#define MENU_H

#include <QHelpEvent>
#include <QMenu>
#include <QToolTip>

/**
 * @brief Класс Menu
 * @details Предназначен для вывода панели меню, контекстного меню и других
 * всплывающих меню. Наследуюется от QMenu и позволяет выводить всплывающие
 * подсказки при наведении указателя мыша.
 */
class Menu : public QMenu
{
    Q_OBJECT
public:
    Menu()
    {
        setAttribute(Qt::WA_TranslucentBackground);
    }
    bool event (QEvent * e)  override
    {
        const QHelpEvent *helpEvent = static_cast <QHelpEvent *>(e);
         if (helpEvent->type() == QEvent::ToolTip && activeAction() != 0)
         {
              QToolTip::showText(helpEvent->globalPos(), activeAction()->toolTip());
         } else
         {
              QToolTip::hideText();
         }
         return QMenu::event(e);
    }
};

#endif // MENU_H
