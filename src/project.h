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
#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QTimer>
#include <QTime>

namespace Ui {
class Project;
}

class Project : public QWidget
{
    Q_OBJECT

public:
    explicit Project(QString name,qint64 total=0, QWidget *parent = 0);
    ~Project();
    QString getName();
    void stop();
    qint64 getTotal();

private:
    Ui::Project *ui;
    qint32 total;
    bool isSender;
    QTime current;
    QTimer timer;
signals:
    void running();
public slots:
    void onRunningSend();

private slots:
    void updateTimeLabel(bool wCurrent = true);
    void on_btRun_toggled(bool checked);
};

#endif // PROJECT_H
