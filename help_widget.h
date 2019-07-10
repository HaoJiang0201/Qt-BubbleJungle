#ifndef HELP_WIDGET_H
#define HELP_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMediaPlayer>

#include "all_define.h"

namespace Ui {
class Help_Widget;
}

class Help_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Help_Widget(QWidget *parent = 0);
    ~Help_Widget();

    /****** Member Functions ******/
    void InitVariable();    // Initialize Member Variables
    void InitWidget();     // Initialize Widgets
    //void SetBtnBG(QPushButton *btnObject, QString strPicName);    // Set Button's BGP
    void PlayMusic(bool bType, QString strName);    // Play Music or Sound

    /****** Member Variables & Widgets ******/
    int m_iCurPage;    // Current Help Page (Totally Four)
//    QLabel *m_labBottom;    // Bottom Grass Label

signals:
    void sigHelpClose(int iUI);    // Gallery Widget Close Signal

private:
    Ui::Help_Widget *ui;

private slots:
    void on_btnLastPage_clicked();    // Last Page Slot (Previous Page)
    void on_btnNextPage_clicked();    // Next Page Slot
    void on_btnBack_clicked();    //Back Slot
};

#endif // HELP_WIDGET_H
