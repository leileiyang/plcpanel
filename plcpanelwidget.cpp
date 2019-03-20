#include "plcpanelwidget.h"
#include "ui_plcpanelwidget.h"

#include "dev/nmlgas.h"

PlcPanelWidget::PlcPanelWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PlcPanelWidget), gas_(NULL)
{
  ui->setupUi(this);
}

PlcPanelWidget::~PlcPanelWidget()
{
  delete ui;

  if (gas_) {
    delete gas_;
  }
}

int PlcPanelWidget::Initialzie() {
  if (plc_client_.Startup("plctask.nml")) {
    gas_ = new NmlGas(plc_client_);
  } else {
    return -1;
  }
  return 0;
}

void PlcPanelWidget::onBlow(bool checked)
{
  if (checked) {
    // open current setting gas
    gas_->Open(ui->gas_cb_box_->currentIndex(), 5);
  } else {
    // close current setting gas
    gas_->Close();
  }
}
