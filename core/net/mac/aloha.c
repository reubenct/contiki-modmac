/**
 * \file
 *         A ALOHA MAC layer
 * \author
 *         rct15
 */

#include "net/mac/aloha.h"
#include "net/netstack.h"

uint16_t n_pkts_transmitted = 0;
uint16_t n_pkts_received = 0;

static void
send_packet(mac_callback_t sent, void *ptr)
{
  n_pkts_transmitted++;
  NETSTACK_RDC.send(sent, ptr);
}

static void
packet_input(void)
{
  n_pkts_received++;
  NETSTACK_NETWORK.input();
}

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
init(void)
{
}

const struct mac_driver aloha_driver = {
  "ALOHA",
  init,
  send_packet,
  packet_input,
  on,
  off,
  channel_check_interval,
};