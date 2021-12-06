import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import QtCharts 2.3

Rectangle { // ChartWidget
    id: item
    color: "#2c2c35"

    property variant abstractBarSeries: chartView.m_series

    ChartView {
        id: chartView
        backgroundColor: "#2c2c35"
        plotAreaColor: "transparent"
        title: "Частота слов в тексте"
        titleFont.pointSize: 12
        titleColor: "white"
        anchors { fill: parent; }
        margins { right: 0; bottom: 0; left: 0; top: 0; }
        legend.alignment: Qt.AlignBottom
        antialiasing: true

        property variant m_series: mySeries

        BarSeries {
            id: mySeries
            visible: true
            axisY: ValueAxis {
                labelsColor: "white"
                labelsFont: Qt.font({pointSize: 8})
                min: 0
                max: 20
            }
            axisX: BarCategoryAxis {
                labelsColor: "white"
                labelsFont: Qt.font({pointSize: 8})
                categories: ["Masha", "walked", "along", "the", "highway", "and", "sucked", "drying"]
            }
            BarSet {
                label: "<font color='white'>Frecuency</font>"
                labelColor: "white"
                color: "#209FDF"
                borderWidth: 5
                borderColor: "transparent"
                values: [0,     0,    0,   0,    0,    0,    0,    0]
            }
        }
    }
}

