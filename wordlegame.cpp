#include "wordlegame.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QDir>

WordleGame::WordleGame(QObject *parent) : QObject(parent), currentRow(0) {
    gameStarted();
}

void WordleGame::gameStarted(){

    //File dirs
    QFile dictionaryFile("C:/Users/Musa/Documents/wordle-task-qml/resources/dictionary.txt");
    QFile wordleFile("C:/Users/Musa/Documents/wordle-task-qml/resources/wordle-words.txt");
    QFile zeroStatisticsFile("C:/Users/Musa/Documents/wordle-task-qml/resources/statistics.json");
    QString path("resources/");
    QDir dir;

    //Check dict exist or not
    if (!dictionaryFile.exists() || !wordleFile.exists()) {
        qDebug() << "One or more resource files do not exist";
        return;
    }

    // Check directory existence
    if (!dir.exists(path)) {
        dir.mkpath(path);
    }

    QFile statisticsFile(path + "statistics.json");

    //Check stats file exist if not create new one with zeros
    if (!statisticsFile.exists()) {
        QString zeroStats;
        if (!zeroStatisticsFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open zero statistics file";
            return;
        }
        zeroStats = zeroStatisticsFile.readAll();
        zeroStatisticsFile.close();

        if (!statisticsFile.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to open statistics file for writing:" << statisticsFile.errorString();
            return;
        }

        QTextStream out(&statisticsFile);
        out << zeroStats;
        statisticsFile.close();
    }

    //Get all words from files
    fillDictionary(dictionaryWords, dictionaryFile);
    fillDictionary(wordleWords, wordleFile);

    //Fill stats obj
    fillJsonObject(statistics, statisticsFile);

    //Get new word for game
    newWordle();
}


void WordleGame::fillDictionary(QStringList &dictionaryWords, QFile &file) {
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList words = line.split(" ", QString::SkipEmptyParts);
            dictionaryWords.append(words);
        }
        file.close();
    }
}

void WordleGame::newWordle(){
    srand(time(NULL));
    correctWord = wordleWords[rand() % wordleWords.length()];
    qDebug() << correctWord;
}

void WordleGame::changePopupText(QString newText) {
    //qDebug() << "Changing popup text to: " << newText;
    emit popupTextChanged(newText); // Send text change signal
}


void WordleGame::onKeyPress(QString letter) {
    //qDebug() << "Button pressed";
    if (currentGuess.length() < 5) {
        currentGuess += letter;
        emit updateGrid(currentRow, currentGuess.length() - 1, "white", letter);
    }
}

void WordleGame::onEnterPress() {
    //qDebug() << "Enter pressed";
    if(dictionaryWords.indexOf(currentGuess) == -1){
                if(currentGuess.length() == 5){
                    changePopupText("Word is not in the list!");
                    return;
                }
                changePopupText("Not enough letters!");
                return;
            }

    checkGuess();

    if (currentGuess == correctWord) {
        emit gameWon();
    } else if (currentRow >= 5) {
        emit gameLost();
    } else {
        currentRow++;
        currentGuess.clear();
    }
}

void WordleGame::onDeletePress() {
    if (!currentGuess.isEmpty()) {
        currentGuess.chop(1);
        emit updateGrid(currentRow, currentGuess.length(), "lightgray", "");
    }
}

void WordleGame::checkGuess() {
    //Change color of letters
    for (int i = 0; i < 5; ++i) {
        QString color = "gray";
        if (correctWord[i] == currentGuess[i]) {
            color = "green";
        } else if (correctWord.contains(currentGuess[i])) {
            color = "yellow";
        }
        emit updateGrid(currentRow, i, color, QString(currentGuess[i]));
    }
}

//Read the stats file and fill the jsonObject wit hdata.
void WordleGame::fillJsonObject(QJsonObject &jsonObject, QFile &file){
    QString data;
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());
    jsonObject = document.object();
}

//Update the exist json file with new data
void WordleGame::updateJsonFile(QFile &file, QJsonObject &jsonObject){
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(QJsonDocument(jsonObject).toJson(QJsonDocument::Indented));
    file.close();
}

/*
void WordleGame::setStatistics(){
    int firstTry = statistics["firstTry"].toString().toInt();
    int secondTry = statistics["secondTry"].toString().toInt();
    int thirdTry = statistics["thirdTry"].toString().toInt();
    int fourthTry = statistics["fourthTry"].toString().toInt();
    int fifthTry = statistics["fifthTry"].toString().toInt();
    int sixthTry = statistics["sixthTry"].toString().toInt();
    int gameCount = statistics["gameCount"].toString().toInt();
    double winCount = statistics["winCount"].toString().toDouble();
    double winRate = gameCount ? (winCount / gameCount) * 100 : 0;
}
*/

int WordleGame::getEmptyCellsCount() const {
    int totalCells = 30;  // 5x6 grid
    int usedCells = currentRow * 5 + currentGuess.length();
    return totalCells - usedCells;
}


void WordleGame::gameWon(){
    QString winMessage = "GG WP, you found it! Wordle is: " + correctWord;
    changePopupText(winMessage);
    int quizLabels = getEmptyCellsCount();
    switch(quizLabels){
        case 25: statistics["firstTry"] = QString::number(statistics["firstTry"].toString().toInt() + 1);
            break;
        case 20: statistics["secondTry"] = QString::number(statistics["secondTry"].toString().toInt() + 1);
            break;
        case 15: statistics["thirdTry"] = QString::number(statistics["thirdTry"].toString().toInt() + 1);
            break;
        case 10: statistics["fourthTry"] = QString::number(statistics["fourthTry"].toString().toInt() + 1);
            break;
        case 5: statistics["fifthTry"] = QString::number(statistics["fifthTry"].toString().toInt() + 1);
            break;
        case 0: statistics["sixthTry"] = QString::number(statistics["sixthTry"].toString().toInt() + 1);
            break;
        }
    statistics["gameCount"] = QString::number(statistics["gameCount"].toString().toInt() + 1);
    statistics["winCount"] = QString::number(statistics["winCount"].toString().toInt() + 1);
    statistics["currentStreak"] = QString::number(statistics["currentStreak"].toString().toInt() + 1);
    if(statistics["currentStreak"].toString().toInt() > statistics["maxStreak"].toString().toInt()){
        statistics["maxStreak"] = QString::number(statistics["currentStreak"].toString().toInt());
    }
    gameEnded();
}

void WordleGame::gameLost(){
    QString loseMessage = "Nice Try, next time! Wordle is: " + correctWord;
    changePopupText(loseMessage);
    statistics["gameCount"] = QString::number(statistics["gameCount"].toString().toInt() + 1);
    statistics["currentStreak"] = "0";

    gameEnded();
}

void WordleGame::gameEnded(){
    QFile statisticsFile("C:/Users/Musa/Documents/worldle-task/resources/statistics.json");

    emit clearGrid();
    updateJsonFile(statisticsFile, statistics);
    currentRow = 0;
    currentGuess = "";
    //setStatistics();
    newWordle();

}
