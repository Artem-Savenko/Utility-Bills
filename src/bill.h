#ifndef BILL_H
#define BILL_H

#include <QDate>  // dates
#include <QString> // for strings + str functions

class Bill
{
    QString mMonth;
    int mBeginOfMonth;
    int mEndOfMonth;
    float mPrice;
    float mPriceRest;       // price details
    float mPercentage;      // price details
    int mUnits;             // price details
    static QStringList mHeaderNames;

public:
    explicit Bill();

    static QStringList &headerNames();
    float price() const;
    int beginOfMonth() const;
    int endOfMonth() const;
    int totalUnits() const;
    float totalCost() const;
    void resetDetailedPrice();
    float priceRest() const;
    int units() const;
    float percentage() const;

    QString month() const;  // show string in VIEW
    QString endOfMonthStr() const;
    QString priceStr() const;
    QString totalUnitsStr() const;
    QString totalCostStr() const;

    void setPrice(QString price);  // it is providing some guards
    void setMonth(const int &month, const int &year);
    void setMonthStr(const QString &monthStr);
    void setBeginOfMonth(int beginOfMonth);
    void setEndOfMonth(int endOfMonth);
    void setPriceRest(QString priceRest);
    void setPercentage(int percentage);
    void setUnits(int units);
};

#endif // BILL_H
