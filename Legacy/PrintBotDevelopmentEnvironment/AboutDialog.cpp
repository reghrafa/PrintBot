#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setupWidgets();
   // this->setFixedSize(360,380);
   // this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //this->setFixedSize(this->size());
}

AboutDialog::~AboutDialog()
{

}

void AboutDialog::setupWidgets()
{
    this->setWindowIcon(QIcon("./data/PB_Logo_114x114.png"));
    this->setModal(true);
    this->setWindowTitle("Über PrintBot Development Environment");
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
//    this->setFixedSize(380, 399);
//    this->resize(380, 399);
//    qGroupBox.setParent(this);
    qLabel.setParent(this);
    pushButtonOK.setParent(this);

//    qGridLayout.setParent(this);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout.addItem(horizontalSpacer);
    horizontalLayout.addWidget(&pushButtonOK);
    horizontalLayout.addItem(horizontalSpacer_2);

    verticalLayout.addWidget(&qLabel);
    verticalLayout.addLayout(&horizontalLayout);

    qGridLayout.addLayout(&verticalLayout, 0, 0, 1, 1);
//    qGridLayout.setParent(&qGroupBox);
//    qGroupBox.setLayout(&qGridLayout);
    this->setLayout(&qGridLayout);

    QString qLabelText = "";
    QTextStream qLabelTextStream(&qLabelText);
    qLabelTextStream << "<html><head/><body>"
                    // << "<p></p>"
                     << "<p align=\"center\"><span style=\" font-size:20pt;\">Print<span style=\"color:rgb(255,0,145)\">Bot</span> <br> Development Environment</span></p>"
                     << "<p align=\"center\"><span style=\" font-size:10pt;\">Version " << VERSION << "</span></p>"
                     << "<p align=\"center\"><span style=\" font-size:10pt;\">Copyright \302\251 2014 Fabian Duffhauß</span></p>"
                     << "<p align=\"center\"><br/></p>"
                     << "<p align=\"center\"><span style=\" font-size:10pt;\">Bitte senden Sie uns auftretende Bugs, <br> Kommentare oder Verbesserungsvorschläge an:</span></p>"
                     << "<p align=\"center\"><a href=\"mailto:PrintBot@outlook.com\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">PrintBot@outlook.com</span></a><br></p>"
                     << "<p align=\"center\"><a href=\"www.PrintBot.info\" style=\"text-decoration: none;\">"
                     << "<span style=\"font-size:18pt;\">www."
                     << "<span style=\"font-size:25pt;\" style=\"color:rgb(0,0,0)\">Print</span>"
                     << "<span style=\"font-size:25pt;\" style=\"color:rgb(255,0,145)\">Bot</span>"
                     << ".info</span></a><br></p>"
                     << "</body></html>";
    qLabel.setText(qLabelText.toLatin1());
    qLabel.setTextInteractionFlags(Qt::TextBrowserInteraction);
    qLabel.setOpenExternalLinks(true);
    pushButtonOK.setText("OK");

    connect(&pushButtonOK, SIGNAL(clicked()), this, SLOT(close()));
}
