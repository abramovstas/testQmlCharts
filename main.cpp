//#include <QGuiApplication>
#include <QApplication>
#include <QScopedPointer>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "databuilder.h"
#include "chartcontroller.h"

extern int topWordCount = 15;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("testQmlCharts Desktop");
    QCoreApplication::setOrganizationDomain("examples.net");
    QCoreApplication::setApplicationName("testQmlCharts");

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QScopedPointer<DataBuilder> dataBuilder(new DataBuilder());
    QScopedPointer<ChartController> chartController(new ChartController());

    QObject::connect(dataBuilder.data(), &DataBuilder::updateData, chartController.data(), &ChartController::onUpdateData, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("dataBuilder", dataBuilder.data());
    engine.rootContext()->setContextProperty("chartController", chartController.data());
    engine.rootContext()->setContextProperty("topWordCount", topWordCount);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
