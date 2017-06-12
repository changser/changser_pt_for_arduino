changer_pt_for_arduino
V20170424   changser收集整理  changser@139.com
V20170611   加入pt-signal.h，完善文字说明

1、关于协程protothreads
   由Adam Dunkels建立，详细介绍见网址      http://dunkels.com/adam/pt/
   最后版本是1.4，下载页面                 http://dunkels.com/adam/pt/download.html
   下下来的压缩文件为pt-1.4.tar.gz。专门存于pt-1.4子目录中。
    个人理解其功能和特点是：
	a、能让人像os编程风格一样编程，分成多个任务，流程清晰明了；
	b、没有太多的花销，特别是RAM的花销。原始PT每个任务只需2字节RAM，用于存储任务恢复时程序地址，
	   相当于OS的TCB。若使用pt-timer.h和pt-signal，则分别加4字节和1字节。
	C、代码非常简单，只是一些宏而已。
	D、毕竟不是OS，不是时间片，不是抢占式，是由用户程序协作出让控制权。名字叫协程。
	   如果要用于实时场合，需好好规划。
	
	
2、主要文件
     lc-addrlabels.h        用GCC语法扩展实现的协程基础
     lc-switch.h            用标准C的switch语句实现的协程基础(默认)
     lc.h                   该文件存在的意义仅仅为了选择以上两者之一
     pt.h                   在实际应用中一般只包含此文件就行了
     pt-sem.h               协程间通信（信号量）的实现，不需要的话则不必包含它
	                        如果只是为了方便转移任务间的控制权，建议用pt-signal.h
	
   上述文件是适用于任何平台(相对于pt-timer.h适用于ARDUINO而言)。
   确切地说，lc-switch方法适用任何平台，但有使用方法上的限制。见节5注意事项
   lc-addrlabels方法适用gcc，因为GCC扩展了语法，它支持标签指针。
   另外增加下面两个文件，应用时会方便许多:
   pt-timer.h               等定时，等待期间控制权转移。
   pt-signal.h              等信号标志，等待期间控制权转移，发信号时控制权不转移。
   
3、任务转移控制权，PT本身提供了无条件转移（PT_YIELD(pt)）以及有条件转移如PT_WAIT_UNTIL等。
    有条件转移的话，需要借助全局变量来实现，不够直观。一般一个任务需等待两种事件的产生：
	一是等待一定时间。用delay()是不合适的，因为它是原地转圈，占用CPU时间，这期间什么也干不成。
	所以将某网友的pt-titer.h加入。
	二是为了便于任务间的配合，等某事件发生（发生后用标志来表达）。虽然pt-sem.h也能实现，
	但需任务外定义全局变量，不够直观。推存用我提供的pt-signal.h
	无论是哪种，其本质都是用的PT的有条件转移控制权。但封装成了等时间和等信号标志，
	任务流程上更清晰。带来的唯一缺点就是，每个协程任务pt型结构指针，增加4字节用于记录时间；
	增加1字节用于记录信号标志。再也不需要外部定义变量。PT出现得较早，当时RAM空间小，
	但是现在，增加5个字节，每个任务占用7个字节，对主流单片机来说，完全没问题。

3.1   pt-timer.h      
   利用ARDUINO 的millis()和micros()来实现延时判断，所以只能用于arduino .
   查找源头，作者应该是“逍遥猪葛亮”，见 http://www.arduino.cn/thread-5833-1-1.html
   注意，为了实现pt-timer.h的功能，需将pt.h进行改动，原来开头为：
#ifndef __PT_H__
#define __PT_H__

#include "lc.h"

struct pt {
  lc_t lc;
};
#endif   

   改为：
 #ifndef __PT_H__
#define __PT_H__

#include "lc.h"
  
#ifdef PT_USE_TIMER
#include "pt-timer.h"
#endif

#ifdef PT_USE_SEM
#include "pt-sem.h"
#endif

struct pt {
  lc_t lc;
#ifdef PT_USE_TIMER
	pt_timer t;
#endif
};
   也就是说，若要使用信号量和延时，在#include "pt.h"之前，加入
	#define PT_USE_TIMER
	#define PT_USE_SEM
	不用包含pt-sem.h和pt-timer.h
	
   需注意的是，相当于任务TCB的结构体，已经不是两字节了，而是加入了pt_timer t，实际上是ul型的，4字节。
   那么一共是6字节了。
	
   那么：若micros()超过最大值怎么办？见《如何处理arduino micros()函数的微秒溢出》 http://www.985z.com/?post=25

3.1.1   还有人用类库的方法实现，见库里额外的一个目录：PT_timer，不过分析程序也是运行于ARDUINO的。
   查找源头，“By Malc”，能查到的使用记录，是小五义的BLOG，《利用 ProtoThreads实现Arduino多线程处理（2）》
   http://www.cnblogs.com/xiaowuyi/p/4355161.html
   库里额外加了一个目录，pt_timer，是一个按注2方法实现的C++类库。
3.1.2   另有人，用C函数，没有封装成类，如：《在protothreads中实现延时函数》
      http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=24129635&id=113110
3.1.3   最终我们还是选用第一种，与PT的风格一致。	
    不过，我修改了pt-timer.h，在头部加入了下列两句：
    #define PT_WAIT_MS(pt,time)		PT_TIMER_DELAY(pt,time) 
    #define PT_WAIT_US(pt,time)		PT_TIMER_MICRODELAY(pt,time)
	与个人的风格更一致。
	
   上述种种，都指明了一个非ARDUINO平台的通用做法：搞一个定时中断，像ARDUINO里实现million()和micros()
  
3.2 pt-signal.h
     用于任务间配合下转移控制权。由我--changser@139.com建立
     pt.h开头最终改为：
#ifndef __PT_H__
#define __PT_H__

#include "lc.h"

#ifdef PT_USE_SIGNAL
#include "pt-signal.h"
#endif


#ifdef PT_USE_TIMER
#include "pt-timer.h"
#endif

#ifdef PT_USE_SEM
#include "pt-sem.h"
#endif

struct pt {
  lc_t lc;
#ifdef PT_USE_TIMER
	pt_timer t;
#endif
#ifdef PT_USE_SIGNAL
	unsigned char signal;
#endif
};

   若要使用pt-signal.h，在#include "pt.h"之前，加入
	#define PT_USE_SIGNAL
	不用包含pt-signal.h,会自动包含。
  注意，相当于任务TCB的结构体，又增加了一字节。如果pt-timer.h和pt-signal.h同时使用，那么一共是7字节了。
  
4、使用方法总结：
4.1、若基于“switch-case”技巧（缺省）来实现，只需lc.h、lc-switch.h、pt.h三个文件。
4.1.1、将它们拷到项目文件夹；或者，如果在arduino平台下，当作新增库文件处理（参见相关文档，
最简单的是，将整个解压缩后的目录changer_pt _for_arduino，拷贝到文件夹C:\Users\ch\Documents\Arduino\libraries，（注意：WIN7系统）；
4.1.2、项目文件中#include "pt.h" 
4.2、若基于gcc的标签指针，只需 lc.h、lc-addrlabel.h、pt.h三个文件。
     用户程序在#include "pt.h"之前，加入 #define lc_include "lc-addrlabels.h"
4.3、若需用信号量，则pt-sem.h有用。项目文件中不需包含，只需在包含pt.h之前加入 #define PT_USE_SEM 
4.4、若需用等延时，则pt-timer有用。项目文件中不需包含，只需在包含pt.h之前加入 #define PT_USE_TIMER
4.5、若需用等信号标志，则pt-signal有用。项目文件中不需包含，只需在包含pt.h之前加入 #define PT_USE_SIGNAL
     也就是说，上述所有配置，都不必更改库里任何文件，只需在用户程序中定义就行了。
	 
5、注意事项：
5.1、由于协程是stackless的，尽量不要使用局部变量，除非该变量对于协程状态是无关紧要的，同理可推，协程所在的代码是不可重入的。
      个人理解，用lc.h中的原语实现的协程是不可重入的。但是，PT.H中的，因为每个协程有一个指针参数，所以可以重入，将需重入的变量
加入结构体就可以。
5.2、如果协程使用 switch-case 原语封装的组件，那么禁止在实际应用中使用 switch-case 语句，除非用 GNU C 语法中的标签指针替代。
5.3、一个协程内部可以调用其它例程，比如库函数或系统调用，但必须保证该例程是非阻塞的，否则所在线程内的所有协程都将被阻塞。毕竟线程才是执行的最小单位，协程不过是按“时间片轮度”的例程而已。

6、我的一些想法：
6.1、定义协程任务函数的方法
    PT_THREAD(producer(struct pt *pt)){}
	<==>char producer(struct pt *pt){}，不过是为了便于识别是协程任务函数
    原语#define PT_THREAD(name_args) char name_args
6.2、简化
    见朱工 《Protothreads的简化-- 微控制器中基于协程的实时协作多任务方法 (1)》
	http://blog.chinaaet.com/fy_zhu/p/31842
6.3、时间触发模式。见闫石 马潮《时间触发模式下的protothreads设计应用》
       https://wenku.baidu.com/view/f260030103d8ce2f006623e9.html
6.4、Adam Dunkels的多个应用都包含了protothreads，其中Contiki系统含有时间系统的，看了下，感觉很复杂。
     见：《Contiki学习笔记3：定时器》  http://blog.csdn.net/zhou307/article/details/51484846
6.5、中断中哪些能用哪些不能用？
6.6、何时用PT_INIT(&pt)的形式，
     何时用PT_WAIT_SIGNAL(PT)的形式？关于指针，还要再学习一下。
	 PT_INIT(pt)宏如下：
	 #define PT_INIT(pt)   LC_INIT((pt)->lc)
	 参数应是用于保存跳转地址和用户用据的pt型结构体。但用了->，表明是指针型传递，所以需将结构体变量pt（不是结构类型pt）的地址给指针。
	 所以应用时需以PT_INIT(&pt)格式。
	 而

7、主要语句   
7.1  pt定义（不含pt-sem.h）
首先，任务应用函数static PT_THREAD(handle_dhcp(void))，这个函数年看起来有点怪，但其实将其宏展开就是static char handle_dhcp(void)，返回char类型静态函数，其实就是向外部返回任务执行状态。
其它的宏定义：
PT_INIT(pt)   初始化任务变量，只在初始化函数中执行一次就行
PT_BEGIN(pt)   启动任务处理，放在函数开始处
PT_END(pt)   结束任务，放在函数的最后
PT_WAIT_UNTIL(pt, condition) 等待某个条件（条件可以为时钟或其它变量，IO等）成立，否则直接退出本函数，下一次进入本            函数就直接跳到这个地方判断
PT_WAIT_WHILE(pt, cond)  和上面一个一样，只是条件取反了
PT_WAIT_THREAD(pt, thread) 等待一个子任务执行完成
PT_SPAWN(pt, child, thread) 新建一个子任务，并等待其执行完退出
PT_RESTART(pt)   重新启动某个任务执行
PT_EXIT(pt)   任务后面的部分不执行，直接退出重新执行
PT_YIELD(pt)   锁死任务
PT_YIELD_UNTIL(pt, cond) 锁死任务并在等待条件成立，恢复执行
在pt中一共定义四种线程状态，在任务函数退出到上一级函数时返回其状态
PT_WAITING  等待
PT_EXITED   退出
PT_ENDED    结束
PT_YIELDED  锁死

7.2 pt-timer.h定义
  PT_TIMER_DELAY(pt,time)  		pt所在任务等time时间，单位ms，转移控制权
  PT_TIMER_MICRODELAY(pt,time)	pt所在任务等time时间，单位us，转移控制权
  
7.3 pt-signal.h定义 
 PT_WAIT_SIGNAL(pt)             pt所在任务等信号标志，转移控制权
 PT_SEND_SIGNAL(pt)             置位pt所在任务的信号信号标志，一般在pt以外的任务中进行。不转移控制权
 
8、参考文档
   protothreads作者网址 http://dunkels.com/adam/
   朱工的微博 http://blog.chinaaet.com/fy_zhu/c/1234
  《一个“蝇量级” C 语言协程库》
  http://coolshell.cn/articles/10975.html
  《FreeRTOS基础以及UIP之协程--C语言剑走偏锋》
  http://www.cnblogs.com/dengxiaojun/p/4385357.html
  《ProtoThreads - Producer&Consumer》
  http://blog.csdn.net/keamoulin/article/details/3311013
  《ProtothreadS学习资料整理》 --没找到出处
  《Arduino教程 ProtoThreads在Arduino中的应用》
  http://www.arduino.cn/thread-5833-1-1.html
  《轻量级线程库 protothreads》 周华军
  《最轻量级的C协程库：Protothreads》
  http://blog.csdn.net/weiwangchao_/article/details/7777385
  
  