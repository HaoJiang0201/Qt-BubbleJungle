#ifndef SETWIDGETBGP_CLASS_H
#define SETWIDGETBGP_CLASS_H

#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QImage>

class SetWidgetBGP_Class
{
public:
    SetWidgetBGP_Class();
    void SetWidgetBGP(QWidget *widObject, QImage imgBGP, int iWidth, int iHeight);
    void SetWidgetBGP(QWidget *widObject, int iRed, int iGreen, int iBlue);
    void SetLabBGP(QLabel *labObject, QImage imgBGP, int iWidth, int iHeight);
    void SetLabBGP(QLabel *labObject, QString strBGP, int iWidth, int iHeight);
    void SetBtnBGP(QPushButton *btnObject, QString strBGP);
    void SetBtnBGP(QPushButton *btnObject, QString strBGP, bool bPress);
    void SetBtnBGP(QPushButton *btnObject, QImage imgBGP, int iWidth, int iHeight);
};

#endif // SETWIDGETBGP_CLASS_H
