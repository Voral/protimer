/********************************************************************************/
/*                                                                              */
/*    Copyright 2012 Alexander Vorobyev (Voral)                                 */
/*    http://va-soft.ru/                                                        */
/*                                                                              */
/*    This file is part of protime.                                             */
/*                                                                              */
/*    Basetest is free software: you can redistribute it and/or modify          */
/*    it under the terms of the GNU General Public License as published by      */
/*    the Free Software Foundation, either version 3 of the License, or         */
/*    (at your option) any later version.                                       */
/*                                                                              */
/*    Basetest is distributed in the hope that it will be useful,               */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*    GNU General Public License for more details.                              */
/*                                                                              */
/*    You should have received a copy of the GNU General Public License         */
/*    along with protime.  If not, see <http://www.gnu.org/licenses/>.          */
/*                                                                              */
/********************************************************************************/
#include "project.h"
#include "ui_project.h"
#include "edit.h"
#include <QMessageBox>

int Project::defaultHoursPerDay = 8;

Project::Project(QString name, qint64 total, int iHoursPerDay, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Project),
    total(total),
    isSender(false),
    hoursePerDay(iHoursPerDay),
    current(QTime(0,0,0,0))
{
    ui->setupUi(this);
    updateTimeLabel();
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateTimeLabel()));
    ui->lbName->setText(name);
    updateTimeLabel(false);
}
Project::~Project()
{
    delete ui;
}
void Project::updateTimeLabel(bool wCurrent)
{
    qint64 tmp = total;
    if (wCurrent) tmp += current.elapsed();
    tmp /= 1000;
    qint64 hours = tmp / 3600;
    qint16 min = (tmp - hours*3600) / 60;
    qint16 sec = tmp -  hours*3600 - min * 60;
    qint16 fulldays =  hours / this->hoursePerDay;

    ui->lbTime->setText(QString("%1:%2:%3 (%4 full days [%5 hour/day])")
                .arg(hours)
                .arg(min,2,10,QChar('0'))
                .arg(sec,2,10,QChar('0'))
                .arg(fulldays)
                .arg(this->hoursePerDay));
}
void Project::stop()
{
    if (ui->btRun->isChecked()) ui->btRun->click();
}

qint64 Project::getTotal()
{
    return total;
}

void Project::onRunningSend()
{
    if (isSender) isSender= false;
    else
    {
        stop();
    }
}

void Project::on_btRun_toggled(bool checked)
{
    if (checked)
    {
        current.start();
        timer.start(1000);
        isSender = true;
        emit running();
        ui->btRun->setIcon(QIcon(":/res/coelho/player_pause.png"));
    }
    else {
        ui->btRun->setIcon(QIcon(":/res/coelho/player_play.png"));
        total = total + current.elapsed();
        timer.stop();
        updateTimeLabel(false);
    }
}
QString Project::getName()
{
    return ui->lbName->text();
}
int Project::getHoursPerDay()
{
    return this->hoursePerDay;
}

void Project::on_btEdit_clicked()
{
    Edit *dlgEdit = new Edit(ui->lbName->text(),this->hoursePerDay,this);
    if (dlgEdit->exec()==QDialog::Accepted)
    {
        this->hoursePerDay = dlgEdit->getHours();
        ui->lbName->setText(dlgEdit->getName());
        updateTimeLabel(false);
    }
    delete dlgEdit;
}

void Project::on_btDelete_clicked()
{
    if (QMessageBox::question(this,tr("Deleting project"),
                       tr("Are you sure to delete the project <b>%1</b>?")
                       .arg(ui->lbName->text()), QMessageBox::Ok | QMessageBox::No,
                          QMessageBox::No
                         ) == QMessageBox::Ok)
    {
        this->deleteLater();
    }
}
