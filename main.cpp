#include "wordlemain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WordleMain w;
    w.show();
    return a.exec();
}
