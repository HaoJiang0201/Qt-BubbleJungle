#include "custom_btn.h"

Custom_Btn::Custom_Btn(QWidget *parent) :
    QPushButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setMouseTracking(1);
    setVisible(1);    // For custom widget, the default of visible is false
    m_iRow = -1;
    m_iCol = -1;
}

void Custom_Btn::mouseReleaseEvent (QMouseEvent *event)
{
    emit sigBtnClick(m_iRow, m_iCol);    // The Original QPushButton's "clicked()" Signal is not Effective, so Rewrite the Button Click Signal
}

