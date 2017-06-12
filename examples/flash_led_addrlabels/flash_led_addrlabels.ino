//example 2: 仅使用原始pt，但不用SWITCH的方式实现，而是使用地址标签.
//任务中可以随便使用switch()语句
/*功能：
arduino 12脚、13脚分别以1s、5s的间隔亮灭（亮时和灭时的时间都是一样的）。
不用pt-timer.h库，用pt.h本身。延时是loop()中delay(1000),再配合变量计数实现。
延时期间就只能原地画圈，别的事是做不了的。当然，本例没别的任务做，所以
对本例没有什么影响。
*/

#define LC_INCLUDE "lc-addrlabels.h" //需放在下一句之前
#include <pt.h>

static int counter1, counter2, state1 = 0, state2 = 0;

static int protothread1(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, counter1 == 1);
    digitalWrite(12, state1);
    state1 = !state1;
    counter1 = 0;
  }
  PT_END(pt);
}


static int protothread2(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1) {
    PT_WAIT_UNTIL(pt, counter2 == 5);
    counter2 = 0;
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
  delay(1000);
  counter1++;
  counter2++;
}
