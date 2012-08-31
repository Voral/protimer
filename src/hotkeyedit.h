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
#ifndef HOTKEYEDIT_H
#define HOTKEYEDIT_H

#include <QWidget>

namespace Ui {
class HotKeyEdit;
}

class HotKeyEdit : public QWidget
{
    Q_OBJECT

public:
    explicit HotKeyEdit(QWidget *parent = 0);
    ~HotKeyEdit();
    QKeySequence getKeySequence();
    void setText(QString text);
protected:
    void keyPressEvent ( QKeyEvent * event );
private slots:
    void on_btEditMode_toggled(bool checked);

private:
    Ui::HotKeyEdit *ui;
};

#endif // HOTKEYEDIT_H
