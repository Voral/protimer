#include "edit.h"
#include "ui_edit.h"

Edit::Edit(QString pName, int pHours, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit)
{
    ui->setupUi(this);
    ui->edHour->setValue(pHours);
    ui->edName->setText(pName);
}

Edit::~Edit()
{
    delete ui;
}
QString Edit::getName()
{
    return ui->edName->text();
}

int Edit::getHours()
{
    return ui->edHour->value();
}
