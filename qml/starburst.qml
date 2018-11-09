import QtQuick 2.13
import QtMultimedia 5.13
import org.ecloud.kaleidospect 1.0

Rectangle {
    color: "black"
    width: 800; height: 800

    property AudioSpectrumModel spectrum: AudioSpectrumModel {
//        deviceId: devicesCB.currentText
//        sampleRate: sampleRateCB.model[sampleRateCB.currentIndex]
        bandCount: 40
    }

    Starburst {
        anchors.fill: parent
        levels: spectrum.levels
    }
}
