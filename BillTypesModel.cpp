#include "BillTypesModel.h"
#include <QFile> // io
#include <QTextStream>
#include <QDebug>

BillTypesModel::BillTypesModel(QAbstractListModel *parent)
    : QAbstractListModel(parent)
{
}

QString BillTypesModel::currBillTypesDir() const
{
    return (Common::getAppDir() + '/'         // PASSING CLEAN NAME
            + Common::delNonPathChars(Common::currAddress()));
}

void BillTypesModel::createDefaultBillTypes()
{
    Common::createDir(Common::delNonPathChars(Common::currAddress()),
                      "utilitybill");  // DEFAULT BILL TYPE VALUE

    clearModel();
    emit beginInsertRows(QModelIndex(), 0, 0);
    mData << "Utility bill";
    emit endInsertRows();

    saveBillTypesToFile();
}

QString BillTypesModel::billTypeAt(const int &index) const
{
    if(index < 0 || index >= mData.count()) {
//        qWarning () << "Bill type model is empty";
        return QString();
    }

    return mData.at(index);
}



void BillTypesModel::loadBillTypes()
{
    clearModel();

    QFile file(currBillTypesDir() + "/billTypes");
    if(!file.exists() || !file.size()) {
        #ifdef QT_DEBUG
            qCritical() << "FAIL: billTypes.load(). currBillTypesDir = "
                        << currBillTypesDir();
        #endif
        return;
    }

    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        while(!in.atEnd()) {
            emit beginInsertRows(QModelIndex(), mData.count(), mData.count());
            mData << in.readLine();
            emit endInsertRows();
        }

        file.close();
        return;  // SUCCESS
    }
}

bool BillTypesModel::contains(QString billType) const
{
    billType = Common::delNonPathChars(billType);

    QStringList pureNames = mData;      // compare with "cleared for OS names"
    for (int i=0; i < pureNames.count(); i++)
        pureNames[i] = Common::delNonPathChars(pureNames.at(i));

    return pureNames.contains(billType);
}

void BillTypesModel::addNew(const QString &newBillType)
{
    beginInsertRows(QModelIndex(), mData.size(), mData.size());
    mData.append(newBillType);
    endInsertRows();

    saveBillTypesToFile();
    Common::createDir(Common::delNonPathChars(Common::currAddress()),
                      Common::delNonPathChars(mData.constLast()));
}

int BillTypesModel::deleteBT(const QString &billType)
{
    int index = mData.indexOf(billType);
    if(index == -1)
        return -1;  // isitsafe?

    Common::deleteDir(currBillTypesDir()
                      + '/' + Common::delNonPathChars(mData.at(index)));

    beginRemoveRows(QModelIndex(), index, index);
//    if(!mData.value(index -1).isEmpty())  // prev address exists
//        newCurrIndex = index -1;
    mData.removeAt(index);
    endRemoveRows();

    saveBillTypesToFile();

    return 0;
}

void BillTypesModel::rename(const QString &oldBillType, const QString &newBillType)
{
    int foundIdx = mData.indexOf(oldBillType);
    if(foundIdx == -1) {
//        qWarning() << "Couldn't rename \"" + oldBillType + "\"" << " in mData";
        return;
    }

    mData[foundIdx] = newBillType;
    emit dataChanged(index(foundIdx, 0), index(foundIdx,0));

    QString oldPath = currBillTypesDir()
            + '/' + Common::delNonPathChars(oldBillType);
    QString newPath = currBillTypesDir()
            + '/' + Common::delNonPathChars(newBillType);

    Common::renameDir(oldPath, newPath);

    saveBillTypesToFile();
}


void BillTypesModel::saveBillTypesToFile()
{
    if(currBillTypesDir().isEmpty())
    {
        #ifdef QT_DEBUG
            qDebug() << Common::currAddress() << " - tried to write empty billType";
        #endif

        return;
    }

    Common::saveStrListToFile(currBillTypesDir() + "/billTypes", mData);
}

void BillTypesModel::clearModel()
{
    if(mData.count() <= 0)
        return;

    emit beginRemoveRows(QModelIndex(), 0, mData.count() -1);
    mData.clear();
    emit endRemoveRows();
}

int BillTypesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mData.size();
}

QVariant BillTypesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != BillTypeRole
       || index.row() < 0 || index.row() >= mData.count())
        return QVariant();

    return mData.at(index.row());
}

QHash<int, QByteArray> BillTypesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[BillTypeRole] = "billType";

    return roles;
}



