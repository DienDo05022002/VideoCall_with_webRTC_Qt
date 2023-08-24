#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "conductor.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //###
    std::shared_ptr<Conductor> p(new Conductor());
    engine.rootContext()->setContextProperty("conductor", p.get());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
