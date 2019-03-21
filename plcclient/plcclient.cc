#include "plcclient.h"

#include <stdio.h>

PlcClient::PlcClient():
    connected_(false),
    plc_cmd_buffer_(NULL),
    plc_stat_buffer_(NULL),
    plc_err_buffer_(NULL),
    plc_timeout_(3.0),
    cmd_serial_number_(0),
    plc_status_(NULL) {

  memset(error_string_, 0, NML_ERROR_LEN);
}

PlcClient::~PlcClient() {
  Shutdown();
}

int PlcClient::Startup(std::string plc_nmlfile) {
  double end;
  int good;
#define RETRY_TIME 3.0
#define RETRY_INTERVAL 1.0

  end = RETRY_TIME;
  good = 0;
  do {
    if (NULL != plc_cmd_buffer_) {
      delete plc_cmd_buffer_;
    }
    plc_cmd_buffer_ = 
        new RCS_CMD_CHANNEL(plcFormat, "plcCommand", "plcClient",
                            plc_nmlfile.c_str());

    if (plc_cmd_buffer_->valid()) {
      good = 1;
      break;
    }
    esleep(RETRY_INTERVAL);
    end -= RETRY_INTERVAL;
  } while (end > 0.0);

  if (!good) {
    return false;
  }

  end = RETRY_TIME;
  good = 0;
  do {
    if (NULL != plc_stat_buffer_) {
      delete plc_stat_buffer_;
    }
    plc_stat_buffer_ = 
        new RCS_STAT_CHANNEL(plcFormat, "plcStatus", "plcClient",
                             plc_nmlfile.c_str());

    if (plc_stat_buffer_->valid()) {
      plc_status_ = (PLC_STAT *)plc_stat_buffer_->get_address();
      good = 1;
      break;
    }
    esleep(RETRY_INTERVAL);
    end -= RETRY_INTERVAL;
  } while (end > 0.0);

  if (!good) {
    return false;
  }
  end = RETRY_TIME;
  good = 0;
  do {
    if (NULL != plc_err_buffer_) {
      delete plc_err_buffer_;
    }
    plc_err_buffer_ =
        new NML(nmlErrorFormat, "plcError", "plcClient", plc_nmlfile.c_str());

    if (plc_err_buffer_->valid()) {
      good = 1;
      break;
    }
    esleep(RETRY_INTERVAL);
    end -= RETRY_INTERVAL;
  } while (end > 0.0);

  if (!good) {
    return false;
  }
  connected_ = true;
  return true;
}

void PlcClient::Shutdown() {
  if (NULL != plc_err_buffer_) {
    delete plc_err_buffer_;
    plc_err_buffer_ = NULL;
  }

  if (NULL != plc_stat_buffer_) {
    delete plc_stat_buffer_;
    plc_stat_buffer_ = NULL;
  }

  if (NULL != plc_cmd_buffer_) {
    delete plc_cmd_buffer_;
    plc_cmd_buffer_ = NULL;
  }
  connected_ = false;
}

int PlcClient::UpdateStatus() {
  NMLTYPE type;
  if (0 == plc_status_ || 0 == plc_stat_buffer_ || !plc_stat_buffer_->valid()) {
    return -1;
  }
  switch (type = plc_stat_buffer_->peek()) {
    case -1:
      return -1;
    case 0: // no new data
    case PLC_STAT_TYPE: // new data
      break;
    default:
      break;
  }
  return 0;
}

int PlcClient::UpdateError() {
  NMLTYPE type;
  if (0 == plc_err_buffer_ || !plc_err_buffer_->valid()) {
    return -1;
  }
  switch (type = plc_err_buffer_->read()) {
    case -1:
      return -1;
      break;
    case 0: // nothing new
      break;
    case NML_ERROR_TYPE:
      strncpy(error_string_,
          ((NML_ERROR *)(plc_err_buffer_->get_address()))->error,
          NML_ERROR_LEN - 1);

      error_string_[NML_ERROR_LEN - 1] = 0;
      break;
    default:
      sprintf(error_string_, "unrecognized error %ld", type);
      return -1;
      break;
  }
  return 0;
}

#define PLC_COMMAND_DELAY 0.01

int PlcClient::CommandWaitReceived(int serial_number) {
  if (!connected_) {
    return -1;
  }
  double end = 0.0;
  while (plc_timeout_ <= 0 || end < plc_timeout_) {
    UpdateStatus();
    if (plc_status_->echo_serial_number == serial_number) {
      return 0;
    }
    esleep(PLC_COMMAND_DELAY);
    end += PLC_COMMAND_DELAY;
  }
  connected_ = false;
  return -1;
}

int PlcClient::CommandWaitDone(int serial_number) {
  double end = 0.0;
  if (0 != CommandWaitReceived(serial_number)) {
    return -1;
  }
  if (!connected_) {
    return -1;
  }
  while (plc_timeout_ <= 0.0 || end < plc_timeout_) {
    UpdateStatus();
    if (plc_status_->status == RCS_DONE) {
      return 0;
    }
    if (plc_status_->status == RCS_ERROR) {
      return -1;
    }
    esleep(PLC_COMMAND_DELAY);
    end += PLC_COMMAND_DELAY;
  }
  connected_ = false;
  return -1;
}

std::string PlcClient::GetErrString() {
  int ret = UpdateError();
  if (ret < 0) {
    return "lost connection";
  }
  if (strlen(error_string_)) {
    std::string err(error_string_);
    memset(error_string_, 0, NML_ERROR_LEN);
    return err;
  } else {
    return "";
  }
}

bool PlcClient::JobAbort() {
  JOB_ABORT_MSG job_abort;
  return WriteCmdMsg(&job_abort) == 0;
}

int PlcClient::PlcTaskInit() {
  PLC_TASK_INIT task_init;
  return WriteCmdMsg(&task_init);
}

inline int PlcClient::WriteCmdMsg(RCS_CMD_MSG *msg, int wait_done) {
  if (!connected_) {
    return -2;
  }
  msg->serial_number = ++cmd_serial_number_;
  plc_cmd_buffer_->write(msg);
  if (wait_done == PLC_WAIT_RECEIVED) {
    return CommandWaitReceived(cmd_serial_number_);
  } else if (wait_done == PLC_WAIT_DONE) {
    return CommandWaitDone(cmd_serial_number_);
  }
  return -1;
}
