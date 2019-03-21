#include "samplecontroller.h"

#include "sampler.h"

#include "plcclient/plcclient.h"

SampleController::SampleController(PlcClient &plc, QObject *parent) :
    QObject(parent), plc_(plc) {

  Sampler *sampler = new Sampler(plc_, lock_);
  sampler->moveToThread(&worker_thread_);
  connect(&worker_thread_, SIGNAL(finished()), sampler, SLOT(deleteLater()));
  connect(this, SIGNAL(setTimer(int)), sampler, SLOT(SetTimer(int)));
  connect(this, SIGNAL(start()), sampler, SLOT(Start()));
  connect(this, SIGNAL(stop()), sampler, SLOT(Stop()));
  worker_thread_.start();
}

void SampleController::Initialize() {
  emit setTimer(100);
}

void SampleController::ReadPlcStatus(PLC_STAT &plc_status) {
  QReadLocker locker(&lock_);
  plc_status = *(plc_.plc_status_);
}
