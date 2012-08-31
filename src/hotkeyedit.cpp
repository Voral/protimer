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
#include "hotkeyedit.h"
#include "ui_hotkeyedit.h"
#include <QKeyEvent>
#include <QShortcut>

HotKeyEdit::HotKeyEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HotKeyEdit)
{
    ui->setupUi(this);
}

HotKeyEdit::~HotKeyEdit()
{
    delete ui;
}
void HotKeyEdit::keyPressEvent ( QKeyEvent * event )
{
    if (!ui->btEditMode->isChecked()) return;
    if ((event->key()>=Qt::Key_Escape)&&(event->key()<=Qt::Key_ScrollLock)  ) return;
    if (event->key()==-1) return;
    QKeySequence keyS(event->key() | event->modifiers());
    ui->edHotkey->setText(keyS.toString());
}
QKeySequence HotKeyEdit::getKeySequence()
{
    return QKeySequence(ui->edHotkey->text());
}

void HotKeyEdit::on_btEditMode_toggled(bool checked)
{
    if (checked) ui->edHotkey->setFocus();
}
void HotKeyEdit::setText(QString text)
{
    ui->edHotkey->setText(text);
}
