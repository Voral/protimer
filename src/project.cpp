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
int Project::lastId = -1;
Project* Project::autostartProject = NULL;

Project::Project(int id, QString name, qint64 total, int iHoursPerDay, QString iKeySequence, bool autostart, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Project),
    total(total),
    isSender(false),
    hoursePerDay(iHoursPerDay),
    current(QTime(0,0,0,0))
{
    acRun = new QAction(name,this);
    connect(acRun,SIGNAL(triggered(bool)),this,SLOT(onActionRun(bool)));
    acRun->setCheckable(true);
    ui->setupUi(this);
    updateTimeLabel();
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateTimeLabel()));
    ui->lbName->setText(name);
    ui->lbShortcut->setText(iKeySequence);
    ui->lbNum->setText(QString("#%1").arg(id));
    if (id > Project::lastId) Project::lastId = id;
    setAutoStart(autostart);
    if (!iKeySequence.isEmpty()) ui->btRun->setShortcut(QKeySequence(iKeySequence));
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

    ui->lbTime->setText(tr("%1:%2:%3 (%4 full days [%5 hour/day])")
                .arg(hours)
                .arg(min,2,10,QChar('0'))
                .arg(sec,2,10,QChar('0'))
                .arg(fulldays)
                .arg(this->hoursePerDay));

    emit setIconToolTip(ui->lbTime->text(),ui->lbName->text());
}
void Project::stop()
{
    if (ui->btRun->isChecked()) ui->btRun->click();
}

qint64 Project::getTotal()
{
    return total;
}
QAction* Project::getAction()
{
    return acRun;
}

void Project::onRunningSend()
{
    if (isSender) isSender= false;
    else
    {
        stop();
    }
}
void Project::onActionRun(bool checked)
{
    ui->btRun->setChecked(checked);
}

void Project::on_btRun_toggled(bool checked)
{
    disconnect(acRun,SIGNAL(triggered(bool)),this,SLOT(onActionRun(bool)));
    acRun->setChecked(checked);
    connect(acRun,SIGNAL(triggered(bool)),this,SLOT(onActionRun(bool)));
    QFont ft = acRun->font();
    if (checked)
    {
        current.start();
        timer.start(1000);
        isSender = true;
        emit running();
        ui->btRun->setIcon(QIcon(":/res/coelho/player_pause.png"));
        ft.setBold(true);
    }
    else {
        ui->btRun->setIcon(QIcon(":/res/coelho/player_play.png"));
        total = total + current.elapsed();
        timer.stop();
        updateTimeLabel(false);
        ft.setBold(false);
    }
    acRun->setFont(ft);
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
    bool autostart = getAutostart();
    Edit *dlgEdit = new Edit(getId(),ui->lbName->text(),this->hoursePerDay,ui->lbShortcut->text(), autostart, this);
    if (dlgEdit->exec()==QDialog::Accepted)
    {
        this->hoursePerDay = dlgEdit->getHours();
        ui->lbName->setText(dlgEdit->getName());
        ui->btRun->setShortcut(dlgEdit->getKeySequence());
        ui->lbShortcut->setText(dlgEdit->getKeySequence().toString());
        if (autostart != dlgEdit->getAutostart())
        {
            setAutoStart(dlgEdit->getAutostart());
        }
        updateTimeLabel(false);
    }
    delete dlgEdit;
}
QString Project::getKeySequence()
{
    return ui->lbShortcut->text();
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
int Project::getId()
{
    QString result = ui->lbNum->text();
    return result.replace("#","").toInt();
}

bool Project::getAutostart()
{
    return (!ui->lbAutostart->text().isEmpty());
}
int Project::getNewId()
{
    return ++lastId;
}
void Project::setAutoStart(bool state)
{
    if (state)
    {
        if ((Project::autostartProject != this)&&(Project::autostartProject != NULL))
        {
            Project::autostartProject->setAutoStart(false);
        }
        Project::autostartProject = this;
        ui->lbAutostart->setText(tr("Autostart"));
    }
    else
    {
        if (Project::autostartProject == this)
        {
            Project::autostartProject = NULL;
        }
        ui->lbAutostart->setText("");
    }
}
void Project::runAuto()
{
    if (Project::autostartProject != NULL) Project::autostartProject->onActionRun(true);
}
