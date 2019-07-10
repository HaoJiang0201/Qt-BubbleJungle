#ifndef RECORD_WIDGET_H
#define RECORD_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QMessageBox>
#include <QMediaPlayer>
#include "all_define.h"

namespace Ui {
class Record_Widget;
}

class Record_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Record_Widget(QWidget *parent = 0);
    ~Record_Widget();

    /****** Member Functions ******/
    void InitVariable();    // Initialize Member Variables
    void InitWidget();     // Initialize Widgets
    void SetRecordLabel();    // Set Record Label BGP
    void PlayMusic(bool bType, QString strName);    // Play Music or Sound
    QString TimeConvert(int iSec);    // Convert Second Time Format to Hour:Min:Sec Format

    /****** Member Variables & Widgets ******/
    int m_iCurRecord;
    QLabel *m_labGalleryPic[GALLERY_NUM];
    QLabel *m_labRecordText[4];    // Record Text (Survive Model Time Record)
    //    QLabel *m_labBottom;

signals:
    void sigRecordClose(int iUI);

private slots:
    void on_btnBack_clicked();
    void on_btnLeft_clicked();
    void on_btnRight_clicked();

private:
    Ui::Record_Widget *ui;
};

#endif // RECORD_WIDGET_H
