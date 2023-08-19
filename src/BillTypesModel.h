#ifndef BILLTYPESMODEL_H
#define BILLTYPESMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "Common.h"

class BillTypesModel : public QAbstractListModel
{
Q_OBJECT
    QStringList mData;
    const int BillTypeRole = Qt::UserRole;

    QString currBillTypesDir() const;
    void saveBillTypesToFile();

public:
    BillTypesModel(QAbstractListModel *parent = nullptr);

    void createDefaultBillTypes();

    Q_INVOKABLE QString billTypeAt(const int &index) const;
    Q_INVOKABLE void loadBillTypes();
    Q_INVOKABLE bool contains(QString billType) const;
    Q_INVOKABLE void addNew(const QString &newBillType);
    Q_INVOKABLE int deleteBT(const QString &billType);    // returns new Current cmbox index
    Q_INVOKABLE void rename(const QString &oldBillType, const QString &newBillType);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void clearModel();
};

#endif // BILLTYPESMODEL_H
