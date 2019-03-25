#include "net/mac/modifiedmac.h"
#include "net/netstack.h"
#include "sys/clock.h"

// library functions for our MAC protocol
static void
randombackoff_blocking(unsigned int delay)
{
    clock_wait(delay);
}

static void
send_frame(mac_callback_t sent, void *ptr)
{
    NETSTACK_RDC.send(sent, ptr);
}

// functions which are from contiki
static int
on(void)
{
  return NETSTACK_RDC.on();
}

static int
off(int keep_radio_on)
{
  return NETSTACK_RDC.off(keep_radio_on);
}

static unsigned short
channel_check_interval(void)
{
  return 0;
}

static void
packet_input(void)
{
    NETSTACK_NETWORK.input();
}

static void
init(void)
{
}

const struct mac_driver modifiedmac_driver = {
  "modifiedmac",
  init,
  send_packet,
  packet_input,
  on,
  off,
  channel_check_interval,
};


static void
send_packet(mac_callback_t sent, void *ptr)
{
randombackoff_blocking(100);
send_frame(sent, ptr);

}
