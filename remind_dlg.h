#ifndef REMIND_DLG_H
#define REMIND_DLG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QMessageBox>
#include "all_define.h"

namespace Ui {
class Remind_Dlg;
}

class Remind_Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit Remind_Dlg(QWidget *parent = 0);
    ~Remind_Dlg();

    /****** Member Functions ******/
    void InitVariable();    // Initialize Member Variables
    void InitWidget();     // Initialize Widgets
    void SetWidget(int iDlgType);    // Set Different Dialog, Called by Other Classes
    void SetBGP(QString strBGP);    // Set Dialog BGP
    void PlayMusic(bool bType, QString strName);    // Play Music or Sound

signals:
    void sigAccept();
    void sigReject();

private slots:
    void on_btnConfirm_clicked();
    void on_btnBack_clicked();

private:
    Ui::Remind_Dlg *ui;
};

#endif // REMIND_DLG_H
