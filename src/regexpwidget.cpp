#include <QBoxLayout>
#include <QSplitter>
#include <Common/Resources>
#include <TLogger/TLogger>
#include "regexpwidget.h"

RegexpWidget::RegexpWidget(QWidget *parent) : QWidget(parent)
{
    initInterface();
    initConnections();
}

void RegexpWidget::initInterface()
{
    // init
    QVBoxLayout* mainLay = new QVBoxLayout;
    QHBoxLayout* bodyLay = new QHBoxLayout;
    QVBoxLayout* textLay = new QVBoxLayout;
    QSplitter* splitter = new QSplitter(this);
    QWidget* leftWidget = new QWidget(this);
    QHBoxLayout* toolBarLay = new QHBoxLayout;
    m_regexpLineEdit = new QLineEdit(this);
    m_textEdit = new QTextEdit(this);
    m_resultTextEdit = new QTextEdit(this);
    m_execButton = new QPushButton(this);
    m_capSpinBox = new QSpinBox(this);
    m_numberSpinBox = new QSpinBox(this);
    // set lay
    toolBarLay->addWidget(m_execButton);
    toolBarLay->addWidget(m_capSpinBox);
    toolBarLay->addWidget(m_numberSpinBox);
    toolBarLay->addStretch(1);
    textLay->addWidget(m_regexpLineEdit);
    textLay->addWidget(m_textEdit);
    leftWidget->setLayout(textLay);
    splitter->addWidget(leftWidget);
    splitter->addWidget(m_resultTextEdit);
    bodyLay->addWidget(splitter);
    mainLay->addLayout(toolBarLay);
    mainLay->addLayout(bodyLay);
    setLayout(mainLay);
    // custom
    m_capSpinBox->setProperty("type","without-border");
    m_numberSpinBox->setProperty("type","without-border");
    m_execButton->setIcon(Resources::icon("dark/play.svg"));
    m_execButton->setProperty("type","flat");
    m_execButton->setIconSize(QSize(fontMetrics().height()*0.8,fontMetrics().height()*0.8));
    m_regexpLineEdit->setProperty("type","without-border");
    m_textEdit->setProperty("type","without-border");
    m_resultTextEdit->setProperty("type","without-border");
    m_regexpLineEdit->setObjectName("RegExpLineEdit");
    m_regexpLineEdit->setPlaceholderText(tr("RegExp"));
    m_textEdit->setPlaceholderText(tr("Text"));
    m_resultTextEdit->setPlaceholderText(tr("Result"));
    splitter->setHandleWidth(1);
    mainLay->setSpacing(0);
    bodyLay->setContentsMargins(0,0,0,0);
    bodyLay->setSpacing(0);
    textLay->setSpacing(0);
    textLay->setContentsMargins(0,0,0,0);
    mainLay->setContentsMargins(0,0,0,0);
}

void RegexpWidget::initConnections()
{
    connect(m_execButton,SIGNAL(clicked(bool)),this,SLOT(exec()));
    connect(m_regexpLineEdit,SIGNAL(textChanged(QString)),this,SLOT(exec()));
    connect(m_textEdit,SIGNAL(textChanged()),this,SLOT(exec()));
}

void RegexpWidget::exec()
{
    QRegExp rx(m_regexpLineEdit->text());

    TDEBUG() << "Start" << rx.pattern();

    QString str = m_textEdit->toPlainText();
    int pos = 0;
    int cap = m_capSpinBox->value();
    int number = m_numberSpinBox->value();
    int i = 0;
    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        QString result = rx.cap( cap );
        TDEBUG() << result;
        pos += rx.matchedLength();
        if( i == number )
        {
            m_resultTextEdit->setText(result);
            return;
        }
        i++;
    }
    m_resultTextEdit->clear();
    // list: ["12", "14", "99", "231", "7"]

}
