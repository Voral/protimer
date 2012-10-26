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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}
class Project;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setVisible(bool visible);

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    bool validateName(QString name);
    Project *addProject(int id, QString name, qint64 total=0, int hoursePerDay=24, QString keySequence="", bool autostart = false);
    void createTrayIcon();
    QAction *quitAction;
    QMenu *trayIconMenu;


public slots:
    void addProject();
    void onRunning();
    void onSetIconToolTip(const QString text,const QString name);
signals:
    void newRunning();
private slots:
    void on_actionAbout_triggered();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
