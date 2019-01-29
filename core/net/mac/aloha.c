/**
 * \file
 *         A ALOHA MAC layer
 * \author
 *         rct15
 */
//#include<unistd.h>

#include "net/mac/aloha.h"
#include "net/netstack.h"

#include "sys/ctimer.h"
#include "sys/clock.h"

#include "lib/random.h"

uint16_t n_pkts_transmitted = 0;
uint16_t n_pkts_received = 0;

struct send_struct {
  void *ptr;
  mac_callback_t sent;
};

static void transmit_packet(void *ptr)
{
  struct send_struct *s = ptr;
  NETSTACK_RDC.send(s->sent, s->ptr);
  n_pkts_transmitted++;
}
static void
send_packet(mac_callback_t sent, void *ptr)
{
  clock_time_t delay;
  struct ctimer c;
  struct send_struct s1;
  s1.ptr = ptr;
  s1.sent = sent;
  delay = ((1 << 4) - 1) * MAX(CLOCK_SECOND / 3125, 1);
  delay = random_rand() % delay;
  ctimer_set(&c, delay, transmit_packet,&s1);

  //NETSTACK_RDC.send(sent, ptr); //sending without delay
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
