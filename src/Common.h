#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QStandardPaths>
#include "Model.h"
#include "CmboxModel.h"
#include <QQmlEngine>
#include <QPoint>

class BillTypesModel;

class Common : public QObject
{
Q_OBJECT
    Common() {}  // private constructor
    Common(Common &other) = delete;
    void operator=(const Common& other) = delete;

    static QString mCurrAddress;    // DIRTY NAME
    static QString mCurrBillType;   // DIRTY NAME

public:
    static QObject *singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

    static Model *mModel;
    static CmboxModel *mCmboxModel;
    static BillTypesModel *mBillTypesModel;

    Q_INVOKABLE static QString delNonPathChars(QString path);
        // deletes "[\\\\ \\/ \\* : \\? < > \" \\| \\.]"
    Q_INVOKABLE static QPoint toMonthYear(const QString &dateStr);
        // converts "[12.2019] December" to 12, 2019
    Q_INVOKABLE static void refreshCurrentAddress(const QString &currAddress);
        // QML calls this when it changes
    Q_INVOKABLE static void refreshCurrentBillType(const int &index);
        // QML calls this when it changes
    Q_INVOKABLE static void createAddress(const QString &newAddress);
        // and it creates default bill type, data..
    Q_INVOKABLE QList <int> loadWindowState();
    Q_INVOKABLE static void saveWindowState(int x, int y, int width,
                                            int height, int visibility);
    static QString getAppDir();
        // %appdata%/utilityBills
    static void setCmboxModel(CmboxModel *value);
    static void setBillTypesModel(BillTypesModel *value);
    static void setModel(Model *value);

    static void createDir(const QString &address, const QString &billType = "");
        // PASS DIRTY NAME
    static bool renameDir(const QString &oldPath, const QString &newPath);
        // PASS CLEAN PATHS
    static void deleteDir(const QString &dirPath);
        // PASS CLEAN NAME

    static void loadEverything();
        // load addresses, billTypes, data
    static void saveStrListToFile(const QString &filePath, QStringList list);
        // saves list to the file  // SAFE filePATH ONLY!
    static QString currAddress();
        // RETURNS DIRTY ADDRESS
    static QString currBillType();
        // RETURNS DIRTY BILL TYPE
};

#endif // COMMON_H
