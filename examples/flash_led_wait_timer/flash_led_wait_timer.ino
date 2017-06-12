//example 3: 用PT-TIMER.H来实现延时。

/*功能：
arduino 12脚、13脚分别以1s、5s的间隔亮灭（亮时和灭时的时间都是一样的）。
用pt-timer.h库
 优缺点:1、延时不是原地画圈，而是切换到别的任务。
      2、不必额外用全局变量来配合实现延时。延时在任务里自己实现。
	  但pt结构体增加了4字节占用。
*/

#define PT_USE_TIMER //放在下一句之前
#include "pt.h"

static int state1 = 0, state2 = 0;

static int protothread1(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1)
  {
    PT_DELAY_MS(pt,1000);//PT_TIMER_DELAY(pt,1000);
    digitalWrite(12, state1);
    state1 = !state1;
  }
  PT_END(pt);
}


static int protothread2(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1) {
    PT_DELAY_MS(pt,5000);//PT_TIMER_DELAY(pt,5000);
    digitalWrite(13, state2);
    state2 = !state2;
  }
  PT_END(pt);
}


static struct pt pt1, pt2;
void setup()
{

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  PT_INIT(&pt1);
  PT_INIT(&pt2);
}

void loop ()
{
  protothread1(&pt1);
  protothread2(&pt2);
}
