#include "CmboxModel.h"
#include <QFile> // io
#include <QDir>
#include <QTextStream>
#include "Common.h"
#include "BillTypesModel.h"

#ifdef QT_DEBUG
    #include <QtDebug> // debug
#endif

CmboxModel::CmboxModel()
{
}

void CmboxModel::saveAddressesToFile()
{
    Common::saveStrListToFile(Common::getAppDir() + "/addresses", mData);
}

void CmboxModel::loadAddresses()
{   // "utilityBills" path exists for sure.
    clearModel();

    QFile file(Common::getAppDir() + "/addresses");
    if(!file.exists() || !file.size())          // PROGRAM IS "EMPTY"
    {                          // seeems like it's first launch ever
        addNew("My house");
        Common::refreshCurrentAddress("My house");

        Common::mBillTypesModel->addNew("Gas"); // DEFAULT BILL TYPE
        Common::refreshCurrentBillType(0);

        Common::mModel->addNew();   // DEFAULT MONTH

        return;
    }

    if(file.open(QFile::ReadOnly))  // file exists and everything is ok
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        while(!in.atEnd()) {
            emit beginInsertRows(QModelIndex(), mData.count(), mData.count());
            mData << in.readLine();
            emit endInsertRows();
        }
    }
    file.close();
}

QString CmboxModel::firstAddress() const
{
    if(mData.isEmpty()) {

#ifdef QT_DEBUG
    Q_ASSERT(mData.size());
#endif
        return "";
    }

    return mData.at(0);
}

bool CmboxModel::contains(QString addres) const
{
    addres = Common::delNonPathChars(addres);

    QStringList pureNames = mData;      // compare with "cleared for OS names"
    for (int i=0; i < pureNames.count(); i++)
        pureNames[i] = Common::delNonPathChars(pureNames.at(i));

    return pureNames.contains(addres);
}

void CmboxModel::addNew(const QString &newAddress)
{
    beginInsertRows(QModelIndex(), mData.size(), mData.size());
    mData.append(newAddress);
    endInsertRows();

    saveAddressesToFile();
    Common::createDir(mData.constLast());   // "utilityBills" path exists for sure.
}

int CmboxModel::deleteAt(const int &index)  // returns new Current cmbox index
{
    if(index < 0 || index >= mData.size())
        return index;

    Common::deleteDir(Common::getAppDir() + '/'
                      + Common::delNonPathChars(mData.at(index)));

    int newCurrIndex = 0;
    beginRemoveRows(QModelIndex(), index, index);
    if(!mData.value(index -1).isEmpty())  // prev address exists
        newCurrIndex = index -1;
    mData.removeAt(index);
    endRemoveRows();

    saveAddressesToFile();

    return newCurrIndex;
}

void CmboxModel::rename(const QString &oldAddress, const QString &newAddress)
{
    int foundIdx = mData.indexOf(oldAddress);
    if(foundIdx == -1)
        return;

    mData[foundIdx] = newAddress;
    emit dataChanged(index(foundIdx, 0), index(foundIdx,0));

    QString oldPath = Common::getAppDir() + '/'
            + Common::delNonPathChars(oldAddress);
    QString newPath = Common::getAppDir() + '/'
            + Common::delNonPathChars(newAddress);

    Common::renameDir(oldPath, newPath);
    saveAddressesToFile();

    Common::refreshCurrentAddress(newAddress);
}


int CmboxModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mData.size();
}

int CmboxModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant CmboxModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != AddressRole
       || index.row() < 0 || index.row() >= mData.count())
        return QVariant();

    return mData.at(index.row());
}


QHash<int, QByteArray> CmboxModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AddressRole] = "address";

    return roles;
}

void CmboxModel::clearModel()
{
    if(mData.count() <= 0)
        return;

    emit beginRemoveRows(QModelIndex(), 0, mData.count() -1);
    mData.clear();
    emit endRemoveRows();
}



