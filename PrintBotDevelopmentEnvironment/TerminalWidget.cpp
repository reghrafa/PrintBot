#include "TerminalWidget.h"

TerminalWidget::TerminalWidget(QWidget *parent) :
    QMainWindow(parent)
{
    qDebug() << "neues TerminalWidget";

    this->setAttribute(Qt::WA_DeleteOnClose);
    textEditedByUser = 1;
    usbConnection = 0;
    format = "ascii";
    mFilename = "";

    this->setupWidgets();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readUSART()));

    this->USBinit();
    if (usbConnection)
    {
        this->pushButtonConnect->setText("Trennen");
        this->actionConnect->setDisabled(1);
        this->actionDisconnect->setEnabled(1);
    }
    this->show();
}

TerminalWidget::~TerminalWidget()
{
    qDebug() << "~TerminalWidget";
    if (usbConnection)
    {
        usb_close(handle);  ///USB-Gerät schließen
    }
    ((MainWindow *) this->parent())->resetTerminalWidgetPointer();
    qDebug() << "~TerminalWidget successfull";
}

void TerminalWidget::setupWidgets()
{
//    qDebug() << "TerminalWidget::setupWidgets";
    this->resize(400, 300);
    this->setWindowTitle(tr("PrintBot Development Environment - Terminal"));
    actionSave = new QAction(this);
    actionExit = new QAction(this);
    actionSave_as = new QAction(this);
    actionSend_String = new QAction(this);
    actionSend_Textfile = new QAction(this);
    actionConnect = new QAction(this);
    actionDisconnect = new QAction(this);
//    centralWidget = new QWidget(this);

    centralWidget = new QWidget(this);
    textEdit = new QTextEdit(centralWidget);
    textEdit->setUndoRedoEnabled(false);
    horizontalLayout = new QHBoxLayout(centralWidget);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);

    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    comboBoxFormat = new QComboBox(centralWidget);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    pushButtonClear = new QPushButton(centralWidget);
    pushButtonConnect = new QPushButton(centralWidget);

    verticalLayout->addWidget(pushButtonClear);
    verticalLayout->addWidget(pushButtonConnect);
    verticalLayout->addItem(verticalSpacer);
    verticalLayout->addWidget(comboBoxFormat);
    horizontalLayout->addWidget(textEdit);
    horizontalLayout->addLayout(verticalLayout);

    this->textEdit->installEventFilter(this);
    this->actionConnect->setEnabled(1);
    this->actionDisconnect->setDisabled(1);

    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 400, 21));
    menuFile = new QMenu(menuBar);
    menuConnection = new QMenu(menuBar);
    statusBar = new QStatusBar(this);

    this->setStatusBar(statusBar);
    this->setMenuBar(menuBar);
    this->setCentralWidget(centralWidget);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuConnection->menuAction());
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSave_as);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menuConnection->addAction(actionConnect);
    menuConnection->addAction(actionDisconnect);

    actionSave->setText("Speichern");
    actionSave_as->setText("Speichern unter...");
    actionExit->setText("Beenden");

//    actionSend_String->setText("Send String");
//    actionSend_Textfile->setText("Send Textfile");
    actionConnect->setText("Verbinden");
    actionDisconnect->setText("Trennen");
    comboBoxFormat->clear();
    comboBoxFormat->insertItems(0, QStringList() << "ascii" << "hex" << "dec" << "bin");
    pushButtonClear->setText("Leeren");
    pushButtonConnect->setText("Verbinden");
    menuFile->setTitle("Datei");
    menuConnection->setTitle("Verbindung");

    connect(this->actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave_triggered()));
    connect(this->actionSave_as, SIGNAL(triggered()), this, SLOT(on_actionSave_as_triggered()));
    connect(this->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(this->actionConnect, SIGNAL(triggered()), this, SLOT(on_actionConnect_triggered()));
    connect(this->actionDisconnect, SIGNAL(triggered()), this, SLOT(on_actionDisconnect_triggered()));

    connect(this->pushButtonClear, SIGNAL(clicked()), this, SLOT(on_pushButtonClear_clicked()));
    connect(this->pushButtonConnect, SIGNAL(clicked()), this, SLOT(on_pushButtonConnect_clicked()));

    connect(this->comboBoxFormat, SIGNAL(currentTextChanged(QString)), this, SLOT(on_comboBoxFormat_currentTextChanged(QString)));

    connect(this->textEdit, SIGNAL(textChanged()), this, SLOT(on_textEdit_textChanged()));
    connect(this->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(on_textEdit_cursorPositionChanged()));
}

void TerminalWidget::textOutput(QString text)
{
    this->textEditedByUser = 0;
    this->textEdit->setTextColor(Qt::red);
    this->textEdit->insertPlainText(text);
    this->textEdit->setTextColor(Qt::black);
    this->textEditedByUser = 1;
}

void TerminalWidget::checkCount(int count)
{
    if (count < 0)
    {
        this->textOutput("USB error\n");
        qDebug() << "USB error\n";
        this->usbConnection = 0;
        this->timer->stop();
        return;
    }
}

void TerminalWidget::USBinit()
{
    qDebug() << "TerminalWidget::USBinit()";
    //usb_dev_handle      *handle = NULL;
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
    int                 count, vid, pid;
    usb_init();

    ///compute VID/PID from usbconfig.h so that there is a central source of information
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    ///The following function is in opendevice.c:
    if (usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
    {
        textOutput("USB-Gerät kann nicht gefunden werden!\n");

        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        getch(); /// wie getchar(), jedoch kann maximal ein Zeichen eingeben kann.
        //exit(1);
        return;
    }
    ///Since we use only control endpoint 0, we don't need to choose a
    ///configuration and interface. Reading device descriptor and setting a
    ///configuration and interface is done through endpoint 0 after all.
    ///However, newer versions of Linux require that we claim an interface
    ///even for endpoint 0. Enable the following code if your operating system
    ///needs it:
#if 0 ///Wird nicht ausgeführt solange #if 0
    int retries = 1, usbConfiguration = 1, usbInterface = 0;
    if (usb_set_configuration(handle, usbConfiguration) && showWarnings)
    {
        textOutput("Warnung: Konfiguration konnte nicht verändert werden!\n");
        fprintf(stderr, "Warning: could not set configuration: %s\n", usb_strerror());
    }

    ///now try to claim the interface and detach the kernel HID driver on
    ///Linux and other operating systems which support the call.
    while ((len = usb_claim_interface(handle, usbInterface)) != 0 && retries-- > 0)
    {
#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
        if (usb_detach_kernel_driver_np(handle, 0) < 0 && showWarnings)
        {
            textOutput("Warnung: could not detach kernel driver!\n");
            fprintf(stderr, "Warning: could not detach kernel driver: %s\n", usb_strerror());
        }
#endif
    }
#endif

#define F_CPU	12000000
#define BAUD	9600
#define UBRR	( (F_CPU/(16*BAUD)) - 1 )

    uint8_t UCSRA = 0;
    uint8_t UCSRB = (1<<4)|(1<<3);          // RXEN, TXEN
    uint8_t UCSRC = (1<<7)|(1<<2)|(1<<1);	// URSEL, asynch, w/o parity, 1 stop-bit, 8-bit char
    uint8_t UBRRL = UBRR & 0x00ff;
    uint8_t UBRRH = ( (UBRR & 0xff00) >> 8 );

    count = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, USBASP_FUNC_SET_UART1, UCSRA|(UCSRB<<8), UCSRC|(UBRRL<<8), 0, 0, 5000);
        checkCount(count);
    count = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, USBASP_FUNC_SET_UART2, UBRRH, 0, 0, 0, 5000);
        checkCount(count);

    this->usbConnection = 1;
    this->timer->start(1);
    this->textOutput("USB verbunden!\n");
}

void TerminalWidget::readUSART()
{
//    static int counter = 0;
//    qDebug() << "Timer: " << counter++;

    if (usbConnection == 1)
    {
        int count;
        char buffer[4];         ///Buffer für Dinge, die vom Gerät kommen

        count = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, USBASP_FUNC_TXRX_UART, 0, 0, buffer, sizeof(buffer), 5000);
        checkCount(count);
        if ( count > 0 ) /// UART byte received
        {
            printCharacter(buffer[0]);
            if ( buffer[1] & (1<<4) )
            {
                printf("E");
                textOutput("ERROR: E\n");
            }
            if ( buffer[1] & (1<<3) )
            {
                printf("O");
                textOutput("ERROR: O\n");
            }
            //this->printTextBrowser();
        }
    }
}

void TerminalWidget::printCharacter(unsigned char c)
{
    this->textEdit->setTextColor(Qt::blue);
    this->textEditedByUser = 0;
    if (QString::compare(format,"ascii") == 0)
    {
        QString newQString = (QChar) c;
        this->textEdit->insertPlainText(newQString);
    }
    else if (QString::compare(format,"bin") == 0)
    {
        this->textEdit->insertPlainText(QString::number(c, 2));
        this->textEdit->insertPlainText(" ");
    }
    else if (QString::compare(format,"dec") == 0)
    {
        this->textEdit->insertPlainText(QString::number(c, 10));
        this->textEdit->insertPlainText(" ");
    }
    else if (QString::compare(format,"hex") == 0)
    {
        this->textEdit->insertPlainText(QString::number(c, 16));
        this->textEdit->insertPlainText(" ");
    }
    else
    {
        textOutput("ERROR: Unbekanntes Textformat!\n");
    }
    this->textEditedByUser = 1;
    this->textEdit->setTextColor(Qt::black);
}

void TerminalWidget::printTextEdit()
{
    this->textEdit->setTextColor(Qt::blue);
    this->textEditedByUser = 0;
    this->textEdit->insertPlainText(qString); //appendPlainText
    this->textEditedByUser = 1;
    this->qString = "";
    this->textEdit->setTextColor(Qt::black);
}

void TerminalWidget::on_textEdit_textChanged()
{
    if (this->textEditedByUser)
    {
        if (this->usbConnection == 1)
        {
            char buffer[4];
            QChar qc = *(this->textEdit->toPlainText().end()-1);
            char c = qc.toLatin1();

            usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
                            USBASP_FUNC_TXRX_UART, 1| (c<<8), 0, buffer, sizeof(buffer), 5000);
        }
        else
        {
            textOutput("\nKeine USB-Verbindung!\n");
        }
    }
}

void TerminalWidget::on_textEdit_cursorPositionChanged()
{
    this->textEdit->moveCursor(QTextCursor::End);
}

void TerminalWidget::on_pushButtonClear_clicked()
{
    this->textEditedByUser = 0;
    this->textEdit->clear();
    this->textEditedByUser = 1;
}

void TerminalWidget::on_pushButtonConnect_clicked()
{
    if (this->pushButtonConnect->text() == "Trennen")
    {
        usb_close(handle); //USB-Gerät schließen
        this->usbConnection = 0;              \
        this->timer->stop();
        this->pushButtonConnect->setText("Verbinden");
        this->actionConnect->setEnabled(1);
        this->actionDisconnect->setDisabled(1);
    }
    else
    {
        textOutput("\n");  ///new line because of the following function call
        this->USBinit();
        if (this->usbConnection)
        {
            this->pushButtonConnect->setText("Trennen");
            this->actionConnect->setDisabled(1);
            this->actionDisconnect->setEnabled(1);
        }
    }
}

void TerminalWidget::on_actionSave_triggered()
{
    if (QString::compare(mFilename,"") == 0)
    {
        on_actionSave_as_triggered();
    }
    else
    {
        QFile sFile(mFilename);
        if (sFile.open(QFile::WriteOnly | QFile::Text))
        {
            QTextStream out(&sFile);
            out << this->textEdit->toPlainText();     ///alternative: toHTML
            sFile.flush();
            sFile.close();
        }
    }

}

void TerminalWidget::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "",
            tr("Data Text (*.txt);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        QFile sFile(fileName);
        if (sFile.open(QFile::WriteOnly | QFile::Text))
        {
            mFilename = fileName;
            on_actionSave_triggered();
        }
        QString newWindowTitle;
        QTextStream(&newWindowTitle) << fileName << " - PrintBot USB-Terminal";
        this->setWindowTitle(newWindowTitle);
    }
}

void TerminalWidget::on_comboBoxFormat_currentTextChanged(const QString &newformat)
{
    this->format = newformat;
}

void TerminalWidget::on_actionConnect_triggered()
{
    textOutput("\n");
    this->USBinit();
    if (this->usbConnection)
    {
        this->pushButtonConnect->setText("Disconnect");
        this->actionConnect->setDisabled(1);
        this->actionDisconnect->setEnabled(1);
    }
}

void TerminalWidget::on_actionDisconnect_triggered()
{
    usb_close(handle); ///USB-Gerät schließen
    usbConnection = 0;              \
    timer->stop();
    this->pushButtonConnect->setText("Connect");
    this->actionConnect->setEnabled(1);
    this->actionDisconnect->setDisabled(1);
}



bool TerminalWidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == this->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return)
        {
            ///Special tab handling
            //qDebug("Enter Key Pressed...");
            return true;
        }
        else
        {
            return QMainWindow::eventFilter(object, event);
        }
    }
    else
    {
        return QMainWindow::eventFilter(object, event);
    }
}
