#include <QBoxLayout>
#include <QSplitter>
#include <TRules/TRules>
#include "rulewidget.h"
#include <QAbstractTextDocumentLayout>
#include <QDebug>
#include <QJsonDocument>
#include <QPainter>
#include "menu.h"
#include <QJsonObject>
#include <QJsonArray>

//===================================================================================================================//
//                                                RULE WIDGET
//===================================================================================================================//

//-----Конструкторы--------------------------------------------------------------------------------------------------//

RuleWidget::RuleWidget(QWidget* parent) : QWidget(parent)
{
    initInterface();
    initConnections();
//    setStartValues();
}

RuleWidget::~RuleWidget()
{
    delete m_messageModel;
}

//-----Методы--------------------------------------------------------------------------------------------------------//

void RuleWidget::initInterface()
{
    // Инициализация
    QSplitter* hSplitter = new QSplitter(this);
    QSplitter* vSplitter = new QSplitter(this);
    QVBoxLayout* mainLay = new QVBoxLayout;
    QVBoxLayout* bodyLay = new QVBoxLayout;
    m_rulesTextEdit = new CodeTextEdit(this);
    m_rulesTextEdit->setLineNumberVisible(true);
    m_msgListView = new MsgListView(this);
    m_resultTextEdit = new QTextEdit(this);
    m_highlighter = new Highlighter(m_rulesTextEdit->document());
    m_rowPosLabel = new QLabel(tr("Row: 0"));
    m_colPosLabel = new QLabel(tr("Col: 0"));
    m_statusBar = new QStatusBar(this);
    // Установка на слой
    m_statusBar->addWidget(m_rowPosLabel);
    m_statusBar->addWidget(m_colPosLabel);
    vSplitter->addWidget(m_rulesTextEdit);
    vSplitter->addWidget(m_msgListView);
    hSplitter->addWidget(vSplitter);
    hSplitter->addWidget(m_resultTextEdit);
    bodyLay->addWidget(hSplitter);
    bodyLay->setContentsMargins(0,0,0,0);
    bodyLay->setSpacing(0);
    mainLay->addLayout(bodyLay);
    mainLay->addWidget(m_statusBar);
    mainLay->setContentsMargins(0,0,0,0);
    mainLay->setSpacing(0);
    setLayout(mainLay);
    // Кастомизация
    m_rulesTextEdit->setObjectName("RuleTextEdit");
    m_msgListView->setObjectName("MessageTextEdit");
    m_resultTextEdit->setObjectName("ResultTextEdit");
    m_statusBar->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->setHandleWidth(1);
    hSplitter->setHandleWidth(1);
    hSplitter->setStretchFactor(0,3);
    hSplitter->setStretchFactor(1,1);
    m_msgListView->setProperty("type","without-border");
    m_rulesTextEdit->setProperty("type","without-border");
    m_resultTextEdit->setProperty("type","without-border");
    m_rulesTextEdit->setPlaceholderText(tr("Rules"));
    m_rulesTextEdit->setFont(QFont("Monospace"));
    m_rulesTextEdit->setLineWrapMode(QTextEdit::NoWrap);
    m_resultTextEdit->setPlaceholderText(tr("Result"));
    int fontWidth = QFontMetrics(m_rulesTextEdit->currentCharFormat().font()).averageCharWidth();
    m_rulesTextEdit->setTabStopWidth( 3 * fontWidth ); //Устанвока ширины табуляции
}

void RuleWidget::initConnections()
{
    connect(m_rulesTextEdit,SIGNAL(cursorPositionChanged()),this,SLOT(changeRuleCursor()));
//    connect(m_msgTextEdit,SIGNAL(cursorPositionChanged()),this,SLOT(changeMsgCursor()));
}

QString RuleWidget::rulesText() const
{
    return m_rulesTextEdit->toPlainText();
}

QJsonArray RuleWidget::msgText() const
{
    QJsonArray result;
    for(int i=0;i<m_msgListView->model()->rowCount();i++)
    {
        Event event = m_msgListView->getEvent(i);
        QJsonObject obj;
        obj["id"] = event.id;
        obj["time"] = event.time.toMSecsSinceEpoch();
        obj["msg"] = event.msg;
        result.append(obj);
    }
    return result;
}

QString RuleWidget::mrfToText() const
{
    QJsonObject obj;
    obj["rule"] = m_rulesTextEdit->toPlainText();
    obj["events"] = msgText();
    QJsonDocument doc(obj);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    return strJson;
}

void RuleWidget::setRulesText(const QString &value)
{
    m_rulesTextEdit->setText(value);
}

void RuleWidget::setMsgText(const QString &/*value*/)
{
    //    m_msgTextEdit->setText(value);
}

void RuleWidget::textToMrf(const QString &text)
{
    QJsonObject obj = QJsonDocument::fromJson(text.toUtf8()).object();
    m_rulesTextEdit->setText( obj["rule"].toString() );
    QJsonArray array = obj["events"].toArray();
    for(int i=0;i<array.count();i++)
    {
        QJsonObject eventObj = array[i].toObject();
        Event e;
        e.id = eventObj["id"].toInt();
        e.msg = eventObj["msg"].toString();
        e.time = QDateTime::fromMSecsSinceEpoch( eventObj["time"].toInt() );
        m_msgListView->addEvent(e);
    }
}

void RuleWidget::setStartValues()
{
    m_rulesTextEdit->setText("find:msg,test|\n"
                             "find:msg,\"Здесь\"|find:msg,\"tt\"|\n"
                             "find:msg,\"то\",find:msg,\"строки\"|");
//    m_msgTextEdit->setText("#include <QRegExp>\n"
//                           "#include <QStringList>\n"
//                           "#include <QDebug>\n"
//                           "\n"
//                           "int main() {\n"
//                               "QRegExp re( \"#include <([^>]+)>\" );\n"
//                               "int lastPos = 0;\n"
//                               "while( ( lastPos = re.indexIn( TEXT, lastPos ) ) != -1 ) {\n"
//                                   "lastPos += re.matchedLength();\n"
//                                   "qDebug() << re.cap( 0 );\n"
//                               "}\n"
//                               "return 0;\n"
//                           "}");
}

void RuleWidget::changeRuleCursor()
{
    QString text;
    text = QString("Row: %0").
                  arg(QString::number(m_rulesTextEdit->textCursor().blockNumber()+1),
                                      4,' ');
    m_rowPosLabel->setText(text);
    text = QString("Col: %0").
                  arg(QString::number(m_rulesTextEdit->textCursor().columnNumber()+1),
                                      4,' ');
    m_colPosLabel->setText(text);
}

void RuleWidget::changeMsgCursor()
{
//    QString text;
//    text = QString("Row: %0").
//                  arg(QString::number(m_msgTextEdit->textCursor().blockNumber()+1),
//                                      4,' ');
//    m_rowPosLabel->setText(text);
//    text = QString("Col: %0").
//                  arg(QString::number(m_msgTextEdit->textCursor().columnNumber()+1),
//                                      4,' ');
//    m_colPosLabel->setText(text);
}

void RuleWidget::execRule()
{

}

void RuleWidget::parseMessage()
{
//    QString rulesText = m_rulesTextEdit->toPlainText();
////    QString msgText = m_msgTextEdit->toPlainText();
//    QList<QString> msgs = m_msgTextEdit->toPlainText().split("\n-\n");
//    Rules rule;
//    rule.setRules(rulesText);
//    for(int i=0;i<msgs.size();i++)
//    {
////        rule.exec(msgs[i],)
//    }
//    TDEBUG() << rule;
//    TINFO() << rule.exec(msgText,100);
}




