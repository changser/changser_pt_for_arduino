changer_pt_for_arduino
V20170424   changser�ռ�����  changser@139.com
V20170611   ����pt-signal.h����������˵��

1������Э��protothreads
   ��Adam Dunkels��������ϸ���ܼ���ַ      http://dunkels.com/adam/pt/
   ���汾��1.4������ҳ��                 http://dunkels.com/adam/pt/download.html
   ��������ѹ���ļ�Ϊpt-1.4.tar.gz��ר�Ŵ���pt-1.4��Ŀ¼�С�
    ��������书�ܺ��ص��ǣ�
	a����������os��̷��һ����̣��ֳɶ�����������������ˣ�
	b��û��̫��Ļ������ر���RAM�Ļ�����ԭʼPTÿ������ֻ��2�ֽ�RAM�����ڴ洢����ָ�ʱ�����ַ��
	   �൱��OS��TCB����ʹ��pt-timer.h��pt-signal����ֱ��4�ֽں�1�ֽڡ�
	C������ǳ��򵥣�ֻ��һЩ����ѡ�
	D���Ͼ�����OS������ʱ��Ƭ��������ռʽ�������û�����Э�����ÿ���Ȩ�����ֽ�Э�̡�
	   ���Ҫ����ʵʱ���ϣ���úù滮��
	
	
2����Ҫ�ļ�
     lc-addrlabels.h        ��GCC�﷨��չʵ�ֵ�Э�̻���
     lc-switch.h            �ñ�׼C��switch���ʵ�ֵ�Э�̻���(Ĭ��)
     lc.h                   ���ļ����ڵ��������Ϊ��ѡ����������֮һ
     pt.h                   ��ʵ��Ӧ����һ��ֻ�������ļ�������
     pt-sem.h               Э�̼�ͨ�ţ��ź�������ʵ�֣�����Ҫ�Ļ��򲻱ذ�����
	                        ���ֻ��Ϊ�˷���ת�������Ŀ���Ȩ��������pt-signal.h
	
   �����ļ����������κ�ƽ̨(�����pt-timer.h������ARDUINO����)��
   ȷ�е�˵��lc-switch���������κ�ƽ̨������ʹ�÷����ϵ����ơ�����5ע������
   lc-addrlabels��������gcc����ΪGCC��չ���﷨����֧�ֱ�ǩָ�롣
   �����������������ļ���Ӧ��ʱ�᷽�����:
   pt-timer.h               �ȶ�ʱ���ȴ��ڼ����Ȩת�ơ�
   pt-signal.h              ���źű�־���ȴ��ڼ����Ȩת�ƣ����ź�ʱ����Ȩ��ת�ơ�
   
3������ת�ƿ���Ȩ��PT�����ṩ��������ת�ƣ�PT_YIELD(pt)���Լ�������ת����PT_WAIT_UNTIL�ȡ�
    ������ת�ƵĻ�����Ҫ����ȫ�ֱ�����ʵ�֣�����ֱ�ۡ�һ��һ��������ȴ������¼��Ĳ�����
	һ�ǵȴ�һ��ʱ�䡣��delay()�ǲ����ʵģ���Ϊ����ԭ��תȦ��ռ��CPUʱ�䣬���ڼ�ʲôҲ�ɲ��ɡ�
	���Խ�ĳ���ѵ�pt-titer.h���롣
	����Ϊ�˱�����������ϣ���ĳ�¼��������������ñ�־��������Ȼpt-sem.hҲ��ʵ�֣�
	���������ⶨ��ȫ�ֱ���������ֱ�ۡ��ƴ������ṩ��pt-signal.h
	���������֣��䱾�ʶ����õ�PT��������ת�ƿ���Ȩ������װ���˵�ʱ��͵��źű�־��
	���������ϸ�������������Ψһȱ����ǣ�ÿ��Э������pt�ͽṹָ�룬����4�ֽ����ڼ�¼ʱ�䣻
	����1�ֽ����ڼ�¼�źű�־����Ҳ����Ҫ�ⲿ���������PT���ֵý��磬��ʱRAM�ռ�С��
	�������ڣ�����5���ֽڣ�ÿ������ռ��7���ֽڣ���������Ƭ����˵����ȫû���⡣

3.1   pt-timer.h      
   ����ARDUINO ��millis()��micros()��ʵ����ʱ�жϣ�����ֻ������arduino .
   ����Դͷ������Ӧ���ǡ���ң����������� http://www.arduino.cn/thread-5833-1-1.html
   ע�⣬Ϊ��ʵ��pt-timer.h�Ĺ��ܣ��轫pt.h���иĶ���ԭ����ͷΪ��
#ifndef __PT_H__
#define __PT_H__

#include "lc.h"

struct pt {
  lc_t lc;
};
#endif   

   ��Ϊ��
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
   Ҳ����˵����Ҫʹ���ź�������ʱ����#include "pt.h"֮ǰ������
	#define PT_USE_TIMER
	#define PT_USE_SEM
	���ð���pt-sem.h��pt-timer.h
	
   ��ע����ǣ��൱������TCB�Ľṹ�壬�Ѿ��������ֽ��ˣ����Ǽ�����pt_timer t��ʵ������ul�͵ģ�4�ֽڡ�
   ��ôһ����6�ֽ��ˡ�
	
   ��ô����micros()�������ֵ��ô�죿������δ���arduino micros()������΢������� http://www.985z.com/?post=25

3.1.1   �����������ķ���ʵ�֣�����������һ��Ŀ¼��PT_timer��������������Ҳ��������ARDUINO�ġ�
   ����Դͷ����By Malc�����ܲ鵽��ʹ�ü�¼����С�����BLOG�������� ProtoThreadsʵ��Arduino���̴߳���2����
   http://www.cnblogs.com/xiaowuyi/p/4355161.html
   ����������һ��Ŀ¼��pt_timer����һ����ע2����ʵ�ֵ�C++��⡣
3.1.2   �����ˣ���C������û�з�װ���࣬�磺����protothreads��ʵ����ʱ������
      http://blog.chinaunix.net/xmlrpc.php?r=blog/article&uid=24129635&id=113110
3.1.3   �������ǻ���ѡ�õ�һ�֣���PT�ķ��һ�¡�	
    ���������޸���pt-timer.h����ͷ���������������䣺
    #define PT_WAIT_MS(pt,time)		PT_TIMER_DELAY(pt,time) 
    #define PT_WAIT_US(pt,time)		PT_TIMER_MICRODELAY(pt,time)
	����˵ķ���һ�¡�
	
   �������֣���ָ����һ����ARDUINOƽ̨��ͨ����������һ����ʱ�жϣ���ARDUINO��ʵ��million()��micros()
  
3.2 pt-signal.h
     ��������������ת�ƿ���Ȩ������--changser@139.com����
     pt.h��ͷ���ո�Ϊ��
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

   ��Ҫʹ��pt-signal.h����#include "pt.h"֮ǰ������
	#define PT_USE_SIGNAL
	���ð���pt-signal.h,���Զ�������
  ע�⣬�൱������TCB�Ľṹ�壬��������һ�ֽڡ����pt-timer.h��pt-signal.hͬʱʹ�ã���ôһ����7�ֽ��ˡ�
  
4��ʹ�÷����ܽ᣺
4.1�������ڡ�switch-case�����ɣ�ȱʡ����ʵ�֣�ֻ��lc.h��lc-switch.h��pt.h�����ļ���
4.1.1�������ǿ�����Ŀ�ļ��У����ߣ������arduinoƽ̨�£������������ļ������μ�����ĵ���
��򵥵��ǣ���������ѹ�����Ŀ¼changer_pt _for_arduino���������ļ���C:\Users\ch\Documents\Arduino\libraries����ע�⣺WIN7ϵͳ����
4.1.2����Ŀ�ļ���#include "pt.h" 
4.2��������gcc�ı�ǩָ�룬ֻ�� lc.h��lc-addrlabel.h��pt.h�����ļ���
     �û�������#include "pt.h"֮ǰ������ #define lc_include "lc-addrlabels.h"
4.3���������ź�������pt-sem.h���á���Ŀ�ļ��в��������ֻ���ڰ���pt.h֮ǰ���� #define PT_USE_SEM 
4.4�������õ���ʱ����pt-timer���á���Ŀ�ļ��в��������ֻ���ڰ���pt.h֮ǰ���� #define PT_USE_TIMER
4.5�������õ��źű�־����pt-signal���á���Ŀ�ļ��в��������ֻ���ڰ���pt.h֮ǰ���� #define PT_USE_SIGNAL
     Ҳ����˵�������������ã������ظ��Ŀ����κ��ļ���ֻ�����û������ж�������ˡ�
	 
5��ע�����
5.1������Э����stackless�ģ�������Ҫʹ�þֲ����������Ǹñ�������Э��״̬���޹ؽ�Ҫ�ģ�ͬ����ƣ�Э�����ڵĴ����ǲ�������ġ�
      ������⣬��lc.h�е�ԭ��ʵ�ֵ�Э���ǲ�������ġ����ǣ�PT.H�еģ���Ϊÿ��Э����һ��ָ����������Կ������룬��������ı���
����ṹ��Ϳ��ԡ�
5.2�����Э��ʹ�� switch-case ԭ���װ���������ô��ֹ��ʵ��Ӧ����ʹ�� switch-case ��䣬������ GNU C �﷨�еı�ǩָ�������
5.3��һ��Э���ڲ����Ե����������̣�����⺯����ϵͳ���ã������뱣֤�������Ƿ������ģ����������߳��ڵ�����Э�̶������������Ͼ��̲߳���ִ�е���С��λ��Э�̲����ǰ���ʱ��Ƭ�ֶȡ������̶��ѡ�

6���ҵ�һЩ�뷨��
6.1������Э���������ķ���
    PT_THREAD(producer(struct pt *pt)){}
	<==>char producer(struct pt *pt){}��������Ϊ�˱���ʶ����Э��������
    ԭ��#define PT_THREAD(name_args) char name_args
6.2����
    ���칤 ��Protothreads�ļ�-- ΢�������л���Э�̵�ʵʱЭ�������񷽷� (1)��
	http://blog.chinaaet.com/fy_zhu/p/31842
6.3��ʱ�䴥��ģʽ������ʯ ����ʱ�䴥��ģʽ�µ�protothreads���Ӧ�á�
       https://wenku.baidu.com/view/f260030103d8ce2f006623e9.html
6.4��Adam Dunkels�Ķ��Ӧ�ö�������protothreads������Contikiϵͳ����ʱ��ϵͳ�ģ������£��о��ܸ��ӡ�
     ������Contikiѧϰ�ʼ�3����ʱ����  http://blog.csdn.net/zhou307/article/details/51484846
6.5���ж�����Щ������Щ�����ã�
6.6����ʱ��PT_INIT(&pt)����ʽ��
     ��ʱ��PT_WAIT_SIGNAL(PT)����ʽ������ָ�룬��Ҫ��ѧϰһ�¡�
	 PT_INIT(pt)�����£�
	 #define PT_INIT(pt)   LC_INIT((pt)->lc)
	 ����Ӧ�����ڱ�����ת��ַ���û��þݵ�pt�ͽṹ�塣������->��������ָ���ʹ��ݣ������轫�ṹ�����pt�����ǽṹ����pt���ĵ�ַ��ָ�롣
	 ����Ӧ��ʱ����PT_INIT(&pt)��ʽ��
	 ��

7����Ҫ���   
7.1  pt���壨����pt-sem.h��
���ȣ�����Ӧ�ú���static PT_THREAD(handle_dhcp(void))����������꿴�����е�֣�����ʵ�����չ������static char handle_dhcp(void)������char���;�̬��������ʵ�������ⲿ��������ִ��״̬��
�����ĺ궨�壺
PT_INIT(pt)   ��ʼ�����������ֻ�ڳ�ʼ��������ִ��һ�ξ���
PT_BEGIN(pt)   �������������ں�����ʼ��
PT_END(pt)   �������񣬷��ں��������
PT_WAIT_UNTIL(pt, condition) �ȴ�ĳ����������������Ϊʱ�ӻ�����������IO�ȣ�����������ֱ���˳�����������һ�ν��뱾            ������ֱ����������ط��ж�
PT_WAIT_WHILE(pt, cond)  ������һ��һ����ֻ������ȡ����
PT_WAIT_THREAD(pt, thread) �ȴ�һ��������ִ�����
PT_SPAWN(pt, child, thread) �½�һ�������񣬲��ȴ���ִ�����˳�
PT_RESTART(pt)   ��������ĳ������ִ��
PT_EXIT(pt)   �������Ĳ��ֲ�ִ�У�ֱ���˳�����ִ��
PT_YIELD(pt)   ��������
PT_YIELD_UNTIL(pt, cond) ���������ڵȴ������������ָ�ִ��
��pt��һ�����������߳�״̬�����������˳�����һ������ʱ������״̬
PT_WAITING  �ȴ�
PT_EXITED   �˳�
PT_ENDED    ����
PT_YIELDED  ����

7.2 pt-timer.h����
  PT_TIMER_DELAY(pt,time)  		pt���������timeʱ�䣬��λms��ת�ƿ���Ȩ
  PT_TIMER_MICRODELAY(pt,time)	pt���������timeʱ�䣬��λus��ת�ƿ���Ȩ
  
7.3 pt-signal.h���� 
 PT_WAIT_SIGNAL(pt)             pt����������źű�־��ת�ƿ���Ȩ
 PT_SEND_SIGNAL(pt)             ��λpt����������ź��źű�־��һ����pt����������н��С���ת�ƿ���Ȩ
 
8���ο��ĵ�
   protothreads������ַ http://dunkels.com/adam/
   �칤��΢�� http://blog.chinaaet.com/fy_zhu/c/1234
  ��һ����Ӭ������ C ����Э�̿⡷
  http://coolshell.cn/articles/10975.html
  ��FreeRTOS�����Լ�UIP֮Э��--C���Խ���ƫ�桷
  http://www.cnblogs.com/dengxiaojun/p/4385357.html
  ��ProtoThreads - Producer&Consumer��
  http://blog.csdn.net/keamoulin/article/details/3311013
  ��ProtothreadSѧϰ�������� --û�ҵ�����
  ��Arduino�̳� ProtoThreads��Arduino�е�Ӧ�á�
  http://www.arduino.cn/thread-5833-1-1.html
  ���������߳̿� protothreads�� �ܻ���
  ������������CЭ�̿⣺Protothreads��
  http://blog.csdn.net/weiwangchao_/article/details/7777385
  
  