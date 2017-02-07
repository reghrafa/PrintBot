#include "mainwindow.h"
#include "OpenFileParser.h"
#include "TerminalWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    terminalWidget(NULL),
//    alreadyMaked(0),
    openFile(""),
    QMainWindow(parent)
{
    qDebug() << "neues MainWindow!";

//    sourcecodeWidget->setHtml("<b>Hallo</b> <span style=\"color:rgb(200,0,200)\">Test</span>du da");

    setWindowTitle(tr("PrintBot Development Environment"));
    setWindowIcon(QIcon("./data/PB_Logo_114x114.png"));

    setupWidgets();
    setupMenus();
    createTestProgram();

    qProcess = new QProcess(this);
    qProcess->setProcessChannelMode(QProcess::MergedChannels);
    qProcess->setReadChannel(QProcess::StandardOutput);
    connect(qProcess,SIGNAL(readyReadStandardOutput()), this, SLOT(readQProcessOutput()));
}

MainWindow::~MainWindow()
{

}

//void MainWindow::setQLabelMousePosition(QPoint mousePosition)
//{
//    //QPoint mousePosition = QCursor::pos();    //Main
//    QString labelText;
//    QTextStream textStream(&labelText);
//    textStream << "Pos: " << mousePosition.x() << ", " << mousePosition.y();
//    qLabelMousePosition.setText(labelText);
//}

void MainWindow::updateSourcecodeWidget()
{
    sourcecodeWidget->clear();
    sourcecodeWidget->setHtml(this->editorWidget->getSourcecode());
}

void MainWindow::newDokument()
{
    qDebug() << "MainWindow::newDokument";

    getProgramCode().resetAll();
    blockListWidget->resetAll();
    editorWidget->resetAll();
//    editorWidget->updateDialog(NULL);

    qDebug() << "MainWindow::newDokument, ENDE";
}

void MainWindow::createTestProgram()
{
    qDebug() << "createTestProgram()"; 

    ProgramFunction mainFunction("Start", "", "", Qt::green, TYPE_MAIN_FUNCTION);
    ProgramFunction function("Funktion", "void", "", QColor(qRgba(200,255,60,255)));
    ProgramLoopBlock loop("Schleife", "PB7 <= 5", Qt::magenta);
    ProgramIfBlock ifBlock("Bedingung", "PB1 == 1", QColor(qRgba(255,150,0,255)), TYPE_IF_ADC);
    ProgramCommandBlock statusLED("Status LED", "PORTD |= (1<<PD6);", Qt::yellow, TYPE_LED);
    ProgramCommandBlock delayBlock("Warten", "_delay_ms(1000);", Qt::red, TYPE_WAIT);
    ProgramCommandBlock portBlock("Digital-Port", "PORTD |= (1<<PD6);", QColor(qRgba(0,180,0,255)), TYPE_PORT);
    ProgramCommandBlock uartBlock("Terminal\nAusgabe", "UART();", QColor(qRgba(0,255,255,255)), TYPE_UART);
    ProgramCommandBlock vehicleBlock("Fahrzeug", "vehicle()", QColor(qRgba(70,160,255,255)), TYPE_VEHICLE);
    ProgramCommandBlock motorBlock("Motor", "motor()", QColor(qRgba(60,60,255,255)), TYPE_MOTOR);
    ProgramCommandBlock servoBlock("Servo", "servo()", QColor(qRgba(150,0,255,255)), TYPE_SERVO);

    addBlockToBlockListWidget(&mainFunction);
    addBlockToBlockListWidget(&function);
    addBlockToBlockListWidget(&loop);
    addBlockToBlockListWidget(&ifBlock);
    addBlockToBlockListWidget(&statusLED);
    addBlockToBlockListWidget(&delayBlock);
    addBlockToBlockListWidget(&portBlock);
    addBlockToBlockListWidget(&uartBlock);
    addBlockToBlockListWidget(&vehicleBlock);
    addBlockToBlockListWidget(&motorBlock);
    addBlockToBlockListWidget(&servoBlock);

//    ProgramFunction normalFunction("nomalFunc", "", "", Qt::green, TYPE_FUNCTION);
//    blockListWidget->addBlock(&normalFunction, 0);
//    blockListWidget->takeItem(0);
//    blockListWidget->insertItem(0, "testter");
}
BlockListWidget *MainWindow::getBlockListWidget()
{
    return blockListWidget;
}

QGroupBox &MainWindow::getQGridGroupBox()
{
    return qGridGroupBox;
}

QGridLayout &MainWindow::getQGridLayout()
{
    return qGridLayout;
}

void MainWindow::setupWidgets()
{
    editorWidget = new EditorWidget(this);
    blockListWidget = new BlockListWidget(editorWidget);
    sourcecodeWidget = new SourcecodeWidget(this);

//    qLabelMousePosition.setText("Maus Position");
//    qLabelMousePosition.setMinimumWidth(BLOCKLIST_WIDTH);
    QPushButton *compileButton = new QPushButton("Kompilieren");
    QPushButton *uploadCodeButton = new QPushButton("Upload");
    QPushButton *openTerminalButton = new QPushButton("Terminal öffnen");

    //compileButton->setMinimumWidth(BLOCKLIST_WIDTH);
    //compileButton->setMaximumWidth(BLOCKLIST_WIDTH);

    //uploadCodeButton->setFixedWidth();
    //uploadCodeButton->setMinimumWidth(BLOCKLIST_WIDTH);
    //uploadCodeButton->setMaximumWidth(BLOCKLIST_WIDTH);
    connect(compileButton, SIGNAL(clicked()), this, SLOT(compileSourcecode()));
    connect(uploadCodeButton, SIGNAL(clicked()), this, SLOT(uploadHexfile()));
    connect(openTerminalButton, SIGNAL(clicked()), this, SLOT(openTerminal()));

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::NoRole);
    scrollArea->setWidget(editorWidget);
    //scrollArea->setAlignment(Qt::AlignLeft);
    scrollArea->setMinimumWidth(EDITORWIDGET_WIDTH);

    //editorWidget->updateSize(); ///TODO: sinnvoll?

    ///Console for make and avrdude
    qPlainTextEdit.setParent(this);
    qPlainTextEdit.setReadOnly(true);
    qPlainTextEdit.setUndoRedoEnabled(false);
    qPlainTextEdit.setFixedHeight(DIALOG_HEIGHT);  //70
//    qPlainTextEdit.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    qPlainTextEdit.setMinimumWidth(400);

    QSplitter *qSplitter = new QSplitter(Qt::Horizontal, this);
    qSplitter->addWidget(scrollArea);
    qSplitter->addWidget(sourcecodeWidget);
    qGridGroupBox.setParent(this);
    qGridLayout.addWidget(blockListWidget,0,0);
    qGridLayout.addWidget(compileButton,1,0);
    qGridLayout.addWidget(uploadCodeButton,2,0);
    qGridLayout.addWidget(openTerminalButton,3,0);
///    qGridLayout.addWidget(&qLabelMousePosition,3,0);
    qGridLayout.addWidget(qSplitter,0,1,1,3);
//    qGridLayout.addWidget(scrollArea,0,1,1,2);
//    qGridLayout.addWidget(dialogWarten,1,1,3,1);
//    qGridLayout.addWidget(sourcecodeWidget,0,3);
    qGridLayout.addWidget(&qPlainTextEdit,1,2,3,2);
    qGridLayout.setColumnStretch(1,10);   //Editorwidget + Dialog
    qGridLayout.setColumnStretch(3,10);   //SourcecodeWidget
    qGridLayout.setColumnStretch(2,1000); //Make-Console
    qGridLayout.setColumnMinimumWidth(1,DIALOG_WIDTH);

    qGridGroupBox.setLayout(&qGridLayout);
    this->setCentralWidget(&qGridGroupBox);
    this->setMinimumSize(BLOCKLIST_WIDTH+EDITORWIDGET_WIDTH+SOURCECODEWIDGET_WIDTH+30, EDITORWIDGET_HEIGHT+90);  //ursprünglich 620,510;
}

void MainWindow::setupMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Datei"));

    QAction *newAction = fileMenu->addAction(tr("&Neu"));
    newAction->setShortcuts(QKeySequence::New);

    QAction *openAction = fileMenu->addAction(tr("&Öffnen"));
    openAction->setShortcuts(QKeySequence::Open);

    QAction *saveAction = fileMenu->addAction(tr("&Speichern"));
    saveAction->setShortcuts(QKeySequence::Save);

    QAction *saveAsAction = fileMenu->addAction(tr("&Speichern unter..."));
    saveAsAction->setShortcuts(QKeySequence::SaveAs);

    QAction *exitAction = fileMenu->addAction(tr("Beenden"));
    exitAction->setShortcuts(QKeySequence::Quit);

    connect(newAction, SIGNAL(triggered()), this, SLOT(newDokument()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openSourcecode()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
//    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveSourcecode()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
//    connect(restartAction, SIGNAL(triggered()), this, SLOT(setupPuzzle()));

///Menü zum Debugging
//    QMenu *debugMenu = menuBar()->addMenu(tr("&Debug"));
//    QAction *updateTextEditorWidgetAction = debugMenu->addAction(tr("&updateTextEditorWidget"));
//    QAction *updateEditorWidgetSizeAction = debugMenu->addAction(tr("&updateEditorWidgetSize"));
//    QAction *openTerminalAction = debugMenu->addAction(tr("&openTerminal"));
////    QAction *updateTextEditorWidgetAction = gameMenu->addAction(tr("&updateTextEditorWidget"));
//    connect(updateTextEditorWidgetAction, SIGNAL(triggered()), this, SLOT(updateSourcecodeWidget()));
//    connect(updateEditorWidgetSizeAction, SIGNAL(triggered()), editorWidget, SLOT(updateSize()));
//    connect(openTerminalAction, SIGNAL(triggered()), this, SLOT(openTerminal()));

///Terminal-Menü
//    QMenu *terminalMenu = menuBar()->addMenu(tr("&Terminal öffnen"));
//    terminalMenu->setTearOffEnabled(true);
//    connect(terminalMenu, SIGNAL(aboutToShow()), this, SLOT(openTerminal()));

    ///Hilfe-Menü
    QMenu *helpMenu = menuBar()->addMenu(tr("&Hilfe"));
    QAction *actionAbout = helpMenu->addAction(tr("Über..."));

    connect(actionAbout, SIGNAL(triggered()), this, SLOT(on_actionAbout_triggered()));
}

void MainWindow::saveAs()
{
    qDebug() << "MainWindow::saveAs";
    QString selectedFilter = "";
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Speichern unter"), "",
            tr("Quelltext (*.c);;Alle Dateien (*)"), &selectedFilter);
            //PrintBot Dokument (*.pbd);;
    if (!fileName.isEmpty())
    {
        QFile sFile(fileName);
        if (sFile.open(QFile::WriteOnly | QFile::Text))
        {
            //Save
            QFile sFile(fileName);
            if (sFile.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream out(&sFile);

                if (selectedFilter.compare("Quelltext (*.c)") == 0 || selectedFilter.compare("Alle Dateien (*)") == 0)
                {
                    out << this->sourcecodeWidget->toPlainText();
                    //out << ui->textEdit->toPlainText();     //alternative: toHTML
                }
                else if (selectedFilter.compare("PrintBot Dokument (*.pbd)") == 0)
                {
                    out << getProgramCode().getSaveData();
                }
                else
                {
                    qDebug() << "ERROR: MainWindow::saveAs(), undefined file filter!";
                }

                sFile.flush();
                sFile.close();
            }
        }
    }
}

//TODO: aktuelles Dokument überspeichern
void MainWindow::saveSourcecode()
{
    QString fileName = "";
    QTextStream fileNameQTextStream(&fileName);
    fileNameQTextStream << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) << "/PrintBot_Development_Environment/make/sourcecode.c";
    if (!fileName.isEmpty())
    {
        QFile sFile(fileName);
        if (sFile.open(QFile::WriteOnly | QFile::Text))
        {
            //Save
            QFile sFile(fileName);
            if (sFile.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream out(&sFile);
                out << this->sourcecodeWidget->toPlainText();
                //qDebug() <<  this->sourcecodeWidget->toPlainText();
                //out << ui->textEdit->toPlainText();     //alternative: toHTML
                sFile.flush();
                sFile.close();
            }
        }
    }
}

void MainWindow::openSourcecode()
{
//    qDebug() << "MainWindow::openSourcecode";
    QString file = QFileDialog::getOpenFileName(this, "Öffnen");

    if(!file.isEmpty())
    {
        QFile sFile(file);
        if(sFile.open(QFile::ReadOnly | QFile::Text))
        {
            openFile = file;
            QTextStream in(&sFile);
            QString openFileQString = in.readAll();
            sFile.close();

            OpenFileParser openFileParser(this, openFileQString);
        }
    }
}

void MainWindow::compileSourcecode()
{
    qDebug() << "MainWindow::compileSourcecode";
    this->saveSourcecode();
    QString makeEXE = "";
    QTextStream makeQTextStream(&makeEXE);
    makeQTextStream << "make.exe -C " << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) << "/PrintBot_Development_Environment/make";
    qProcess->start(makeEXE);

//    makeQTextStream << "make.exe -C " << QStandardPaths::displayName(QStandardPaths::DocumentsLocation) << "/PrintBot_Development_Environment/make";
//    alreadyMaked = true;
}

void MainWindow::uploadHexfile()
{
    qDebug() << "MainWindow::uploadHexfile";

    ///Gefahr, dass fehlerhafter Code übertragen wird.
//    if (!alreadyMaked)
//    {
//        compileSourcecode();
//    }

    QString avrdudeEXE = "";
    QTextStream avrdudeQTextStream(&avrdudeEXE);
//    avrdudeQTextStream << "avrdude.exe -P usb -c usbasp -p m1284p -U flash:w:" << QStandardPaths::displayName(QStandardPaths::DocumentsLocation) << "/PrintBot_Development_Environment/make/sourcecode.hex:a";
//    QDir::homePath() << "/" << QStandardPaths::displayName(QStandardPaths::DocumentsLocation)
    avrdudeQTextStream << "avrdude.exe -P usb -c usbasp -p m1284p -U flash:w:" << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) << "/PrintBot_Development_Environment/make/sourcecode.hex:a";
    qProcess->start(avrdudeEXE);
//    qProcess->start("avrdude.exe -P usb -c usbasp -p m1284p -U flash:w:./make/sourcecode.hex:a");
}

void MainWindow::readQProcessOutput()
{
//    qDebug() << "MainWindow::readQProcessOutput()";
    QByteArray outputProcess = qProcess->readAllStandardOutput(); ///Zeilenweise
    qPlainTextEdit.moveCursor(QTextCursor::End);
    qPlainTextEdit.insertPlainText(outputProcess); //appendPlainText(output);
    if (outputProcess.contains("Error"))
    {
        qDebug() << "ERROR beim Kompilieren!";
//        QMessageBox::warning(this, tr("Fehler"), tr("Fehler beim Kompilieren"));
    }
    else if (outputProcess.contains("error"))
    {
        qDebug() << "ERROR beim Upload!";
//        QMessageBox::warning(this, tr("Fehler"), tr("Fehler beim Upload!"));
    }
}

void MainWindow::openTerminal()
{
    qDebug() << "MainWindow::openTerminal()";

    if (terminalWidget)
    {
        qDebug() << "MainWindow::openTerminal(), delete terminalWidget;";
        delete terminalWidget;
        qDebug() << "MainWindow::openTerminal(), delete terminalWidget successfull;";
    }
    terminalWidget = new TerminalWidget(this);
     qDebug() << "MainWindow::openTerminal(), ENDE";
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog* aboutDialog = new AboutDialog(this);
    aboutDialog->show();
}

void MainWindow::resetTerminalWidgetPointer()
{
    qDebug() << "MainWindow::resetTerminalWidgetPointer()";
    this->terminalWidget = NULL;
}

void MainWindow::addBlockToBlockListWidget(ProgramBlock *newProgramBlock, int row)
{
    blockListWidget->addBlock(newProgramBlock, row);
}

ProgramCode &MainWindow::getProgramCode()
{
    return this->editorWidget->getProgramCode();
}

EditorWidget *MainWindow::getEditorWidget()
{
    return this->editorWidget;
}
