/**
 * \file
 *         ALOHA MAC layer
 * \author
 *         rct15
 */
//#include<unistd.h>

#include "net/mac/aloha.h"
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

static void transmit_packet(void *ptr)
{
  struct send_struct *s = ptr;
  printf("!! ALOHA sending packet %ld!!\n",CLOCK_SECOND);
  NETSTACK_RDC.send(s->sent, s->ptr);
  n_pkts_transmitted++;
}
static void
send_packet(mac_callback_t sent, void *ptr)
{
  clock_time_t delay;
  static struct ctimer c;

  struct send_struct s1;
  s1.ptr = ptr;
  s1.sent = sent;
  void *ct_ptr = &s1;

  /*btime = NETSTACK_RDC.channel_check_interval();
  if(btime == 0) {
    btime = MAX(CLOCK_SECOND / 3125, 1);
  }*/
  delay = 1;//((1 << 1) - 1) *1;
  //delay=CLOCK_SECOND*0.01;
  delay = random_rand() % delay;

  //clock_wait(delay);
  //transmit_packet(ct_ptr);
  ctimer_set(&c, delay, transmit_packet,ct_ptr);
  /*while(!ctimer_expired(&c)){
    n--;
    if(n==100){
      n=100;
      printf("Waiting to send packet\n");
    }
  }*/
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
