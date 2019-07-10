#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMediaPlayer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QMessageBox>
#include <time.h>

#include "math.h"
#include "remind_dlg.h"
#include "record_widget.h"
#include "help_widget.h"
#include "gallery_widget.h"
#include "junglebubble_widget.h"
#include "all_define.h"

namespace Ui {
class Main_Window;
}

class Main_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main_Window(QWidget *parent = 0);
    ~Main_Window();

    /****** Member Functions ******/
    void InitVariable();    // Initialize Member Variables
    void InitWidget();     // Initialize Widgets
    void CopyMusicFile();    // Copy Music Files from QT "qrc" to Local of Device
    void LoadRecordList();    // Load Record List File
    void PlayMusic(bool bType, QString strName);    // Play Music or Sound
    void FitGameAreaSize();    // Calculate Screen Size of the Device and Adjust the App's Window Size
    //void SetLabBG(QLabel *labObject, QString strPicName);
    //void SetBtnBG(QPushButton *btnObject, QString strPicName, bool bPress);    // Set Button's BGP
    void SetBubbleLabelBG(QLabel *labObj, int iColor);    // Set Bubble Label BGP
    void GetBubbleDirection(int &iPosX, int &iPosY, int iDirection);    // Get the Buttons' Futhure Position (where to move)
    void ShowUI(int iUIIdx);    // 0: Starting UI, 1: Model Select UI, 2: Role & Lev Select UI
    void LevAdjust(int iLev);    // Adjust Game Parms According to the Level Selected
    //void GameStart(int iRole);    // Start the Jungle Bubble Game after Selecting a Role
    void Interlude(int iUI);

    /****** Member Variables & Widgets ******/
    // Window Basic Control
    int m_iWinMoveX;    // App Window Position in X Direction after Adjust (not using)
    int m_iWinMoveY;    // App Window Position in Y Direction after Adjust (not using)
    int m_iCurUI;    // UI Index
    int m_iCloseUI;    // Last Closed UI Index
    // Animate Cloud & Bubbles
    QTimer *m_timerCloud;    // Timer controls the Cloud's move
    QLabel *m_labBubble[8];    // Animate Bubbles, Move Randomly
    QTimer *m_timerBubble;    // Timer controls all the Bubbles' move
    int m_iDirection[8];    // 0: Up, 1: Down, 2: Left, 3: Right, 4: Up&Left, 5: Up&Right, 6: Down&Left, 7: Down&Right
    int m_iChangeDirectionTime;    // Time Count of Bubbles' Move before their Direction Changed
    // Member Widgets and Dialogs
    Record_Widget *m_widRecordBoard;    // Record Borad Widget
    Help_Widget *m_widHelp;    // Game Help Widget
    Gallery_Widget *m_widGallery;    // Gallery Widget
    Remind_Dlg *m_dlgRemind;    // Remind Info. Dialog
    JungleBubble_Widget *m_widGameStart;    // Jungle Bubble Game Main Widget
    // Assistant Label
//    QLabel *m_labBottom;    // Bottom Cover Grass Label
//    QLabel *m_labCover;      // ???
//    QLabel *m_labInterlude;    // Interlude Animate Label
    QTimer *m_timerInterlude;    // Interlude QTimer
    QGraphicsScene m_sceneInterlude;    // Interlude Sence
    QLabel *m_labInterludeBubbleForward[INTERLUDE_BUBBLE_ROW][INTERLUDE_BUBBLE_COL];
    QLabel *m_labInterludeBubbleBack[INTERLUDE_BUBBLE_ROW+1][INTERLUDE_BUBBLE_COL+1];
    QLabel *m_labelDebug;    // Debug Label

public slots:
    void slot_CloudTimer();    // Cloud Move slot
    void slot_BubbleTimer();    // Bubbles Move slot
    void slot_BGMStop(QMediaPlayer::State);    // Music Loop Slot
    void slot_GameOver();    // Game Over Slot
    void slot_InfoClose(int iUI);    // Info UI(Record & Help UI) Close Slot
    void slot_InterludeHalf();    // Interlude Slot
    void slot_InterludeFinish();    // Interlude Finish Slot

private slots:
    void on_btnModelSelect_clicked();
    void on_btnRecord_clicked();
    void on_btnHelp_clicked();
    void on_btnSetting_clicked();
    void on_btnBack_clicked();
    void on_btnSelectXS_clicked();
    void on_btnSelectZJ_clicked();
    void on_btnSelectYH_clicked();
    void on_btnSelectDC_clicked();
    void on_btnScoreModel_clicked();
    void on_btnSurviveModel_clicked();
    void on_btnKingModel_clicked();
    void on_btnEasy_clicked();
    void on_btnNormal_clicked();
    void on_btnHard_clicked();
    void on_btnGallery_clicked();

private:
    Ui::Main_Window *ui;
};

#endif // MAIN_WINDOW_H
