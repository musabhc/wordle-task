#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "wordlegame.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    WordleGame wordleGame;
    engine.rootContext()->setContextProperty("wordleGame", &wordleGame);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
