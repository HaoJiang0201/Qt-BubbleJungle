#ifndef JUNGLEBUBBLE_WIDGET_H
#define JUNGLEBUBBLE_WIDGET_H

#include <QWidget>
//#include <QCoreApplication>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QMediaPlayer>
#include <QMessageBox>
#include <time.h>

#include "custom_btn.h"
#include "remind_dlg.h"
#include "all_define.h"

typedef struct
{
    //QPushButton *btnBubble;    // Bubble for Clicking
    Custom_Btn *btnBubble;    // Custom Bubble for Clicking
    QLabel *labAnimateMask;    // Bubble Mask for Displaying Animate
    int iColor;    // Bubble Color (BGP) 0~7
    bool bCheck;    // The Check Sign when Finding out Same Color Bubbles
} BUBBLE;   //  Bubble Property

namespace Ui {
class JungleBubble_Widget;
}

class JungleBubble_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit JungleBubble_Widget(QWidget *parent = 0);
    ~JungleBubble_Widget();

    /****** Member Functions ******/
    // Basic Function
    void InitVariable();    // Initialize Member Variables
    void InitWidget();     // Initialize Widgets
    void SetWidget();    // Set Widgets Properties
    // Game Main Algorithm Function
    void InitBasicParm();    // Initial Basic Parameters
    void InitNewBubbleRelated();    // Initial New Bubble Related
    void InitBubbleCheckSign();    // Initial Bubble Check Sign
    void InitSameBubbleRecord();    // Initial Record of Bubble in the Same Color
    void InitFallBubbleRecord();    // Initial Record of Bubble need to be Fall Down
    void InitEmptyBubbleColRecord();   // Initial Record of Empty Bubble Column
    void GenerateNewBubble();    // Generate New Bubble in Different Color Radomly
    void GenerateDefaultBubbleWall();    // Generate Default Bubble Wall
    void FindSameColorBubble(int iRow, int iCol);    // Find out Same Color Bubbles
    void FindFallDownBubble();    // Find out Bubble need to be Fall Down
    void FindEmptyBubbleColumn();    // Fine out Empty BubbleColumn
    void AllBubbleMoveUp();    // All Bubbles Move Up after a Whole New Line of Bubbles are Generated
    void RemoveBubble();    // Remove Same Color Bubbles Usr Clicked
    void BubbleFallDown();    // The Bubbles Fall Down after Some of the Bubbles Below Them has been Removed
    void BubbleColumnMove();    // Adjust Empty Bubble Column
    void GameEnd(bool bSuccess);    // Game End
    void ScoreEnd(bool bSuccess);    // Score Model Game End
    void SurviveEnd();    // Survive Model Game End
    void KingEnd(bool bSuccess);    // King Model Game End
    void GameRestart();    // Restart Current Game
    void UpdateRecord();    // Update Record List Ini File
    // Assistant Function
    void LevAdjust(int iLev);    // Adjust Game Parms According to the Level Selected
    int CalculateScore();    // Calculate Score after Click the Bubble (Score Model Only)
    void CalculateTime();    // Calculate Time after Click the Bubble (Survive Model Only)
    int CalculateLove();    // Calculate Love after Click the Bubble (King Model Only)
    void PaintBubble(int iRow, int iCol, int iColor);    // Paint Bubble Based on the Color index
    void PaintAnimateLabel(QLabel *labObj, int iColor);  // , int iWidth, int iHeight);    // Paint Bubble's Animate Label
    void SetBubbleBGP(Custom_Btn *btnObject, QString strPicName, int iWidth, int iHeight);    // Set Bubble Button's BGP
    QString TimeConvert(int iSec);    // Convert Second Time Format to Hour:Min:Sec Format
    void PlayMusic(bool bType, QString strName);    // Play Music or Sound

    /****** Member Variables & Widgets ******/
    int m_iBubbleSize;    // Bubble's Size on Device
    int m_iCurScore;    // Current Score for Score Model
    int m_iCurTime;    // Current Time for Survive Model
    int m_iCurLove;    // Current Love for King Model
    int m_iGameStatus;    // Game Status
    bool m_bClick;    // Click Event Control
    bool m_bPause;    // Game Pause Control
    // Main Bubble Related
    BUBBLE m_bubbleClick[GAME_BUBBLE_ROW_NUM][GAME_BUBBLE_COL_NUM];    // Click Bubble Structure
    QTimer *m_timerBubbleVanish;    // Bubble Collect QTimer
    QTimer *m_timerBubbleFall;    // Bubble Fall QTimer
    QTimer *m_timerSurvive;    // Time Count for Survive Model
    int m_iCurClickColor;    // The Clicked Bubble's Color
    int m_iSameBubbleCount;    // Same Color Bubble Number
    int m_iSameBubbleRowIdx[GAME_BUBBLE_NUM];    // Same Color Bubble Row Index
    int m_iSameBubbleColIdx[GAME_BUBBLE_NUM];    // Same Color Bubble Column Index
    int m_iFallBubbleCount;    // Fall Bubble Number
    int m_iFallBubbleRowIdx[GAME_BUBBLE_NUM];    //Fall Bubble Row Index
    int m_iFallObjRowIdx[GAME_BUBBLE_NUM];    //Fall Bubble Object Position Row Index
    int m_iFallBubbleColIdx[GAME_BUBBLE_NUM];    //Fall Bubble Column Index
    int m_iFallBubbleColor[GAME_BUBBLE_NUM];    //Fall Bubble Object Position Column Index
    int m_iLastEmptyCol;    // The Sidest Empty Coloum
    int m_iEmptyBubbleCol[GAME_BUBBLE_COL_NUM];    // Empty Bubble Column Index
    // New Bubble Related
    QLabel *m_labNewBubble[GAME_BUBBLE_COL_NUM];    // New Bubbles' Labels
    QTimer *m_timerGenerateBubble;    // Generate New Bubble QTimer
    int m_iNewBubbleCount;    // Current Visible New Bubble Number
    int m_iNewBubbleColor[GAME_BUBBLE_COL_NUM];    // New Bubbles' Color Index
    // Assistance Labels
    QLabel *m_labScore;    // Calculate Score Animate Label
    //QLabel *m_labDebug;
    //QLabel *m_labBottom;    // Bottom Grass Label

signals:
    void sigBubbleVanishEnd();    // Bubble Collect Animate Finish Slot
    void sigBubbleFallEnd();    // Bubble Fall Animate Finish Slot
    void sigGameOver();    // Game Over Slot

private:
    Ui::JungleBubble_Widget *ui;

public slots:
    void slot_GenerateBubble();    // Generate New Bubble Slot
    void slot_BubbleMoveUpEnd();    // Bubble Move Up Time End Slot
    void slot_BubbleVanishEnd();    // Bubble Vanish Time End Slot
    void slot_BubbleFallEnd();    // Bubble Fall Time End Slot
    void slot_BubbleMoveColumnEnd();    // Bubble Column Move Slot
    void slot_BubbleClick(int iRow, int iCol);    // Bubble Click Slot
    void slot_TimeSurvive();    // Survive Time Slot

private slots:
    void on_btnPause_clicked();    // Pause Button Slot
    void on_btnBack_clicked();    // Back Button Slot
};

#endif // JUNGLEBUBBLE_WIDGET_H
