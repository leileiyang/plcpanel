#include "plcpanelwidget.h"
#include "ui_plcpanelwidget.h"

#include "dev/nmlgas.h"

#include <QDebug>

PlcPanelWidget::PlcPanelWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PlcPanelWidget), gas_(NULL), sample_ctrller_(plc_client_)
{
  ui->setupUi(this);
  connect(&update_timer_, SIGNAL(timeout()), this, SLOT(updateStatus()));
  update_timer_.start(100);
  t.start();
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
    sample_ctrller_.Initialize();
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

void PlcPanelWidget::updateStatus() {
  qDebug("Time elapsed in updateStatue: %d ms", t.elapsed());
  t.restart();
  sample_ctrller_.ReadPlcStatus(plc_status_);
  ui->blow_btn_->onSwitch(CheckStatusBit(plc_status_.gas_stat_.current_gas_,
                                         plc_status_.gas_stat_.gas_status_));

}
