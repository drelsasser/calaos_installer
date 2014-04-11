#include "DialogNewAVReceiver.h"
#include "ui_DialogNewAVReceiver.h"
#include "ListeRoom.h"

using namespace Calaos;

DialogNewAVReceiver::DialogNewAVReceiver(Room *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewAVReceiver),
    room(r)
{
    ui->setupUi(this);
    ui->labelError->hide();

    ui->comboBoxType->addItem("Pioneer", QString("pioneer"));
    ui->comboBoxType->addItem("Denon", QString("denon"));
    ui->comboBoxType->addItem("Onkyo", QString("onkyo"));
    ui->comboBoxType->addItem("Marantz", QString("marantz"));
    ui->comboBoxType->addItem("Yamaha", QString("yamaha"));
}

DialogNewAVReceiver::~DialogNewAVReceiver()
{
    delete ui;
}

void DialogNewAVReceiver::on_buttonBox_accepted()
{
    if (ui->lineEditName->text().isEmpty())
    {
        ui->labelError->show();
        return;
    }

    Params p;
    p.Add("name", ui->lineEditName->text().toUtf8().constData());
    p.Add("model", ui->comboBoxType->currentData().toUtf8().constData());
    p.Add("host", ui->lineEditHost->text().toUtf8().constData());
    p.Add("visible", "true");
    p.Add("type", "AVReceiver");

    output = ListeRoom::Instance().createOutput(p, room);

    accept();
}