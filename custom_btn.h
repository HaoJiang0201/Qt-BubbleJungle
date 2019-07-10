#ifndef CUSTOM_BTN_H
#define CUSTOM_BTN_H

#include <QPushButton>
#include <QMouseEvent>

class Custom_Btn : public QPushButton
{
    Q_OBJECT
public:
    explicit Custom_Btn(QWidget *parent = 0);

    int m_iRow;    // Custom Button Row Index
    int m_iCol;    // Custom Button Column Index

    void mouseReleaseEvent (QMouseEvent *event);

signals:
    void sigBtnClick(int iRow, int iCol);

public slots:

};

#endif // CUSTOM_BTN_H
