import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 400
    height: 600
    title: "Wordle"

    Column {
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10

        // 5x6 grid
        Grid {
            id: grid
            rows: 6
            columns: 5
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.bottom: keyboard.top

            Repeater {
                model: 30 // 5x6 = 30
                delegate: Rectangle {
                    width: 50
                    height: 50
                    color: "lightgray"
                    border.color: "black"
                    Text {
                        id: gridText
                        anchors.centerIn: parent
                        text: "" // Default value
                        color: "black"
                        font.pixelSize: 20
                    }
                }
            }
        }

        // Keyboard Layout
        Column {
            spacing: 5
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottomMargin: 10
            // Klavye düzenini buraya ekleyin
            Row {
                spacing: 5
                width: 30
                height: 40
                Repeater {
                    model: "QWERTYUIOP".split("")
                    delegate: Button {
                        text: modelData
                        onClicked: {
                            wordleGame.onKeyPress(modelData)
                        }
                    }
                }
            }
            Row {
                spacing: 5
                width: 30
                height: 40
                Repeater {
                    model: "ASDFGHJKL".split("")
                    delegate: Button {
                        text: modelData
                        onClicked: {
                            wordleGame.onKeyPress(modelData)
                        }
                    }
                }
            }
            Row {
                spacing: 5
                width: 30
                height: 40
                Button {
                    text: "Enter"
                    onClicked: {
                        wordleGame.onEnterPress()
                    }
                }
                Repeater {
                    model: "ZXCVBNM".split("")
                    delegate: Button {
                        text: modelData
                        onClicked: {
                            wordleGame.onKeyPress(modelData)
                        }
                    }
                }
                Button {
                    text: "Delete"
                    onClicked: {
                        wordleGame.onDeletePress()
                    }
                }
            }
        }
    }
    Popup{
        property string popupText: "Boo! This is popup!"
        id: popup
        width: 400
        height: 150
        focus: true
        anchors.centerIn: parent

        Rectangle{
            width: parent.width
            height: parent.height
            radius: 10

            Text {
                id: popupTextItem
                anchors.centerIn: parent
                text: popup.popupText
                color: "black"  // Yazının rengi
                font.pixelSize: 20
            }

        }
    }

    function showPopup(){
        popup.open();
    }

    Connections {
        target: wordleGame
        onUpdateGrid: {
            var item = grid.children[row * 5 + col]
            item.color = color
            item.children[0].text = letter // Harfleri güncelle
        }
        onPopupTextChanged: {
            console.log("Popup text is changing to: " + newText);
            popup.popupText = newText; // popupText güncellenir
            showPopup(); // Popup gösterilir
        }
        onClearGrid: {
            // Grid içindeki tüm hücrelerin içeriğini temizle
            for (var i = 0; i < 30; i++) {
            var item = grid.children[i];
            item.color = "lightgray";
            item.children[0].text = "";
            }
        }
    }
}
