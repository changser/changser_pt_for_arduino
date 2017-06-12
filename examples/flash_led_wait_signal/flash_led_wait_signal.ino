
//example 4: 用PT-signal.H来实现任务间的配合、控制权的转移。

/*功能：
arduino 13脚是ARDUINO板上的LED指示灯。在接收到串口指令--字符"w"后，指示灯会闪烁。第一次接收，闪2下，等待接收；
第二次闪3下，等待接收。然后循环如此。
用pt-signal.h库
 优缺点:1、任务之间的配合不必额外定义标志位，流程更清晰明了。
      2、 但pt结构体增加了1字节占用。
*/

#define PT_USE_TIMER
#define PT_USE_SIGNAL
#define LC_INCLUDE "lc-addrlabels.h"

#include "pt.h"
static struct pt thread1, thread2;
unsigned char i;

void setup() {
  //Set Hardware
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  //Initialize the Threads
  PT_INIT(&thread1);
  PT_INIT(&thread2);
}

//Blinky LED, 0.5Hz
static int thread1_entry(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1)
  {
    if (Serial.available() > 0)
    {
      switch (Serial.read())
      {
        case 'w':
          PT_SEND_SIGNAL(&thread2);
          break;
        default:
          break;
      }
    }
    PT_YIELD(pt); //Check the other events.关键就在于这个出让，否则还是会转到本任务开始执行
  }
  PT_END(pt);
}

//If a series of chars are sent through COM, blink LED13 quickly for 2times,next for 5 times
static int thread2_entry(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_SIGNAL(pt);
    for (i = 0; i < 2; i++)  //闪两下
    {
      digitalWrite(13, HIGH);
      PT_TIMER_DELAY(pt, 200);
      digitalWrite(13, LOW);
      PT_TIMER_DELAY(pt, 200);
    }

    PT_WAIT_SIGNAL(pt);
    for (i = 0; i < 5; i++)	//闪5下
    {
      digitalWrite(13, HIGH);
      PT_TIMER_DELAY(pt, 200);
      digitalWrite(13, LOW);
      PT_TIMER_DELAY(pt, 200);
    }
  }
  PT_END(pt);
}

void loop() {
  thread1_entry(&thread1);
  thread2_entry(&thread2);
}
