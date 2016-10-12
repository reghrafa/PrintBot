#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QObject>
#include <QDialog>
#include <QTextStream>
#include <QLabel>
//#include <QGroupBox>
#include <QGridLayout>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QSizePolicy>
#include "Main.h"

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
    void setupWidgets();

    QLabel qLabel;
    QPushButton pushButtonOK;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
//    QGroupBox qGroupBox;
    QGridLayout qGridLayout;
    QVBoxLayout verticalLayout;
    QHBoxLayout horizontalLayout;
};

#endif // ABOUTDIALOG_H
