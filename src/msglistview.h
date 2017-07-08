#ifndef MSGLISTVIEW_H
#define MSGLISTVIEW_H

#include <QWidget>
#include <QListView>
#include "menu.h"
#include "msgeditwidget.h"
#include "messagemodel.h"

class MsgListView : public QListView
{
    Q_OBJECT
public:
    explicit MsgListView(QWidget *parent = 0);
    ~MsgListView();

    void addEvent(const Event& event);
    Event getEvent(int row) const;
    QList<Event> getEvents() const;
private:
    MessageModel* m_messageModel;
signals:

public slots:
    void deleteSelectedMsgWindow();
    void lowerSelectionOneStep();
    void raiseSelectionOneStep();
    void showContextMenu(const QPoint &pos);
    void showAddNewMsgWindow();
    void showMsgEditWindow(const QModelIndex &index);
};

#endif // MSGLISTVIEW_H
