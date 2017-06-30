#include "msglistview.h"
#include <QDebug>

MsgListView::MsgListView(QWidget *parent) : QListView(parent)
{
    m_messageModel = new MessageModel;
    setModel(m_messageModel);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showMsgEditWindow(QModelIndex)));
}

MsgListView::~MsgListView()
{
    delete m_messageModel;
}

void MsgListView::deleteSelectedMsgWindow()
{
    QModelIndexList indexes = selectionModel()->selectedIndexes();
    qDebug() << indexes;
    for(int i=0;i<indexes.size();i++)
    {
        m_messageModel->removeRow(indexes[i].row());
    }
}

void MsgListView::showContextMenu(const QPoint &pos)
{
    // Handle global position
    QPoint globalPos = mapToGlobal( pos );

    // Create menu and insert some actions
    Menu myMenu;

    myMenu.addAction(tr("Add new event"),  this, SLOT(showAddNewMsgWindow()));
    myMenu.actions().last()->setToolTip(tr("Add new event"));
    myMenu.addSeparator();
    myMenu.addAction(tr("Delete event"),  this, SLOT(deleteSelectedMsgWindow()));
    myMenu.actions().last()->setToolTip(tr("Delete this event"));

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void MsgListView::showAddNewMsgWindow()
{
    MsgEditWidget msgEditWidget;
    msgEditWidget.move(QCursor::pos());
    if (msgEditWidget.exec() == QDialog::Accepted)
    {
        m_messageModel->addRow(msgEditWidget.getEvent());
    }
}

void MsgListView::showMsgEditWindow(const QModelIndex &index)
{
    Event event = m_messageModel->getEvent(index);
    MsgEditWidget msgEditWidget(event);
    msgEditWidget.move(QCursor::pos());
    if (msgEditWidget.exec() == QDialog::Accepted)
    {
        m_messageModel->updateRow( msgEditWidget.getEvent(), index.row());
    }
}
