#include "main_window.h"
#include "ui_main_window.h"

extern int g_iScreenW;    // Device Screen Width
extern int g_iScreenH;    // Device Screen Height

int g_iWinW;    // App Window Width after Adjust
int g_iWinH;    // App Window Height after Adjust
double g_dSizeRatio;    // The Width-Height Ratio required to be adjusted for all the Widgets in App
QString g_strSourcePath;    // Source Files' Path in Device
int g_iModel;    // 0: Normal Model, 1: Live Model, 2: Free Model, 3: King Model (Hide Model)
int g_iBubbleTypeLev;    // Bubble Type Number Level: 4, 6, 8
int g_iBubbleHeight;    // Bubble Wall Default Height Level: 2, 4, 6
int g_iSpeedLev;    // Bubble Generate Speed Level: 1-fast; 5-normal; 10-slow
int g_iScoreLimit;    // lev1-500, lev2-1000, lev3-2000
int g_iLoveLimit;    // lev1-500, lev2-1000, lev3-2000
int g_iCurLev;    // Game Level Selected
int g_iRole;    // Four Roles
bool g_bSurviveUnlock;    // Survive Model Unlock Sign
bool g_bKingUnlock;    // King Model Unlock Sign
QMediaPlayer *g_playerBGM;    // Game Background Music, Loop
QMediaPlayer *g_playerSound;    // All kinds of Sound, Only Play for One Time, such as Button Click Sound
RECORD g_recordGame;    // Game Record Structure

SetWidgetBGP_Class g_classSetBGP;    // Set Widget BGP

Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);
    //QMessageBox::information(this, "Info", "DebugJH: Main Window Created !");
    InitVariable();
    InitWidget();
}

Main_Window::~Main_Window()
{
    delete ui;
}

void Main_Window::InitVariable()
{
    // 1. Globle Variables Setting
    // 1.1 Setting Source Path On Device.
    QDir dirSource;
    g_strSourcePath = dirSource.currentPath() + "/"; //music/";
    QDir dirCreate(g_strSourcePath);
    if(!dirCreate.exists())    // If the source path does not exist, create one.
        if(!dirCreate.mkdir(g_strSourcePath))    // If path creating failed, show a msg box.
            { QMessageBox::information(this, "Warning", "Create Dir Failed!"); close(); return; }
    // 1.2 Mode and Character ID
    g_iModel = 0;
    g_iRole = 0;
    // 1.3 Survival and King Mode Unlock Flag
    g_bSurviveUnlock = 0;
    g_bKingUnlock = 0;

    // 2. Main Window Relevant Setting
    // 2.1 Member Varaibles
    m_iWinMoveX = 0;
    m_iWinMoveY = 0;
    m_iCurUI = START_UI;
    m_iCloseUI = -1;
    // 2.2 Cloud Timer & Bubble Deriction Timer
    m_timerCloud = new QTimer(this);
    connect(m_timerCloud, SIGNAL(timeout()), this, SLOT(slot_CloudTimer()));
    m_iChangeDirectionTime = 0;
    m_timerBubble = new QTimer(this);
    connect(m_timerBubble, SIGNAL(timeout()), this, SLOT(slot_BubbleTimer()));
    // 2.3 Get Screen Size and Calculate App's Size need to be Adjusted
    FitGameAreaSize();

    // 3. Load Record
    QFile copyFile(":android/RecordList.ini");
    QFile objFile(g_strSourcePath + "RecordList.ini");
    if(!objFile.exists())
    {
        if(copyFile.copy(g_strSourcePath + "RecordList.ini") == 0)
             { QMessageBox::information(this, "Warning", "Copy <RecordList.ini> Failed!"); close(); return; }
    }
    LoadRecordList();

    // 4. Play BGM
    CopyMusicFile();
    g_playerSound = new QMediaPlayer(this);
    g_playerBGM = new QMediaPlayer(this);
    connect(g_playerBGM, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(slot_BGMStop(QMediaPlayer::State)));
    PlayMusic(MUSIC_TYPE, "MainBGM.mp3");

    // 5. Interlude Timer Prepare
    m_timerInterlude = new QTimer(this);
    /*m_sceneInterlude.setSceneRect(0, 0, g_iWinW, g_iWinH);
    for(int i=0; i<INTERLUDE_BUBBLE_NUM; i++)
    {
        m_itemBubble[i] = new InterludeBubble_Item;
        m_itemBubble[i]->setPos(i*100, i*100);
        m_itemBubble[i]->SetBGP(7);
        m_sceneInterlude.addItem(m_itemBubble[i]);
    }
    QGraphicsView viewInterlude(&m_sceneInterlude);
    viewInterlude.setRenderHint(QPainter::Antialiasing);
    viewInterlude.setBackgroundBrush(QPixmap(":android/WhiteCover.png"));
    viewInterlude.setCacheMode(QGraphicsView::CacheBackground);
    viewInterlude.setGeometry(0, 0, g_iWinW, g_iWinH);
    viewInterlude.show();*/
}

void Main_Window::InitWidget()
{
    /****** 0. BPG & Labels ******/
//    m_labelDebug = new QLabel(this);
//    m_labelDebug->setGeometry(0,0,1080,30);
//    m_labelDebug->setText(" ");

    /****** 1. BPG & Labels ******/
    // 1.1 Set Main Window BGP to Black
    setAutoFillBackground(true);
    QPalette paletteBG;
    paletteBG.setColor(QPalette::Window, QColor(0, 0, 0));    // Black BG Color
    setPalette(paletteBG);
    // 1.2 Main BGP Label
    ui->labMainBGP->setGeometry(m_iWinMoveX, m_iWinMoveY, g_iWinW, g_iWinH);
    g_classSetBGP.SetLabBGP(ui->labMainBGP, ":android/MainMenuBGP.jpg", g_iWinW, g_iWinH);
    // 1.3 Select BGP Label
    ui->labSelectBGP->setGeometry(m_iWinMoveX, m_iWinMoveY, g_iWinW, g_iWinH);
    g_classSetBGP.SetLabBGP(ui->labSelectBGP, ":android/CharacterSelectBGP.jpg", g_iWinW, g_iWinH);

    /****** 2. Animate Cloud & Bubbles ******/
    // 2.1 Animate Cloud in Background
//    ui->labCloud->setGeometry((int)(-1*CLOUD_W*g_dSizeRatio+0.5), (int)(CLOUD_Y*g_dSizeRatio+0.5), (int)(CLOUD_W*g_dSizeRatio+0.5), (int)(CLOUD_H*g_dSizeRatio+0.5));
//    g_classSetBGP.SetLabBGP(ui->labCloud, ":android/Cloud.png",  (int)(CLOUD_W*g_dSizeRatio+0.5), (int)(CLOUD_H*g_dSizeRatio+0.5));
//    m_timerCloud->start(CLOUD_SPEED);
    // 2.2 Animate Bubbles
    int iBubbleSize = (int)(ANIMATE_BUBBLE_SIZE*g_dSizeRatio+0.5);
    for(int i=0; i<8; i++)
    {
        m_labBubble[i] = new QLabel(this);
        g_classSetBGP.SetLabBGP(m_labBubble[i], ":android/Bubble" + QString::number(i) + ".png", iBubbleSize, iBubbleSize);
        QTime timeSeed;
        timeSeed = QTime::currentTime();
        qsrand(timeSeed.msec() + timeSeed.second()*1000);
//        int iPosX = (qrand()%5)*200;
//        int iPosY = (qrand()%8)*200;
        int iPosX = -200;
        int iPosY = i*200;
        m_labBubble[i]->setGeometry((int)(iPosX*g_dSizeRatio+0.5), (int)(iPosY*g_dSizeRatio+0.5), iBubbleSize, iBubbleSize);
    }
    m_timerBubble->start(ANIMATE_BUBBLE_SPEED);

    /****** 3. Three Main Menu Buttons ******/
    int iBtnW = (int)(MAIN_BTN_W*g_dSizeRatio+0.5);
    int iBtnH = (int)(MAIN_BTN_H*g_dSizeRatio+0.5);
    ui->btnModelSelect->setGeometry((int)(FIRST_BTN_X*g_dSizeRatio+0.5), (int)(FIRST_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
    ui->btnRecord->setGeometry((int)(SECOND_BTN_X*g_dSizeRatio+0.5), (int)(SECOND_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
    ui->btnHelp->setGeometry((int)(THIRD_BTN_X*g_dSizeRatio+0.5), (int)(THIRD_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
    g_classSetBGP.SetBtnBGP(ui->btnModelSelect, "MainMenu_ModeBtn");
    g_classSetBGP.SetBtnBGP(ui->btnRecord, "MainMenu_CreditsBtn");
    g_classSetBGP.SetBtnBGP(ui->btnHelp, "MainMenu_HelpBtn");

    /****** 4. Three Model Select Buttons ******/
    ui->btnScoreModel->setGeometry((int)(FIRST_BTN_X*g_dSizeRatio+0.5), (int)(FIRST_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
    ui->btnSurviveModel->setGeometry((int)(SECOND_BTN_X*g_dSizeRatio+0.5), (int)(SECOND_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
    ui->btnKingModel->setGeometry((int)(THIRD_BTN_X*g_dSizeRatio+0.5), (int)(THIRD_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
    g_classSetBGP.SetBtnBGP(ui->btnScoreModel, "ModeSelect_RegularBtn");
    g_classSetBGP.SetBtnBGP(ui->btnSurviveModel, "ModeSelect_SurviveBtn");
    g_classSetBGP.SetBtnBGP(ui->btnKingModel, "ModeSelect_KingBtn");

    /****** 5. Four Roles Select Buttons ******/
    int iBtnSize = (int)(SELECT_BTN_SIZE*g_dSizeRatio+0.5);
    ui->btnSelectXS->setGeometry((int)(SELECT_XS_X*g_dSizeRatio+0.5), (int)(SELECT_XS_Y*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    ui->btnSelectZJ->setGeometry((int)(SELECT_ZJ_X*g_dSizeRatio+0.5), (int)(SELECT_ZJ_Y*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    ui->btnSelectYH->setGeometry((int)(SELECT_YH_X*g_dSizeRatio+0.5), (int)(SELECT_YH_Y*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    ui->btnSelectDC->setGeometry((int)(SELECT_DC_X*g_dSizeRatio+0.5), (int)(SELECT_DC_Y*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    g_classSetBGP.SetBtnBGP(ui->btnSelectXS, "CharacterSelectBtn");
    g_classSetBGP.SetBtnBGP(ui->btnSelectZJ, "CharacterSelectBtn");
    g_classSetBGP.SetBtnBGP(ui->btnSelectYH, "CharacterSelectBtn");
    g_classSetBGP.SetBtnBGP(ui->btnSelectDC, "CharacterSelectBtn");

    /****** 6. Three Difficulty Select Buttons ******/
    iBtnW = (int)(LEV_BTN_W*g_dSizeRatio+0.5);
    iBtnH = (int)(LEV_BTN_H*g_dSizeRatio+0.5);
    ui->btnEasy->setGeometry((int)(EASY_X*g_dSizeRatio+0.5), (int)(LEV_BTN_Y*g_dSizeRatio+0.5)-(int)(50*g_dSizeRatio+0.5), iBtnW, iBtnH);
    ui->btnNormal->setGeometry((int)(NORMAL_X*g_dSizeRatio+0.5), (int)(LEV_BTN_Y*g_dSizeRatio+0.5)-(int)(50*g_dSizeRatio+0.5), iBtnW, iBtnH);
    ui->btnHard->setGeometry((int)(HARD_X*g_dSizeRatio+0.5), (int)(LEV_BTN_Y*g_dSizeRatio+0.5)-(int)(50*g_dSizeRatio+0.5), iBtnW, iBtnH);
    LevAdjust(NORMAL_LEV);  // Default Difficulty Level is Normal

    /****** 7. Three Assistant Buttons ******/
    iBtnSize = (int)(BACK_SIZE*g_dSizeRatio+0.5);
    ui->btnBack->setGeometry(g_iWinW-iBtnSize, g_iWinH-iBtnSize-(int)(50*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    ui->btnGallery->setGeometry(g_iWinW-iBtnSize, g_iWinH-iBtnSize-(int)(50*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    g_classSetBGP.SetBtnBGP(ui->btnBack, "BackBtn");
    g_classSetBGP.SetBtnBGP(ui->btnGallery, "GalleryBtn");

    /****** 8. Init UI ******/
    m_widRecordBoard = new Record_Widget(this);
    connect(m_widRecordBoard, SIGNAL(sigRecordClose(int)), this, SLOT(slot_InfoClose(int)));
    m_widRecordBoard->hide();
    m_widHelp = new Help_Widget(this);
    connect(m_widHelp, SIGNAL(sigHelpClose(int)), this, SLOT(slot_InfoClose(int)));
    m_widHelp->hide();
    m_widGallery = new Gallery_Widget(this);
    connect(m_widGallery, SIGNAL(sigGalleryClose(int)), this, SLOT(slot_InfoClose(int)));
    m_widGallery->hide();
    m_widGameStart = new JungleBubble_Widget(this);
    connect(m_widGameStart, SIGNAL(sigGameOver()), this, SLOT(slot_GameOver()));
    m_widGameStart->hide();

    /****** 9. Cover & Animate Cover Label ******/
    iBubbleSize = (int)((ANIMATE_BUBBLE_SIZE*6/5)*g_dSizeRatio+0.5);
    for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i++)
    {
        for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j++)
        {
            QTime timeSeed;
            timeSeed = QTime::currentTime();
            qsrand(timeSeed.msec() + timeSeed.second()*1000);
            int iColor = qrand()%12;
            if(i<INTERLUDE_BUBBLE_ROW && j<INTERLUDE_BUBBLE_COL)
            {
                m_labInterludeBubbleForward[i][j] = new QLabel(this);
                m_labInterludeBubbleForward[i][j]->setGeometry(-iBubbleSize, -iBubbleSize, iBubbleSize, iBubbleSize);
                SetBubbleLabelBG(m_labInterludeBubbleForward[i][j], iColor);
                m_labInterludeBubbleForward[i][j]->setVisible(0);
            }
            m_labInterludeBubbleBack[i][j] = new QLabel(this);
            m_labInterludeBubbleBack[i][j]->setGeometry(-iBubbleSize, -iBubbleSize, iBubbleSize, iBubbleSize);
            iColor = 11-iColor;
            SetBubbleLabelBG(m_labInterludeBubbleBack[i][j], iColor);
            m_labInterludeBubbleBack[i][j]->setVisible(0);
        }
    }

    /****** 10. Bottom Cover Label ******/
//    m_labBottom = new QLabel(this);
//    //int iX = (int)(BOTTOM_COVER_X*g_dSizeRatio+0.5);
//    //int iY = (int)(BOTTOM_COVER_Y*g_dSizeRatio+0.5);
//    //int iW = (int)(WIN_W*g_dSizeRatio+0.5);
//    int iX = 0;
//    int iY = (int)(g_iWinH-(BOTTOM_COVER_H/2)*g_dSizeRatio+0.5);
//    int iW = g_iWinW;
//    int iH = (int)(BOTTOM_COVER_H*g_dSizeRatio+0.5);
//    m_labBottom->setGeometry(iX, iY, iW, iH);
//    g_classSetBGP.SetLabBGP(m_labBottom, ":android/BottomLabel.png", iW, iH);

    /****** 11. Layer Setting ******/
    ui->btnBack->raise();
    ui->btnGallery->raise();

    /****** 12. Present Current UI ******/
    ShowUI(m_iCurUI);
}

void Main_Window::CopyMusicFile()
{
    QFile fileMusicList(":android/MusicList.ini");
    if(!fileMusicList.open(QIODevice::ReadOnly | QIODevice::Text))
        { QMessageBox::information(this, "Warning", "< MusicList.ini > Open Failed£¡\nPath = < " + g_strSourcePath + " > "); close(); return; }
    QTextStream floStream(&fileMusicList);
    while(!floStream.atEnd())
    {
        QString strLine = floStream.readLine();
        if(!strLine.isEmpty())
        {
            QFile copyFile(":android/" + strLine);
            QFile objFile(g_strSourcePath + strLine);
            if(!objFile.exists())
            {
                if(copyFile.copy(g_strSourcePath + strLine) == 0)
                     { QMessageBox::information(this, "Warning", "Copy Music File Failed !\nFile = < " + strLine + " >\nPath = < " + g_strSourcePath + " > "); close(); return; }
            }
        }    // if(!strLine.isEmpty())
    }    // while(!floStream.atEnd())
}

void Main_Window::LoadRecordList()
{
    for(int i=0; i<4; i++)
    {
        g_recordGame.iScoreLev[i] = -1;
        g_recordGame.iTimeRecord[i] = -1;
        g_recordGame.iKingLev[i] = -1;
    }
    QFile fileRecordList(g_strSourcePath + "RecordList.ini");
    if(!fileRecordList.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this, "Warning", "The file cannot be read !"); close(); return;
    }
    QTextStream floStream(&fileRecordList);
    int iSurviveUnlock = 0;
    int iKingUnlock = 0;
    while(!floStream.atEnd())
    {
        QString strLine = floStream.readLine();
        if(!strLine.isEmpty() && strLine.indexOf("=") != -1)
        {
            if(strLine.section("=", 0, 0) == "Score")
            {
                strLine = strLine.section("=", 1, 1);
                for(int i=0; i<4; i++)
                {
                    g_recordGame.iScoreLev[i] = strLine.section(",", i, i).toInt();
                    if(g_recordGame.iScoreLev[i] > 0)
                        iSurviveUnlock ++;
                    if(g_recordGame.iScoreLev[i] > 1)
                        iKingUnlock ++;
                }
            }
            if(strLine.section("=", 0, 0) == "Survive")
            {
                strLine = strLine.section("=", 1, 1);
                for(int i=0; i<4; i++)
                    g_recordGame.iTimeRecord[i] = strLine.section(",", i, i).toInt();
            }
            if(strLine.section("=", 0, 0) == "King")
            {
                strLine = strLine.section("=", 1, 1);
                for(int i=0; i<5; i++)
                    g_recordGame.iKingLev[i] = strLine.section(",", i, i).toInt();
            }
        }    // if(!strLine.isEmpty())
    }    // while(!floStream.atEnd())
    if(iSurviveUnlock == 4)
        g_bSurviveUnlock = 1;
    if(iKingUnlock == 4)
        g_bKingUnlock = 1;
    // Check Record Result
    bool bResult = 0;
    for(int i=0; i<4; i++)
    {
        if(g_recordGame.iScoreLev[i] == -1)
            { bResult = 1; break; }
        if(g_recordGame.iTimeRecord[i] == -1)
            { bResult = 1; break; }
        if(g_recordGame.iKingLev[i] == -1)
            { bResult = 1; break; }
    }
    if(bResult)
        { QMessageBox::information(this, "Warning", "< RecordList.ini > Read Failed!"); close(); return; }
    fileRecordList.close();
}

void Main_Window::PlayMusic(bool bType, QString strName)
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

void Main_Window::FitGameAreaSize()
{
    double dWidth = (double)g_iScreenW;
    double dHeight = (double)g_iScreenH;
    if(dWidth*WH_RATIO < dHeight)    // If the Height of Screen if larger
    {
        g_iWinW = (int)dWidth;
        g_iWinH = (int)(dWidth*WH_RATIO + 0.5);
    }
    if(dWidth*WH_RATIO > dHeight)   // If the Width of Screen if larger
    {
        g_iWinW = (int)(dHeight/WH_RATIO + 0.5);
        g_iWinH = (int)dHeight;
    }
    if(dWidth*WH_RATIO == dHeight)   // If the Width-Height Ratio of Screen is equal to Default
    {
        g_iWinW = g_iScreenW;
        g_iWinH = g_iScreenH;
    }
    g_dSizeRatio = (double)g_iWinW/(double)WIN_W;
    this->setFixedSize(g_iWinW, g_iWinH);
}

/*void Main_Window::SetLabBG(QLabel *labObject, QString strPicName)
{
    labObject->setAutoFillBackground(1);
    labObject->setStyleSheet("QLabel {image: url(:android/" + strPicName + ".png);}");
}

void Main_Window::SetBtnBG(QPushButton *btnObject, QString strPicName, bool bPress)
{
    btnObject->setAutoFillBackground(1);
    QString strStyle = "QPushButton {border-image: url(" + strPicName + ".png);}";    // Normal Status
    strStyle += "QPushButton:hover{border-image: url(" + strPicName + ".png);}";    // Hover Status
    if(bPress)
        strStyle += "QPushButton:hover:pressed{border-image: url(" + strPicName + "Press.png);}";    // Press Status
    else    // No Press Pic Source
        strStyle += "QPushButton:hover:pressed{border-image: url(" + strPicName + ".png);}";    // Press Status
    btnObject->setStyleSheet(strStyle);
}*/

void Main_Window::SetBubbleLabelBG(QLabel *labObj, int iColor)
{
    QString strBGP = "QLabel {image: url(:android/AnimateBubble" + QString::number(iColor) + ".png);}";
    labObj->setStyleSheet(strBGP);
}

void Main_Window::GetBubbleDirection(int &iPosX, int &iPosY, int iDirection)
{
    switch(iDirection)    // Find out where the Bubble is going to move
    {
        case 0: iPosY -= ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Up
        case 1: iPosY += ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Down
        case 2: iPosX -= ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Left
        case 3: iPosX += ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Right
        case 4: iPosX -= ANIMATE_BUBBLE_MOVE_STEP; iPosY -= ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Up & Left
        case 5: iPosX += ANIMATE_BUBBLE_MOVE_STEP; iPosY -= ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Up & Right
        case 6: iPosX -= ANIMATE_BUBBLE_MOVE_STEP; iPosY += ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Down & Left
        case 7: iPosX += ANIMATE_BUBBLE_MOVE_STEP; iPosY += ANIMATE_BUBBLE_MOVE_STEP; break;    // Move Down & Right
    }
    // If the Bubble is going to move out of Screen
    if(iPosX < -1*(int)(ANIMATE_BUBBLE_SIZE*g_dSizeRatio+0.5))
        iPosX = g_iScreenW;
    if(iPosX > g_iScreenW)
        iPosX = -1*(int)(ANIMATE_BUBBLE_SIZE*g_dSizeRatio+0.5);
    if(iPosY < -1*(int)(ANIMATE_BUBBLE_SIZE*g_dSizeRatio+0.5))
        iPosY =  g_iScreenH;
    if(iPosY > g_iScreenH)
        iPosY = -1*(int)(ANIMATE_BUBBLE_SIZE*g_dSizeRatio+0.5);
}

void Main_Window::ShowUI(int iUIIdx)
{
    m_iCurUI = iUIIdx;
    switch(iUIIdx)
    {
        case START_UI :
            ui->btnModelSelect->setVisible(1);
            ui->btnRecord->setVisible(1);
            ui->btnHelp->setVisible(1);
            ui->btnSetting->setVisible(0);
            ui->btnScoreModel->setVisible(0);
            ui->btnSurviveModel->setVisible(0);
            ui->btnKingModel->setVisible(0);
            ui->labSelectBGP->setVisible(0);
            ui->btnSelectXS->setVisible(0);
            ui->btnSelectZJ->setVisible(0);
            ui->btnSelectYH->setVisible(0);
            ui->btnSelectDC->setVisible(0);
            ui->btnEasy->setVisible(0);
            ui->btnNormal->setVisible(0);
            ui->btnHard->setVisible(0);
            ui->btnBack->setVisible(0);
            ui->btnGallery->setVisible(1);
            g_classSetBGP.SetLabBGP(ui->labMainBGP, ":android/MainMenuBGP.jpg", g_iWinW, g_iWinH);
            break;
        case MODEL_SELECT_UI :
            ui->btnModelSelect->setVisible(0);
            ui->btnRecord->setVisible(0);
            ui->btnHelp->setVisible(0);
            ui->btnSetting->setVisible(0);
            ui->btnScoreModel->setVisible(1);
            ui->btnSurviveModel->setVisible(1);
            ui->btnKingModel->setVisible(1);
            ui->labSelectBGP->setVisible(0);
            ui->btnSelectXS->setVisible(0);
            ui->btnSelectZJ->setVisible(0);
            ui->btnSelectYH->setVisible(0);
            ui->btnSelectDC->setVisible(0);
            ui->btnEasy->setVisible(0);
            ui->btnNormal->setVisible(0);
            ui->btnHard->setVisible(0);
            ui->btnBack->setVisible(1);
            ui->btnGallery->setVisible(0);
            g_classSetBGP.SetLabBGP(ui->labMainBGP, ":android/ModeSelectBGP.jpg", g_iWinW, g_iWinH);
            break;
        case ROLE_SELECT_UI :
            PlayMusic(MUSIC_TYPE, "Memory.mp3");
            ui->btnModelSelect->setVisible(0);
            ui->btnRecord->setVisible(0);
            ui->btnHelp->setVisible(0);
            ui->btnSetting->setVisible(0);
            ui->btnScoreModel->setVisible(0);
            ui->btnSurviveModel->setVisible(0);
            ui->btnKingModel->setVisible(0);
            ui->labSelectBGP->setVisible(1);
            ui->btnSelectXS->setVisible(1);
            ui->btnSelectZJ->setVisible(1);
            ui->btnSelectYH->setVisible(1);
            ui->btnSelectDC->setVisible(1);
            ui->labSelectBGP->raise();
            ui->btnSelectXS->raise();
            ui->btnSelectZJ->raise();
            ui->btnSelectYH->raise();
            ui->btnSelectDC->raise();
            if(g_iModel != KING_MODEL)
            {
                ui->btnEasy->setVisible(1);
                ui->btnNormal->setVisible(1);
                ui->btnHard->setVisible(1);
                ui->btnEasy->raise();
                ui->btnNormal->raise();
                ui->btnHard->raise();
                LevAdjust(g_iCurLev);
            }
            ui->btnBack->setVisible(1);
            ui->btnBack->raise();
            ui->btnGallery->setVisible(0);
            break;
        case RECORD_UI :
            PlayMusic(MUSIC_TYPE, "Info.mp3");
            m_widRecordBoard->SetRecordLabel();
            m_widRecordBoard->show();
            break;
        case HELP_UI :
            PlayMusic(MUSIC_TYPE, "Info.mp3");
            m_widHelp->InitVariable();
            m_widHelp->show();
            break;
        case GALLERY_UI :
            PlayMusic(MUSIC_TYPE, "Memory.mp3");
            m_widGallery->InitVariable();
            m_widGallery->show();
            break;
         case GAME_UI :
            if(g_iModel == KING_MODEL)
                LevAdjust(0);
            m_widGameStart->SetWidget();
            m_widGameStart->show();
    }
}

void Main_Window::LevAdjust(int iLev)
{
    g_iCurLev = iLev;
    g_iBubbleTypeLev = 4+iLev*2;
    g_iBubbleHeight = 2+iLev*2;
    g_iSpeedLev = 1000-GENERATE_BUBBLE_SPEED*(iLev*3+1);
    g_iScoreLimit = 100+iLev*LEV_ADJUST;    // 500 + iLev*500;
    g_iLoveLimit = 100+iLev*LEV_ADJUST;    // 500 + iLev*500;

    // Set Level Button Status
    switch(iLev)
    {
        case EASY_LEV:
            g_classSetBGP.SetBtnBGP(ui->btnEasy, "Difficulty_EasyBtn_Select", 1);    // Easy Button Selected
            g_classSetBGP.SetBtnBGP(ui->btnNormal, "Difficulty_NormalBtn", 1);
            g_classSetBGP.SetBtnBGP(ui->btnHard, "Difficulty_HardBtn", 1);
            break;
        case NORMAL_LEV:
            g_classSetBGP.SetBtnBGP(ui->btnEasy, "Difficulty_EasyBtn", 1);
            g_classSetBGP.SetBtnBGP(ui->btnNormal, "Difficulty_NormalBtn_Select", 1);    // Normal Button Selected
            g_classSetBGP.SetBtnBGP(ui->btnHard, "Difficulty_HardBtn", 1);
            break;
        case HARD_LEV :
            g_classSetBGP.SetBtnBGP(ui->btnEasy, "Difficulty_EasyBtn", 1);
            g_classSetBGP.SetBtnBGP(ui->btnNormal, "Difficulty_NormalBtn", 1);
            g_classSetBGP.SetBtnBGP(ui->btnHard, "Difficulty_HardBtn_Select", 1);    // Hard Button Selected
            break;
    }
}

void Main_Window::Interlude(int iUI)
{
    /****** 1. Adjust Interlude Bubbles Layer ******/
    for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i ++)
    {
        for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j ++)
        {
            if(i<INTERLUDE_BUBBLE_ROW && j<INTERLUDE_BUBBLE_COL)
                m_labInterludeBubbleForward[i][j]->raise();
             m_labInterludeBubbleBack[i][j]->raise();
        }
    }

    /****** 2. Bubbles Animation ******/
    int iBubbleSize = (int)((ANIMATE_BUBBLE_SIZE)*g_dSizeRatio+0.5);
    QPropertyAnimation *animateInterludeForward[INTERLUDE_BUBBLE_ROW][INTERLUDE_BUBBLE_COL];
    QPropertyAnimation *animateInterludeBack[INTERLUDE_BUBBLE_ROW+1][INTERLUDE_BUBBLE_COL+1];
    for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i ++)
    {
        int iTime = INTERLUDE_TIME + (INTERLUDE_BUBBLE_ROW-i)*50;
        for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j ++)
        {
            if(i<INTERLUDE_BUBBLE_ROW && j<INTERLUDE_BUBBLE_COL)
            {
                m_labInterludeBubbleForward[i][j]->setVisible(1);
                int iPosX = j*iBubbleSize;
                int iPosY = i*iBubbleSize;
                QTime timeSeed;
                timeSeed = QTime::currentTime();
                qsrand(timeSeed.msec() + timeSeed.second()*1000);
                iTime += (qrand()%6)*10;
                animateInterludeForward[i][j] = new QPropertyAnimation(m_labInterludeBubbleForward[i][j], "pos");
                animateInterludeForward[i][j]->setDuration(iTime);
                animateInterludeForward[i][j]->setEasingCurve(QEasingCurve::OutBounce);
                animateInterludeForward[i][j]->setStartValue(QPoint(iPosX, -1*iBubbleSize));
                animateInterludeForward[i][j]->setEndValue(QPoint(iPosX, iPosY));
                animateInterludeForward[i][j]->start();
            }
            m_labInterludeBubbleBack[i][j]->setVisible(1);
            int iPosX = j*iBubbleSize-iBubbleSize/2;
            int iPosY = i*iBubbleSize-iBubbleSize/2;
            QTime timeSeed;
            timeSeed = QTime::currentTime();
            qsrand(timeSeed.msec() + timeSeed.second()*1000);
            iTime += (qrand()%6)*10;
            animateInterludeBack[i][j] = new QPropertyAnimation(m_labInterludeBubbleBack[i][j], "pos");
            animateInterludeBack[i][j]->setDuration(iTime);
            animateInterludeBack[i][j]->setEasingCurve(QEasingCurve::OutBounce);
            animateInterludeBack[i][j]->setStartValue(QPoint(iPosX, -1*iBubbleSize));
            animateInterludeBack[i][j]->setEndValue(QPoint(iPosX, iPosY));
            animateInterludeBack[i][j]->start();
        }    // for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j ++)
    }    // for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i ++)

    QString strBGP;
    switch(iUI)
    {
        case START_UI : strBGP = "WhiteCover"; m_iCurUI = START_UI; break;
        case MODEL_SELECT_UI : strBGP = "WhiteCover"; m_iCurUI = MODEL_SELECT_UI; break;
        case RECORD_UI : strBGP = "WhiteCover"; m_iCurUI = RECORD_UI; break;
        case HELP_UI : strBGP = "WhiteCover"; m_iCurUI = HELP_UI; break;
        case GALLERY_UI : strBGP = "GameBGP00"; m_iCurUI = GALLERY_UI; break;
        case ROLE_SELECT_UI : strBGP = "SelectBGP"; m_iCurUI = ROLE_SELECT_UI; break;
        case GAME_UI : strBGP = "SelectBGP"; m_iCurUI = GAME_UI; break;
    }
    connect(animateInterludeBack[0][0], SIGNAL(finished()), this, SLOT(slot_InterludeHalf()));
//    m_labBottom->raise();
}

void Main_Window::slot_CloudTimer()
{
    int iCloudPosX = ui->labCloud->x();    // Cloud's Old Position in X Direction
    if(iCloudPosX < g_iScreenW)    // If the Cloud is still in the Screen's range
        iCloudPosX += CLOUD_MOVE_STEP;
    else    // If the Cloud is out of the Screen's range
        iCloudPosX = -1*CLOUD_W;
    ui->labCloud->move((int)(iCloudPosX*g_dSizeRatio+0.5), (int)(CLOUD_Y*g_dSizeRatio+0.5));    // Move to New Position
}

void Main_Window::slot_BubbleTimer()
{
    for(int i=0; i<8; i++)
    {
        if(m_iChangeDirectionTime < ANIMATE_BUBLE_MOVE_TIME_MAX)    // If the Time Count of Bubbles' Move is not enough
            m_iChangeDirectionTime ++;
        else    // If the Bubble has been moved for a while
        {
            // Clear the Time Count and Get a Random Direction for the Bubble
            //srand( (unsigned)time( NULL ) );
            QTime timeSeed;
            timeSeed = QTime::currentTime();
            qsrand(timeSeed.msec() + timeSeed.second()*1000);
            m_iDirection[i] = qrand()%8;    // from 0 to 7
            m_iChangeDirectionTime = 0;
        }
        int iCurPosX = m_labBubble[i]->x();    // Old Position in X Direction
        int iCurPosY = m_labBubble[i]->y();    // Old Position in Y Direction
        GetBubbleDirection(iCurPosX, iCurPosY, m_iDirection[i]);    // Get Future Steps in X and Y Direction according to the Direction Index (m_iDirection[i])
        m_labBubble[i]->move(iCurPosX, iCurPosY);    // Move to New Position
    }
}

void Main_Window::slot_BGMStop(QMediaPlayer::State)
{
    if(g_playerBGM->state() == 0)    // Music Stop Status
        g_playerBGM->play();
}

void Main_Window::slot_GameOver()
{
    //PlayMusic(MUSIC_TYPE, "RoleSelect.mp3");
    m_iCloseUI = GAME_UI;
    Interlude(ROLE_SELECT_UI);
}

void Main_Window::slot_InfoClose(int iUI)
{
    m_iCloseUI = iUI;
    Interlude(START_UI);
}

void Main_Window::slot_InterludeHalf()
{
    ShowUI(m_iCurUI);
    if(m_iCloseUI != -1)
    {
        switch(m_iCloseUI)
        {
            case RECORD_UI : m_widRecordBoard->hide(); PlayMusic(MUSIC_TYPE, "MainBGM.mp3"); break;
            case HELP_UI : m_widHelp->hide(); PlayMusic(MUSIC_TYPE, "MainBGM.mp3"); break;
            case GALLERY_UI : m_widGallery->hide(); PlayMusic(MUSIC_TYPE, "MainBGM.mp3"); break;
            case GAME_UI : m_widGameStart->hide(); break;
        }
        m_iCloseUI = -1;
    }
    for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i ++)
    {
        for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j ++)
        {
            if(i<INTERLUDE_BUBBLE_ROW && j<INTERLUDE_BUBBLE_COL)
                m_labInterludeBubbleForward[i][j]->raise();
             m_labInterludeBubbleBack[i][j]->raise();
        }
    }
    QPropertyAnimation *animateInterludeForward[INTERLUDE_BUBBLE_ROW][INTERLUDE_BUBBLE_COL];
    QPropertyAnimation *animateInterludeBack[INTERLUDE_BUBBLE_ROW+1][INTERLUDE_BUBBLE_COL+1];
    for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i ++)
    {
        for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j ++)
        {
            if(i<INTERLUDE_BUBBLE_ROW && j<INTERLUDE_BUBBLE_COL)
            {
                int iPosX = m_labInterludeBubbleForward[i][j]->x();
                int iPosY = m_labInterludeBubbleForward[i][j]->y();
                QTime timeSeed;
                timeSeed = QTime::currentTime();
                qsrand(timeSeed.msec() + timeSeed.second()*1000);
                int iTime = (qrand()%5 + 1)*500;
                int iXMove = (qrand()%10-5)*20;
                animateInterludeForward[i][j] = new QPropertyAnimation(m_labInterludeBubbleForward[i][j], "pos");
                animateInterludeForward[i][j]->setDuration(iTime);
                animateInterludeForward[i][j]->setEasingCurve(QEasingCurve::OutBounce);
                animateInterludeForward[i][j]->setStartValue(QPoint(iPosX, iPosY));
                animateInterludeForward[i][j]->setEndValue(QPoint(iPosX+iXMove, g_iWinH));
                animateInterludeForward[i][j]->start();
            }
            int iPosX = m_labInterludeBubbleBack[i][j]->x();
            int iPosY = m_labInterludeBubbleBack[i][j]->y();
            QTime timeSeed;
            timeSeed = QTime::currentTime();
            qsrand(timeSeed.msec() + timeSeed.second()*1000);
            int iTime = (qrand()%5 + 1)*500;
            if(i == INTERLUDE_BUBBLE_ROW && j== INTERLUDE_BUBBLE_COL)
                iTime = 2500;
            int iXMove = (qrand()%10-5)*20;
            animateInterludeBack[i][j] = new QPropertyAnimation(m_labInterludeBubbleBack[i][j], "pos");
            animateInterludeBack[i][j]->setDuration(iTime);
            animateInterludeBack[i][j]->setEasingCurve(QEasingCurve::OutBounce);
            animateInterludeBack[i][j]->setStartValue(QPoint(iPosX, iPosY));
            animateInterludeBack[i][j]->setEndValue(QPoint(iPosX+iXMove, g_iWinH));
            animateInterludeBack[i][j]->start();
        }    // for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j ++)
    }    // for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i ++)
    connect(animateInterludeBack[INTERLUDE_BUBBLE_ROW][INTERLUDE_BUBBLE_COL], SIGNAL(finished()), this, SLOT(slot_InterludeFinish()));
//    m_labBottom->raise();
}

void Main_Window::slot_InterludeFinish()
{
    for(int i=0; i<INTERLUDE_BUBBLE_ROW+1; i ++)
    {
        for(int j=0; j<INTERLUDE_BUBBLE_COL+1; j ++)
        {
            if(i<INTERLUDE_BUBBLE_ROW && j< INTERLUDE_BUBBLE_COL)
                m_labInterludeBubbleForward[i][j]->setVisible(0);
            else
                m_labInterludeBubbleBack[i][j]->setVisible(0);
        }
    }
    if(m_iCurUI == GAME_UI)
    {
        switch(g_iModel)
        {
            case SCORE_MODEL :
                m_dlgRemind = new Remind_Dlg(this);
                m_dlgRemind->SetWidget(SCORE_REMIND_DLG);
                m_dlgRemind->exec();
                break;
            case SURVIVE_MODEL :
                m_dlgRemind = new Remind_Dlg(this);
                m_dlgRemind->SetWidget(SURVIVE_REMIND_DLG);
                m_dlgRemind->exec();
                m_widGameStart->m_timerSurvive->start(1000);
                break;
            case KING_MODEL :
                m_dlgRemind = new Remind_Dlg(this);
                m_dlgRemind->SetWidget(KING_REMIND_DLG);
                m_dlgRemind->exec();
                break;
        }    // switch(g_iModel)
        m_widGameStart->m_bClick = 1;
        m_widGameStart->m_timerGenerateBubble->start(g_iSpeedLev);
    }    // if(m_iCurUI == GAME_UI)
}

void Main_Window::on_btnModelSelect_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");

    int iBtnW = (int)(MAIN_BTN_W*g_dSizeRatio+0.5);
    QPropertyAnimation *animateMove1 = new QPropertyAnimation(ui->btnModelSelect, "pos");
    animateMove1->setDuration(ANIMATE_TIME);
    animateMove1->setEasingCurve(QEasingCurve::Linear);
    animateMove1->setStartValue(QPoint((int)(FIRST_BTN_X*g_dSizeRatio+0.5), (int)(FIRST_BTN_Y*g_dSizeRatio+0.5)));
    animateMove1->setEndValue(QPoint(g_iWinW+iBtnW, (int)(FIRST_BTN_Y*g_dSizeRatio+0.5)));
    QPropertyAnimation *animateMove2 = new QPropertyAnimation(ui->btnRecord, "pos");
    animateMove2->setDuration(ANIMATE_TIME);
    animateMove2->setEasingCurve(QEasingCurve::Linear);
    animateMove2->setStartValue(QPoint((int)(SECOND_BTN_X*g_dSizeRatio+0.5), (int)(SECOND_BTN_Y*g_dSizeRatio+0.5)));
    animateMove2->setEndValue(QPoint(-iBtnW, (int)(SECOND_BTN_Y*g_dSizeRatio+0.5)));
    QPropertyAnimation *animateMove3 = new QPropertyAnimation(ui->btnHelp, "pos");
    animateMove3->setDuration(ANIMATE_TIME);
    animateMove3->setEasingCurve(QEasingCurve::Linear);
    animateMove3->setStartValue(QPoint((int)(THIRD_BTN_X*g_dSizeRatio+0.5), (int)(THIRD_BTN_Y*g_dSizeRatio+0.5)));
    animateMove3->setEndValue(QPoint(g_iWinW+iBtnW,  (int)(THIRD_BTN_Y*g_dSizeRatio+0.5)));
    QPropertyAnimation *animateMove4 = new QPropertyAnimation(ui->btnScoreModel, "pos");
    animateMove4->setDuration(ANIMATE_TIME);
    animateMove4->setEasingCurve(QEasingCurve::Linear);
    animateMove4->setStartValue(QPoint(g_iWinW+iBtnW, (int)(FIRST_BTN_Y*g_dSizeRatio+0.5)));
    animateMove4->setEndValue(QPoint((int)(FIRST_BTN_X*g_dSizeRatio+0.5), (int)(FIRST_BTN_Y*g_dSizeRatio+0.5)));
    QPropertyAnimation *animateMove5 = new QPropertyAnimation(ui->btnSurviveModel, "pos");
    animateMove5->setDuration(ANIMATE_TIME);
    animateMove5->setEasingCurve(QEasingCurve::Linear);
    animateMove5->setStartValue(QPoint(-iBtnW, (int)(SECOND_BTN_Y*g_dSizeRatio+0.5)));
    animateMove5->setEndValue(QPoint((int)(SECOND_BTN_X*g_dSizeRatio+0.5), (int)(SECOND_BTN_Y*g_dSizeRatio+0.5)));
    QPropertyAnimation *animateMove6 = new QPropertyAnimation(ui->btnKingModel, "pos");
    animateMove6->setDuration(ANIMATE_TIME);
    animateMove6->setEasingCurve(QEasingCurve::Linear);
    animateMove6->setStartValue(QPoint(g_iWinW+iBtnW,  (int)(THIRD_BTN_Y*g_dSizeRatio+0.5)));
    animateMove6->setEndValue(QPoint((int)(THIRD_BTN_X*g_dSizeRatio+0.5), (int)(THIRD_BTN_Y*g_dSizeRatio+0.5)));

    animateMove1->start();
    animateMove2->start();
    animateMove3->start();
    animateMove4->start();
    animateMove5->start();
    animateMove6->start();

    ShowUI(MODEL_SELECT_UI);
}

void Main_Window::on_btnRecord_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    Interlude(RECORD_UI);
}

void Main_Window::on_btnHelp_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    Interlude(HELP_UI);
}

void Main_Window::on_btnSetting_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
}

void Main_Window::on_btnBack_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    switch(m_iCurUI)
    {
        case START_UI :
            break;
        case MODEL_SELECT_UI :
        {
            int iBtnW = (int)(MAIN_BTN_W*g_dSizeRatio+0.5);
            QPropertyAnimation *animateMove1 = new QPropertyAnimation(ui->btnModelSelect, "pos");
            animateMove1->setDuration(ANIMATE_TIME);
            animateMove1->setEasingCurve(QEasingCurve::Linear);
            animateMove1->setStartValue(QPoint(g_iWinW+iBtnW, (int)(FIRST_BTN_Y*g_dSizeRatio+0.5)));
            animateMove1->setEndValue(QPoint((int)(FIRST_BTN_X*g_dSizeRatio+0.5), (int)(FIRST_BTN_Y*g_dSizeRatio+0.5)));
            QPropertyAnimation *animateMove2 = new QPropertyAnimation(ui->btnRecord, "pos");
            animateMove2->setDuration(ANIMATE_TIME);
            animateMove2->setEasingCurve(QEasingCurve::Linear);
            animateMove2->setStartValue(QPoint(-iBtnW, (int)(SECOND_BTN_Y*g_dSizeRatio+0.5)));
            animateMove2->setEndValue(QPoint((int)(SECOND_BTN_X*g_dSizeRatio+0.5), (int)(SECOND_BTN_Y*g_dSizeRatio+0.5)));
            QPropertyAnimation *animateMove3 = new QPropertyAnimation(ui->btnHelp, "pos");
            animateMove3->setDuration(ANIMATE_TIME);
            animateMove3->setEasingCurve(QEasingCurve::Linear);
            animateMove3->setStartValue(QPoint(g_iWinW+iBtnW,  (int)(THIRD_BTN_Y*g_dSizeRatio+0.5)));
            animateMove3->setEndValue(QPoint((int)(THIRD_BTN_X*g_dSizeRatio+0.5), (int)(THIRD_BTN_Y*g_dSizeRatio+0.5)));
            QPropertyAnimation *animateMove4 = new QPropertyAnimation(ui->btnScoreModel, "pos");
            animateMove4->setDuration(ANIMATE_TIME);
            animateMove4->setEasingCurve(QEasingCurve::Linear);
            animateMove4->setStartValue(QPoint((int)(SCORE_MODEL_X*g_dSizeRatio+0.5), (int)(SCORE_MODEL_Y*g_dSizeRatio+0.5)));
            animateMove4->setEndValue(QPoint(g_iWinW+iBtnW, (int)(SCORE_MODEL_Y*g_dSizeRatio+0.5)));
            QPropertyAnimation *animateMove5 = new QPropertyAnimation(ui->btnSurviveModel, "pos");
            animateMove5->setDuration(ANIMATE_TIME);
            animateMove5->setEasingCurve(QEasingCurve::Linear);
            animateMove5->setStartValue(QPoint((int)(SURVIVE_MODEL_X*g_dSizeRatio+0.5), (int)(SURVIVE_MODEL_Y*g_dSizeRatio+0.5)));
            animateMove5->setEndValue(QPoint(-iBtnW, (int)(SURVIVE_MODEL_Y*g_dSizeRatio+0.5)));
            QPropertyAnimation *animateMove6 = new QPropertyAnimation(ui->btnKingModel, "pos");
            animateMove6->setDuration(ANIMATE_TIME);
            animateMove6->setEasingCurve(QEasingCurve::Linear);
            animateMove6->setStartValue(QPoint((int)(KING_MODEL_X*g_dSizeRatio+0.5), (int)(KING_MODEL_Y*g_dSizeRatio+0.5)));
            animateMove6->setEndValue(QPoint(g_iWinW+iBtnW,  (int)(KING_MODEL_Y*g_dSizeRatio+0.5)));
            animateMove4->start();
            animateMove5->start();
            animateMove6->start();
            animateMove1->start();
            animateMove2->start();
            animateMove3->start();
            ShowUI(START_UI);
            break;
        }
        case ROLE_SELECT_UI :
            PlayMusic(MUSIC_TYPE, "MainBGM.mp3");
            Interlude(MODEL_SELECT_UI);
            //ShowUI(MODEL_SELECT_UI);
            break;
        case RECORD_UI :
            break;
    }
}

void Main_Window::on_btnSelectXS_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_iRole = XS_ROLE;
    Interlude(GAME_UI);
}

void Main_Window::on_btnSelectZJ_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_iRole = ZJ_ROLE;
    Interlude(GAME_UI);
}

void Main_Window::on_btnSelectYH_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_iRole = YH_ROLE;
    Interlude(GAME_UI);
}

void Main_Window::on_btnSelectDC_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_iRole = DC_ROLE;
    Interlude(GAME_UI);
}

void Main_Window::on_btnScoreModel_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_iModel = SCORE_MODEL;
    Interlude(ROLE_SELECT_UI);
}

void Main_Window::on_btnSurviveModel_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_bSurviveUnlock = 1;
    for(int i=0; i<4; i++)
        if(g_recordGame.iScoreLev[i] == 0)
            { g_bSurviveUnlock = 0; break; }
    if(g_bSurviveUnlock)
    {
        g_iModel = SURVIVE_MODEL;
        Interlude(ROLE_SELECT_UI);
    }
    else
    {
        PlayMusic(SOUND_TYPE, "Locked.mp3");
        m_dlgRemind = new Remind_Dlg(this);
        m_dlgRemind->SetWidget(SURVIVE_LOCKED_DLG);
        m_dlgRemind->exec();
    }
}

void Main_Window::on_btnKingModel_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    g_bKingUnlock = 1;
    for(int i=0; i<4; i++)
        if(g_recordGame.iScoreLev[i] <= 1)
            { g_bKingUnlock = 0; break; }
    if(g_bKingUnlock)
    {
        g_iModel = KING_MODEL;
        ShowUI(ROLE_SELECT_UI);
    }
    else
    {
        PlayMusic(SOUND_TYPE, "Locked.mp3");
        m_dlgRemind = new Remind_Dlg(this);
        m_dlgRemind->SetWidget(KING_LOCKED_DLG);
        m_dlgRemind->exec();
    }
}

void Main_Window::on_btnEasy_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    LevAdjust(EASY_LEV);
}

void Main_Window::on_btnNormal_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    LevAdjust(NORMAL_LEV);
}

void Main_Window::on_btnHard_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    LevAdjust(HARD_LEV);
}

void Main_Window::on_btnGallery_clicked()
{
    PlayMusic(SOUND_TYPE, "Camera.mp3");
    Interlude(GALLERY_UI);
}
