#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QString>
#include <QAbstractTableModel>
#include <QStandardPaths>   // path to appdir
#include "bill.h"
//#include <QDebug> // debug only

class Model : public QAbstractTableModel
{
Q_OBJECT
    struct UndoRedoItem {
        enum Type { Edited, Deleted, Added, AllItemsCleared };

        Bill month;
        Type editType; // was it deleted/edited/added?
    };

    std::vector <Bill> mData;
    std::vector <UndoRedoItem> mUndoBuffer;
    std::vector <UndoRedoItem> mRedoBuffer;

    int modelCount() const; // just to ignore warnings
    void clearModel();
        // (PASS DIRTY NAMES)
    void saveData(const QString &address, const QString &billType);
    void addUndo(const Bill &month, UndoRedoItem::Type actionType);
    void addRedo(const UndoRedoItem &undoItem);

protected:
    enum AvtemRoleColumns {
        Month = 0,
        Beginning,
        End,
        Price,
        TotalUnits,
        TotalCost,
    };
    enum AvtemRoles {
        HeaderName = Qt::UserRole
    };

public:
    Model();

    Bill getAt(const int &row) const;
    Q_INVOKABLE void addNew();
    Q_INVOKABLE void deleteAt(const int &indexRow);
    Q_INVOKABLE void loadData(const QString &address, const QString &billType);
        // PASS DIRTY NAMES
    Q_INVOKABLE bool setDataAt(const int &row, const int &column,
                               const QVariant &value);
    Q_INVOKABLE void repeatPrevPrice(const int &indexRow);
        // copies price to indexRow if prev row has price set
    Q_INVOKABLE QString toolTipFor(const int &row) const;
    Q_INVOKABLE QList <QVariant> getDetailedPriceInfo(const int &indexRow) const;
    Q_INVOKABLE bool hasDetailedPrice(const int &row) const;
    Q_INVOKABLE int monthNumber(const int &row) const;


    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;    
    virtual QHash<int, QByteArray> roleNames() const override;    
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

signals:
    void undoRedoBufferChanged() const;
};



#endif // MODEL_H
