#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QVariant>
#include <QVideoSink>
#include <QDebug>
#include <QMetaType>

#include <opencv2/opencv.hpp>

#include "source/application_controller.h"
#include "source/camera/mask_detection_filter.h"
#include "source/core/mask_detection.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qRegisterMetaType<cv::Mat>("cv::Mat");

    QQmlApplicationEngine engine;

    ApplicationController appController;

    MaskDetection maskDetection(appController.shape());

    MaskDetectionFilter maskDetectionFilter(&maskDetection);

    engine.rootContext()->setContextProperty("maskDetectionFilter", &maskDetectionFilter);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [&app, url, &maskDetectionFilter](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                qWarning() << "main.cpp :: Failed to load QML.";
                QCoreApplication::exit(-1);
                return;
            }

            if (url != objUrl || !obj) {
                return;
            }

            QObject *videoOutput = obj->findChild<QObject *>("videoOutput");

            if (!videoOutput) {
                qWarning() << "main.cpp :: Could not find VideoOutput by objectName.";
                return;
            }

            QVariant sinkVariant = videoOutput->property("videoSink");

            if (!sinkVariant.isValid()) {
                qWarning() << "main.cpp :: VideoOutput has no valid videoSink property.";
                return;
            }

            QVideoSink *videoSink = sinkVariant.value<QVideoSink *>();

            if (!videoSink) {
                qWarning() << "main.cpp :: Could not get QVideoSink from VideoOutput.";
                return;
            }

            QObject::connect(
                videoSink,
                &QVideoSink::videoFrameChanged,
                &maskDetectionFilter,
                &MaskDetectionFilter::processFrame,
                Qt::QueuedConnection
                );

            qDebug() << "main.cpp :: QVideoSink connected to MaskDetectionFilter.";
        },
        Qt::QueuedConnection
        );

    engine.load(url);

    return app.exec();
}