#include <QApplication>
#include <QSplashScreen>
#include <QtSvg>

#include "mainmdidisplay.h"
#include "maindockwindow.h"

#include "mission.h"
#include "decoder.h"
#include "interop.h"
#include "qttabtest.h"
#include "interop.h"
#include <QDebug>

int main(int argc, char *argv[]) {

    static const int splash_width = 600;
    static const int splash_height = 450;
    static const QString filename(":/res/images/UAV Forge Logo 2015.svg");

    QApplication a(argc, argv);
    //Construct and load the SVG image
    QSvgRenderer svg(filename);
    //Calculate the scaled size within a bounded size
    QSize SplashSize = svg.viewBox().size();
    SplashSize.scale(splash_width, splash_height, Qt::KeepAspectRatio);
    //Construct the pixmap to contain the SVG image
    QPixmap pixmap(SplashSize);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    //Render the SVG to the pixmap
    svg.render(&painter, pixmap.rect());
    //Construct and show the splash screen
    QSplashScreen splash(pixmap);
    splash.show();

   // MainMDIDisplay x;
    MainDockWindow dockWindow;
    //x.showNormal();
    dockWindow.showNormal();
   // Interop interop_server("testuser", "testpass");
   // qDebug() << interop_server.getMission(1);

    //Hide the splash screen.
<<<<<<< HEAD
    splash.finish(&dockWindow);
=======
    splash.finish(&x);
    try {
        Interop interop("testuser", "testpass");
        interop.getMissions();
        interop.deleteODLC(1);
    } catch (std::exception& err) {
        qDebug() << "borked";
        qCritical() << err.what();
    }
>>>>>>> origin/master
    return a.exec();
}

