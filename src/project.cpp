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

Project::Project(QString name, qint64 total, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Project),
    isSender(false),
    current(QTime(0,0,0,0)),
    total(total)
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
    qint16 fulldays =  hours / 24;

    ui->lbTime->setText(QString("%1:%2:%3 (%4 full days)")
                .arg(hours)
                .arg(min,2,10,QChar('0'))
                .arg(sec,2,10,QChar('0'))
                .arg(fulldays));
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
