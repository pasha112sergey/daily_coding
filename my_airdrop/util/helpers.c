#include "helpers.h"
#include <signal.h>

sig_atomic_t connection_status_change;

char *mtype_to_s(M_TYPE m)
{
      switch (m)
      {
            case M_ACK:
                  return "ACK";
            case M_BROADCAST:
                  return "BROADCAST";
            case M_IDENTIFY:
                  return "IDENTIFY";
            case M_SEND:
                  return "SEND";
            case M_NACK:
                  return "NACK";
            default:
                  return "UNKNOWN";
      }
}

M_TYPE parse_mtype(uint8_t *buf)
{
      M_HEADER header;
      memcpy(&header, buf, sizeof(M_HEADER));

      return header.type;
}

void signal_handler(int sig)
{
      if (sig == NEW_CONNECTION)
      {
            connection_status_change = NEW_CONNECTION;
      }
      else if (sig == DISCONNECT)
      {
            connection_status_change = DISCONNECT;
      }
      else
      {
            connection_status_change = -1;
      }
}