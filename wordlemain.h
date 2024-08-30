#ifndef WORDLEMAIN_H
#define WORDLEMAIN_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QLabel>
#include <QJsonObject>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class WordleMain; }
QT_END_NAMESPACE

class WordleMain : public QMainWindow
{
    Q_OBJECT

public:
    WordleMain(QWidget *parent = nullptr);
    ~WordleMain();

public slots:
    void keyClicked(QAbstractButton *key);



private:
    Ui::WordleMain *ui;
    bool isGameStopped = false;
    QString wordle="";
    QStringList quizWords;
    QStringList dictionary;
    QJsonObject statistics;


private slots:
    void newWordle();
    void gameStarted();
    void gameEnded();
    void winEvent();
    void loseEvent();
    void setStatistics();

    void on_key_ENTER_clicked();
    void on_key_REMOVE_clicked();
    void on_wordleBtn_clicked();
    void on_helpBtn_clicked();
    void on_statsBtn_clicked();

    int emptyLabelIndex(QList<QLabel *> &labelList);
    void fillDictionary(QStringList &dictionary, QFile &file);
    void fillJsonObject(QJsonObject &jsonObject, QFile &file);
    QList<QLabel *>& availableLabels(QList<QLabel *> &labelList);
    void updateJsonFile(QFile &file, QJsonObject &jsonObject);

};
#endif // WORDLEMAIN_H
