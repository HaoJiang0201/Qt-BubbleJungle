#include "main_window.h"
#include <QApplication>
#include <QScreen>
#include <QMessageBox>

int g_iScreenW;    // Device Screen Width
int g_iScreenH;    // Device Screen Height

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Get Device Screen Size
    g_iScreenW = a.primaryScreen()->size().width();
    g_iScreenH = a.primaryScreen()->size().height();
    //Debug in Windows
//    g_iScreenW = 540;
//    g_iScreenH = 960;
    // Create Main Window
    Main_Window w;
    w.show();
    return a.exec();
}
