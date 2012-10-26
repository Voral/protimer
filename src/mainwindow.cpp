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
#include "edit.h"

#include <QMessageBox>
#include <QSettings>
#include <QPixmap>
#include <QCloseEvent>
#include <QToolBar>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#ifndef Q_OS_MAC
    qApp->setWindowIcon((QPixmap(":/res/coelho/player_time.ico")));
#endif
    ui->setupUi(this);
    qApp->setApplicationVersion("1.4");
    qApp->setApplicationName(tr("Projects Timer"));
    setWindowTitle(qApp->applicationName());

    trayIconMenu = new QMenu(this);

    int runOnCommandId = -1;
    Project *runOnCommand = NULL;

    QListIterator<QString> i(qApp->arguments());
    while (i.hasNext())
    {
        QString arg = i.next();
        if (arg.startsWith("-r",Qt::CaseInsensitive)) runOnCommandId = arg.replace("-r","").toInt();
    }


    QSettings *cfg = new QSettings(QSettings::IniFormat, QSettings::UserScope,"voral","protimer",this);
    cfg->setIniCodec("UTF-8");
    QStringList presetList = cfg->childGroups();
    Project *project;
    foreach(QString name, presetList)
    {

        cfg->beginGroup(name);

        int id = name.replace("project","").toInt();
        project = addProject(id,cfg->value("name",QString("noname")).toString(),
                             cfg->value("total",int(0)).toInt(),
                             cfg->value("hoursPerDay",int(Project::defaultHoursPerDay)).toInt(),
                             cfg->value("hotKey","").toString(),
                             cfg->value("autostart",false).toBool());
        trayIconMenu->addAction(project->getAction());
        if (runOnCommandId == id) runOnCommand = project;
        cfg->endGroup();
    }
    delete cfg;
    trayIconMenu->addSeparator();

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setIcon(QPixmap(":/res/yellowicon/exit.png"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    quitAction->setShortcut(QKeySequence("Ctrl+Q"));
    trayIconMenu->addAction(quitAction);

    QToolBar *tbar = ui->mainToolBar;
    tbar->addAction(quitAction);

    createTrayIcon();
    if (runOnCommand==NULL)
    {
        Project::runAuto();
    }
    else
    {
        runOnCommand->onActionRun(true);
    }
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
            cfg->beginGroup(QString("project%1").arg(pr->getId()));
            cfg->setValue("name",pr->getName());
            cfg->setValue("total",int(pr->getTotal()));
            cfg->setValue("hoursPerDay",int(pr->getHoursPerDay()));
            cfg->setValue("hotKey",pr->getKeySequence());
            cfg->setValue("autostart",pr->getAutostart());
            cfg->endGroup();
        }
    }
    delete cfg;
    delete ui;
}
void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QPixmap(":/res/coelho/protimer.png"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(qApp->applicationName());
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason==QSystemTrayIcon::Trigger) setVisible(!this->isVisible());
}
void MainWindow::setVisible(bool visible)
{
    QMainWindow::setVisible(visible);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}
bool MainWindow::validateName(QString name)
// Validate project name on blank and clone
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
void MainWindow::onSetIconToolTip(const QString text, const QString name)
{
    trayIcon->setToolTip(QString("<b>%1</b><br>\n%2<br>\n%3").arg(qApp->applicationName()).arg(name).arg(text));
}

void MainWindow::addProject()
{
    int ok;
    Edit *dlgEdit = new Edit(-1,"",Project::defaultHoursPerDay,"",false,this);
    do {
        ok = dlgEdit->exec();
    } while (ok==QDialog::Accepted && !validateName(dlgEdit->getName()));
    if (ok == QDialog::Accepted)
    {
        addProject(Project::getNewId(), dlgEdit->getName(),0,dlgEdit->getHours(),dlgEdit->getKeySequence(),dlgEdit->getAutostart());
    }
    delete dlgEdit;
}
Project* MainWindow::addProject(int id, QString name, qint64 total, int hoursePerDay, QString keySequence, bool autostart )
{
    QVBoxLayout *layout =static_cast <QVBoxLayout *>(ui->frame->layout());
    Project *project = new Project(id, name, total, hoursePerDay,keySequence, autostart, this);
    layout->insertWidget(0,project,0,Qt::AlignTop);
    connect(project,SIGNAL(running()),this,SLOT(onRunning()));
    connect(this,SIGNAL(newRunning()),project,SLOT(onRunningSend()));
    connect(project,SIGNAL(setIconToolTip(QString,QString)),this,SLOT(onSetIconToolTip(QString,QString)));
    return project;
}

void MainWindow::onRunning()
{
    emit newRunning();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,tr("About %1").arg(qApp->applicationName()),
                       tr("%1 - small program to account for time spent on projects<br>Version: %2<br>"
                          "Copyright 2012 Alexander Vorobyev (Voral)<br>License: GNU/GPL<br>Author: Alexander Vorobyev<br>Site: http://va-soft.ru/")
                       .arg(qApp->applicationName())
                       .arg(qApp->applicationVersion()));
}
