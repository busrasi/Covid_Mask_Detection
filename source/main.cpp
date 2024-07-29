#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "source/application_controller.h"
#include "source/camera/mask_detection_filter.h"
#include "source/core/mask_detection.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Application Controller
    ApplicationController appController;

    // Create Mask Detection
    MaskDetection maskDetection(appController.shape().toStdString());

    // Create Mask Detection Filter
    MaskDetectionFilter maskDetectionFilter(&maskDetection);

    // Set object to qml context
    engine.rootContext()->setContextProperty("maskDetectionFilter", &maskDetectionFilter);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
