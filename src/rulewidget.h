#ifndef RULEWIDGET_H
#define RULEWIDGET_H

#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QStatusBar>
#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QWidget>
#include "codetextedit.h"
#include "highlighter.h"
#include "messagemodel.h"
#include "msglistview.h"


class RuleWidget : public QWidget
{
    Q_OBJECT
public:
    // Конструкторы
    RuleWidget(QWidget* parent = 0);
    ~RuleWidget();

    QString rulesText() const;
    QString msgText() const;
    void setRulesText(const QString& value);
    void setMsgText(const QString& value);
private:
    // Виджеты
    CodeTextEdit* m_rulesTextEdit;
    MsgListView* m_msgListView;
    MessageModel* m_messageModel;
    QTextEdit* m_resultTextEdit;
    QPushButton* m_parseButton;
    Highlighter* m_highlighter;
    QLabel* m_rowPosLabel;
    QLabel* m_colPosLabel;
    QStatusBar* m_statusBar;
    // Методы
    void initInterface();
    void initConnections();
    void setStartValues();
private slots:
    void changeRuleCursor();
    void changeMsgCursor();
public slots:
    void execRule();
    void parseMessage();
};

#endif // RULEWIDGET_H
