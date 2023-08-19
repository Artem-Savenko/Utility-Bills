#ifndef CMBOXMODEL_H
#define CMBOXMODEL_H

#include <QAbstractTableModel>
#include <QObject>

class CmboxModel : public QAbstractTableModel
{
Q_OBJECT
    QStringList mData;
    const int AddressRole = Qt::UserRole;

    void saveAddressesToFile();
    void clearModel();

public:
    CmboxModel();

    void loadAddresses();
    QString firstAddress() const;

    Q_INVOKABLE bool contains(QString addres) const;
    Q_INVOKABLE void addNew(const QString &newAddress);
    Q_INVOKABLE int deleteAt(const int &index);    // returns new Current cmbox index
    Q_INVOKABLE void rename(const QString &oldAddress, const QString &newAddress);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif // CMBOXMODEL_H
