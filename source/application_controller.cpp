#include "application_controller.h"

#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QIODevice>

ApplicationController::ApplicationController()
    : m_homePath("")
    , m_maskModelPath("")
{
    if (!RequestPermissions())
    {
        qDebug() << "ApplicationController :: Permissions were not granted.";
    }

    if (!initializeHomePath())
    {
        qDebug() << "ApplicationController :: Failed to initialize home path.";
    }

    if (!initializeMaskModelPath())
    {
        qDebug() << "ApplicationController :: Failed to initialize mask model path.";
    }
}

ApplicationController::~ApplicationController()
{}

const QString& ApplicationController::shape() const
{
    return m_maskModelPath;
}

bool ApplicationController::RequestPermissions()
{
    // Qt6 Android:
    // We are using QStandardPaths::AppDataLocation, which is app-private storage.
    // Runtime READ/WRITE_EXTERNAL_STORAGE permission is not needed for this path.
    return true;
}

bool ApplicationController::initializeHomePath()
{
    m_homePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    qDebug() << "ApplicationController :: Home path:" << m_homePath;

    QDir homeDir(m_homePath);

    if (!homeDir.exists())
    {
        const bool ok = homeDir.mkpath(m_homePath);

        if (!ok)
        {
            qDebug() << "ApplicationController :: Couldn't create dir:" << m_homePath;
            return false;
        }

        qDebug() << "ApplicationController :: Created directory path:" << m_homePath;
    }

    return true;
}

bool ApplicationController::initializeMaskModelPath()
{
    const QString dlibDataPath = m_homePath + "/dlibdata/";

    QDir homeDir(m_homePath);

    if (!homeDir.exists())
    {
        qDebug() << "ApplicationController :: Couldn't find correct home path:" << m_homePath;
        return false;
    }

    const bool ok = homeDir.mkpath(dlibDataPath);

    if (!ok)
    {
        qDebug() << "ApplicationController :: Couldn't create dir:" << dlibDataPath;
        return false;
    }

    qDebug() << "ApplicationController :: Created directory path:" << dlibDataPath;

    QFile maskModelDataFile(":/mask_detect_v2.svm");

    if (!maskModelDataFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "ApplicationController :: Couldn't open file:" << maskModelDataFile.fileName();
        return false;
    }

    m_maskModelPath = dlibDataPath + "mask_detect_v2.svm";

    if (QFile::exists(m_maskModelPath))
    {
        QFile::remove(m_maskModelPath);
    }

    if (!maskModelDataFile.copy(m_maskModelPath))
    {
        qDebug() << "ApplicationController :: Couldn't copy file to dir:" << m_maskModelPath;
        return false;
    }

    qDebug() << "ApplicationController :: Managed to copy file to path:" << m_maskModelPath;

    return true;
}