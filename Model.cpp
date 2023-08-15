#include "Model.h"
#include <QDir>  // for creating folders
#include <QTextStream> // io
#include "Common.h"

#ifdef QT_DEBUG
    #include <QDebug>
#endif

const QChar separator = 0xF00F;
#define tostr(x) QString::number(x)
#define tostrF(x) QString::number(static_cast<double>(x), 'f', 2)

Model::Model()
{
}

Bill Model::getAt(const int &row) const
{
    if(row < 0 || row >= modelCount())
        return Bill{};

    return mData.at(static_cast<uint>(row));
}

int Model::modelCount() const
{
    return static_cast<int>(mData.size());
}

void Model::clearModel()
{
    if(modelCount() <= 0)
        return;

    emit beginRemoveRows(QModelIndex(), 0, modelCount() -1);
    mData.clear();
    emit endRemoveRows();
}

void Model::addNew()
{
    emit beginInsertRows(QModelIndex(), 0, 0);
    mData.insert(mData.begin(), Bill());
    emit endInsertRows();
//    addUndoRedo(mData.back(), UndoRedoItem::Type::Added);

    saveData(Common::currAddress(), Common::currBillType());
}

void Model::deleteAt(const int &indexRow)
{
    if(indexRow < 0 || indexRow >= modelCount())
        return;

    emit beginRemoveRows(QModelIndex(), indexRow, indexRow);
    mData.erase(mData.begin() + indexRow);
    emit endRemoveRows();

    saveData(Common::currAddress(), Common::currBillType());
}

void Model::loadData(const QString &address, const QString &billType)
// PASS DIRTY NAMES
{
    clearModel();
    mUndoBuffer.clear(); // new bill type is loading. Wipe all data
    emit undoRedoBufferChanged();

    QString dirPath = Common::getAppDir() + '/'+ Common::delNonPathChars(address)
            + '/' + Common::delNonPathChars(billType);
    QFile file(dirPath + "/data");
    if(!file.exists() || !file.size())
        return;

    emit beginResetModel();
    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");

        uint i=0;
        while(!in.atEnd()) {
            QStringList monthInfo = in.readLine().split(separator);
            mData.push_back(Bill());
            mData.at(i).setMonthStr(monthInfo.at(0));
            mData.at(i).setBeginOfMonth(monthInfo.at(1).toInt());
            mData.at(i).setEndOfMonth(monthInfo.at(2).toInt());
            mData.at(i).setPrice(monthInfo.at(3));
            if(monthInfo.count() >= 7) {
                mData.at(i).setPriceRest(monthInfo.at(4));
                mData.at(i).setPercentage(monthInfo.at(5).toInt());
                mData.at(i).setUnits(monthInfo.at(6).toInt());
            }
            i++;
        }

        file.close();
    }

    emit endResetModel();
}

void Model::saveData(const QString &address, const QString &billType)
// PASS DIRTY NAMES
{
    if(address.isEmpty() || billType.isEmpty()) {
        #ifdef QT_DEBUG
            qCritical() << "FAIL: Model.saveData()" << address << billType;
        #endif
        return;
    }

    QString dirPath = Common::getAppDir() + '/'+ Common::delNonPathChars(address)
            + '/' + Common::delNonPathChars(billType);
    QFile file(dirPath + "/data");

    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    out.setCodec("UTF-8");

    for(uint i=0; i < mData.size(); i++) {
        QString monthInfo = mData.at(i).month();
        monthInfo += separator + QString::number(mData.at(i).beginOfMonth());
        monthInfo += separator + QString::number(mData.at(i).endOfMonth());
        monthInfo += separator + QString::number(mData.at(i).price());
        monthInfo += separator + QString::number(mData.at(i).priceRest(), 'f');
        monthInfo += separator + QString::number(mData.at(i).percentage());
        monthInfo += separator + QString::number(mData.at(i).units());

        out << monthInfo << '\n';
    }

    file.close();
}

void Model::addUndo(const Bill &month, Model::UndoRedoItem::Type actionType)
{
    // prevent groving undo/redo vector
    if(mUndoBuffer.size() > 70
       && actionType != Model::UndoRedoItem::AllItemsCleared)
        mUndoBuffer.erase(mUndoBuffer.begin());

    mUndoBuffer.push_back(UndoRedoItem{month, actionType});
    emit undoRedoBufferChanged();
}

void Model::addRedo(const UndoRedoItem &undoItem)
{
    mRedoBuffer.push_back(undoItem);
    emit undoRedoBufferChanged();
}


int Model::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(mData.size());
}

int Model::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;  // because we need 6 columns.
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)
    uint indexRow = static_cast<uint>(index.row());

    if(!index.isValid() || index.row() < 0 || indexRow >= mData.size())
        return QVariant ();


    QList <QString> colors {"#08979c", "#006d75", // winter
                "#a0d911", "#7cb305","#5b8c00",  //
                "#fadb14", "#d4b106", "#ad8b00",   // summer
                "#faad14", "#d48806", "#ad6800",   // fall
                "#13c2c2" };          // winter

    if(role == Qt::DecorationRole)
        return colors[monthNumber(index.row())];

    if(index.column() == Month)
        return mData.at(indexRow).month();
    if(index.column() == Beginning)
        return mData.at(indexRow).beginOfMonth();
    if(index.column() == End)
        return mData.at(indexRow).endOfMonthStr();
    if(index.column() == Price)
        return mData.at(indexRow).priceStr();
    if(index.column() == TotalUnits)
        return mData.at(indexRow).totalUnitsStr();
    if(index.column() == TotalCost)
        return mData.at(indexRow).totalCostStr();

    return "";
}


QHash<int, QByteArray> Model::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Qt::DecorationRole] = "decorationRole";
    roles[HeaderName] = "headerName";

    return roles;
}

bool Model::setDataAt(const int &row, const int &column, const QVariant &value)
{
    if(row < 0 || row >= modelCount()
      || column < 0 || column > 3)
        return false;

    uint rowUint = static_cast<uint>(row);

    switch(column) {
    case Month:
        mData.at(rowUint).setMonth(value.toStringList().value(0).toInt(),
                         value.toStringList().value(1).toInt());
    break;
    case Beginning:
        mData.at(rowUint).setBeginOfMonth(value.toInt());
        if(row +1 < modelCount() && value.toInt()) {
            mData.at(rowUint +1).setEndOfMonth(value.toInt());
            emit dataChanged(index(row +1, 0),
                             index(row +1, columnCount(QModelIndex()) -1));
        }
    break;
    case End:
        mData.at(rowUint).setEndOfMonth(value.toInt());
    break;
    case Price:
    {
        QStringList listInfo = value.toStringList();
        if(listInfo.length() == 4) {
            mData.at(rowUint).resetDetailedPrice();
            mData.at(rowUint).setPrice(listInfo[2]);
            mData.at(rowUint).setPriceRest(listInfo[3]);

            if(listInfo.at(1) == "%")
                mData.at(rowUint).setPercentage(listInfo[0].toInt());
            else
                mData.at(rowUint).setUnits(listInfo[0].toInt());
        }
        else {
            mData.at(rowUint).resetDetailedPrice();
            mData.at(rowUint).setPrice(value.toString());
        }
    }
    break;

    default:    // total units + cost are not editable!
        return false;
//        qWarning() << "what did try to set data??" ;
    }

    emit dataChanged(index(row, 0), index(row, columnCount(QModelIndex()) -1),
    { Qt::DisplayRole, Qt::DecorationRole });
    saveData(Common::currAddress(), Common::currBillType());
    return true;
}

void Model::repeatPrevPrice(const int &indexRow)
{
    if(indexRow == -1 || indexRow +1 >= modelCount())
        return;

    uint rowUint = static_cast<uint>(indexRow);

    mData.at(rowUint).setPriceRest
            (QString::number(mData.at(rowUint +1).priceRest()));
    mData.at(rowUint).setPercentage(mData.at(rowUint +1).percentage());
    mData.at(rowUint).setUnits(mData.at(rowUint +1).units());
    mData.at(rowUint).setPrice(QString::number(mData.at(rowUint +1).price(),
                                                            'f', 2));
    emit dataChanged(index(indexRow, 0), index(indexRow +1, 5));
    saveData(Common::currAddress(), Common::currBillType());
}

QString Model::toolTipFor(const int &row) const
{
    if(row < 0 || row >= modelCount())
        return  "";

    uint rowUint = static_cast<uint>(row);
    QString str = "First ";
    if(mData.at(rowUint).units())
        str += tostr(mData.at(rowUint).units()) + " units";
    else
        str += tostr(static_cast<int>(mData.at(rowUint).percentage())) + "%";

    str += " price: " + tostrF(mData.at(rowUint).price()) + '\n';
    str += "Rest price: " + tostrF(mData.at(rowUint).priceRest());

    return str;
}

QList<QVariant> Model::getDetailedPriceInfo(const int &indexRow) const
{
    if(indexRow < 0 || indexRow >= modelCount())
        return QList<QVariant>();

    QList<QVariant> list;
    uint row = static_cast<uint>(indexRow);  // casting....
    list << mData.at(row).units() << mData.at(row).percentage()
     << QString::number(static_cast<double>(mData.at(row).price()), 'f', 2)
     << QString::number(static_cast<double>(mData.at(row).priceRest()), 'f', 2);
    return list;
}

bool Model::hasDetailedPrice(const int &row) const
{
    if(row < 0 || row >= modelCount())
        return false;

    return (mData.at(static_cast<uint>(row)).units()
            || mData.at(static_cast<uint>(row)).percentage());
}

int Model::monthNumber(const int &row) const
{
    if(row < 0 || row >= modelCount())
        return -1;

    int pos = mData.at(row).month().indexOf(QRegExp("\\d"));
    if(pos > -1)
        return mData.at(row).month().mid(pos, 2).toInt() -1;

    return -1;
}

QVariant Model::headerData(int section, Qt::Orientation orientation,
                           int role) const
{
    if(section < 0 || section >= columnCount(QModelIndex())
       || role != Qt::DisplayRole || orientation != Qt::Orientation::Horizontal)
        return QVariant();

    return Bill::headerNames().at(section);
}
