#include "msgeditwidget.h"
#include <QBoxLayout>
#include <QDebug>

MsgEditWidget::MsgEditWidget(QWidget *parent) : QDialog(parent)
{
    m_event.id = -1;
    m_event.msg = QString::null;
    m_event.time = QDateTime();

    initInterface();
    initConnections();
}

MsgEditWidget::MsgEditWidget(const Event &event, QWidget *parent) :
    MsgEditWidget(parent)
{
    m_event = event;
    eventToInterface();
}

Event MsgEditWidget::getEvent()
{
    return m_event;
}

void MsgEditWidget::initInterface()
{
    // init
    QVBoxLayout* mainLay = new QVBoxLayout;
    QHBoxLayout* buttonLay = new QHBoxLayout;
    m_dateTimeEdit = new QDateTimeEdit(this);
    m_msgTextEdit = new QTextEdit(this);
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply |
                                       QDialogButtonBox::Cancel, this);
    // set lay
    buttonLay->addStretch(1);
    buttonLay->addWidget(m_buttonBox);
    mainLay->addWidget(m_dateTimeEdit);
    mainLay->addWidget(m_msgTextEdit);
    mainLay->addLayout(buttonLay);
    setLayout(mainLay);
    // custom
    mainLay->setSpacing(0);
    mainLay->setContentsMargins(0,0,0,0);
    m_msgTextEdit->setFocus();
    // func
    eventToInterface();
}

void MsgEditWidget::initConnections()
{
    connect(m_buttonBox->button(QDialogButtonBox::Apply),SIGNAL(clicked()),this,SLOT(acceptAndClose()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void MsgEditWidget::eventToInterface()
{
    m_dateTimeEdit->setDateTime( m_event.time );
    m_msgTextEdit->setText( m_event.msg );
}

void MsgEditWidget::interfaceToEvent()
{
    m_event.time = m_dateTimeEdit->dateTime();
    m_event.msg = m_msgTextEdit->toPlainText();
}

void MsgEditWidget::acceptAndClose()
{
    interfaceToEvent();
    accept();
}


