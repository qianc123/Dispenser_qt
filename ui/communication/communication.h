﻿#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QWidget>
#include <QtWidgets>

#include "../../sdk/QNavigationBar/qnavigationbar.h"
#include "../../sdk/QCommunication/qcommunication.h"
class QVNavigationBar;
class QMySocket;
class QMySerial;

class Communication : public QWidget
{
    Q_OBJECT
public:
    explicit Communication(QWidget *parent = nullptr);

private:
    QStackedWidget  *stackedWidget;
    QVNavigationBar *vnavigationbar;
};

#endif // COMMUNICATION_H
