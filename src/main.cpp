#include <QApplication>
#include <QTranslator>
#include <Common/PreLoad>
#include <TLogger/TLogger>
#include "rulecreator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString configPath = PreLoad::getConfigPath(argc,argv,CONFIG_PATH);
    RuleCreator* ruleCreator;
    if( PreLoad::configIsValid(configPath+"settings.conf") )
    {
        TLogger::get().init(configPath.toStdString()+"settings.conf");
        // Загрузка переводов
        QTranslator myTranslator;
        QString translationPath = RESOURCES_PATH;
        translationPath += "translation/";
        myTranslator.load("trifecta_ru",translationPath);
        a.installTranslator(&myTranslator);
        TINFO() << "Start the server with the" << configPath <<
                   "configuration path";
        ruleCreator = new RuleCreator(configPath);
        ruleCreator->show();
    }
    else
    {
        TERROR() << "Configuration file doesn't exist in folder - " <<
                    configPath;
        TERROR() << "Closing application";
        return 1;
    }
    int r = a.exec();
    ruleCreator->deleteLater();
    return r;
}
