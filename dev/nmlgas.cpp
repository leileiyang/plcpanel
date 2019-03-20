#include "nmlgas.h"

#include "plcclient/plcclient.h"

int NmlGas::Open(int gas_id, double pressure) {
  GAS_OPEN gas_open;
  gas_open.gas_id_ = gas_id;
  gas_open.pressure_ = pressure;
  return plc_client_.WriteCmdMsg(&gas_open);
}

int NmlGas::Close() {
  GAS_CLOSE gas_close;
  return plc_client_.WriteCmdMsg(&gas_close);
}
