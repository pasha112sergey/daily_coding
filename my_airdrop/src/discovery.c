#include <bluetooth/bluetooth.h>
#include <bluetooth/gatt.h>
#include <stdio.h>
#include "helpers.h"

static uint8_t static_addr[6] = {0};

void gatt_server_start()
{
      struct sockaddr_l2 addr;

      int att_fd = socket(PF_BLUETOOTH, SOCK_SEQPACKET | SOCK_CLOEXEC, BTPROTO_L2CAP);
      if (att_fd < 0)
      {
            fprintf(stderr, "Failed to create ATT server socket\n");
            return;
      }

      memset(&addr, 0, sizeof(addr));

      addr.l2_family = AF_BLUETOOTH;
      addr.l2_cid = htobs(ATT_CID);
      memcpy(&addr.l2_bdaddr, static_addr, 6);
      addr.l2_bdaddr_type = BDADDR_LE_RANDOM;
      
      if(bind(att_fd, (struct sockaddr *) &addr, sizeof(addr)) <  0)
      {
            fprintf(stderr, "Failed to bind ATT server to socket");
            close(att_fd);
            att_fd = -1;
            reutrn;
      }

}


int main()
{
      gatt_server_start();

}