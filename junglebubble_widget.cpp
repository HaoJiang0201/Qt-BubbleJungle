#include "junglebubble_widget.h"
#include "ui_junglebubble_widget.h"

extern int g_iWinW;    // App Window Width after Adjust
extern int g_iWinH;    // App Window Height after Adjust
extern double g_dSizeRatio;    // The Width-Height Ratio required to be adjusted for all the Widgets in App
extern QTextCodec *g_codec;    // Chinese Codec
extern QString g_strSourcePath;    // Source Files' Path in Device

extern int g_iModel;    // 0: Normal Model, 1: Live Model, 2: Free Model, 3: King Model (Hide Model)
extern int g_iBubbleTypeLev;    // Bubble Type Number Level: 4, 6, 8
extern int g_iBubbleHeight;    // Bubble Wall Default Height Level: 2, 4, 6
extern int g_iSpeedLev;    // Bubble Generate Speed Level: 1-fast; 5-normal; 10-slow
extern int g_iScoreLimit;    // lev1-500, lev2-1000, lev3-2000
extern int g_iLoveLimit;    // lev1-500, lev2-1000, lev3-2000
extern int g_iCurLev;    // Game Level Selected
extern int g_iRole;    // Four Roles
extern RECORD g_recordGame;    // Game Record Structure

extern bool g_bSurviveUnlock;    // Survive Model Unlock Sign
extern bool g_bKingUnlock;    // King Model Unlock Sign

extern QMediaPlayer *g_playerBGM;    // Game Background Music, Loop
extern QMediaPlayer *g_playerSound;    // All kinds of Sound, Only Play for One Time, such as Button Click Sound

extern SetWidgetBGP_Class g_classSetBGP;    // Set Widget BGP

JungleBubble_Widget::JungleBubble_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JungleBubble_Widget)
{
    ui->setupUi(this);
    InitVariable();
    InitWidget();
}

JungleBubble_Widget::~JungleBubble_Widget()
{
    delete ui;
}

void JungleBubble_Widget::InitVariable()
{
    /****** Init Member Varaibles and Stuctures ******/
    InitBasicParm();
    InitSameBubbleRecord();
    InitFallBubbleRecord();
    InitEmptyBubbleColRecord();
    InitNewBubbleRelated();
}

void JungleBubble_Widget::InitWidget()
{
    /****** Widget Basic ******/
    setGeometry(0, 0, g_iWinW, g_iWinH);

    /****** Generate New Bubble QTimer & Survive Model QTimer ******/
    m_timerGenerateBubble = new QTimer(this);
    m_timerSurvive = new QTimer(this);

    /****** Contorl Btn ******/
    int iBtnSize = (int)(CONTROL_BTN_SIZE*g_dSizeRatio+0.5);
    ui->btnPause->setGeometry((int)(PAUSE_BTN_X*g_dSizeRatio+0.5), (int)(CONTROL_BTN_Y*g_dSizeRatio+0.5), iBtnSize,iBtnSize);
    ui->btnBack->setGeometry((int)(BACK_BTN_X*g_dSizeRatio+0.5),  (int)(CONTROL_BTN_Y*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    g_classSetBGP.SetBtnBGP(ui->btnPause, "PauseBtn");
    g_classSetBGP.SetBtnBGP(ui->btnBack, "BackBtn");

    /****** Cover Bar Label ******/
    // Left Cover Bar Label
    ui->labLeftCoverBar->setGeometry(0, 0, (int)(COVER_BAR_VERTICAL_W*g_dSizeRatio+0.5), g_iWinH);
    // Right Cover Bar Label
    ui->labRightCoverBar->setGeometry(g_iWinW-(int)(COVER_BAR_VERTICAL_W*g_dSizeRatio+0.5), 0, (int)(COVER_BAR_VERTICAL_W*g_dSizeRatio+0.5), g_iWinH);
    // Top Cover Bar Label
    ui->labTopCoverBar->setGeometry(0, 0, g_iWinW, (int)(COVER_BAR_TOP_H*g_dSizeRatio+0.5));
    // Bottom Cover Bar Label
    ui->labBottomCoverBar->setGeometry(0, g_iWinH-(int)(COVER_BAR_BOTTOM_H*g_dSizeRatio+0.5), g_iWinW, (int)(COVER_BAR_BOTTOM_H*g_dSizeRatio+0.5));

    /****** Bubble Related ******/
    int iNewBubblePosY = (GAME_BUBBLE_ROW_NUM+1)*GAME_BUBBLE_SIZE + COVER_BAR_W;
    for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
    {
        // New Bubble Labels
        m_labNewBubble[j] = new QLabel(this);
        int iPosX = GAME_BUBBLE_X + j*GAME_BUBBLE_SIZE;
        m_labNewBubble[j]->setGeometry((int)(iPosX*g_dSizeRatio+0.5), (int)(iNewBubblePosY*g_dSizeRatio+0.5), m_iBubbleSize, m_iBubbleSize);
        // Click Bubble Buttons and Mask Labels
        for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
        {
            int iPosY = (i+1)*GAME_BUBBLE_SIZE;
            // Animate Mask Label
            m_bubbleClick[i][j].labAnimateMask = new QLabel(this);
            m_bubbleClick[i][j].labAnimateMask->setGeometry((int)(iPosX*g_dSizeRatio+0.5), (int)(iPosY*g_dSizeRatio+0.5), m_iBubbleSize, m_iBubbleSize);
            PaintAnimateLabel(m_bubbleClick[i][j].labAnimateMask, -1);  //, 0, 0);
            // Click Bubble
            m_bubbleClick[i][j].iColor = -1;
            m_bubbleClick[i][j].bCheck = 0;
            m_bubbleClick[i][j].btnBubble = new Custom_Btn(this);
            m_bubbleClick[i][j].btnBubble->m_iRow = i;
            m_bubbleClick[i][j].btnBubble->m_iCol = j;
            m_bubbleClick[i][j].btnBubble->setGeometry((int)(iPosX*g_dSizeRatio+0.5), (int)(iPosY*g_dSizeRatio+0.5), m_iBubbleSize, m_iBubbleSize);
            m_bubbleClick[i][j].btnBubble->setVisible(0);
            connect(m_bubbleClick[i][j].btnBubble, SIGNAL(sigBtnClick(int, int)), this, SLOT(slot_BubbleClick(int, int)));
            PaintBubble(i, j, -1);
        }    // for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
    }    // for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)

    /****** Mark & Record Label ******/
    int iX = (int)((MARK_LAB_W+MARK_LAB_X)*g_dSizeRatio+0.5);
    int iY = (int)(RECORD_LAB_Y*g_dSizeRatio+0.5);
    int iW = (int)(RECORD_LAB_W*g_dSizeRatio+0.5);
    int iH = (int)(RECORD_LAB_H*g_dSizeRatio+0.5);
    ui->labMarkText->setGeometry((int)(MARK_LAB_X*g_dSizeRatio+0.5), (int)(MARK_LAB_Y*g_dSizeRatio+0.5), (int)(MARK_LAB_W*g_dSizeRatio+0.5), (int)(MARK_LAB_H*g_dSizeRatio+0.5));
    ui->labRecord->setGeometry(iX, iY, iW, iH);
    m_labScore = new QLabel(this);
    m_labScore->setGeometry(iX, iY, iW, iH);
    QFont fontText;
    fontText.setFamily("Timer");
    fontText.setBold(1);
    fontText.setPointSize(RECORD_FONT_SIZE);
    ui->labRecord->setFont(fontText);
    fontText.setPointSize(RECORD_FONT_SIZE+5);
    m_labScore->setFont(fontText);
    m_labScore->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QPalette paletteText;
    paletteText.setColor(QPalette::WindowText,QColor(230, 30, 30));
    paletteText.setColor(QPalette::ButtonText, QColor(230, 30, 30));
    paletteText.setColor(QPalette::Text, QColor(230, 30, 30));
    m_labScore->setPalette(paletteText);
    m_labScore->setVisible(0);

    /****** Raise Cover Label & Control Btn ******/
    ui->labLeftCoverBar->raise();
    ui->labRightCoverBar->raise();
    ui->labTopCoverBar->raise();
    ui->labBottomCoverBar->raise();
    ui->labMarkText->raise();
    ui->labRecord->raise();
    ui->btnPause->raise();
    ui->btnBack->raise();

    /****** Bottom Cover Label & Pause Cover Label ******/
    ui->labCover->setGeometry(0, -g_iWinH, g_iWinW, g_iWinH);
    g_classSetBGP.SetLabBGP(ui->labCover, ":android/WhiteCover.png", g_iWinW, g_iWinH);
    ui->labCover->setVisible(0);
    ui->labCover->raise();

//    m_labBottom = new QLabel(this);
//    iX = (int)(-1*BOTTOM_COVER_X*g_dSizeRatio+0.5);
//    int iY = g_iWinH - (int)(BOTTOM_COVER_Y*g_dSizeRatio+0.5);
//    iW = g_iWinW + 2*(int)(BOTTOM_COVER_X*g_dSizeRatio+0.5);
//    iH = (int)(BOTTOM_COVER_H*g_dSizeRatio+0.5);
//    m_labBottom->setGeometry(iX, iY, iW, iH);
//    g_classSetBGP.SetLabBGP(m_labBottom, ":android/BottomLabel.png", iW, iH);
}

void JungleBubble_Widget::SetWidget()
{
    InitVariable();

    /****** Main BGP ******/
    int iPicIdx;
    if(g_iCurLev >= g_recordGame.iKingLev[g_iRole])
        iPicIdx = g_recordGame.iKingLev[g_iRole];
    else
        iPicIdx = g_iCurLev;
    QImage imgBGP(":android/GameBGP" + QString::number(g_iRole) + QString::number(iPicIdx) + ".jpg");
    g_classSetBGP.SetWidgetBGP(this, imgBGP, g_iWinW, g_iWinH);

    /****** Contorl Btn ******/
    g_classSetBGP.SetBtnBGP(ui->btnPause, "PauseBtn");

    /****** Cover Bar Label ******/
    // Left Cover Bar Label
    g_classSetBGP.SetLabBGP(ui->labLeftCoverBar, ":android/LeftCoverBar" + QString::number(g_iRole) + ".png", (int)(COVER_BAR_VERTICAL_W*g_dSizeRatio+0.5), g_iWinH);
    g_classSetBGP.SetLabBGP(ui->labRightCoverBar, ":android/RightCoverBar" + QString::number(g_iRole) + ".png", (int)(COVER_BAR_VERTICAL_W*g_dSizeRatio+0.5), g_iWinH);
    g_classSetBGP.SetLabBGP(ui->labTopCoverBar, ":android/TopCoverBar" + QString::number(g_iRole) + ".png", g_iWinW, (int)(COVER_BAR_TOP_H*g_dSizeRatio+0.5));
    g_classSetBGP.SetLabBGP(ui->labBottomCoverBar, ":android/BottomCoverBar" + QString::number(g_iRole) + ".png", g_iWinW, (int)(COVER_BAR_BOTTOM_H*g_dSizeRatio+0.5));

    /****** Bubble Related ******/
    for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
    {
        m_labNewBubble[j]->setVisible(0);
        for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
        {
            m_bubbleClick[i][j].btnBubble->setVisible(0);
            m_bubbleClick[i][j].labAnimateMask->setVisible(0);
            m_bubbleClick[i][j].iColor = -1;
            m_bubbleClick[i][j].bCheck = 0;
            PaintBubble(i, j, -1);
        }
    }
    GenerateDefaultBubbleWall();

    /****** Mark & Record Label ******/
    QString strMarkText;
    switch(g_iModel)
    {
        case SCORE_MODEL :
            strMarkText = "ScoreMark";
            ui->labRecord->setText(QString::number(m_iCurScore) + " / " + QString::number(g_iScoreLimit));
            break;
        case SURVIVE_MODEL :
            strMarkText = "TimeMark";
            ui->labRecord->setText("00:00:00");
            connect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
            break;
        case KING_MODEL :
            strMarkText = "LoveMark";
            ui->labRecord->setText(QString::number(m_iCurLove) + " / " + QString::number(g_iLoveLimit));
            break;
    }

    g_classSetBGP.SetLabBGP(ui->labMarkText, ":android/" + strMarkText + ".jpg",  (int)(MARK_LAB_W*g_dSizeRatio+0.5), (int)(MARK_LAB_H*g_dSizeRatio+0.5));

    /****** Play Music ******/
    if(g_iCurLev == 0)
        PlayMusic(MUSIC_TYPE, "Role" + QString::number(g_iRole) + ".mp3");
    else
        PlayMusic(MUSIC_TYPE, "Lev" + QString::number(g_iCurLev) + ".mp3");
    connect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
}

void JungleBubble_Widget::InitBasicParm()
{
    m_iBubbleSize = (int)(GAME_BUBBLE_SIZE*g_dSizeRatio+0.5);
    m_iCurScore = 0;
    m_iCurTime = 0;
    m_iCurLove = 0;
    m_bClick = 0;
    m_bPause = 0;
    m_iGameStatus = GAMING;
}

void JungleBubble_Widget::InitNewBubbleRelated()
{
    m_iNewBubbleCount = 0;
    for(int i=0; i< GAME_BUBBLE_COL_NUM; i++)
        m_iNewBubbleColor[i] = -1;
}

void JungleBubble_Widget::InitBubbleCheckSign()
{
    for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
        for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
            m_bubbleClick[i][j].bCheck = 0;
}

void JungleBubble_Widget::InitSameBubbleRecord()
{
    m_iCurClickColor = 0;
    m_iSameBubbleCount = 0;
    for(int i=0; i< GAME_BUBBLE_NUM; i++)
    {
        m_iSameBubbleRowIdx[i] = -1;
        m_iSameBubbleColIdx[i] = -1;
    }
}

void JungleBubble_Widget::InitFallBubbleRecord()
{
    m_iFallBubbleCount = 0;
    for(int i=0; i< GAME_BUBBLE_NUM; i++)
    {
        m_iFallBubbleRowIdx[i] = -1;
        m_iFallObjRowIdx[i] = -1;
        m_iFallBubbleColIdx[i] = -1;
        m_iFallBubbleColor[i] = -1;
    }
}

void JungleBubble_Widget::InitEmptyBubbleColRecord()
{
    m_iLastEmptyCol = -1;
    for(int i=0; i< GAME_BUBBLE_COL_NUM; i++)
        m_iEmptyBubbleCol[i] = -1;
}

void JungleBubble_Widget::GenerateNewBubble()
{
    QTime timeSeed;
    timeSeed = QTime::currentTime();
    qsrand(timeSeed.msec() + timeSeed.second()*1000);
    int iColor = -1;
    if(g_iModel == KING_MODEL)
    {
        iColor  = qrand()%(g_iBubbleTypeLev+g_iBubbleTypeLev/2);// + KING_HARD_CONTROL);
        if(iColor >= g_iBubbleTypeLev)
            iColor = g_iRole;
    }
    else
        iColor = qrand()%g_iBubbleTypeLev;
    m_iNewBubbleColor[m_iNewBubbleCount]  = iColor;
}

void JungleBubble_Widget::GenerateDefaultBubbleWall()
{
    for(int i=GAME_BUBBLE_ROW_NUM-g_iBubbleHeight; i<GAME_BUBBLE_ROW_NUM; i++)
        for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
        {
            QTime timeSeed;
            timeSeed = QTime::currentTime();
            qsrand(timeSeed.msec() + timeSeed.second()*1000);
            PaintBubble(i, j, qrand()%g_iBubbleTypeLev);
        }
}

void JungleBubble_Widget::FindSameColorBubble(int iRow, int iCol)
{
    if(iRow > 0)    // Find Up
    {
        if(!m_bubbleClick[iRow-1][iCol].bCheck && m_bubbleClick[iRow-1][iCol].iColor == m_iCurClickColor)
        {
            m_iSameBubbleRowIdx[m_iSameBubbleCount] = iRow-1;
            m_iSameBubbleColIdx[m_iSameBubbleCount] = iCol;
            m_iSameBubbleCount ++;
            m_bubbleClick[iRow-1][iCol].bCheck = 1;
            FindSameColorBubble(iRow-1, iCol);
        }
    }
    if(iRow < GAME_BUBBLE_ROW_NUM-1)    // Find Down
    {
        if(!m_bubbleClick[iRow+1][iCol].bCheck && m_bubbleClick[iRow+1][iCol].iColor == m_iCurClickColor)
        {
            m_iSameBubbleRowIdx[m_iSameBubbleCount] = iRow+1;
            m_iSameBubbleColIdx[m_iSameBubbleCount] = iCol;
            m_iSameBubbleCount ++;
            m_bubbleClick[iRow+1][iCol].bCheck = 1;
            FindSameColorBubble(iRow+1, iCol);
        }
    }
    if(iCol > 0)    // Find Left
    {
        if(!m_bubbleClick[iRow][iCol-1].bCheck && m_bubbleClick[iRow][iCol-1].iColor == m_iCurClickColor)
        {
            m_iSameBubbleRowIdx[m_iSameBubbleCount] = iRow;
            m_iSameBubbleColIdx[m_iSameBubbleCount] = iCol-1;
            m_iSameBubbleCount ++;
            m_bubbleClick[iRow][iCol-1].bCheck = 1;
            FindSameColorBubble(iRow, iCol-1);
        }
    }
    if(iCol < GAME_BUBBLE_COL_NUM-1)    // Find Right
    {
        if(!m_bubbleClick[iRow][iCol+1].bCheck && m_bubbleClick[iRow][iCol+1].iColor == m_iCurClickColor)
        {
            m_iSameBubbleRowIdx[m_iSameBubbleCount] = iRow;
            m_iSameBubbleColIdx[m_iSameBubbleCount] = iCol+1;
            m_iSameBubbleCount ++;
            m_bubbleClick[iRow][iCol+1].bCheck = 1;
            FindSameColorBubble(iRow, iCol+1);
        }
    }
}

void JungleBubble_Widget::FindFallDownBubble()
{
    for(int k=0; k<m_iSameBubbleCount; k++)
    {
        int iRow = -1;
        int iCol = m_iSameBubbleColIdx[k];
        if(!m_bubbleClick[0][iCol].bCheck)
        {
            m_bubbleClick[0][iCol].bCheck = 1;    // Set the Column's Check Sign
            for(int i=GAME_BUBBLE_ROW_NUM-1; i>=0 ; i--)
                if(m_bubbleClick[i][iCol].iColor == -1)
                    { iRow = i; break; }
            if(iRow > 0)
            {
                int iObjRow = iRow;
                for(int i=iRow-1; i>=0; i--)
                {
                    if(m_bubbleClick[i][iCol].iColor != -1)
                    {
                        m_iFallBubbleRowIdx[m_iFallBubbleCount] = i;
                        m_iFallObjRowIdx[m_iFallBubbleCount] = iObjRow;
                        m_iFallBubbleColIdx[m_iFallBubbleCount] = iCol;
                        m_iFallBubbleColor[m_iFallBubbleCount] = m_bubbleClick[i][iCol].iColor;
                        m_iFallBubbleCount ++;
                        iObjRow --;
                    }    // if(m_bubbleClick[i][iCol].iColor != -1)
                }    // for(int i=iRow; i>=0; i--)
            }    // if(iRow > 0)
        }    // if(!m_bubbleClick[0][iCol].bCheck)
    }    // for(int k=0; k<m_iSameBubbleCount; k++)
}

void JungleBubble_Widget::FindEmptyBubbleColumn()
{
    int iObjCol = GAME_BUBBLE_COL_NUM/2-1;
    int iCurCol = GAME_BUBBLE_COL_NUM/2-1;
    while(iCurCol >= 0)    // Left Half Columns
    {
        if(m_bubbleClick[GAME_BUBBLE_ROW_NUM-1][iCurCol].iColor != -1)    // If the Current Check Column is not Empty
        {
            if(iCurCol != iObjCol)
                m_iLastEmptyCol = iCurCol;
            m_iEmptyBubbleCol[iCurCol] = iObjCol;
            iObjCol --;
        }
        iCurCol --;
    }
    iObjCol = GAME_BUBBLE_COL_NUM/2;
    iCurCol = GAME_BUBBLE_COL_NUM/2;
    while(iCurCol < GAME_BUBBLE_COL_NUM)    // Right Half Columns
    {
        if(m_bubbleClick[GAME_BUBBLE_ROW_NUM-1][iCurCol].iColor != -1)    // If the Current Check Column is not Empty
        {
            if(iCurCol != iObjCol)
                m_iLastEmptyCol = iCurCol;
            m_iEmptyBubbleCol[iCurCol] = iObjCol;
            iObjCol ++;
        }
        iCurCol ++;
    }
}

void JungleBubble_Widget::AllBubbleMoveUp()
{
    for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
    {
        PaintAnimateLabel(m_bubbleClick[GAME_BUBBLE_ROW_NUM-1][j].labAnimateMask, m_iNewBubbleColor[j]);
        m_bubbleClick[GAME_BUBBLE_ROW_NUM-1][j].btnBubble->setVisible(0);
    }
    for(int i=0; i<GAME_BUBBLE_ROW_NUM-1; i++)
        for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
        {
            PaintAnimateLabel(m_bubbleClick[i][j].labAnimateMask, m_bubbleClick[i+1][j].iColor);
            m_bubbleClick[i][j].btnBubble->setVisible(0);
        }
    for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
    {
        for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
        {
            QPropertyAnimation *animteMoveUp = new QPropertyAnimation(m_bubbleClick[i][j].labAnimateMask, "pos");
            animteMoveUp->setDuration(BUBBLE_MOVE_TIME);
            animteMoveUp->setEasingCurve(QEasingCurve::Linear);
            int iPosX = m_bubbleClick[i][j].btnBubble->x();
            int iPosY = m_bubbleClick[i][j].btnBubble->y();
            animteMoveUp->setStartValue(QPoint(iPosX, iPosY+m_iBubbleSize));
            animteMoveUp->setEndValue(QPoint(iPosX, iPosY));
            animteMoveUp->start();
            if(i == GAME_BUBBLE_ROW_NUM-1 && j == GAME_BUBBLE_COL_NUM-1)
                connect(animteMoveUp, SIGNAL(finished()), this, SLOT(slot_BubbleMoveUpEnd()));
        }
    }
}

void JungleBubble_Widget::RemoveBubble()
{
    PlayMusic(SOUND_TYPE, "BubbleRemove.mp3");
    int iRow[m_iSameBubbleCount], iCol[m_iSameBubbleCount];
    int iPosX[m_iSameBubbleCount], iPosY[m_iSameBubbleCount];
    for(int i=0; i<m_iSameBubbleCount; i++)
    {
        iRow[i] = m_iSameBubbleRowIdx[i];
        iCol[i] = m_iSameBubbleColIdx[i];
        iPosX[i] = m_bubbleClick[iRow[i]][iCol[i]].btnBubble->x();
        iPosY[i] = m_bubbleClick[iRow[i]][iCol[i]].btnBubble->y();
        m_bubbleClick[iRow[i]][iCol[i]].labAnimateMask->setGeometry(iPosX[i], iPosY[i], m_iBubbleSize, m_iBubbleSize);
        PaintAnimateLabel(m_bubbleClick[iRow[i]][iCol[i]].labAnimateMask, m_iCurClickColor);  // , m_iBubbleSize, m_iBubbleSize);
        PaintBubble(iRow[i], iCol[i], -1);
    }
    for(int i=0; i<m_iSameBubbleCount; i++)
    {
        QPropertyAnimation *animteCollect = new QPropertyAnimation(m_bubbleClick[iRow[i]][iCol[i]].labAnimateMask, "pos");
        animteCollect->setDuration(BUBBLE_COLLECT_TIME);
        animteCollect->setEasingCurve(QEasingCurve::Linear);
        animteCollect->setStartValue(QPoint(iPosX[i], iPosY[i]));
        animteCollect->setEndValue(QPoint(-m_iBubbleSize, -m_iBubbleSize));
        animteCollect->start();
        //if(i == m_iSameBubbleCount - 1)
        //    connect(animteCollect, SIGNAL(finished()), this, SLOT(slot_BubbleVanishEnd()));
    }

    int iCurScore = m_iSameBubbleCount*m_iSameBubbleCount*2;
    m_labScore->setText(QString::number(iCurScore));
    m_labScore->setVisible(1);
    m_labScore->raise();
    int iX = (int)((MARK_LAB_W+MARK_LAB_X)*g_dSizeRatio+0.5);
    int iH = (int)(RECORD_LAB_H*g_dSizeRatio+0.5);
    QPropertyAnimation *animteScore = new QPropertyAnimation(m_labScore, "pos");
    animteScore->setDuration(BUBBLE_COLLECT_TIME);
    animteScore->setEasingCurve(QEasingCurve::Linear);
    animteScore->setStartValue(QPoint(iX, iH));
    animteScore->setEndValue(QPoint(iX, 0));
    animteScore->start();
    connect(animteScore, SIGNAL(finished()), this, SLOT(slot_BubbleVanishEnd()));
}

void JungleBubble_Widget::BubbleFallDown()
{
    PlayMusic(SOUND_TYPE, "BubbleFall.mp3");
    int iRow[m_iFallBubbleCount], iCol[m_iFallBubbleCount];
    int iPosX[m_iFallBubbleCount], iBeginY[m_iFallBubbleCount], iEndY[m_iFallBubbleCount];
    for(int i=0; i<m_iFallBubbleCount; i++)
    {
        iRow[i] = m_iFallBubbleRowIdx[i];
        iCol[i] = m_iFallBubbleColIdx[i];
        int iObjRow = m_iFallObjRowIdx[i];
        int iColor = m_iFallBubbleColor[i];
        iBeginY[i] = m_bubbleClick[iRow[i]][iCol[i]].btnBubble->y();
        iEndY[i] = m_bubbleClick[iObjRow][iCol[i]].btnBubble->y();
        iPosX[i] = m_bubbleClick[iRow[i]][iCol[i]].btnBubble->x();
        m_bubbleClick[iRow[i]][iCol[i]].labAnimateMask->setGeometry(iPosX[i], iBeginY[i], m_iBubbleSize, m_iBubbleSize);
        PaintAnimateLabel(m_bubbleClick[iRow[i]][iCol[i]].labAnimateMask, iColor);  // , m_iBubbleSize, m_iBubbleSize);
        PaintBubble(iRow[i], iCol[i], -1);
    }
    for(int i=0; i<m_iFallBubbleCount; i++)
    {
        QPropertyAnimation *animteFall = new QPropertyAnimation(m_bubbleClick[iRow[i]][iCol[i]].labAnimateMask, "pos");
        animteFall->setDuration(BUBBLE_FALL_TIME);
        animteFall->setEasingCurve(QEasingCurve::OutBounce);
        animteFall->setStartValue(QPoint(iPosX[i], iBeginY[i]));
        animteFall->setEndValue(QPoint(iPosX[i], iEndY[i]));
        animteFall->start();
        if(i == m_iFallBubbleCount-1)
            connect(animteFall, SIGNAL(finished()), this, SLOT(slot_BubbleFallEnd()));
    }
}

void JungleBubble_Widget::BubbleColumnMove()
{
    bool bAnimateSig = 0;
    /****** Left Part ******/
    for(int j=GAME_BUBBLE_COL_NUM/2-1; j>=0; j--)
    {
        int iObjCol = m_iEmptyBubbleCol[j];
        if(iObjCol != -1 && iObjCol != j)
        {
            for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
            {
                PaintAnimateLabel(m_bubbleClick[i][j].labAnimateMask, m_bubbleClick[i][j].iColor);
                m_bubbleClick[i][iObjCol].iColor = m_bubbleClick[i][j].iColor;
                PaintBubble(i, j, -1);
                QPropertyAnimation *animteMoveColumn = new QPropertyAnimation(m_bubbleClick[i][j].labAnimateMask, "pos");
                animteMoveColumn->setDuration(BUBBLE_MOVE_TIME);
                animteMoveColumn->setEasingCurve(QEasingCurve::Linear);
                int iBeginX = m_bubbleClick[i][j].btnBubble->x();
                int iEndX = m_bubbleClick[i][iObjCol].btnBubble->x();
                int iPosY = m_bubbleClick[i][j].btnBubble->y();
                animteMoveColumn->setStartValue(QPoint(iBeginX, iPosY));
                animteMoveColumn->setEndValue(QPoint(iEndX, iPosY));
                animteMoveColumn->start();
                if(i == GAME_BUBBLE_ROW_NUM-1 && j == m_iLastEmptyCol)
                {
                    bAnimateSig = 1;
                    connect(animteMoveColumn, SIGNAL(finished()), this, SLOT(slot_BubbleMoveColumnEnd()));
                }
            }    // for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
        }    // if(iObjCol != -1 && iObjCol != j)
    }    // for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
    /****** Right Part ******/
    for(int j=GAME_BUBBLE_COL_NUM/2; j<GAME_BUBBLE_COL_NUM; j++)
    {
        int iObjCol = m_iEmptyBubbleCol[j];
        if(iObjCol != -1 && iObjCol != j)
        {
            for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
            {
                PaintAnimateLabel(m_bubbleClick[i][j].labAnimateMask, m_bubbleClick[i][j].iColor);
                m_bubbleClick[i][iObjCol].iColor = m_bubbleClick[i][j].iColor;
                PaintBubble(i, j, -1);
                QPropertyAnimation *animteMoveColumn = new QPropertyAnimation(m_bubbleClick[i][j].labAnimateMask, "pos");
                animteMoveColumn->setDuration(BUBBLE_MOVE_TIME);
                animteMoveColumn->setEasingCurve(QEasingCurve::Linear);
                int iBeginX = m_bubbleClick[i][j].btnBubble->x();
                int iEndX = m_bubbleClick[i][iObjCol].btnBubble->x();
                int iPosY = m_bubbleClick[i][j].btnBubble->y();
                animteMoveColumn->setStartValue(QPoint(iBeginX, iPosY));
                animteMoveColumn->setEndValue(QPoint(iEndX, iPosY));
                animteMoveColumn->start();
                if(i == GAME_BUBBLE_ROW_NUM-1 && j == m_iLastEmptyCol && !bAnimateSig)
                    connect(animteMoveColumn, SIGNAL(finished()), this, SLOT(slot_BubbleMoveColumnEnd()));
            }    // for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
        }    // if(iObjCol != -1 && iObjCol != j)
    }    // for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
}

void JungleBubble_Widget::GameEnd(bool bSuccess)
{
    switch(g_iModel)
    {
        case SCORE_MODEL :
            ScoreEnd(bSuccess);
            break;
        case SURVIVE_MODEL :
            SurviveEnd();
            break;
        case KING_MODEL :
            KingEnd(bSuccess);
            break;
    }
}

void JungleBubble_Widget::ScoreEnd(bool bSuccess)
{
    if(bSuccess)    // If the Game is Succeed
    {
        g_playerBGM->pause();
        // Game Succedd Dialog
        Remind_Dlg *dlgRemind = new Remind_Dlg(this);
        dlgRemind->SetWidget(GAME_SUCCEED_DLG);
        dlgRemind->exec();
        // Update Record
        int iOrgLev = g_recordGame.iScoreLev[g_iRole];
        if(iOrgLev < g_iCurLev)    // Normal Lev & Hard Lev
        {
            g_recordGame.iScoreLev[g_iRole] = g_iCurLev;
            UpdateRecord();
            bool bResult = 1;
            if(iOrgLev == EASY_LEV)
            {
                for(int i=0; i<4; i++)
                    if(g_recordGame.iScoreLev[i] == EASY_LEV)
                        { bResult = 0; break; }
                if(bResult)    // Survive Model Unlock
                {
                    g_playerBGM->pause();
                    Remind_Dlg *dlgRemind = new Remind_Dlg(this);
                    dlgRemind->SetWidget(SURVIVE_UNLOCK_DLG);
                    dlgRemind->exec();
                }
            }
            bResult = 1;
            if(g_iCurLev == HARD_LEV)
            {
                for(int i=0; i<4; i++)
                    if(g_recordGame.iScoreLev[i] < HARD_LEV)
                        { bResult = 0; break; }
                if(bResult)    // King Model Unlock
                {
                    g_playerBGM->pause();
                    Remind_Dlg *dlgRemind = new Remind_Dlg(this);
                    dlgRemind->SetWidget(KING_UNLOCK_DLG);
                    dlgRemind->exec();
                }
            }
        }    // if(iOrgLev < g_iCurLev)
        emit sigGameOver(); //this->deleteLater(); close();
    }    // if(bSuccess)
    else    //  Game Failed
    {
        g_playerBGM->pause();
        // Gme Failed Dialog
        Remind_Dlg *dlgRemind = new Remind_Dlg(this);
        dlgRemind->SetWidget(GAME_FAILED_DLG);
        if(dlgRemind->exec() == QDialog::Accepted)
        {
            ui->labRecord->setText(QString::number(m_iCurScore) + " / " + QString::number(g_iScoreLimit));
            GameRestart();
        }
        else
             { emit sigGameOver(); } //this->deleteLater(); close(); }
    }
}

void JungleBubble_Widget::SurviveEnd()
{
    g_playerBGM->pause();
    m_timerSurvive->stop();
    disconnect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
    // Update Record
    if(m_iCurTime >= g_recordGame.iTimeRecord[0])
    {
        Remind_Dlg *dlgRemind = new Remind_Dlg(this);
        dlgRemind->SetWidget(NEW_RECORD_DLG);
        dlgRemind->exec();
    }
    for(int i=0; i<4; i++)
    {
        if(m_iCurTime > g_recordGame.iTimeRecord[i])
        {
            if(i < 3)
                for(int j=2; j>=0; j--)
                    g_recordGame.iTimeRecord[j+1] = g_recordGame.iTimeRecord[j];
            g_recordGame.iTimeRecord[i] = m_iCurTime;
            break;
        }
    }
    UpdateRecord();
    // Game Over Dialog (No Success or Failed in Survive Model)
    PlayMusic(SOUND_TYPE, "GameSucceed.mp3");
    Remind_Dlg *dlgRemind = new Remind_Dlg(this);
    dlgRemind->SetWidget(GAME_OVER_DLG);
    if(dlgRemind->exec() == QDialog::Accepted)
    {
        ui->labRecord->setText("00 : 00 : 00");
        GameRestart();
    }
    else
        { emit sigGameOver(); } // this->deleteLater(); close(); }
}

void JungleBubble_Widget::KingEnd(bool bSuccess)
{
    if(bSuccess)    // Game Succeed
    {
        // Update Record
        if(g_iCurLev > g_recordGame.iKingLev[g_iRole])
        {
            g_recordGame.iKingLev[g_iRole] = g_iCurLev;
            UpdateRecord();
        }
        if(g_iCurLev == 2)    // No More King Model Game
        {
            g_playerBGM->pause();
            // Game Success Dialog
            Remind_Dlg *dlgRemind = new Remind_Dlg(this);
            dlgRemind->SetWidget(GAME_SUCCEED_DLG);
            dlgRemind->exec();
            // Game Over Dialog
            dlgRemind = new Remind_Dlg(this);
            dlgRemind->SetWidget(GAME_OVER_DLG);
            if(dlgRemind->exec() == QDialog::Accepted)
            {
                ui->labRecord->setText(QString::number(0) + " / " + QString::number(g_iLoveLimit));
                GameRestart();
            }
            else
                { emit sigGameOver(); } //this->deleteLater(); close(); }
        }
        else   // Not the Last Lev
        {
            g_playerBGM->pause();
            // Continue Dialog
            Remind_Dlg *dlgRemind = new Remind_Dlg(this);
            dlgRemind->SetWidget(CONTINUE_DLG);
            dlgRemind->exec();
            g_iCurLev ++;
            QPalette paletteBG;
            QImage imgBGP(":android/GameBGP" + QString::number(g_iRole) + QString::number(g_iCurLev) + ".jpg");
            paletteBG.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(imgBGP).scaled(g_iWinW, g_iWinH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            setPalette(paletteBG);
            LevAdjust(g_iCurLev);
            GameRestart();
            ui->labRecord->setText(QString::number(m_iCurLove) + " / " + QString::number(g_iLoveLimit));
        }
    }    // if(bSuccess)
    else    // Game Failed
    {
        if(g_iCurLev > g_recordGame.iKingLev[g_iRole])
        {
            g_recordGame.iKingLev[g_iRole] = g_iCurLev;
            UpdateRecord();
        }
        g_playerBGM->pause();
        // Game Failed Dialog
        Remind_Dlg *dlgRemind = new Remind_Dlg(this);
        dlgRemind->SetWidget(GAME_FAILED_DLG);
        if(dlgRemind->exec() == QDialog::Accepted)
        {
            GameRestart();
            ui->labRecord->setText(QString::number(m_iCurLove) + " / " + QString::number(g_iLoveLimit));
        }
        else
             { emit sigGameOver(); } //this->deleteLater(); close(); }
    }
}

void JungleBubble_Widget::GameRestart()
{
    InitVariable();
    for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
    {
        m_labNewBubble[j]->setVisible(0);
        for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
        {
            m_bubbleClick[i][j].btnBubble->setVisible(0);
            m_bubbleClick[i][j].labAnimateMask->setVisible(0);
            m_bubbleClick[i][j].iColor = -1;
            m_bubbleClick[i][j].bCheck = 0;
            PaintBubble(i, j, -1);
        }
    }
    GenerateDefaultBubbleWall();
    QString strMarkText;
    switch(g_iModel)
    {
        case SCORE_MODEL :
            strMarkText = "ScoreMark";
            ui->labRecord->setText(QString::number(m_iCurScore) + " / " + QString::number(g_iScoreLimit));
            break;
        case SURVIVE_MODEL :
            strMarkText = "TimeMark";
            ui->labRecord->setText("00 : 00 : 00");
            connect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
            break;
        case KING_MODEL :
            strMarkText = "LoveMark";
            ui->labRecord->setText(QString::number(m_iCurLove) + " / " + QString::number(g_iLoveLimit));
            break;
    }
    m_bClick = 1;
    connect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
    m_timerGenerateBubble->start(g_iSpeedLev);
    if(g_iModel == SURVIVE_MODEL)
    {
        connect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
        m_timerSurvive->start(1000);
    }
    if(g_iCurLev == 0)
        PlayMusic(MUSIC_TYPE, "Role" + QString::number(g_iRole) + ".mp3");
    else
        PlayMusic(MUSIC_TYPE, "Lev" + QString::number(g_iCurLev) + ".mp3");
}

void JungleBubble_Widget::UpdateRecord()
{
    QFile fileRecordList(g_strSourcePath + "RecordList.ini");
    //QMessageBox::information(this, "Warning", "JH_Debug: g_strSourcePath = " + g_strSourcePath + "RecordList.ini");
    if(!fileRecordList.exists())
    {
        QMessageBox::information(this, "Warning", "The file does not exist !");
        //fileRecordList.remove();
    }
//    if(!fileRecordList.open(QIODevice::ReadOnly | QIODevice::Text))
//        { QMessageBox::information(this, "Warning", "The file cannot be Read !"); }
//    if(!fileRecordList.open(QIODevice::WriteOnly | QIODevice::Text))
//        { QMessageBox::information(this, "Warning", "The file cannot be Write !"); }
//    if(!fileRecordList.open(QIODevice::ReadWrite | QIODevice::Text))
//        { QMessageBox::information(this, "Warning", "The file cannot be Read or Write !"); }

    if(fileRecordList.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QMessageBox::information(this, "Warning", "Begin to Write File !");
        QTextStream floStream(&fileRecordList);
        QString strLine = "Score=";
        for(int i=0; i<4; i++)
        {
            strLine += QString::number(g_recordGame.iScoreLev[i]);
            if(i==3)
                strLine += "\n";
            else
                strLine += ",";
        }
        strLine += "Survive=";
        for(int i=0; i<4; i++)
        {
            strLine += QString::number(g_recordGame.iTimeRecord[i]);
            if(i==3)
                strLine += "\n";
            else
                strLine += ",";
        }
        strLine += "King=";
        for(int i=0; i<4; i++)
        {
            strLine += QString::number(g_recordGame.iKingLev[i]);
            if(i<3)
                strLine += ",";
        }
        floStream << strLine;
        fileRecordList.close();
    }
//    else
//        { QMessageBox::information(this, "Warning", "The file cannot be Read or Write !"); }
}

void JungleBubble_Widget::LevAdjust(int iLev)
{
    g_iCurLev = iLev;
    g_iBubbleTypeLev = 4+iLev*2;
    g_iBubbleHeight = 2+iLev*2;
    g_iSpeedLev = 1000-GENERATE_BUBBLE_SPEED*(iLev*3+1);
     g_iLoveLimit = 100+iLev*LEV_ADJUST;    // 500 + iLev*500;
}

int JungleBubble_Widget::CalculateScore()
{
    m_iCurScore += m_iSameBubbleCount*m_iSameBubbleCount*2;
    ui->labRecord->setText(QString::number(m_iCurScore) + " / " + QString::number(g_iScoreLimit));
    if(m_iCurScore >= g_iScoreLimit)    // Current Game Finished
        { GameEnd(GAME_SUCCEED); return -1; }
    return 0;
}

void JungleBubble_Widget::CalculateTime()
{
    m_iCurTime += m_iSameBubbleCount*2;
    QString strTime = TimeConvert(m_iCurTime);
    ui->labRecord->setText(strTime);
}

int JungleBubble_Widget::CalculateLove()
{
    if(m_iCurClickColor == g_iRole)
        m_iCurLove += m_iSameBubbleCount*m_iSameBubbleCount;
    else
        m_iCurLove -= m_iSameBubbleCount;
    ui->labRecord->setText(QString::number(m_iCurLove) + " / " + QString::number(g_iLoveLimit));
    if(m_iCurLove >= g_iLoveLimit)    // Current Game Finished
        { GameEnd(GAME_SUCCEED); return -1; }
    return 0;
}

void JungleBubble_Widget::PaintBubble(int iRow, int iCol, int iColor)
{
    m_bubbleClick[iRow][iCol].iColor = iColor;
    if(iColor == -1)
        m_bubbleClick[iRow][iCol].btnBubble->setVisible(0);
    else
    {
        m_bubbleClick[iRow][iCol].btnBubble->setVisible(1);
        SetBubbleBGP(m_bubbleClick[iRow][iCol].btnBubble, "Bubble" + QString::number(iColor), m_iBubbleSize, m_iBubbleSize);
    }
}

void JungleBubble_Widget::PaintAnimateLabel(QLabel *labObj, int iColor)  // , int iWidth, int iHeight)
{
    if(iColor == -1)
        labObj->setVisible(0);
    else
    {
        g_classSetBGP.SetLabBGP(labObj, ":android/Bubble" + QString::number(iColor) + ".png", m_iBubbleSize, m_iBubbleSize);
        labObj->setVisible(1);
    }
}

void JungleBubble_Widget::SetBubbleBGP(Custom_Btn *btnObject, QString strPicName, int iWidth, int iHeight)
{
    g_classSetBGP.SetBtnBGP(btnObject, strPicName, 1);
}

QString JungleBubble_Widget::TimeConvert(int iTime)
{
    QString strTime, strHour, strMin, strSec;
    int iHour = 0, iMin = 0, iSec = 0;
    if(iTime < 60)
        iSec = iTime;
    else
    {
        iMin = iTime/60;
        iSec = iTime%60;
        if(iMin >= 60)
        {
            iHour = iMin/60;
            iMin = iMin%60;
        }
    }
    if(iHour < 10)
        strHour = "0";
    strHour += QString::number(iHour);
    if(iMin < 10)
        strMin = "0";
    strMin += QString::number(iMin);
    if(iSec < 10)
        strSec = "0";
    strSec += QString::number(iSec);
    strTime = strHour + ":" + strMin + ":" + strSec;
    return strTime;
}

void JungleBubble_Widget::PlayMusic(bool bType, QString strName)
{
    switch(bType)
    {
        case MUSIC_TYPE :
            g_playerBGM->setMedia(QUrl::fromLocalFile(g_strSourcePath + strName));
            g_playerBGM->play();
            break;
        case SOUND_TYPE :
            g_playerSound->setMedia(QUrl::fromLocalFile(g_strSourcePath + strName));
            g_playerSound->play();
            break;
        default : break;
    }
}

void JungleBubble_Widget::slot_GenerateBubble()
{
    if(m_iNewBubbleCount < GAME_BUBBLE_COL_NUM)
    {
        GenerateNewBubble();
        PaintAnimateLabel(m_labNewBubble[m_iNewBubbleCount], m_iNewBubbleColor[m_iNewBubbleCount]);
        m_iNewBubbleCount ++;
    }
    else
    {
        m_timerGenerateBubble->stop();
        disconnect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
        m_bClick = 0;
        for(int i=0; i<GAME_BUBBLE_COL_NUM; i++)
            m_labNewBubble[i]->setVisible(0);
        m_iNewBubbleCount = 0;
        for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
            if(m_bubbleClick[0][j].iColor != -1)
                {  m_iGameStatus= GAME_FAILED; break; }
        if(m_iGameStatus == GAMING)
            AllBubbleMoveUp();
        if(m_iGameStatus == GAME_FAILED)
            GameEnd(GAME_FAILED);
    }
}

void JungleBubble_Widget::slot_BubbleMoveUpEnd()
{
    for(int i=0; i<GAME_BUBBLE_ROW_NUM-1; i++)
        for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
        {
            PaintAnimateLabel(m_bubbleClick[i][j].labAnimateMask, -1);
            PaintBubble(i, j, m_bubbleClick[i+1][j].iColor);
        }
    for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
    {
        PaintAnimateLabel(m_bubbleClick[GAME_BUBBLE_ROW_NUM-1][j].labAnimateMask, -1);
        PaintBubble(GAME_BUBBLE_ROW_NUM-1, j, m_iNewBubbleColor[j]);
    }
    if(!m_bPause)    // Pause or Back Status
    {
        connect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
        m_timerGenerateBubble->start(g_iSpeedLev);
        m_bClick = 1;
    }
}

/*
void JungleBubble_Widget::slot_BubbleVanish()
{
    m_labDebug->setText("      Vanishing " + QString::number(m_iBubbleVanishTimeCount) +  " ! ");
    if(m_iBubbleVanishTimeCount < VANISH_TIME/TIME_DELTA)
        m_iBubbleVanishTimeCount ++;
    else
    {
        for(int i=0; i<m_iSameBubbleCount; i++)
        {
            int iRow = m_iSameBubbleRowIdx[i];
            int iCol = m_iSameBubbleColIdx[i];
            m_bubbleClick[iRow][iCol].labAnimateMask->setVisible(0);
        }
        m_iBubbleVanishTimeCount = 0;
        m_timerBubbleVanish->stop();
        disconnect(m_timerBubbleVanish, SIGNAL(timeout()), this, SLOT(slot_BubbleVanish()));
        emit sigBubbleVanishEnd();
    }
    */
    /* Use QTimer to Relize the Animate
    m_timerBubbleVanish->stop();
    disconnect(m_timerBubbleVanish, SIGNAL(timeout()), this, SLOT(slot_BubbleVanish()));
    if(m_iBubbleVanishTimeCount <= GAME_BUBBLE_SIZE)
    {
        double dStep = (double)m_iBubbleSize/(double)GAME_BUBBLE_SIZE;
        for(int i=0; i<m_iSameBubbleCount; i++)
        {
            int iRow = m_iSameBubbleRowIdx[i];
            int iCol = m_iSameBubbleColIdx[i];
            int iSize = (int)(m_iBubbleVanishTimeCount*dStep+0.5);
            int iPosX = (int)(m_bubbleClick[iRow][iCol].labAnimateMask->x() + dStep/2.0 + 0.5);
            int iPosY = (int)(m_bubbleClick[iRow][iCol].labAnimateMask->y() + dStep/2.0 + 0.5);
            m_bubbleClick[iRow][iCol].labAnimateMask->setGeometry(iPosX, iPosY, m_iBubbleSize-iSize, m_iBubbleSize-iSize);
            PaintAnimateLabel(m_bubbleClick[iRow][iCol].labAnimateMask, m_iCurClickColor, m_iBubbleSize-iSize, m_iBubbleSize-iSize);
            if(m_iBubbleVanishTimeCount == GAME_BUBBLE_SIZE)
                m_bubbleClick[iRow][iCol].labAnimateMask->setVisible(0);
        }
        m_iBubbleVanishTimeCount ++;
        connect(m_timerBubbleVanish, SIGNAL(timeout()), this, SLOT(slot_BubbleVanish()));
        m_timerBubbleVanish->start(VANISH_SPEED);
    }
    else
    {
        for(int i=0; i<m_iSameBubbleCount; i++)
        {
            int iRow = m_iSameBubbleRowIdx[i];
            int iCol = m_iSameBubbleColIdx[i];
            m_bubbleClick[iRow][iCol].labAnimateMask->setVisible(0);
        }
        m_iBubbleVanishTimeCount = 0;
        emit sigBubbleVanishEnd();
    }
}
*/

void JungleBubble_Widget::slot_BubbleVanishEnd()
{
    m_labScore->setVisible(0);
    // Calculate Scores
    if(g_iModel == SCORE_MODEL)
        if(CalculateScore() == -1)
            return;
    if(g_iModel == SURVIVE_MODEL)
        CalculateTime();
    if(g_iModel == KING_MODEL)
        if(CalculateLove() == -1)
            return;
    // Reset Bubble Click Mask
    for(int i=0; i<m_iSameBubbleCount; i++)
    {
        int iRow = m_iSameBubbleRowIdx[i];
        int iCol = m_iSameBubbleColIdx[i];
        m_bubbleClick[iRow][iCol].labAnimateMask->setVisible(0);
    }
    // Find Fall Bubble
    InitBubbleCheckSign();
    FindFallDownBubble();
    if(m_iFallBubbleCount > 0)
        BubbleFallDown();
    else
        slot_BubbleFallEnd();
}

/*
void JungleBubble_Widget::slot_BubbleFall()
{
    m_labDebug->setText("      Falling " + QString::number(m_iBubbleFallTimeCount) +  " ! ");
    if(m_iBubbleFallTimeCount < FALL_TIME/TIME_DELTA)
        m_iBubbleFallTimeCount ++;
    else
    {
        for(int i=0; i<m_iFallBubbleCount; i++)
        {
            int iRow = m_iFallBubbleRowIdx[i];
            int iObjRow = m_iFallObjRowIdx[i];
            int iCol = m_iFallBubbleColIdx[i];
            int iColor = m_iFallBubbleColor[i];
            PaintBubble(iObjRow, iCol, iColor);
            m_bubbleClick[iRow][iCol].labAnimateMask->setVisible(0);
        }
        m_iBubbleFallTimeCount = 0;
        m_timerBubbleFall->stop();
        disconnect(m_timerBubbleFall, SIGNAL(timeout()), this, SLOT(slot_BubbleFall()));
        emit sigBubbleFallEnd();
    }
}
*/

void JungleBubble_Widget::slot_BubbleFallEnd()
{
    if(m_iFallBubbleCount > 0)
    {
        for(int i=0; i<m_iFallBubbleCount; i++)
        {
            int iRow = m_iFallBubbleRowIdx[i];
            int iObjRow = m_iFallObjRowIdx[i];
            int iCol = m_iFallBubbleColIdx[i];
            int iColor = m_iFallBubbleColor[i];
            PaintBubble(iObjRow, iCol, iColor);
            m_bubbleClick[iRow][iCol].labAnimateMask->setVisible(0);
        }
    }
    InitBubbleCheckSign();
    InitSameBubbleRecord();
    InitFallBubbleRecord();
    FindEmptyBubbleColumn();
    if(m_iLastEmptyCol >= 0)
        BubbleColumnMove();
    else
        slot_BubbleMoveColumnEnd();
}

void JungleBubble_Widget::slot_BubbleMoveColumnEnd()
{
    if(m_iLastEmptyCol >= 0)
        for(int j=0; j<GAME_BUBBLE_COL_NUM; j++)
        {
            int iObjCol = m_iEmptyBubbleCol[j];
            if(iObjCol != -1 && iObjCol != j)
            {
                for(int i=0; i<GAME_BUBBLE_ROW_NUM; i++)
                {
                    int iColor = m_bubbleClick[i][iObjCol].iColor;
                    PaintBubble(i, iObjCol, iColor);
                    PaintAnimateLabel(m_bubbleClick[i][j].labAnimateMask, -1);
                }
            }
        }
    InitEmptyBubbleColRecord();
    m_bClick = 1;
    connect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
    m_timerGenerateBubble->start(g_iSpeedLev);
}

void JungleBubble_Widget::slot_BubbleClick(int iRow, int iCol)
{
    if(!m_bClick)    // If Click is Enable
        return;
    m_bClick = 0;    // Set Click Disable
    m_iCurClickColor = m_bubbleClick[iRow][iCol].iColor;    // Get Current Clicked Bubble's Color
    if(m_iCurClickColor != -1)    // If the Bubble is not Empty
    {
        // Set the Clicked Bubble Checked
        m_bubbleClick[iRow][iCol].bCheck = 1;
        m_iSameBubbleRowIdx[0] = iRow;
        m_iSameBubbleColIdx[0] = iCol;
        m_iSameBubbleCount = 1;
        // Find Same Color Adjant Bubbles
        FindSameColorBubble(iRow, iCol);
        if(m_iSameBubbleCount >= BUBBLE_REMOVE_MIN_NUM)    // If Same Color Bubble Number is Enough
        {
            m_timerGenerateBubble->stop();
            disconnect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
            RemoveBubble();
        }
        else    // Reset Sign if Same Color Bubble Number is not Enough
        {
            InitBubbleCheckSign();
            InitSameBubbleRecord();
            m_bClick = 1;
        }
    }
    else    //  Clicked an Empty Bubble
        m_bClick = 1;
}

void JungleBubble_Widget::slot_TimeSurvive()
{
    m_iCurTime ++;
    QString strTime = TimeConvert(m_iCurTime);
    ui->labRecord->setText(strTime);
}

void JungleBubble_Widget::on_btnPause_clicked()
{
    m_bPause = 1;
    m_bClick = 0;
    m_timerGenerateBubble->stop();
    disconnect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_classSetBGP.SetBtnBGP(ui->btnPause, "RunBtn");
    if(g_iModel == SURVIVE_MODEL)
    {
        m_timerSurvive->stop();
        disconnect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
    }
    ui->labCover->setVisible(1);
    g_playerBGM->pause();
    Remind_Dlg *dlgRemind = new Remind_Dlg(this);
    dlgRemind->SetWidget(GAME_PAUSE_DLG);
    if(dlgRemind->exec() == QDialog::Rejected)
    {
        ui->labCover->setVisible(0);
        g_classSetBGP.SetBtnBGP(ui->btnPause, "PauseBtn");
        m_bPause = 0;
        m_bClick = 1;
        g_playerBGM->play();
        connect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
        m_timerGenerateBubble->start(g_iSpeedLev);
        if(g_iModel == SURVIVE_MODEL)
        {
            connect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
            m_timerSurvive->start(1000);
        }
    }
}

void JungleBubble_Widget::on_btnBack_clicked()
{
    m_bPause = 1;
    m_bClick = 0;
    m_timerGenerateBubble->stop();
    disconnect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    //int iBtnSize = (int)(CONTROL_BTN_SIZE*g_dSizeRatio+0.5);
    g_classSetBGP.SetBtnBGP(ui->btnPause, "RunBtn");
    //g_classSetBGP.SetBtnBGP(ui->btnPause, ":android/RunBtn.png", iBtnSize, iBtnSize);
    if(g_iModel == SURVIVE_MODEL)
    {
        m_timerSurvive->stop();
        disconnect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
    }
    ui->labCover->setVisible(1);
    g_playerBGM->pause();
    Remind_Dlg *dlgRemind = new Remind_Dlg(this);
    dlgRemind->SetWidget(GIVE_UP_DLG);
    if(dlgRemind->exec() == QDialog::Accepted)    // Give Up Current Game
        { emit sigGameOver(); } //this->deleteLater(); close(); }
    else
    {
        ui->labCover->setVisible(0);
        //int iBtnSize = (int)(CONTROL_BTN_SIZE*g_dSizeRatio+0.5);
        g_classSetBGP.SetBtnBGP(ui->btnPause, "PauseBtn");
        //g_classSetBGP.SetBtnBGP(ui->btnPause, ":android/PauseBtn.png", iBtnSize, iBtnSize);
        m_bPause = 0;
        m_bClick = 1;
        g_playerBGM->play();
        connect(m_timerGenerateBubble, SIGNAL(timeout()), this, SLOT(slot_GenerateBubble()));
        m_timerGenerateBubble->start(g_iSpeedLev);
        if(g_iModel == SURVIVE_MODEL)
        {
            connect(m_timerSurvive, SIGNAL(timeout()), this, SLOT(slot_TimeSurvive()));
            m_timerSurvive->start(1000);
        }
    }
}
