
//example 5: PT-SEM.H的应用
/*功能：arduino第13脚灯，平常以1Hz亮灭（亮灭各0.5秒）；若收到串口数据，则快速闪烁5秒。
用到pt-sem.h，资源只有一个即13脚，但上述两种功能选哪种，还是通过程序安排，谁先按排选谁，
实质还是任务间配合，应该不算是互斥、临界区加锁什么的。
还是得定义全局变量，不推存使用，建议使用PT-SIGNAL.H
*/
#define PT_USE_TIMER
#define PT_USE_SEM
#define LC_INCLUDE "lc-addrlabels.h"

#include "pt.h"
static struct pt thread1,thread2;
static struct pt_sem sem_LED;
unsigned char i;

void setup() {
  //Set Hardware
  pinMode(13,OUTPUT);
  Serial.begin(115200);
  //Initialize Semaphore
  PT_SEM_INIT(&sem_LED,1);
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
    PT_SEM_WAIT(pt,&sem_LED);   //因初始信号为1，所以不会等，往下执行，并减1
    digitalWrite(13,!digitalRead(13));
    PT_TIMER_DELAY(pt,1000);
    PT_SEM_SIGNAL(pt,&sem_LED);  //信号又+1
    PT_YIELD(pt); //Check the other events.关键就在于这个出让，否则还是会转到本任务开始执行
  }
  PT_END(pt);
}

//If a series of chars are sent through COM, blink LED13 quickly for 5 times
static int thread2_entry(struct pt *pt)
{
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, Serial.available());      //没有串口数据，出让，也不改变sem_LED信号的值。又转让tread1_entry()运行闪灯
    PT_SEM_WAIT(pt,&sem_LED);
     for (i=0;i<5;i++)
    {
      digitalWrite(13,HIGH);
      PT_TIMER_DELAY(pt,200);
      digitalWrite(13,LOW);
      PT_TIMER_DELAY(pt,200);
    }
    while (Serial.available())
      Serial.read();
    PT_SEM_SIGNAL(pt,&sem_LED);
  }
  PT_END(pt);
}

void loop() {
  //Check each thread by priority
  thread1_entry(&thread1);
  thread2_entry(&thread2);
}
