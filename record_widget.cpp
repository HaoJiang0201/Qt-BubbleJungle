#include "record_widget.h"
#include "ui_record_widget.h"

extern int g_iWinW;    // App Window Width after Adjust
extern int g_iWinH;    // App Window Height after Adjust
extern double g_dSizeRatio;    // The Width-Height Ratio required to be adjusted for all the Widgets in App
extern RECORD g_recordGame;    // Game Record Structure
extern QMediaPlayer *g_playerBGM;    // Game Background Music, Loop
extern QMediaPlayer *g_playerSound;    // All kinds of Sound, Only Play for One Time, such as Button Click Sound
extern QString g_strSourcePath;    // Source Files' Path in Device
extern SetWidgetBGP_Class g_classSetBGP;    // Set Widget BGP

Record_Widget::Record_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Record_Widget)
{
    ui->setupUi(this);
    InitVariable();
    InitWidget();
}

Record_Widget::~Record_Widget()
{
    delete ui;
}

void Record_Widget::InitVariable()
{
    m_iCurRecord = RECORD_SCORE;
}

void Record_Widget::InitWidget()
{
    /****** Main BGP ******/
    setAutoFillBackground(true);
    setGeometry(0, 0, g_iWinW, g_iWinH);
    QPalette paletteBG;
    QImage imgBGP(":android/WhiteCover.png");
    paletteBG.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(imgBGP).scaled(g_iWinW, g_iWinH)));
    setPalette(paletteBG);

    /****** Title Label and Page Button ******/
    ui->labTitle->setAutoFillBackground(1);
    ui->labFirstRecord->setAutoFillBackground(1);
    ui->labSecondRecord->setAutoFillBackground(1);
    ui->labThirdRecord->setAutoFillBackground(1);
    ui->labFourthRecord->setAutoFillBackground(1);
    ui->labTitle->setGeometry((int)(RECORD_TITLE_X*g_dSizeRatio+0.5), (int)(RECORD_TITLE_Y*g_dSizeRatio+0.5),  (int)(RECORD_BOARD_W*g_dSizeRatio+0.5), (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    ui->btnLeft->setGeometry(0, (int)((RECORD_TITLE_Y+50)*g_dSizeRatio+0.5), (int)(RECORD_BTN_SIZE*g_dSizeRatio+0.5), (int)(RECORD_BTN_SIZE*g_dSizeRatio+0.5));
    ui->btnRight->setGeometry(g_iWinW-(int)(RECORD_BTN_SIZE*g_dSizeRatio+0.5), (int)((RECORD_TITLE_Y+50)*g_dSizeRatio+0.5), (int)(RECORD_BTN_SIZE*g_dSizeRatio+0.5), (int)(RECORD_BTN_SIZE*g_dSizeRatio+0.5));
    g_classSetBGP.SetBtnBGP(ui->btnLeft, "LeftBtn");
    g_classSetBGP.SetBtnBGP(ui->btnRight, "RightBtn");
    ui->btnLeft->setVisible(0);

    /****** Record Board Label ******/
    ui->labFirstRecord->setGeometry((int)(RECORD_X*g_dSizeRatio+0.5), (int)(RECORD_FIRST_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    ui->labSecondRecord->setGeometry((int)(RECORD_X*g_dSizeRatio+0.5), (int)(RECORD_SECOND_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    ui->labThirdRecord->setGeometry((int)(RECORD_X*g_dSizeRatio+0.5), (int)(RECORD_THIRD_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    ui->labFourthRecord->setGeometry((int)(RECORD_X*g_dSizeRatio+0.5), (int)(RECORD_FOURTH_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    QFont fontText;
    fontText.setPointSize(30);
    fontText.setFamily("Timer");
    fontText.setUnderline(1);
    fontText.setBold(1);
    fontText.setItalic(1);
    for(int i=0; i<4; i++)
    {
        m_labRecordText[i] = new QLabel(this);
        m_labRecordText[i]->setFont(fontText);
        m_labRecordText[i]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        QString strTime = TimeConvert(g_recordGame.iTimeRecord[i]);
        m_labRecordText[i]->setText(strTime);
        m_labRecordText[i]->setVisible(0);
    }
    paletteBG.setColor(QPalette::Text, QColor(230, 30, 30));
    m_labRecordText[0]->setPalette(paletteBG);

    m_labRecordText[0]->setGeometry((int)(RECORD_TEXT_X*g_dSizeRatio+0.5), (int)(RECORD_FIRST_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    m_labRecordText[1]->setGeometry((int)(RECORD_TEXT_X*g_dSizeRatio+0.5), (int)(RECORD_SECOND_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    m_labRecordText[2]->setGeometry((int)(RECORD_TEXT_X*g_dSizeRatio+0.5), (int)(RECORD_THIRD_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));
    m_labRecordText[3]->setGeometry((int)(RECORD_TEXT_X*g_dSizeRatio+0.5), (int)(RECORD_FOURTH_Y*g_dSizeRatio+0.5), g_iWinW, (int)(RECORD_BOARD_H*g_dSizeRatio+0.5));

    // Set Current Page
    SetRecordLabel();

    /****** Back Button ******/
    int iBtnSize = (int)(BACK_SIZE*g_dSizeRatio+0.5);
    ui->btnBack->setGeometry(g_iWinW-iBtnSize, g_iWinH-iBtnSize-(int)(50*g_dSizeRatio+0.5), iBtnSize, iBtnSize);
    g_classSetBGP.SetBtnBGP(ui->btnBack, "BackBtn");
    ui->btnBack->raise();

    /****** Bottom Cover Label ******/
//    m_labBottom = new QLabel(this);
//    int iX = (int)(-1*BOTTOM_COVER_X*g_dSizeRatio+0.5);
//    int iY = g_iWinH - (int)(BOTTOM_COVER_Y*g_dSizeRatio+0.5);
//    int iW = g_iWinW + 2*(int)(BOTTOM_COVER_X*g_dSizeRatio+0.5);
//    int iH = (int)(BOTTOM_COVER_H*g_dSizeRatio+0.5);
//    m_labBottom->setGeometry(iX, iY, iW, iH);
//    g_classSetBGP.SetLabBGP(m_labBottom, ":android/BottomLabel.png", iW, iH);

}

void Record_Widget::SetRecordLabel()
{
    QString strTitle, strFirstRecord, strSecondRecord, strThirdRecord, strFourthRecord;
    switch(m_iCurRecord)
    {
        case RECORD_SCORE :
            strTitle = "Score";
            strFirstRecord = "ScoreXSLV" + QString::number(g_recordGame.iScoreLev[0]);
            strSecondRecord = "ScoreZJLV" + QString::number(g_recordGame.iScoreLev[1]);
            strThirdRecord = "ScoreYHLV" + QString::number(g_recordGame.iScoreLev[2]);
            strFourthRecord = "ScoreDCLV" + QString::number(g_recordGame.iScoreLev[3]);
            for(int i=0; i<4; i++)
                m_labRecordText[i]->setVisible(0);
            break;
        case RECORD_SURVIVE :
            strTitle = "Survive";
            strFirstRecord = "Survive0";
            strSecondRecord = "Survive1";
            strThirdRecord = "Survive2";
            strFourthRecord = "Survive3";
            for(int i=0; i<4; i++)
            {
                QString strTime = TimeConvert(g_recordGame.iTimeRecord[i]);
                m_labRecordText[i]->setText(strTime);
                m_labRecordText[i]->setVisible(1);
            }
            break;
        case RECORD_KING :
            strTitle = "King";
            strFirstRecord = "KingXSLV" + QString::number(g_recordGame.iKingLev[0]);
            strSecondRecord = "KingZJLV" + QString::number(g_recordGame.iKingLev[1]);
            strThirdRecord = "KingYHLV" + QString::number(g_recordGame.iKingLev[2]);
            strFourthRecord = "KingDCLV" + QString::number(g_recordGame.iKingLev[3]);
            for(int i=0; i<4; i++)
                m_labRecordText[i]->setVisible(0);
            break;
        default : break;
    }
    ui->labTitle->setStyleSheet("QLabel {image: url(:android/Record" + strTitle + "Title.png);}");
    ui->labFirstRecord->setStyleSheet("QLabel {image: url(:android/Record" + strFirstRecord + ".png);}");
    ui->labSecondRecord->setStyleSheet("QLabel {image: url(:android/Record" + strSecondRecord + ".png);}");
    ui->labThirdRecord->setStyleSheet("QLabel {image: url(:android/Record" + strThirdRecord + ".png);}");
    ui->labFourthRecord->setStyleSheet("QLabel {image: url(:android/Record" + strFourthRecord + ".png);}");

    ui->labFirstRecord->setVisible(1);
    ui->labSecondRecord->setVisible(1);
    ui->labThirdRecord->setVisible(1);
    ui->labFourthRecord->setVisible(1);
    ui->btnBack->setVisible(1);
}

/*void Record_Widget::SetBtnBG(QPushButton *btnObject, QString strPicName)
{
    btnObject->setAutoFillBackground(1);
    QString strStyle = "QPushButton {border-image: url(" + strPicName + ".png);}";    // Normal Status
    strStyle += "QPushButton:hover{border-image: url(" + strPicName + ".png);}";    // Hover Status
    strStyle += "QPushButton:hover:pressed{border-image: url(" + strPicName + "Press.png);}";    // Press Status
    btnObject->setStyleSheet(strStyle);
}*/

void Record_Widget::PlayMusic(bool bType, QString strName)
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

QString Record_Widget::TimeConvert(int iTime)
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
    strTime = strHour + " : " + strMin + " : " + strSec;
    return strTime;
}

void Record_Widget::on_btnBack_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    ui->btnBack->setVisible(0);
    emit sigRecordClose(RECORD_UI);
    //close();
}

void Record_Widget::on_btnLeft_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    if(m_iCurRecord > 0)
    {
        m_iCurRecord --;
        if(m_iCurRecord == 0)
            ui->btnLeft->setVisible(0);
        ui->btnRight->setVisible(1);
        SetRecordLabel();
    }
}

void Record_Widget::on_btnRight_clicked()
{
    PlayMusic(SOUND_TYPE, "BtnClick.mp3");
    if(m_iCurRecord < 2)
    {
        m_iCurRecord ++;
        if(m_iCurRecord == 2)
            ui->btnRight->setVisible(0);
        ui->btnLeft->setVisible(1);
        SetRecordLabel();
    }
}
