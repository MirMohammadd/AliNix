#ifndef __ALINIX_KERNEL_NET_TCP_IMPL_H
#define __ALINIX_KERNEL_NET_TCP_IMPL_H

#include <alinix/types.h>
#include <net/tcp.h>

extern union tcp_listen_pcbs_t tcp_listen_pcbs;

extern uint8_t tcp_active_pcbs_changed;
extern struct tcp_pcb *tcp_active_pcbs;  /* List of all TCP PCBs that are in a
              state in which they accept or send
              data. */


union tcp_listen_pcbs_t { /* List of all TCP PCBs in LISTEN state. */
  struct tcp_pcb_listen *listen_pcbs; 
  struct tcp_pcb *pcbs;
};


#define TCP_PCB_REMOVE_ACTIVE(pcb)                 \
  do {                                             \
    tcp_pcb_remove(&tcp_active_pcbs, pcb);         \
    tcp_active_pcbs_changed = 1;                   \
  } while (0)



struct tcp_seg {
  struct tcp_seg *next;    /* used when putting segements on a queue */
  struct pbuf *p;          /* buffer containing data + TCP header */
  uint16_t len;               /* the TCP length of this segment */
#if TCP_OVERSIZE_DBGCHECK
  uint16_t oversize_left;     /* Extra bytes available at the end of the last
                              pbuf in unsent (used for asserting vs.
                              tcp_pcb.unsent_oversized only) */
#endif /* TCP_OVERSIZE_DBGCHECK */ 
#if TCP_CHECKSUM_ON_COPY
  uint16_t chksum;
  uint8_t  chksum_swapped;
#endif /* TCP_CHECKSUM_ON_COPY */
  uint8_t  flags;
#define TF_SEG_OPTS_MSS         (uint8_t)0x01U /* Include MSS option. */
#define TF_SEG_OPTS_TS          (uint8_t)0x02U /* Include timestamp option. */
#define TF_SEG_DATA_CHECKSUMMED (uint8_t)0x04U /* ALL data (not the header) is
                                               checksummed into 'chksum' */
  struct tcp_hdr *tcphdr;  /* the TCP header */
};


#define TCP_EVENT_ERR(errf,arg,err)                            \
  do {                                                         \
    if((errf) != NULL)                                         \
      (errf)((arg),(err));                                     \
  } while (0)

#endif