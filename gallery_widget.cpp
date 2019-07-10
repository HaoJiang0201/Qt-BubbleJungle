#include "gallery_widget.h"
#include "ui_gallery_widget.h"

extern int g_iWinW;    // App Window Width after Adjust
extern int g_iWinH;    // App Window Height after Adjust
extern double g_dSizeRatio;    // The Width-Height Ratio required to be adjusted for all the Widgets in App
extern QMediaPlayer *g_playerBGM;    // Game Background Music, Loop
extern QMediaPlayer *g_playerSound;    // All kinds of Sound, Only Play for One Time, such as Button Click Sound
extern QString g_strSourcePath;    // Source Files' Path in Device
extern RECORD g_recordGame;    // Game Record Structure
extern SetWidgetBGP_Class g_classSetBGP;    // Set Widget BGP

Gallery_Widget::Gallery_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gallery_Widget)
{
    ui->setupUi(this);
    InitVariable();
    InitWidget();
}

Gallery_Widget::~Gallery_Widget()
{
    delete ui;
}

void Gallery_Widget::InitVariable()
{
    // Able to Receive All Mouse Action Signal
    setMouseTracking(1);
    // Set Image File List
    InitImgList();
    // Set Signs and Timer
    m_iImgIdx = 0;
    m_bPressing = 0;
    m_bMoving = 0;
    m_iOrgX = -1;
    m_iOrgY = -1;
    m_iDistance = 0;
    g_classSetBGP.SetLabBGP(ui->labMove1, m_strImgFile[m_iImgIdx], g_iWinW, g_iWinH);
    ui->btnBack->setVisible(1);
}

void Gallery_Widget::InitWidget()
{
    SetBGP();

    /****** Animate QTimer ******/
    m_timerAnimate = new QTimer(this);

    /****** Animate Label ******/
    ui->labMove1->setGeometry(0, 0, g_iWinW, g_iWinH);
    ui->labMove2->setGeometry(0, 0, g_iWinW, g_iWinH);

    /****** Back Button ******/
    int iBtnSize = (int)(BACK_SIZE*g_dSizeRatio+0.5);
    ui->btnBack->setGeometry(g_iWinW-iBtnSize, g_iWinH-iBtnSize-(int)(50*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    g_classSetBGP.SetBtnBGP(ui->btnBack, "BackBtn");

    /****** Bottom Cover Label ******/
//    m_labBottom = new QLabel(this);
//    int iX = 0;
//    int iY = (int)(g_iWinH-(BOTTOM_COVER_H/2)*g_dSizeRatio+0.5);
//    int iW = g_iWinW;
//    int iH = (int)(BOTTOM_COVER_H*g_dSizeRatio+0.5);
//    m_labBottom->setGeometry(iX, iY, iW, iH);
//    g_classSetBGP.SetLabBGP(m_labBottom, ":android/BottomLabel.png", iW, iH);
}

void Gallery_Widget::InitImgList()
{
    int iImgIdx = 0;
    for(int i=0; i<4; i++)
        for(int j=0; j<3; j++)
        {
            m_strImgFile[iImgIdx] = ":android/GameBGP";
            m_strImgFile[iImgIdx] += QString::number(i) + QString::number(j);
            if(g_recordGame.iKingLev[i] < j)
                m_strImgFile[iImgIdx] += "Hide";
            m_strImgFile[iImgIdx] += ".jpg";
            iImgIdx ++;
        }
}

void Gallery_Widget::GoNextImage()
{
    if(m_iImgIdx < GALLERY_NUM-1)
    {
        PlayMusic(SOUND_TYPE, "Camera.mp3");
        m_iImgIdx ++;
        ui->labMove2->move(g_iWinW, 0);
        g_classSetBGP.SetLabBGP(ui->labMove2, m_strImgFile[m_iImgIdx], g_iWinW, g_iWinH);
        m_bMoving = 1;
        connect(m_timerAnimate, SIGNAL(timeout()), this, SLOT(slot_Moving()));
        m_timerAnimate->start(ANIMATE_INTER);
    }
}

void Gallery_Widget::GoPrevImage()
{
    if(m_iImgIdx > 0)
    {
        PlayMusic(SOUND_TYPE, "Camera.mp3");
        m_iImgIdx --;
        ui->labMove2->move(-g_iWinW, 0);
        g_classSetBGP.SetLabBGP(ui->labMove2, m_strImgFile[m_iImgIdx], g_iWinW, g_iWinH);
        m_bMoving = 1;
        connect(m_timerAnimate, SIGNAL(timeout()), this, SLOT(slot_Moving()));
        m_timerAnimate->start(ANIMATE_INTER);
    }
}

void Gallery_Widget::SetBGP()
{
    /****** Set Widget BGP ******/
    setGeometry(0, 0, g_iWinW, g_iWinH);
    QImage imgBGP(m_strImgFile[m_iImgIdx]);
    g_classSetBGP.SetWidgetBGP(this, imgBGP, g_iWinW, g_iWinH);
}

void Gallery_Widget::PlayMusic(bool bType, QString strName)
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

void Gallery_Widget::mousePressEvent(QMouseEvent *event)
{
    if(m_bMoving)    // If the Picture is Moving, no more Mouse Event Accepted
        return;
    if(!m_bPressing)    // Get the First Touching Point Position
    {
        m_iOrgX = event->x();
        m_iOrgY = event->y();
    }
    m_bPressing = 1;
}

void Gallery_Widget::mouseReleaseEvent (QMouseEvent *event)
{
    if(m_bMoving)    // If the Picture is Moving, no more Mouse Event Accepted
        return;
    int iCurX = event->x();
    m_iDistance = iCurX - m_iOrgX;
    if(abs(m_iDistance) >= MIN_SWIP_SIZE)
    {
        if(m_iDistance < 0)    // Swip Left
            GoNextImage();
        else                          // Swip Right
            GoPrevImage();
    }
}

void Gallery_Widget::on_btnBack_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    ui->btnBack->setVisible(0);
    emit sigGalleryClose(GALLERY_UI);
}

void Gallery_Widget::slot_Moving()
{
    int iCurX1 = ui->labMove1->x();
    int iCurX2 = ui->labMove2->x();
    if(iCurX1 > -g_iWinW && m_iDistance < 0)    // Moving Left
    {
        ui->labMove1->move(iCurX1-ANIMATE_STEP, 0);
        ui->labMove2->move(iCurX2-ANIMATE_STEP, 0);
        if(iCurX1+g_iWinW <= ANIMATE_STEP)    // If the Left Space is Less than Animate Step
        {
            ui->labMove1->move(-g_iWinW, 0);
            ui->labMove2->move(0, 0);
        }
    }
    if(iCurX1 < g_iWinW && m_iDistance > 0)    // Moving Right
    {
        ui->labMove1->move(iCurX1+ANIMATE_STEP, 0);
        ui->labMove2->move(iCurX2+ANIMATE_STEP, 0);
        if(g_iWinW-iCurX1 <= ANIMATE_STEP)    // If the Left Space is Less than Animate Step
        {
            ui->labMove1->move(g_iWinW, 0);
            ui->labMove2->move(0, 0);
        }
    }
    if(iCurX1 <= -g_iWinW || iCurX1 >= g_iWinW)    // Moving End
    {
        m_timerAnimate->stop();
        disconnect(m_timerAnimate, SIGNAL(timeout()), this, SLOT(slot_Moving()));
        slot_MovingFinish();
    }
}

void Gallery_Widget::slot_MovingFinish()
{
    // Reset Labels and Signs after Moving Finished
    g_classSetBGP.SetLabBGP(ui->labMove1, m_strImgFile[m_iImgIdx], g_iWinW, g_iWinH);
    ui->labMove1->move(0, 0);
    ui->labMove2->move(-g_iWinW, 0);
    m_bPressing = 0;
    m_bMoving = 0;
    m_iOrgX = -1;
    m_iOrgY = -1;
}
