#include <QBoxLayout>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabBar>
#include <Common/Resources>
#include <TRules/TRules>
#include <fstream>
#include "rulecreator.h"
#include <QDebug>

//===================================================================================================================//
//                                                  INFO WIDGET
//===================================================================================================================//

InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent)
{
    // init
    QHBoxLayout* mainLay = new QHBoxLayout;
    QVBoxLayout* lay = new QVBoxLayout;
    QString fontSize = QString::number( font().pointSize()*1.2 );
    QLabel* titleLabel = new QLabel("<p style='font-size:"+fontSize+"pt'>OPEN DOCUMENT</p>",this);
    QLabel* textLabel = new QLabel("· File > New document (Ctrl + N)\n"
                                   "· File > Open document (Ctrl + O)\n"
                                   "· File > Open rule\n",this);
    // set lay
    lay->addStretch(1);
    lay->addWidget(titleLabel);
    lay->addWidget(textLabel);
    lay->addStretch(1);
    mainLay->addStretch(1);
    mainLay->addLayout(lay);
    mainLay->addStretch(1);
    setLayout(mainLay);
    // custom
    textLabel->setObjectName("InfoSeparator");
    titleLabel->setAlignment(Qt::AlignCenter);
}

//===================================================================================================================//
//                                                RULE CREATOR
//===================================================================================================================//

//-----Конструкторы--------------------------------------------------------------------------------------------------//

RuleCreator::RuleCreator(const QString& configPath, QWidget *parent)
    : QWidget(parent),
      m_configPath(configPath)
{
    initInterface();
    initConnections();
    setStartValues();
}

RuleCreator::~RuleCreator()
{

}

//-----Методы--------------------------------------------------------------------------------------------------------//

void RuleCreator::initInterface()
{
    // Инициализация
    QVBoxLayout* mainLay = new QVBoxLayout;
    QVBoxLayout* bodyLay = new QVBoxLayout;
    m_menuBar = new QMenuBar(this);
    m_tabWidget = new TabWidget(this);
    m_infoWidget = new InfoWidget(this);
    // Меню
    QMenu* fileMenu = new QMenu("File",this);
    m_newAction = new QAction("New",fileMenu);
    m_openAction = new QAction("Open",fileMenu);
    m_openRuleAction = new QAction("Open rule",fileMenu);
    m_closeAction = new QAction("Close",fileMenu);
    QAction* saveAction = new QAction("Save",fileMenu);
    m_saveAsAction = new QAction("Save as ...",fileMenu);
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_openRuleAction);
    fileMenu->addAction(m_closeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(m_saveAsAction);
    QMenu* editMenu = new QMenu("Edit",this);
    QAction* undoAction = new QAction("Undo",editMenu);
    QAction* redoAction = new QAction("Redo",editMenu);
    QAction* cutAction = new QAction("Cut",editMenu);
    QAction* copyAction = new QAction("Copy",editMenu);
    QAction* pasteAction = new QAction("Paste",editMenu);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    QMenu* parseMenu = new QMenu("Parse",this);
    m_parseAction = new QAction(tr("Parse"),parseMenu);
    m_execAction = new QAction(tr("Exec"),parseMenu);
    parseMenu->addAction(m_parseAction);
    parseMenu->addAction(m_execAction);
    QMenu* regexpMenu = new QMenu("Regexp",this);
    m_regexpAction = new QAction(tr("Regexp"),regexpMenu);
    regexpMenu->addAction(m_regexpAction);
    QMenu* aboutMenu = new QMenu("About...",this);
    m_aboutAction = new QAction(tr("About RuleCreator"),aboutMenu);
    aboutMenu->addAction(m_aboutAction);
    m_menuBar->addMenu(fileMenu);
    m_menuBar->addMenu(editMenu);
    m_menuBar->addMenu(parseMenu);
    m_menuBar->addMenu(regexpMenu);
    m_menuBar->addMenu(aboutMenu);
    // Установка на слой
    bodyLay->addWidget(m_infoWidget);
    bodyLay->addWidget(m_tabWidget);
    bodyLay->setContentsMargins(0,5,0,0);
    bodyLay->setSpacing(0);
    mainLay->addWidget(m_menuBar);
    mainLay->addLayout(bodyLay);
    mainLay->setContentsMargins(0,0,0,0);
    mainLay->setSpacing(0);
    setLayout(mainLay);
    // Кастомизация
    m_infoWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);
    m_tabWidget->setFocusPolicy(Qt::NoFocus);
    m_tabWidget->setProperty("type","minimalize");
    loadStyle( "dark" );
    resize(500,350);
    setWindowIcon(Resources::icon("/dark/rc.svg"));
}

void RuleCreator::initConnections()
{
    connect(m_newAction,SIGNAL(triggered(bool)),this,SLOT(createNewTab()));
    connect(m_openAction,SIGNAL(triggered(bool)),this,SLOT(openTab()));
    connect(m_openRuleAction,SIGNAL(triggered(bool)),this,SLOT(openRuleTab()));
    connect(m_closeAction,SIGNAL(triggered(bool)),this,SLOT(closeCurrentTab()));
    connect(m_saveAsAction,SIGNAL(triggered(bool)),this,SLOT(saveCurrentTabAs()));
    connect(m_parseAction,SIGNAL(triggered(bool)),this,SLOT(parseMessage()));
    connect(m_execAction,SIGNAL(triggered(bool)),this,SLOT(execRule()));
    connect(m_regexpAction,SIGNAL(triggered(bool)),this,SLOT(createRegexpTab()));
    connect(m_aboutAction,SIGNAL(triggered(bool)),this,SLOT(showAboutDialog()));
    connect(m_tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updateMenu(int)));
    connect(m_tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
}

void RuleCreator::setStartValues()
{
    createNewTab();
}

RuleWidget* RuleCreator::createNewTab()
{
    RuleWidget* newTab = new RuleWidget(this);
    m_ruleTabs.push_back( newTab );
    m_tabWidget->addTab( newTab, "New" );
    m_tabWidget->setCurrentWidget( newTab );
    if(m_tabWidget->count() > 0)
    {
        m_infoWidget->setVisible(false);
        m_tabWidget->setVisible(true);
    }
    return newTab;
}

RegexpWidget *RuleCreator::createRegexpTab()
{
    RegexpWidget* newTab = new RegexpWidget(this);
    m_tabWidget->addTab( newTab, "Regexp" );
    m_tabWidget->setCurrentWidget( newTab );
    if(m_tabWidget->count() > 0)
    {
        m_infoWidget->setVisible(false);
        m_tabWidget->setVisible(true);
    }
    return newTab;
}

void RuleCreator::openTab()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open MessageRule"), "",
                                                    tr("MessageRule (*.mrf);;All Files (*)"));
    openTab(fileName);
}

void RuleCreator::openRuleTab()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Rule"), "",
                                                    tr("Rule (*.rule);;All Files (*)"));
    openTab(fileName);
}

void RuleCreator::openTab(const QString &fileName)
{
    for(int i=0;i<m_ruleTabs.size();i++)
    {
        if(m_tabWidget->tabToolTip(i) == fileName)
        {
            m_tabWidget->setCurrentIndex(i);
            return;
        }
    }


    RuleWidget* currentTab = createNewTab();
    std::ifstream f;
    f.open(fileName.toStdString(), std::ios::in);
    std::string str;
    f.seekg(0, std::ios::end);
    str.reserve(f.tellg());
    f.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(f)),
                std::istreambuf_iterator<char>());
    QString mrfString = QString::fromStdString(str);
    currentTab->textToMrf(mrfString);
    f.close();
    QString tabName = fileName.right( fileName.size() - fileName.lastIndexOf('/') -1 );
    m_tabWidget->setTabToolTip( m_tabWidget->currentIndex(), fileName );
    m_tabWidget->setTabText( m_tabWidget->currentIndex(), tabName );
}

void RuleCreator::saveCurrentTabAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save this MessageRule"), "",
                                                    tr("MessageRule (*.mrf);;All Files (*)"));
    RuleWidget* currentTab = static_cast<RuleWidget*>(m_tabWidget->currentWidget());
    std::ofstream f;
    f.open(fileName.toStdString(), std::ios::trunc);
    f << currentTab->mrfToText().toStdString();
    f.close();
}

void RuleCreator::closeCurrentTab()
{
    closeTab( m_tabWidget->currentIndex() );
}

void RuleCreator::closeTab(int index)
{
    m_tabWidget->removeTab(index);
    if(m_tabWidget->count() == 0)
    {
        m_infoWidget->setVisible(true);
        m_tabWidget->setVisible(false);
    }
}

void RuleCreator::parseMessage()
{
    RuleWidget* tab = static_cast<RuleWidget*>(m_tabWidget->currentWidget());
    tab->parseMessage();
}

void RuleCreator::execRule()
{
    RuleWidget* tab = static_cast<RuleWidget*>(m_tabWidget->currentWidget());
    tab->execRule();
}

void RuleCreator::updateMenu(int index)
{
    m_parseAction->setEnabled(false);
    m_execAction->setEnabled(false);

    if( dynamic_cast<RuleWidget*>(m_tabWidget->widget(index)) != 0 )
    {
        m_parseAction->setEnabled(true);
        m_execAction->setEnabled(true);
    }
}

void RuleCreator::showAboutDialog()
{
    QString version = APP_VERSION;
    QString revision = APP_REVISION;
    QString date = APP_DATE;
    QString qtVersion = QT_VERSION_STR;
    QString dateStr = QDateTime::fromTime_t( date.toInt() ).toString("dd.MM.yyyy hh:mm:ss");
    QString fontSize = QString::number( font().pointSize()*1.5 );
    QString text = "<b style='font-size:"+fontSize+"pt'>RuleCreator "+version+"</b><br><br>"
                   "Based on Qt "+qtVersion+"<br><br>"
                   "From revision "+revision+"<br><br>"
                   "Built on "+dateStr+"<br><br>"
                   "Copyright 2016-2017 The Trifecta Ltd. All rights reserved.<br><br>"
                   "The software is provided \"as is\" and the author disclaims all warranties"
                   "with regard to this software including all implied warranties of"
                   "merchantability and fitness. in no event shall the author be liable for"
                   "any special, direct, indirect, or consequential damages or any damages"
                   "whatsoever resulting from loss of use, data or profits, whether in an"
                   "action of contract, negligence or other tortious action, arising out of"
                   "or in connection with the use or performance of this software.";
    QMessageBox::about(this,"About RuleCreator",text);
}


void RuleCreator::loadStyle(const QString& name)
{
    // Применение стилей ко всему приложению
    QFile style;
    QString path = RESOURCES_PATH;
    path+="style/"+name+".qss";
    style.setFileName(path);
    if(style.open(QFile::ReadOnly))
    {
        TINFO() << "Style "<<path<<" [OK]";
        QString qssStr = style.readAll();
        qApp->setStyleSheet( Resources::style(name)+
                             qssStr.replace("$$PREFIX",RESOURCES_PATH) );
    }
    else
    {
        TWARNING() << "Style "<<path<<" [FAIL]";
    }
}
