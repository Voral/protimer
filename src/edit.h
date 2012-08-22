#ifndef EDIT_H
#define EDIT_H

#include <QDialog>

namespace Ui {
class Edit;
}

class Edit : public QDialog
{
    Q_OBJECT
    
public:
    explicit Edit(QString pName, int pHours, QWidget *parent = 0);
    ~Edit();
    QString getName();
    int getHours();
    
private:
    Ui::Edit *ui;
};

#endif // EDIT_H
