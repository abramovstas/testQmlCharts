#include "chartcontroller.h"

#include <QDebug>

ChartController::ChartController(QObject *parent) : QObject(parent)
{

}

void ChartController::initBarSeries(QAbstractSeries *series)
{
    if (series)
        initSeries(series);
}

void ChartController::clearData()
{
    if (set1 && axisX && axisY)
    {
        set1->remove(0, set1->count());
        axisX->clear();
    }
}

void ChartController::onUpdateData(const QVector<QPair<QString, int> > data, int maxValue)
{
    if (data.size() > 0)
    {
        clearData();
        axisY->setRange(0, maxValue + 5);
        for(auto pair: data)
        {
            axisX->append(pair.first);
            set1->append(pair.second);
        }
    }
}

void ChartController::initSeries(QAbstractSeries *series)
{
    xySeries = static_cast<QBarSeries *>(series);

    xySeries->setLabelsVisible(true);

    QList<QBarSet *> barsets = xySeries->barSets();
    for (int i = 0; i < barsets.count(); i++)
    {
        if (barsets.at(i)->label() == QString("<font color='white'>Frecuency</font>"))
        {
            set1 = barsets.at(i);
        }
    }

    QAbstractAxis *aaxes = xySeries->attachedAxes().at(0);
    if (aaxes->type() == QAbstractAxis::AxisTypeBarCategory)
    {
        axisX = static_cast<QBarCategoryAxis *>(aaxes);
        axisY = static_cast<QValueAxis *>(xySeries->attachedAxes().at(1));
    }
    else if (aaxes->type() == QAbstractAxis::AxisTypeValue)
    {
        axisX = static_cast<QBarCategoryAxis *>(xySeries->attachedAxes().at(1));
        axisY = static_cast<QValueAxis *>(aaxes);
    }

    clearData();
}
