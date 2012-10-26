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
#include "edit.h"
#include "ui_edit.h"
#include "hotkeyedit.h"

Edit::Edit(int id, QString pName, int pHours, QString pKeySequence, bool autostart, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit)
{
    ui->setupUi(this);
    ui->edHour->setValue(pHours);
    ui->edName->setText(pName);
    ui->chAutostart->setChecked(autostart);
    if (id>-1) setWindowTitle(QString("%1 #%2").arg(windowTitle()).arg(id));
    if (!pKeySequence.isEmpty()) ui->edHotKey->setText(pKeySequence);
    ui->lbHours->adjustSize();
    adjustSize();
    setFixedWidth(width());
    setFixedHeight(height());
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

QKeySequence Edit::getKeySequence()
{
    return ui->edHotKey->getKeySequence();
}

bool Edit::getAutostart()
{
    return ui->chAutostart->isChecked();
}
