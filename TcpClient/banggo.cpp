#include "banggo.h"
#include "ui_banggo.h"

Banggo::Banggo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Banggo)
{
    ui->setupUi(this);
}

Banggo::~Banggo()
{
    delete ui;
}
