#ifndef MSGEDITWIDGET_H
#define MSGEDITWIDGET_H

#include <QWidget>
#include <TDB/structs.h>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDialog>

class MsgEditWidget : public QDialog
{
    Q_OBJECT
public:
    explicit MsgEditWidget(QWidget *parent = 0);
    explicit MsgEditWidget(const Event &event, QWidget *parent = 0);

    Event getEvent();
private:
    Event m_event;
    QDateTimeEdit* m_dateTimeEdit;
    QTextEdit* m_msgTextEdit;
    QDialogButtonBox* m_buttonBox;

    void initInterface();
    void initConnections();

signals:

public slots:
    void eventToInterface();
    void interfaceToEvent();
    void acceptAndClose();
};

#endif // MSGEDITWIDGET_H
