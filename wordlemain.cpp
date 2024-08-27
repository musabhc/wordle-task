#include "wordlemain.h"
#include "ui_wordlemain.h"

WordleMain::WordleMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WordleMain)
{
    ui->setupUi(this);
}

WordleMain::~WordleMain()
{
    delete ui;
}

