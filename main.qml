import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3


Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Top " + topWordCount + " words in text")

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        nameFilters: ["Texts Files (*.txt)"];
        folder: shortcuts.home
        modality: Qt.NonModal
        onAccepted: {
            visible = false
            chartController.initBarSeries(chartWidget.abstractBarSeries)
            dataBuilder.multiThreadedWordCount(fileDialog.fileUrls);
        }
        onRejected: {
            visible = false
        }
    }

    ProgressBar {
        id: prg
        from: 0
        to: 100
        implicitHeight: 40
        width: mainWindow.width - btnOpenFile.width

        Connections {
            target: dataBuilder
            onProgressChanged: prg.value = progress;
        }
    }

    Button {
        id: btnOpenFile
        text: qsTr("Open file")
        anchors.left: prg.right

        contentItem: Text {
            text: btnOpenFile.text
            font: btnOpenFile.font
            opacity: enabled ? 1.0 : 0.3
            color: btnOpenFile.down ? "#17a81a" : "#21be2b"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 40
            opacity: enabled ? 1 : 0.3
            border.color: btnOpenFile.down ? "#17a81a" : "#21be2b"
            border.width: 1
            radius: 2
        }
        onClicked: {
            fileDialog.visible = true
        }
    }

    ChartWidget {
            id: chartWidget
            anchors.top: btnOpenFile.bottom
            width: mainWindow.width
            height: mainWindow.height - btnOpenFile.height
    }
}
