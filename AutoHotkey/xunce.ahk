/*
*1.shutdown -s -t 3600    1Сʱ��ػ� shutdown  -a  ȡ��
*2.������ĵ���Ҫ��22:00�ػ�������ѡ�񡰿�ʼ�����С������롰at 22:00 shutdown -s��������*  ����22�����*   �ͻ���֡�ϵͳ�ػ����Ի���Ĭ����30���ӵĵ���ʱ����ʾ�㱣�湤��.
*
*/


/*ϵͳĬ�ϵ� Win ��ݼ���
* 
*  1.  Win + E������Դ��������
*  2.  Win + D����ʾ���棻
*  3.  Win + F���򿪲��ҶԻ���
*  4.  Win + R�������жԻ���
*  5.  Win + L���������ԣ�
*  6.  Win + PauseBreak����ϵͳ���ԶԻ���
*
*/
/* ��ô Ctrl��Alt��Shift �� AHK ��ô��ʾ
*
*  1.Ctrl  �ķ�����  ^
*  2.Alt   �ķ�����  !
*  3.Shift �ķ�����  +
*  4.Win   �ķ�����  #
*/

; Insert the current date time when I tell her "Ctrl+T"
^T::
    FormatTime, CurrentDateTime, , // yyyy-MM-dd HH:mm:ss+08:00
    Clipboard := CurrentDateTime
    SendInput ^v{Return} ; It seems that "^V{Return}" not works as expected.
Return


/*����cmd
*/
^!t::run   D:\Portable\cmder\Cmder.exe
^!e::run   D:\sqtools\cmde



/*���н�ͼ����
*/
#a::run   C:\windows\system32\SnippingTool.exe


/*����visios
*/
#v::Run  C:\Program Files\Microsoft Office\root\Office16\VISIO.EXE

/* notepad++ 
*/
#n::run   D:\Portable\Notepad++7.5\Notepadplus.exe


/* eclipse java  ���磺 D:\eclipse\eclipse.exe -data  E:\workspace
*/
#j::run   D:\eclipse\eclipse.exe 

/*  ����CodeBlocks  
*/
#c::Run   D:\Program Files (x86)\CodeBlocks\codeblocks.exe

/*  ����QQ 
*/
#q::Run   D:\Program Files (x86)\Tencent\QQ\Bin\QQ.exe


/*  ���ҵ�csdn���� 
*/
#y::Run  http://blog.csdn.net/sunquana



/*  ��  google 
*/
#g::Run https://www.google.com
/*  ��  �ٶ� 
*/
#b::Run https://www.baidu.com

/*  �򿪶���  
*/
#s::Run https://www.douban.com/people/SQ_SZY/

/*  �򿪼��� 
*/
#i::Run http://www.jianshu.com







