#include <QGuiApplication>
#include <QQuickStyle>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QThread>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "campreviewhandler.h"
#include "matimageprovider.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fontawesome-webfont.ttf");

    qmlRegisterType<CamPreviewHandler>("my.components", 1, 0, "CamPreviewHandler");

    std::cout << "main thread id " << QThread::currentThreadId() << std::endl;

    QQmlApplicationEngine engine;
    auto imageProvider = MatImageProvider::instance();
    engine.addImageProvider(QString("mat"), imageProvider);
    engine.load(QUrl("qrc:/main.qml"));

    return app.exec();
}
