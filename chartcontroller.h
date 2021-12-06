#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QObject>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

class ChartController : public QObject
{
    Q_OBJECT
public:
    explicit ChartController(QObject *parent = nullptr);

signals:

public slots:
    void initBarSeries(QAbstractSeries *series);
    void clearData();
    void onUpdateData(const QVector<QPair<QString, int> > data, int maxValue);

private:
    void initSeries(QAbstractSeries *series);

    QBarSet *set1;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QBarSeries *xySeries;


};

#endif // CHARTCONTROLLER_H
