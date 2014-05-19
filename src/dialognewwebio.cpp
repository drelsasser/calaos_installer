#include "dialognewwebio.h"
#include "ui_DialogNewWebIO.h"
#include "formrules.h"
#include "formanalogproperties.h"

DialogNewWebIO::DialogNewWebIO(Room *r, int item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewWebIO), io(NULL), room(r), item(item)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    QString label;

    //hide error labels by default.
    ui->label_error_empty->hide();
    ui->label_error_path_empty->hide();
    ui->label_error_url_empty->hide();

    ui->label_value_off->hide();
    ui->label_value_on->hide();
    ui->edit_value_off->hide();
    ui->edit_value_on->hide();

    analogWidget= nullptr;
    // Set default combo type value
    switch (item)
      {
      case ITEM_INPUT_SWITCH:
        ui->io_type->setCurrentIndex(0);
        label = tr("Create an new Web Input Switch");
        break;
      case ITEM_LIGHT:
        ui->io_type->setCurrentIndex(1);
        ui->label_value_off->show();
        ui->label_value_on->show();
        ui->edit_value_off->show();
        ui->edit_value_on->show();
        label = tr("Create an new Web Light");
        break;
      case ITEM_LIGHT_RGB:
        ui->io_type->setCurrentIndex(2);
        label = tr("Create an new Web RGB Light");
        break;
      case ITEM_SHUTTER:
        ui->io_type->setCurrentIndex(3);
        label = tr("Create an new Web Shutter");
        break;
      case ITEM_TEMP:
        ui->io_type->setCurrentIndex(4);
        label = tr("Create an new Web Temperature Input");
        analogWidget = new FormAnalogProperties;
        ui->verticalLayout_3->insertWidget(1, analogWidget);
        break;
      case ITEM_ANALOG:
        ui->io_type->setCurrentIndex(5);
        label = tr("Create an new Web Analog Input");
        analogWidget = new FormAnalogProperties;
        ui->verticalLayout_3->insertWidget(1, analogWidget);
        break;
      case ITEM_STRING:
        if (type.find("Input") != string::npos)
        {
          ui->io_type->setCurrentIndex(7);
          label = tr("Create an new Web String Input");
        }
        else
        {
          ui->io_type->setCurrentIndex(8);
          label = tr("Create an new Web String Output");
        }
        break;
      default:
        break;
      }

    ui->io_type_label->hide();
    ui->io_type->hide();

    ui->groupBox->setTitle(label);
}

DialogNewWebIO::~DialogNewWebIO()
{
    delete ui;
}

void DialogNewWebIO::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogNewWebIO::on_buttonBox_accepted()
{
    if (ui->edit_name->text().isEmpty())
    {
        ui->label_error_empty->show();
        return;
    }
    else if (ui->label_error_path_empty->text().isEmpty())
    {
        ui->label_error_path_empty->show();
        return;
    }
    else if (ui->label_error_url_empty->text().isEmpty())
    {
        ui->label_error_url_empty->show();
        return;
    }

    Params p;
    p.Add("name", ui->edit_name->text().toUtf8().constData());



    p.Add("url", to_string(ui->edit_url->text().toUtf8().constData()));
    p.Add("path", to_string(ui->edit_path->text().toUtf8().constData()));
    p.Add("file_type", to_string(ui->data_type->currentText().toLower().toUtf8().constData()));
    if (analogWidget)
    {
        p.Add("unit", analogWidget->getUnit().toUtf8().constData());
        p.Add("coeff",  QString::number(analogWidget->getCoeff()).toUtf8().constData());
        p.Add("offset",  QString::number(analogWidget->getOffset()).toUtf8().constData());
        p.Add("step",  QString::number(analogWidget->getStep()).toUtf8().constData());
        p.Add("min",  QString::number(analogWidget->getMin()).toUtf8().constData());
        p.Add("max",  QString::number(analogWidget->getMax()).toUtf8().constData());
    }
    switch (ui->io_type->currentIndex())
      {
      case 0:
      case 1:
        type = "WebOutputLight";
        p.Add("on_value", to_string(ui->edit_value_on->text().toUtf8().constData()));
        p.Add("off_value", to_string(ui->edit_value_off->text().toUtf8().constData()));
        p.Add("type", type);
        io = ListeRoom::Instance().createOutput(p, room);
        break;
      case 2:
        type = "WebOutputLightRGB";
        p.Add("type", type);
        io = ListeRoom::Instance().createOutput(p, room);
        break;
      case 4:
        type = "WebInputTemp";
        p.Add("type", type);
        io = ListeRoom::Instance().createInput(p, room);
        break;
      case 5:
        type = "WebInputAnalog";
        p.Add("type", type);
        io = ListeRoom::Instance().createInput(p, room);
        break;
      case 7:
        type = "WebInputString";
        p.Add("type", type);
        io = ListeRoom::Instance().createInput(p, room);
        break;
      case 8:
        type = "WebOutputString";
        p.Add("type", type);
        io = ListeRoom::Instance().createOutput(p, room);
        break;
      default:
        break;
      }
    accept();
}
