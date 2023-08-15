#include "bill.h"
//#include <QDebug>  // debug only

QStringList Bill::mHeaderNames = { "Month", "Beginning of the month",
                                 "End of the month", "Price", "Total (units)",
                                 "Total (cost)" };

float Bill::priceRest() const
{
    return mPriceRest;
}

void Bill::setPriceRest(QString priceRest)
{
    priceRest.replace(",", "."); // delete spaces

    mPriceRest = priceRest.toFloat();
}

float Bill::percentage() const
{
    return mPercentage;
}



void Bill::setPercentage(int percentage)
{
    mPercentage = percentage;
}

int Bill::units() const
{
    return mUnits;
}

void Bill::setUnits(int units)
{
    mUnits = units;
}

Bill::Bill()
    : mBeginOfMonth(0),
      mEndOfMonth(0),
      mPrice(0),
      mPriceRest(0),
      mPercentage(0),
      mUnits(0)
{
    QDate today = QDate::currentDate();
    setMonth(today.month(), today.year());
}

QString Bill::month() const
{
    return mMonth;
}

QString Bill::endOfMonthStr() const
{
    if(mEndOfMonth == 0)
        return "-";

    return QString::number(mEndOfMonth);
}

void Bill::setMonth(const int &month, const int &year)
{
    QDate newMonth(year, month, 1);
    mMonth = newMonth.toString(" [MM.yyyy]    MMMM");
}

void Bill::setMonthStr(const QString &monthStr)
{
    mMonth = monthStr;
}

int Bill::beginOfMonth() const
{
    return mBeginOfMonth;
}

void Bill::setBeginOfMonth(int beginOfMonth)
{
    mBeginOfMonth = beginOfMonth;
}

int Bill::endOfMonth() const
{
    return mEndOfMonth;
}

void Bill::setEndOfMonth(int endOfMonth)
{
    mEndOfMonth = endOfMonth;
}



int Bill::totalUnits() const
{
    if(!mEndOfMonth)    // end of the month hasn't been set. Don't count units
        return 0;

    return mEndOfMonth - mBeginOfMonth;
}

float Bill::totalCost() const
{
    if(mUnits || mPercentage) // it uses detailed price!
    {
        float firstPart = 0;
        if(mUnits) {
            if(totalUnits() <= mUnits)  // less than 100
                return totalUnits() *mPrice;
            else {
                firstPart = mUnits *mPrice;
                return firstPart + (totalUnits() - mUnits) *mPriceRest;
            }
        }
        else {
            firstPart = (totalUnits() *(mPercentage/100)) *mPrice;
            float percentageRest = (100 - mPercentage) /100;
            return firstPart + (totalUnits() *percentageRest) *mPriceRest;
        }
    }

    return totalUnits() *mPrice;
}

void Bill::resetDetailedPrice()
{
    mPercentage = 0;
    mPriceRest = 0;
    mUnits = 0;
}

QString Bill::totalCostStr() const
{
    if((mPrice == 0.0f && mPriceRest == 0.0f) || mEndOfMonth == 0)
        return "-";

    return QString::number(static_cast<double>(totalCost()), 'f', 1);
}

QStringList &Bill::headerNames()
{
    return mHeaderNames;
}

float Bill::price() const
{
    return mPrice;
}

QString Bill::priceStr() const
{
    if(mPrice == 0.0f && (!mUnits && !mPercentage))
        return "-";

    QString tempStr = QString::number(static_cast<double>(price()), 'f', 2);


    if(mUnits || mPercentage) // it uses detailed price!
    {
        tempStr += '/' + QString::number(static_cast<double>(priceRest()),
                                                                'f', 2);
    }

    return tempStr;
}



QString Bill::totalUnitsStr() const
{
    if(mEndOfMonth == 0)
        return "-";

    return QString::number(totalUnits());
}

void Bill::setPrice(QString price) // it is providing some guards
{
    price.replace(",", "."); // delete spaces

    mPrice = price.toFloat();
}
