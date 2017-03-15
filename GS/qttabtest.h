#ifndef QTTABTEST_H
#define QTTABTEST_H

#include <QMainWindow>
#include "options.h"
#include <QPropertyAnimation>
#include <QShortcut>
#include <QPixmap>
#include <QDebug>
#include <iostream>
#include <QUrl>

#include "options.h"
#include "tutorial.h"
#include "mapplanning.h"
#include "mapexecution.h"
#include "missionrecap.h"
#include "maprecap.h"
#include "connectiondialog.h"

#include "ui_qttabtest.h"

namespace Ui {

    class QtTabTest;

}

class QtTabTest : public QMainWindow {

    Q_OBJECT

public:

    explicit QtTabTest(QWidget *parent = 0);

    ~QtTabTest();

    void addNewTab( QWidget * , QString );

    void deleteTabIndex( int );

private:

    Ui::QtTabTest *ui;

    bool isFirstTabEmpty;

};

#endif // QTTABTEST_H
