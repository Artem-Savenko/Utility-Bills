#include "Common.h"
#include <QDir> // for dirs
#include <QTextStream> // io
#include <QSettings> // saving user settings
#include "BillTypesModel.h"

#ifdef QT_DEBUG
    #include <QDebug>
#endif

QString Common::mCurrAddress = "";
QString Common::mCurrBillType = "";
CmboxModel * Common::mCmboxModel = nullptr;
BillTypesModel * Common::mBillTypesModel = nullptr;
Model * Common::mModel = nullptr;

QString Common::currAddress()
{
    return mCurrAddress;
}

QString Common::currBillType()
{
    return mCurrBillType;
}

void Common::setCmboxModel(CmboxModel *value)
{
    mCmboxModel = value;
}

void Common::setBillTypesModel(BillTypesModel *value)
{
    mBillTypesModel = value;
}

void Common::setModel(Model *value)
{
    mModel = value;
}

QObject *Common::singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Common *singletonClass = new Common();
    return singletonClass;
}

void Common::loadEverything()  // load addresses, billTypes, data
{
    QDir appDir(getAppDir()); // if app folder in Roaming doesn't exist - create
    if(!appDir.exists())
        appDir.mkpath(appDir.absolutePath());

    mCmboxModel->loadAddresses();
    mCurrAddress = mCmboxModel->firstAddress();

    mBillTypesModel->loadBillTypes();
    mCurrBillType = mBillTypesModel->billTypeAt(0);
}

void Common::createAddress(const QString &newAddress)
{
    mCmboxModel->addNew(newAddress);
    mBillTypesModel->createDefaultBillTypes();
}

QString Common::delNonPathChars(QString path)
{
    path = path.toLower();
    return path.remove(QRegExp("[\\\\ \\/ \\* : \\? < > \" \\| \\.]"));
}

QPoint Common::toMonthYear(const QString &dateStr)
{
    int xMonth, yYear;
    int idxMonth, idxYear;
    idxMonth = dateStr.indexOf(QRegExp("[01][0-9]\\D"));
    idxYear = dateStr.indexOf(QRegExp("\\d{4}"));
    xMonth = dateStr.midRef(idxMonth, 2).toInt();
    yYear = dateStr.midRef(idxYear, 4).toInt();

    QPoint af(xMonth, yYear);
    return af;
}

void Common::refreshCurrentAddress(const QString &currAddress)
// gets current Address from QML
{
    mCurrAddress = currAddress;
}

void Common::refreshCurrentBillType(const int &index)
{
    if(index < 0 || index >= mBillTypesModel->rowCount(QModelIndex()))
        return;

    mCurrBillType = mBillTypesModel->billTypeAt(index);
}

void Common::createDir(const QString &address, const QString &billType)
{
    QString safeAddresPath = delNonPathChars(address);
    QString safeBillType = delNonPathChars(billType);

    QString path = getAppDir() + '/' + safeAddresPath;
    if(!billType.isEmpty())
        path += '/' + billType;

    QDir dir;
    dir.mkpath(path);
}

bool Common::renameDir(const QString &oldPath, const QString &newPath)
{
    bool result = true;

    QDir dir(oldPath);
    if(oldPath == newPath)
        result = true;    // oldPath == newPath already. Desired result reached.
    else if(!dir.rename(dir.path(), newPath))
        result = false;

    #ifdef QT_DEBUG
        if(!result) qInfo() << "FAIL: renameDir - " << dir.path() << " to:"
                            << newPath;
    #endif
    return result;    // dir successfully renamed
}

void Common::deleteDir(const QString &dirPath)
{
    QDir dir(dirPath);
    dir.removeRecursively();
}

QList <int> Common::loadWindowState()
{
    QSettings settings(getAppDir() + "/userSettings.ini",
                       QSettings::IniFormat);

    int x = settings.value("mainWnd_x", 200).toInt();
    int y = settings.value("mainWnd_y", 200).toInt();
    int width = settings.value("mainWnd_width", 640).toInt();
    int heigth = settings.value("mainWnd_height", 480).toInt();
                /*QWindow::AutomaticVisibility*/
    int visibility = settings.value("mainWnd_visibility", 1).toInt();

    return QList<int>() << x << y << width << heigth << visibility;
}

void Common::saveWindowState(int x, int y, int width, int height,
                             int visibility)
{
    QSettings settings(getAppDir() + "/userSettings.ini",
                       QSettings::IniFormat);

    settings.setValue("mainWnd_x", x);
    settings.setValue("mainWnd_y", y);
    settings.setValue("mainWnd_width", width);
    settings.setValue("mainWnd_height", height);
    settings.setValue("mainWnd_visibility", visibility);
}

void Common::saveStrListToFile(const QString &filePath, QStringList list)
{   // SAFE filePATH ONLY!
#ifdef QT_DEBUG
    ;//qDebug() << "saved string list to: " << filePath;
#endif

    QFile file(filePath);    // save names(addr,billtypes) with specChars
    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    for(int i=0; i < list.size(); i++)
        out << list.at(i) << '\n';

    file.close();
}

QString Common::getAppDir()    // already with app name!
{
#ifdef QT_DEBUG
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
            + "TEST";
#endif

    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}
