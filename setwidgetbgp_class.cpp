#include "setwidgetbgp_class.h"

SetWidgetBGP_Class::SetWidgetBGP_Class()
{
}

void SetWidgetBGP_Class::SetWidgetBGP(QWidget *widObject, QImage imgBGP, int iWidth, int iHeight)
{
    widObject->setAutoFillBackground(true);
    QPalette paletteBGP;
    paletteBGP.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(imgBGP).scaled(iWidth, iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    widObject->setPalette(paletteBGP);
}

void SetWidgetBGP_Class::SetWidgetBGP(QWidget *widObject, int iRed, int iGreen, int iBlue)
{
    widObject->setAutoFillBackground(true);
    QPalette paletteBGP;
    paletteBGP.setColor(QPalette::Window, QColor(iRed, iGreen, iBlue));
    widObject->setPalette(paletteBGP);
}

void SetWidgetBGP_Class::SetLabBGP(QLabel *labObject, QImage imgBGP, int iWidth, int iHeight)
{
    labObject->setPixmap(QPixmap::fromImage(imgBGP).scaled(iWidth, iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void SetWidgetBGP_Class::SetLabBGP(QLabel *labObject, QString strBGP, int iWidth, int iHeight)
{
    QImage imgBGP(strBGP);
    labObject->setPixmap(QPixmap::fromImage(imgBGP).scaled(iWidth, iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void SetWidgetBGP_Class::SetBtnBGP(QPushButton *btnObject, QString strBGP)
{
    QString strStyle = "QPushButton {border-image: url(:android/" + strBGP + ".png);}";
    strStyle += "QPushButton:hover{border-image: url(:android/" + strBGP + ".png);}";
    strStyle += "QPushButton:hover:pressed{border-image: url(:android/" + strBGP + "Press.png);}";
    btnObject->setStyleSheet(strStyle);
}

void SetWidgetBGP_Class::SetBtnBGP(QPushButton *btnObject, QString strBGP, bool bPresss)
{
    QString strStyle = "QPushButton {border-image: url(:android/" + strBGP + ".png);}";
    strStyle += "QPushButton:hover{border-image: url(:android/" + strBGP + ".png);}";
    strStyle += "QPushButton:hover:pressed{border-image: url(:android/" + strBGP + ".png);}";
    btnObject->setStyleSheet(strStyle);
}

void SetWidgetBGP_Class::SetBtnBGP(QPushButton *btnObject, QImage imgBGP, int iWidth, int iHeight)
{
    QIcon iconBGP;
    iconBGP.addPixmap(QPixmap::fromImage(imgBGP).scaled(iWidth, iHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    btnObject->setIcon(iconBGP);
    btnObject->setIconSize(QSize(iWidth, iHeight));
    btnObject->setFlat(1);
}
