/*
   Copyright (c) 2017, by changser.
   All rights reserved.
*/

#ifndef __PT_SIGNAL_H__
#define __PT_SIGNAL_H__

//#define PT_SIGNAL_INIT(pt, c) (pt)->signal = c //不用初始化

#define PT_WAIT_SIGNAL(pt)  \
  do {            \
   (pt)->signal=0;  \
    PT_WAIT_UNTIL(pt, (pt)->signal > 0);        \
  } while(0)

#define PT_SEND_SIGNAL(pt) (pt)->signal = 1

#endif /* __PT_SIGNAL_H__ */

/** @} */
/** @} */

