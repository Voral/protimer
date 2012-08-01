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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "project.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#ifndef Q_OS_MAC
    qApp->setWindowIcon((QPixmap(":/res/coelho/player_time.ico")));
#endif

    ui->setupUi(this);
    qApp->setApplicationVersion("1.0");
    qApp->setApplicationName(tr("Projects Timer"));
    setWindowTitle(qApp->applicationName());

    QSettings *cfg = new QSettings(QSettings::IniFormat, QSettings::UserScope,"voral","protimer",this);
    cfg->setIniCodec("UTF-8");
    QStringList presetList = cfg->childGroups();
    foreach(QString name, presetList)
    {
        cfg->beginGroup(name);
        addProject(cfg->value("name",QString("noname")).toString(),cfg->value("total",int(0)).toInt());
        cfg->endGroup();
    }
    delete cfg;


}

MainWindow::~MainWindow()
{
    QSettings *cfg = new QSettings(QSettings::IniFormat, QSettings::UserScope,"voral","protimer",this);
    cfg->setIniCodec("UTF-8");

    QStringList presetList = cfg->childGroups();
    foreach(QString name, presetList)
    {
        cfg->remove(name);
    }

    QVBoxLayout *layout =static_cast <QVBoxLayout *>(ui->frame->layout());
    for (int i = 0; i < layout->count(); ++i)
    {
        if (Project *pr = qobject_cast<Project *>(layout->itemAt(i)->widget()))
        {
            pr->stop();
            cfg->beginGroup(QString("project%1").arg(i));
            cfg->setValue("name",pr->getName());
            cfg->setValue("total",int(pr->getTotal()));
            cfg->endGroup();
        }
    }
    delete cfg;
    delete ui;
}
bool MainWindow::validateName(QString name)
{
    if (name.isEmpty())
    {
        QMessageBox::warning(this,
                             this->windowTitle(),
                             tr("Project name can't be empty."),
                             QMessageBox::Cancel,
                             QMessageBox::Cancel);
        return false;
    }
    QVBoxLayout *layout =static_cast <QVBoxLayout *>(ui->frame->layout());
    for (int i = 0; i < layout->count(); ++i)
    {
        if (Project *pr = qobject_cast<Project *>(layout->itemAt(i)->widget()))
        {
            if (name.compare(pr->getName())==0)
            {
                QMessageBox::warning(this,
                                     this->windowTitle(),
                                     tr("Project <b>%1</b> already exists.").arg(name),
                                     QMessageBox::Cancel,
                                     QMessageBox::Cancel);
                return false;
            }
        }
    }
    return true;
}

void MainWindow::addProject()
{
    bool ok;
    QString text;
    do {
        text = QInputDialog::getText(this, tr("New project"),tr("Project name:"), QLineEdit::Normal,"", &ok);
    } while (ok && !validateName(text));
    if (ok)
    {
        addProject(text);
    }
}
void MainWindow::addProject(QString name, qint64 total)
{
    QVBoxLayout *layout =static_cast <QVBoxLayout *>(ui->frame->layout());
    Project *project = new Project(name, total, this);
    layout->insertWidget(0,project,0,Qt::AlignTop);
    connect(project,SIGNAL(running()),this,SLOT(onRunning()));
    connect(this,SIGNAL(newRunning()),project,SLOT(onRunningSend()));
}

void MainWindow::onRunning()
{
    emit newRunning();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,tr("About %1").arg(qApp->applicationName()),
                       tr("%1 - small program to account for time spent on projects<br>Version: %2<br>"
                          "Copyright 2012 Alexander Vorobyev (Voral)<br>License: GNU/GPL<br>Autor: Alexander Vorobyev<br>Site: http://va-soft.ru/")
                       .arg(qApp->applicationName())
                       .arg(qApp->applicationVersion()));
}
