#include "ModelSummary.h"
#include <QtDebug>

ModelSummary::ModelSummary()
{

}

int ModelSummary::monthNumber(const int& row) const
{
    if(row < 0 || row >= modelCount())
        return -1;

    int pos = mData.at(uint(row)).month.indexOf(QRegExp("\\d"));
    if(pos > -1)
        return mData.at(uint(row)).month.mid(pos, 2).toInt() -1;

    return -1;
}

QString ModelSummary::totalCost() const
{
    double total = 0;
    for(uint i=0; i < mData.size(); i++)
        total += mData.at(i).totalCost;

    return QString::number(total, 'f', 1);
}

void ModelSummary::deleteAt(const int &row)
{
    if(row < 0 || row >= modelCount())
        return;

    emit beginRemoveRows(QModelIndex(), row, row);
    mData.erase(mData.begin() + row);
    emit endRemoveRows();
    emit totalCostChanged();
}

void ModelSummary::add(const int &row, const bool &isPayment)
{
    if(row < 0)
        return;

    emit beginInsertRows(QModelIndex(), modelCount(), modelCount());
    Bill modelItem = Common::mModel->getAt(row);
    SummaryItem item;
    if(isPayment)
       item = {Common::currBillType(), modelItem.month(),
         "-", 0, float(modelItem.beginOfMonth())
              , isPayment};
    else
       item = {Common::currBillType(), modelItem.month(),
         modelItem.priceStr(), modelItem.totalUnits(), modelItem.totalCost()
              , isPayment};

    mData.push_back(item);
    emit endInsertRows();
}

void ModelSummary::clearModel()
{
    if(!modelCount())
        return;

    emit beginRemoveRows(QModelIndex(), 0, modelCount() -1);
    mData.clear();
    emit endRemoveRows();
    emit totalCostChanged();
}


int ModelSummary::modelCount() const
{
    return static_cast<int>(mData.size());
}

int ModelSummary::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return modelCount();
}

int ModelSummary::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;  // because 5.
}



QVariant ModelSummary::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= modelCount())
        return {};

    uint row = static_cast<uint>(index.row());
    emit totalCostChanged();

    QList <QString> colors {"#08979c", "#006d75", // winter
                "#a0d911", "#7cb305","#5b8c00",  //
                "#fadb14", "#d4b106", "#ad8b00",   // summer
                "#faad14", "#d48806", "#ad6800",   // fall
                "#13c2c2" };          // winter
    QString paymentMonthColor = "white";

    if(role == Qt::DecorationRole) { // colors of the TableView
        if(mData.at(row).isPayment)
            return paymentMonthColor;
        return colors[monthNumber(int(row))];
    }

    switch (index.column()) {
    case 0:
        return mData.at(row).billType;
    case 1:
        return mData.at(row).month;
    case 2:
        if(mData.at(row).isPayment)
            return "";
        return mData.at(row).price;
    case 3:
        if(mData.at(row).isPayment)
            return "";
        return mData.at(row).totalUnits;
    case 4:
        return QString::number(static_cast<double>(mData.at(row).totalCost),
                                                                'f', 2);
    }

    return QVariant();
}


QVariant ModelSummary::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
    case 0:
            return "Bill type";
    case 1:
        return "Month";
    case 2:
        return "Price";
    case 3:
        return "Total units";
    case 4:
        return "Total cost";
    }

    return {};
}
