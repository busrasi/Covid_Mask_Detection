#include "application_controller.h"

#include <QString>
#include <QtAndroid>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

ApplicationController::ApplicationController()
    : m_homePath("")
    , m_maskModelPath("")
{
    if(!RequestPermissions())
    {
        // TODO cut application here or request again!
    }

    if(!initializeHomePath())
    {
        // TODO cut application here and fix home path!
    }

    if(!initializeMaskModelPath())
    {
        // TODO cut application here and fix mask base path!
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
    // READ EXTERNAL DEVICES
    auto  readResult = QtAndroid::checkPermission(QString("android.permission.READ_EXTERNAL_STORAGE"));
    if(readResult == QtAndroid::PermissionResult::Denied)
    {
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.READ_EXTERNAL_STORAGE"}));
        if(resultHash["android.permission.READ_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied)
        {
            return false;
        }
    }

    // WRITE EXTERNAL DEVICES
    auto  writeResult = QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE"));
    if(writeResult == QtAndroid::PermissionResult::Denied)
    {
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.WRITE_EXTERNAL_STORAGE"}));
        if(resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied)
        {
            return false;
        }
    }

    return true;
}

bool ApplicationController::initializeHomePath()
{
    // Android: HomeLocation works, iOS: not writable
    // Android: AppDataLocation works out of the box, iOS you must create the DIR first !!
    m_homePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    //m_homePath = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);

    qDebug() << "ApplicationController :: Home path: " << m_homePath;
    QDir homeDir(m_homePath);
    if (!homeDir.exists())
    {
        bool ok = homeDir.mkpath(m_homePath);
        if(!ok)
        {
            qDebug() << "ApplicationController :: Couldn't create dir: " << m_homePath;
            return false;
        }
        qDebug() << "ApplicationController :: Created directory path: " << m_homePath;
    }

    return true;
}

bool ApplicationController::initializeMaskModelPath()
{
    // Check and create to the writable path
    QString dlibDataPath = m_homePath + "/dlibdata/";
    QDir homeDir(m_homePath);
    if (!homeDir.exists())
    {
        qDebug() << "ApplicationController :: Couldn't find correct home path: " << m_homePath;
        return false;
    }
    else
    {
        bool ok = homeDir.mkpath(dlibDataPath);
        if(!ok)
        {
            qDebug() << "ApplicationController :: Couldn't create dir: " << dlibDataPath;
            return false;
        }
        qDebug() << "ApplicationController :: Created directory path: " << dlibDataPath;
    }

    // liveness model data
    QFile maskModelDataFile(":/mask_detect_v2.svm");
    if (!maskModelDataFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "ApplicationController :: Couldn't open file: " << maskModelDataFile.fileName();
        return false;
    }


    m_maskModelPath = dlibDataPath + "mask_detect_v2.svm";

    if(!maskModelDataFile.copy(m_maskModelPath))
    {
        qDebug() << "ApplicationController :: Couldn't copy file to dir: " << m_maskModelPath;
        return false;
    }
    else {
        qDebug() << "Managed to copy file to path" << m_maskModelPath;
    }

    return true;
}
