#ifndef PLCCLIENT_H_
#define PLCCLIENT_H_

#include <string>

#include "plcmsg/plc_nml.h"

#ifdef toLine
#undef toLine
#endif

enum PLC_WAIT_TYPE {
  PLC_WAIT_NONE,
  PLC_WAIT_RECEIVED,
  PLC_WAIT_DONE,
};

class PlcClient {
 public:
  PlcClient();
  ~PlcClient();

  int Startup(std::string plc_nmlfile);
  void Shutdown();
  bool connected_;

  int WriteCmdMsg(RCS_CMD_MSG *msg, int wait_done = PLC_WAIT_RECEIVED);

  std::string GetErrString();
  bool JobAbort();
  int PlcTaskInit();
  void SetTimeout(double timeout) {
    plc_timeout_ = timeout;
  }
  double GetTimeout() const {
    return plc_timeout_;
  }

  int UpdateStatus();
  int UpdateError();

  friend class SampleController;

 private:
  // communication channel
  RCS_CMD_CHANNEL *plc_cmd_buffer_;
  RCS_STAT_CHANNEL *plc_stat_buffer_;
  NML *plc_err_buffer_;

  char error_string_[NML_ERROR_LEN];
  double plc_timeout_;
  int cmd_serial_number_;

  PLC_STAT *plc_status_;

  int CommandWaitReceived(int serial_number);
  int CommandWaitDone(int serial_number);

};

#endif
