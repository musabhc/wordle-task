#ifndef WORDLEMAIN_H
#define WORDLEMAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class WordleMain; }
QT_END_NAMESPACE

class WordleMain : public QMainWindow
{
    Q_OBJECT

public:
    WordleMain(QWidget *parent = nullptr);
    ~WordleMain();

private:
    Ui::WordleMain *ui;
};
#endif // WORDLEMAIN_H
