#include "help_widget.h"
#include "ui_help_widget.h"

extern int g_iWinW;    // App Window Width after Adjust
extern int g_iWinH;    // App Window Height after Adjust
extern double g_dSizeRatio;    // The Width-Height Ratio required to be adjusted for all the Widgets in App
extern QMediaPlayer *g_playerBGM;    // Game Background Music, Loop
extern QMediaPlayer *g_playerSound;    // All kinds of Sound, Only Play for One Time, such as Button Click Sound
extern QString g_strSourcePath;    // Source Files' Path in Device
extern SetWidgetBGP_Class g_classSetBGP;    // Set Widget BGP

Help_Widget::Help_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help_Widget)
{
    ui->setupUi(this);
    InitVariable();
    InitWidget();
}

Help_Widget::~Help_Widget()
{
    delete ui;
}

void Help_Widget::InitVariable()
{
    ui->btnBack->setVisible(1);
}

void Help_Widget::InitWidget()
{
    m_iCurPage = 0;
    /****** Main BGP ******/
    setGeometry(0, 0, g_iWinW, g_iWinH);
    QImage imgBGP(":android/WhiteCover.png");
    g_classSetBGP.SetWidgetBGP(this, imgBGP, g_iWinW, g_iWinH);

    /****** Main Button & Label ******/
    int iBtnSize = (int)(HELP_BTN_SIZE*g_dSizeRatio+0.5);
    ui->btnLastPage->setGeometry((int)(HELP_LEFT_X*g_dSizeRatio+0.5), (int)(HELP_BTN_Y*g_dSizeRatio+0.5)-(int)(50*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    ui->btnNextPage->setGeometry((int)(HELP_RIGHT_X*g_dSizeRatio+0.5), (int)(HELP_BTN_Y*g_dSizeRatio+0.5)-(int)(50*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    g_classSetBGP.SetBtnBGP(ui->btnLastPage, "LeftBtn");
    g_classSetBGP.SetBtnBGP(ui->btnNextPage, "RightBtn");
//    ui->labPage->setGeometry((int)(HELP_PAGE_X*g_dSizeRatio+0.5), (int)(HELP_BTN_Y*g_dSizeRatio+0.5), (int)(HELP_PAGE_W*g_dSizeRatio+0.5), iBtnSize);
//    ui->labPage->setText(QString::number(m_iCurPage+1) + "/4");
//    int iX = (int)(HELP_LAB_X*g_dSizeRatio+0.5);
//    int iY = (int)(HELP_LAB_Y*g_dSizeRatio+0.5);
//    int iW = (int)(HELP_LAB_W*g_dSizeRatio+0.5);
//    int iH = (int)(HELP_LAB_H*g_dSizeRatio+0.5);
    ui->labHelp->setGeometry(0, 0, g_iWinW, g_iWinH);
    g_classSetBGP.SetLabBGP(ui->labHelp, ":android/HelpBGP_Rule.jpg", g_iWinW, g_iWinH);
    ui->btnLastPage->raise();
    ui->btnNextPage->raise();

    /****** Back Button ******/
    iBtnSize = (int)(BACK_SIZE*g_dSizeRatio+0.5);
    ui->btnBack->setGeometry(g_iWinW-iBtnSize, g_iWinH-iBtnSize-(int)(50*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    g_classSetBGP.SetBtnBGP(ui->btnBack, "BackBtn");
    ui->btnBack->raise();

    /****** Bottom Cover Label ******/
//    m_labBottom = new QLabel(this);
//    iX = (int)(-1*BOTTOM_COVER_X*g_dSizeRatio+0.5);
//    iY = g_iWinH - (int)(BOTTOM_COVER_Y*g_dSizeRatio+0.5);
//    iW = g_iWinW + 2*(int)(BOTTOM_COVER_X*g_dSizeRatio+0.5);
//    iH = (int)(BOTTOM_COVER_H*g_dSizeRatio+0.5);
//    m_labBottom->setGeometry(iX, iY, iW, iH);
//    g_classSetBGP.SetLabBGP(m_labBottom, ":android/BottomLabel.png", iW, iH);
}

void Help_Widget::PlayMusic(bool bType, QString strName)
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

void Help_Widget::on_btnLastPage_clicked()
{
    if(m_iCurPage == 1)
    {
        PlayMusic(SOUND_TYPE, "Paper.mp3");
        m_iCurPage = 0;
        g_classSetBGP.SetLabBGP(ui->labHelp, ":android/HelpBGP_Rule.jpg", g_iWinW, g_iWinH);
    }
//    if(m_iCurPage > 0)
//    {
//        m_iCurPage --;
//        QImage imgHelp(":android/HelpDlgPage" + QString::number(m_iCurPage) + ".png");
//        int iW = (int)(HELP_LAB_W*g_dSizeRatio+0.5);
//        int iH = (int)(HELP_LAB_H*g_dSizeRatio+0.5);
//        ui->labHelp->setPixmap(QPixmap::fromImage(imgHelp).scaled(iW, iH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//        ui->labPage->setText(QString::number(m_iCurPage+1) + "/4");
//    }
}

void Help_Widget::on_btnNextPage_clicked()
{

    if(m_iCurPage == 0)
    {
        PlayMusic(SOUND_TYPE, "Paper.mp3");
        m_iCurPage = 1;
        g_classSetBGP.SetLabBGP(ui->labHelp, ":android/HelpBGP_Contact.jpg", g_iWinW, g_iWinH);
    }
//    if(m_iCurPage < 3)
//    {
//        m_iCurPage ++;
//        QImage imgHelp(":android/HelpDlgPage" + QString::number(m_iCurPage) + ".png");
//        int iW = (int)(HELP_LAB_W*g_dSizeRatio+0.5);
//        int iH = (int)(HELP_LAB_H*g_dSizeRatio+0.5);
//        ui->labHelp->setPixmap(QPixmap::fromImage(imgHelp).scaled(iW, iH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//        ui->labPage->setText(QString::number(m_iCurPage+1) + "/4");
//    }
}

void Help_Widget::on_btnBack_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    ui->btnBack->setVisible(0);
    emit sigHelpClose(HELP_UI);
}
