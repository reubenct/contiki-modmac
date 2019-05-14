/**
 * \file
 *         CSMA layer
 * \author
 *         rct15
 */
//#include<unistd.h>

#include "net/mac/csma_man.h"
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

static uint8_t channel_sense()
{
  uint8_t interference = 0;
  uint8_t c, waspacket = 0;
  for(c = 0; c < 2; c++) {
    if(!NETSTACK_RADIO.channel_clear())
      interference++;
  }
  return interference;
}
static void transmit_packet(void *ptr)
{
  struct send_struct *s = ptr;
  //printf("!! CSMA_man sending packet %ld!!\n",CLOCK_SECOND);
  NETSTACK_RDC.send(s->sent, s->ptr);
  n_pkts_transmitted++;
}
static void
send_packet(mac_callback_t sent, void *ptr)
{
  clock_time_t delay;
  int be=0;
  //static struct ctimer c;

  struct send_struct s1;
  s1.ptr = ptr;
  s1.sent = sent;
  void *ct_ptr = &s1;

  delay = 1;//((1 << 1) - 1) *1;
  delay = random_rand() % delay;
  while(1){
    delay = 100 << be;
    delay = random_rand() % delay;
    clock_wait(delay);
    if(channel_sense()==0){
      transmit_packet(ct_ptr);
      break;
    }
    else{
      printf("!!    delay = %d\n",be);
      be = be+1;
    }
  }
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

const struct mac_driver csmaman_driver = {
  "CSMAMAN",
  init,
  send_packet,
  packet_input,
  on,
  off,
  channel_check_interval,
};
