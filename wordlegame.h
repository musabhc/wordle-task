#ifndef WORDLEGAME_H
#define WORDLEGAME_H

#include <QObject>
#include <QStringList>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>

class WordleGame : public QObject
{
    Q_OBJECT
public:
    explicit WordleGame(QObject *parent = nullptr);

    Q_INVOKABLE void onKeyPress(QString letter);
    Q_INVOKABLE void onEnterPress();
    Q_INVOKABLE void onDeletePress();

    Q_INVOKABLE void changePopupText(QString newText);
    Q_INVOKABLE int getEmptyCellsCount() const;

signals:
    void popupTextChanged(QString newText);
    void updateGrid(int row, int col, QString color, QString letter);
    void showMessage(QString message);
    void clearGrid();  // Gridi temizle sinyali


private:
    void fillDictionary(QStringList &dictionaryWords, QFile &file);
    QStringList dictionaryWords;
    QStringList wordleWords;
    QString currentGuess;
    QString correctWord;

    int currentRow;
    QJsonObject statistics;

    void gameWon();
    void gameLost();
    void gameEnded();
    void checkGuess();
    void setStatistics();

    void fillJsonObject(QJsonObject &jsonObject, QFile &file);
    void updateJsonFile(QFile &file, QJsonObject &jsonObject);

    void newWordle();
    void gameStarted();
};

#endif // WORDLEGAME_H
