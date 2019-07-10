#include "remind_dlg.h"
#include "ui_remind_dlg.h"

extern int g_iWinW;    // App Window Width after Adjust
extern int g_iWinH;    // App Window Height after Adjust
extern double g_dSizeRatio;    // The Width-Height Ratio required to be adjusted for all the Widgets in App
extern QMediaPlayer *g_playerBGM;    // Game Background Music, Loop
extern QMediaPlayer *g_playerSound;    // All kinds of Sound, Only Play for One Time, such as Button Click Sound
extern QString g_strSourcePath;    // Source Files' Path in Device
extern SetWidgetBGP_Class g_classSetBGP;    // Set Widget BGP

Remind_Dlg::Remind_Dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Remind_Dlg)
{
    ui->setupUi(this);
    InitVariable();
    InitWidget();
}

Remind_Dlg::~Remind_Dlg()
{
    delete ui;
}

void Remind_Dlg::InitVariable()
{

}

void Remind_Dlg::InitWidget()
{
    setGeometry((int)(REMIND_X*g_dSizeRatio+0.5), (int)(REMIND_Y*g_dSizeRatio+0.5), (int)(REMIND_W*g_dSizeRatio+0.5), (int)(REMIND_H*g_dSizeRatio+0.5));
    setFixedSize((int)(REMIND_W*g_dSizeRatio+0.5), (int)(REMIND_H*g_dSizeRatio+0.5));
    setAutoFillBackground(true);
    int iBtnW = (int)(REMING_BTN_W*g_dSizeRatio+0.5);
    int iBtnH = (int)(REMING_BTN_H*g_dSizeRatio+0.5);
//    QMessageBox::information(this, "Info.", "g_dSizeRatio = " + QString::number(g_dSizeRatio));
//    QMessageBox::information(this, "Info.", "iBtnW = " + QString::number(iBtnW) + ", iBtnH = " + QString::number(iBtnH));
    ui->btnConfirm->setGeometry((int)(REMING_BTN_X1*g_dSizeRatio+0.5), (int)(REMING_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
    ui->btnBack->setGeometry((int)(REMING_BTN_X2*g_dSizeRatio+0.5), (int)(REMING_BTN_Y*g_dSizeRatio+0.5), iBtnW, iBtnH);
}

void Remind_Dlg::SetWidget(int iDlgType)
{
    switch(iDlgType)
    {
        case GAME_FAILED_DLG :
            PlayMusic(SOUND_TYPE, "GameFailed.mp3");
            SetBGP("GameFailed");
            g_classSetBGP.SetBtnBGP(ui->btnConfirm, "RestartBtn");
            ui->btnConfirm->setVisible(1);
            g_classSetBGP.SetBtnBGP(ui->btnBack, "RejectBtn");
            ui->btnBack->setVisible(1);
            break;
        case GAME_SUCCEED_DLG :
            PlayMusic(SOUND_TYPE, "GameSucceed.mp3");
            SetBGP("GameSucceed");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ConfirmBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
        case SURVIVE_UNLOCK_DLG :
            PlayMusic(SOUND_TYPE, "Unlock.mp3");
            SetBGP("SurviveUnlock");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ConfirmBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
        case KING_UNLOCK_DLG :
            PlayMusic(SOUND_TYPE, "Unlock.mp3");
            SetBGP("KingUnlock");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ConfirmBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
        case SURVIVE_LOCKED_DLG :
            PlayMusic(SOUND_TYPE, "Locked.mp3");
            SetBGP("SurviveRemind");
            ui->btnConfirm->setVisible(0);
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ConfirmBtn");
            ui->btnBack->setVisible(1);
            break;
        case KING_LOCKED_DLG :
            PlayMusic(SOUND_TYPE, "Locked.mp3");
            SetBGP("KingRemind");
            ui->btnConfirm->setVisible(0);
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ConfirmBtn");
            ui->btnBack->setVisible(1);
            break;
        case GAME_OVER_DLG :
            SetBGP("GameOver");
            g_classSetBGP.SetBtnBGP(ui->btnConfirm, "RestartBtn");
            ui->btnConfirm->setVisible(1);
            g_classSetBGP.SetBtnBGP(ui->btnBack, "RejectBtn");
            ui->btnBack->setVisible(1);
            break;
        case NEW_RECORD_DLG :
            PlayMusic(SOUND_TYPE, "Unlock.mp3");
            SetBGP("NewRecord");
            ui->btnConfirm->setVisible(0);
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ConfirmBtn");
            ui->btnBack->setVisible(1);
            break;
        case CONTINUE_DLG :
            PlayMusic(SOUND_TYPE, "GameSucceed.mp3");
            SetBGP("GameSucceed");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "NextLevBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
        case GAME_PAUSE_DLG :
            SetBGP("GamePause");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ReturnBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
        case GIVE_UP_DLG :
            SetBGP("GiveUpGame");
            g_classSetBGP.SetBtnBGP(ui->btnConfirm, "ConfirmBtn");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "ReturnBtn");
            ui->btnConfirm->setVisible(1);
            ui->btnBack->setVisible(1);
            break;
        case SCORE_REMIND_DLG :
            SetBGP("ScoreModelRemind");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "BeginBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
        case SURVIVE_REMIND_DLG :
            SetBGP("SurviveModelRemind");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "BeginBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
        case KING_REMIND_DLG :
            SetBGP("KingModelRemind");
            g_classSetBGP.SetBtnBGP(ui->btnBack, "BeginBtn");
            ui->btnConfirm->setVisible(0);
            ui->btnBack->setVisible(1);
            break;
    }
}

void Remind_Dlg::SetBGP(QString strBGP)
{
    QImage imgBGP(":android/" + strBGP + ".jpg");
    g_classSetBGP.SetWidgetBGP(this, imgBGP, (int)(REMIND_W*g_dSizeRatio+0.5), (int)(REMIND_H*g_dSizeRatio+0.5));
}

void Remind_Dlg::PlayMusic(bool bType, QString strName)
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

void Remind_Dlg::on_btnConfirm_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    accept();
}

void Remind_Dlg::on_btnBack_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    reject();
}
