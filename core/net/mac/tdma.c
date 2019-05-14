/**
 * \file
 *         TDMA MAC layer
 * \author
 *         rct15
 */
//#include<unistd.h>

#include "net/mac/tdma.h"
#include "net/netstack.h"

#include "sys/ctimer.h"
#include "sys/clock.h"

#include "lib/random.h"

#include <string.h>
#include <stdio.h>

uint16_t n_pkts_transmitted = 0;
uint16_t n_pkts_received = 0;

struct send_struct {
  void *ptr;
  mac_callback_t sent;
};

static void
send_packet(mac_callback_t sent, void *ptr)
{
  clock_time_t delay;
  static struct ctimer c;

  //int n=5, slot=0,i;

  printf("!! TDMA sending packet %ld!!\n",CLOCK_SECOND);
  n_pkts_transmitted++;
  NETSTACK_RDC.send(sent, ptr); //sending without delay
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

const struct mac_driver tdma_driver = {
  "TDMA",
  init,
  send_packet,
  packet_input,
  on,
  off,
  channel_check_interval,
};
