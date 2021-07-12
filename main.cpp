#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    QStringList args = app.arguments();
    QString configPath = "config.ini";
    if(args.length()>1){
        configPath = args[1];
    }
    window.settings = new QSettings(configPath, QSettings::IniFormat);

    window.loadSettings();
    window.show();
    return app.exec();
}
