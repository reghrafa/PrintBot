#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTextEdit>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QString>
#include <QComboBox>
#include <QSpacerItem>
#include <QBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>
#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Main.h"

#include "lusb0_usb.h"		///this is libusb
#include "OpenDevice.h"		///common code moved to separate module
#include "MainWindow.h"

#define USB_CFG_VENDOR_ID   0xc0, 0x16  /* 5824 in dec, stands for VOTI */
#define USB_CFG_DEVICE_ID   0xdc, 0x05  /* 1500 in dec, obdev's free PID */
#define	USB_CFG_VENDOR_NAME     'w', 'w', 'w', '.', 'f', 'i', 's', 'c', 'h', 'l', '.', 'd', 'e'
#define	USB_CFG_DEVICE_NAME		'U', 'S', 'B', 'a', 's', 'p'
#define USBASP_FUNC_SET_UART1        17
#define USBASP_FUNC_SET_UART2        18
#define USBASP_FUNC_TXRX_UART        19

class Console;
class MainWindow;

class TerminalWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit TerminalWidget(QWidget *parent = 0);
    ~TerminalWidget();
    void USBinit();
    void printCharacter(unsigned char c);
    void printTextEdit();

protected:
    bool eventFilter(QObject *target, QEvent *event);

signals:
    void keyPressed(char c);

private slots:
    void on_textEdit_textChanged();
    void on_textEdit_cursorPositionChanged();
    void readUSART();
    void on_pushButtonClear_clicked();
    void on_pushButtonConnect_clicked();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_comboBoxFormat_currentTextChanged(const QString &newformat);
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();

private:
    void setupWidgets();
    void textOutput(QString text);
    void checkCount(int count);

    Console *console;
    QString qString;
    QString mFilename;
    QString format;
//    QTextEdit *m_pTxBox;
    bool textEditedByUser;
    bool usbConnection;
    usb_dev_handle *handle;
    QTimer *timer;

    QAction *actionSave;
    QAction *actionExit;
    QAction *actionSave_as;
    QAction *actionSend_String;
    QAction *actionSend_Textfile;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEdit;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBoxFormat;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonConnect;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuConnection;
    QStatusBar *statusBar;
};

#endif // TERMINALWIDGET_H
