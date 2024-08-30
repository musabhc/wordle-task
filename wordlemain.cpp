#include "wordlemain.h"
#include "ui_wordlemain.h"
#include <QButtonGroup>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>
#include <QDebug>
#include <QTimer>


WordleMain::WordleMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WordleMain)
{
    ui->setupUi(this);

    //Creating Button Group
    QButtonGroup* buttonGroup = new QButtonGroup(this);

    //Adding keyboard layout buttons to button group
    buttonGroup->addButton(ui->key_Q);
    buttonGroup->addButton(ui->key_W);
    buttonGroup->addButton(ui->key_E);
    buttonGroup->addButton(ui->key_R);
    buttonGroup->addButton(ui->key_T);
    buttonGroup->addButton(ui->key_Y);
    buttonGroup->addButton(ui->key_U);
    buttonGroup->addButton(ui->key_I);
    buttonGroup->addButton(ui->key_O);
    buttonGroup->addButton(ui->key_P);
    buttonGroup->addButton(ui->key_A);
    buttonGroup->addButton(ui->key_S);
    buttonGroup->addButton(ui->key_D);
    buttonGroup->addButton(ui->key_F);
    buttonGroup->addButton(ui->key_G);
    buttonGroup->addButton(ui->key_H);
    buttonGroup->addButton(ui->key_J);
    buttonGroup->addButton(ui->key_K);
    buttonGroup->addButton(ui->key_L);
    buttonGroup->addButton(ui->key_Z);
    buttonGroup->addButton(ui->key_X);
    buttonGroup->addButton(ui->key_C);
    buttonGroup->addButton(ui->key_V);
    buttonGroup->addButton(ui->key_B);
    buttonGroup->addButton(ui->key_N);
    buttonGroup->addButton(ui->key_M);

    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &WordleMain::keyClicked);
    gameStarted();
}

WordleMain::~WordleMain()
{
    delete ui;
}

void WordleMain::gameStarted() {
    qDebug() << "gameStarted called";

    // Set game screen to main screen
    ui->Screens->setCurrentWidget(ui->gameScreen);

    QFile dictionaryFile("C:/Users/Musa/Documents/worldle-task/resources/dictionary.txt");
    QFile quizWordsFile("C:/Users/Musa/Documents/worldle-task/resources/wordle-words.txt");
    QFile zeroStatisticsFile("C:/Users/Musa/Documents/worldle-task/resources/statistics.json");
    QString path("resources/");
    QDir dir;

    // Check if files exist and are accessible
    if (!dictionaryFile.exists() || !quizWordsFile.exists()) {
        qDebug() << "One or more resource files do not exist";
        return;
    }

    // Check directory existence
    if (!dir.exists(path)) {
        dir.mkpath(path);
    }

    QFile statisticsFile(path + "statistics.json");

    // If statistics file doesn’t exist, create it with zero stats
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

    // Fill dictionaries with words
    fillDictionary(dictionary, dictionaryFile);
    fillDictionary(quizWords, quizWordsFile);

    // Read JSON file into QJsonObject
    fillJsonObject(statistics, statisticsFile);

    // Update UI with statistics
    setStatistics();

    // Generate a new word for the game
    newWordle();
}


//Fill dictionary with wordle words
void WordleMain::fillDictionary(QStringList &dictionary, QFile &file){
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in.setCodec("UTF-8"); // Dosyanın UTF-8 olarak okunmasını sağlar
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList words = line.split(" ", QString::SkipEmptyParts);
            dictionary.append(words);
        }
        file.close();
    }
}


//Read the stats file and fill the jsonObject wit hdata.
void WordleMain::fillJsonObject(QJsonObject &jsonObject, QFile &file){
    QString data;
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());
    jsonObject = document.object();
}

//Update the exist json file with new data
void WordleMain::updateJsonFile(QFile &file, QJsonObject &jsonObject){
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(QJsonDocument(jsonObject).toJson(QJsonDocument::Indented));
    file.close();
}

void WordleMain::setStatistics(){
    int firstTry = statistics["firstTry"].toString().toInt();
    int secondTry = statistics["secondTry"].toString().toInt();
    int thirdTry = statistics["thirdTry"].toString().toInt();
    int fourthTry = statistics["fourthTry"].toString().toInt();
    int fifthTry = statistics["fifthTry"].toString().toInt();
    int sixthTry = statistics["sixthTry"].toString().toInt();
    int gameCount = statistics["gameCount"].toString().toInt();
    double winCount = statistics["winCount"].toString().toDouble();
    double winRate = gameCount ? (winCount / gameCount) * 100 : 0;

    ui->playedCount->setText(QString::number(gameCount));
    ui->winPercentCount->setText(QString::number(winRate, 'f', 2));
    ui->currentStreakCount->setText(statistics["currentStreak"].toString());
    ui->maxStreakCount->setText(statistics["maxStreak"].toString());

    ui->progressBar_1->setValue(firstTry * 100 / winCount);
    ui->progressBar_2->setValue(secondTry * 100 / winCount);
    ui->progressBar_3->setValue(thirdTry * 100 / winCount);
    ui->progressBar_4->setValue(fourthTry * 100 / winCount);
    ui->progressBar_5->setValue(fifthTry * 100 / winCount);
    ui->progressBar_6->setValue(sixthTry * 100 / winCount);

    ui->progressBar_1->setFormat(QString::number(firstTry));
    ui->progressBar_2->setFormat(QString::number(secondTry));
    ui->progressBar_3->setFormat(QString::number(thirdTry));
    ui->progressBar_4->setFormat(QString::number(fourthTry));
    ui->progressBar_5->setFormat(QString::number(fifthTry));
    ui->progressBar_6->setFormat(QString::number(sixthTry));
}




bool labelLessThan(const QLabel *label1, const QLabel *label2){
    return label1->objectName() < label2->objectName();
}

void WordleMain::newWordle(){
    srand(time(NULL));
    wordle = quizWords[rand() % quizWords.length()];
    qDebug() << wordle;
}

int WordleMain::emptyLabelIndex(QList<QLabel *> &labelList){
    int emptyLabelIndex = 0;
    labelList = availableLabels(labelList);

    for (auto& label : labelList){
        if(label->text() == ""){
            break;
        }
        emptyLabelIndex++;
    }

    if (emptyLabelIndex > labelList.length() || labelList.isEmpty()){
        return -1;
    }

    return emptyLabelIndex;
}

QList<QLabel *>& WordleMain::availableLabels(QList<QLabel *> &labelList){
    int currentIndex = 0;
    while(currentIndex < labelList.length()){
        if (labelList[currentIndex]->styleSheet().indexOf("background-color") != -1){
            labelList.removeAt(currentIndex);
            currentIndex = 0;
            continue;
        }
        currentIndex++;
    }
    return labelList;
}

void WordleMain::gameEnded(){
    QFile statisticsFile("C:/Users/Musa/Documents/worldle-task/resources/statistics.json");
    isGameStopped = true;

    QTimer::singleShot(3500, [this](){
        isGameStopped = false;
        QList<QLabel *> quizLabels = ui->gameScreen->findChildren<QLabel *>();
        for (auto& label : quizLabels){
            label->clear();
            label->setStyleSheet("");
        }

        newWordle();
    });

    updateJsonFile(statisticsFile, statistics);
    setStatistics();
}

void WordleMain::winEvent(){
    QString loseMessage = "GG WP, you found it! Wordle is: " + wordle;
    QMessageBox::warning(
        this,
        tr("Wordle App"),
        loseMessage);
    QList<QLabel *> quizLabels = ui->gameScreen->findChildren<QLabel *>();
    std::sort(quizLabels.begin(), quizLabels.end(), labelLessThan);
    quizLabels = availableLabels(quizLabels);

    switch(quizLabels.length()){
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

void WordleMain::loseEvent(){
    QString loseMessage = "Nice Try, next time! Wordle is: " + wordle;
    QMessageBox::warning(
        this,
        tr("Wordle App"),
        loseMessage);
    statistics["gameCount"] = QString::number(statistics["gameCount"].toString().toInt() + 1);
    statistics["currentStreak"] = "0";

    gameEnded();
}

void WordleMain::keyClicked(QAbstractButton *key){

    // Game Stopped
    if(isGameStopped)
    {
        return;
    }

    QList<QLabel *> quizLabels = ui->gameScreen->findChildren<QLabel *>();
    std::sort(quizLabels.begin(), quizLabels.end(),labelLessThan);
    int labelIndex = emptyLabelIndex(quizLabels);

    //Check if it´s in the line end or not
    if(labelIndex == -1 || (labelIndex % 5 == 0 && labelIndex != 0))
    {
        return;
    }

    // Set the label text to key, make a border to highlight
    quizLabels[labelIndex]->setText(key->text());
    quizLabels[labelIndex]->setStyleSheet("border: 2px solid black;\n");

}

void WordleMain::on_key_ENTER_clicked()
{
    if(isGameStopped){
            return;
        }

        QString currentWord;
        QString currentWordle = wordle;
        /*
        QMessageBox::warning(
            this,
            tr("Word"),
            currentWordle );
        return;
        */
        QList <QLabel *> quizLabels = ui->gameScreen->findChildren<QLabel *>();
        std::sort(quizLabels.begin(), quizLabels.end(), labelLessThan);

        quizLabels = availableLabels(quizLabels);

        for (int i = 0; i < 5; i++){
            currentWord += quizLabels[i]->text();
        }
        if(dictionary.indexOf(currentWord) == -1){
            if(currentWord.length() == 5){
                QMessageBox::warning(
                    this,
                    tr("Wordle App"),
                    tr("Not in the list!") );
                return;
            }

            QMessageBox::warning(
                this,
                tr("Wordle App"),
                tr("Not enough letters") );
            return;
        }

        for (int i = 0; i < 5; i++){
            if(quizLabels[i]->text() == wordle[i]){
                quizLabels[i]->setStyleSheet("background-color: #6aac64;\ncolor: white;\nborder-style:hidden;\n");
                currentWordle[i] = '0';
            }
        }

        for (int i = 0; i < 5; i++){
            if(quizLabels[i]->styleSheet().indexOf("background-color") != -1){
                continue;
            }
            if(wordle.indexOf(quizLabels[i]->text()) != -1 && currentWordle.indexOf(quizLabels[i]->text()) != -1){
                currentWordle[wordle.indexOf(quizLabels[i]->text())] = '0';
                quizLabels[i]->setStyleSheet("background-color: #cab557;\ncolor: white;\nborder-style:hidden;\n");
            }
            else {
                quizLabels[i]->setStyleSheet("background-color: #797d7f;\ncolor: white;\nborder-style:hidden;\n");
            }
        }

        quizLabels = availableLabels(quizLabels);

        if (currentWord == wordle){
            winEvent();
        }

        else if (quizLabels.isEmpty()){
            loseEvent();
        }
}

void WordleMain::on_key_REMOVE_clicked()
{
    if (isGameStopped) {
        return;
    }

    QList<QLabel *> quizLabels = ui->gameScreen->findChildren<QLabel *>();
    std::sort(quizLabels.begin(), quizLabels.end(), labelLessThan);

    int labelIndex = emptyLabelIndex(quizLabels);

    qDebug() << "Label Index:" << labelIndex;

    // Ensure labelIndex is valid
    if (labelIndex <= 0 || labelIndex >= quizLabels.size()) {
        qDebug() << "Invalid label index";
        return;
    }

    // Clear the label text and style if it's not empty
    if (!quizLabels[labelIndex - 1]->text().isEmpty()) {
        qDebug() << "Clearing text for label at index:" << (labelIndex - 1);
        quizLabels[labelIndex - 1]->setText("");
        quizLabels[labelIndex - 1]->setStyleSheet("");
    }
}



void WordleMain::on_wordleBtn_clicked()
{
    ui->Screens->setCurrentWidget(ui->gameScreen);
}

void WordleMain::on_helpBtn_clicked()
{
    ui->Screens->setCurrentWidget(ui->helpScreen);
}

void WordleMain::on_statsBtn_clicked()
{
    ui->Screens->setCurrentWidget(ui->statsScreen);
}
