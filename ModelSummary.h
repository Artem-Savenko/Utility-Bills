#ifndef MODELSUMMARY_H
#define MODELSUMMARY_H

#include <QObject>
#include <QString>
#include <QAbstractTableModel>
//#include <QDebug> // debug only
#include "Common.h"
#include "bill.h"

struct SummaryItem {
    QString billType;
    QString month;
    QString price;
    int totalUnits;
    float totalCost;

    bool isPayment;
};

class ModelSummary: public QAbstractTableModel
{
    Q_PROPERTY(QString totalCost READ totalCost NOTIFY totalCostChanged)
Q_OBJECT
    std::vector <SummaryItem> mData;

    int modelCount() const; // just to ignore warnings

public:
    ModelSummary();

    // QAbstractItemModel interface
    Q_INVOKABLE int monthNumber(const int &row) const;
    Q_INVOKABLE QString totalCost() const;
    Q_INVOKABLE void deleteAt(const int &row);
    Q_INVOKABLE void add(const int &row, const bool &isPayment);
    Q_INVOKABLE void clearModel();
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section,
                          Qt::Orientation orientation, int role) const override;

signals:
    void totalCostChanged() const;

};

#endif // MODELSUMMARY_H
