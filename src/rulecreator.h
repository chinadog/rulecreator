#ifndef RULECREATOR_H
#define RULECREATOR_H

#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QString>
#include <QTabWidget>
#include <QTextEdit>
#include <QWidget>
#include "highlighter.h"
#include "regexpwidget.h"
#include "rulewidget.h"
#include "tabbar.h"
#include "tabwidget.h"

//-------------------------------------------------------------------------------------------------------------------//
/**
 * @brief The InfoWidget class
 */
class InfoWidget : public QWidget
{
public:
    InfoWidget(QWidget* parent = 0);
};

//-------------------------------------------------------------------------------------------------------------------//
/**
 * @brief The RuleCreator class
 */
class RuleCreator : public QWidget
{
    Q_OBJECT
public:
    RuleCreator(const QString& configPath, QWidget *parent = 0);
    ~RuleCreator();
private:
    // Виджеты
    RuleWidget* m_tab;
    TabBar* m_bar;
    QVector<RuleWidget*> m_ruleTabs;
    QMenuBar* m_menuBar;
    InfoWidget* m_infoWidget;
    TabWidget* m_tabWidget;
    Highlighter* m_highlighter;
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_openRuleAction;
    QAction* m_closeAction;
    QAction* m_saveAsAction;
    QAction* m_parseAction;
    QAction* m_execAction;
    QAction* m_aboutAction;
    QAction* m_regexpAction;
    // Поля
    QString m_configPath;
    // Методы
    void loadStyle(const QString& name);
    void initInterface();
    void initConnections();
    void setStartValues();
private slots:
    RuleWidget* createNewTab();
    RegexpWidget* createRegexpTab();
    void openTab();
    void openRuleTab();
    void openTab(const QString& fileName);
    void saveCurrentTabAs();
    void closeCurrentTab();
    void closeTab(int);
    void parseMessage();
    void execRule();
    void updateMenu(int);
    void showAboutDialog();
};

#endif // RULECREATOR_H
