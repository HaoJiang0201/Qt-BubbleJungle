#ifndef GALLERY_WIDGET_H
#define GALLERY_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QImage>
#include <QMediaPlayer>
#include <QMouseEvent>

#include "math.h"
#include "all_define.h"

namespace Ui {
class Gallery_Widget;
}

class Gallery_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Gallery_Widget(QWidget *parent = 0);
    ~Gallery_Widget();

    /****** Member Functions ******/
    void InitVariable();    // Initialize Member Variables
    void InitWidget();     // Initialize Widgets
    void InitImgList();    // Initialize Image File List
    void GoNextImage();    // Show Next Picture
    void GoPrevImage();    // Show Previous Picture
    void SetBGP();    // Set Widget BGP
    //void SetBtnBG(QPushButton *btnObject, QString strPicName);    // Set Button's BGP
    void PlayMusic(bool bType, QString strName);    // Play Music or Sound

    /****** Member Variables & Widgets ******/
    QString m_strImgFile[GALLERY_NUM];    // Image File Name List
    QTimer *m_timerAnimate;    // Swip Animate QTimer
    int m_iImgIdx;    // Image Index
    bool m_bPressing;    // Pressing Sign
    bool m_bMoving;    // Moving Sign
    int m_iOrgX;    // Original X Position When First Touching
    int m_iOrgY;    // Original Y Position When First Touching
    int m_iDistance;    // Distance Between First Touch Point & Release Point
    //QLabel *m_labBottom;    // Bottom Grass Label

private:
    Ui::Gallery_Widget *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);

signals:
    void sigGalleryClose(int iUI);    // Gallery Widget Close Signal

private slots:
    void on_btnBack_clicked();    //Back Slot
    void slot_Moving();    // Picture Changing Slot
    void slot_MovingFinish();    // Picture Moving Finished Slot
};

#endif // GALLERY_WIDGET_H
