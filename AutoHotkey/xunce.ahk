/*
*1.shutdown -s -t 3600    1小时后关机 shutdown  -a  取消
*2.比如你的电脑要在22:00关机，可以选择“开始→运行”，输入“at 22:00 shutdown -s”，这样*  到了22点电脑*   就会出现“系统关机”对话框，默认有30秒钟的倒计时并提示你保存工作.
*
*/


/*系统默认的 Win 快捷键：
* 
*  1.  Win + E：打开资源管理器；
*  2.  Win + D：显示桌面；
*  3.  Win + F：打开查找对话框；
*  4.  Win + R：打开运行对话框
*  5.  Win + L：锁定电脑；
*  6.  Win + PauseBreak：打开系统属性对话框；
*
*/
/* 那么 Ctrl、Alt、Shift 在 AHK 怎么表示
*
*  1.Ctrl  的符号是  ^
*  2.Alt   的符号是  !
*  3.Shift 的符号是  +
*  4.Win   的符号是  #
*/

; Insert the current date time when I tell her "Ctrl+T"
^T::
    FormatTime, CurrentDateTime, , // yyyy-MM-dd HH:mm:ss+08:00
    Clipboard := CurrentDateTime
    SendInput ^v{Return} ; It seems that "^V{Return}" not works as expected.
Return


/*运行cmd
*/
^!t::run   D:\Portable\cmder\Cmder.exe
^!e::run   D:\sqtools\cmde



/*运行截图程序
*/
#a::run   C:\windows\system32\SnippingTool.exe


/*运行visios
*/
#v::Run  C:\Program Files\Microsoft Office\root\Office16\VISIO.EXE

/* notepad++ 
*/
#n::run   D:\Portable\Notepad++7.5\Notepadplus.exe


/* eclipse java  比如： D:\eclipse\eclipse.exe -data  E:\workspace
*/
#j::run   D:\eclipse\eclipse.exe 

/*  运行CodeBlocks  
*/
#c::Run   D:\Program Files (x86)\CodeBlocks\codeblocks.exe

/*  运行QQ 
*/
#q::Run   D:\Program Files (x86)\Tencent\QQ\Bin\QQ.exe


/*  打开我的csdn博客 
*/
#y::Run  http://blog.csdn.net/sunquana



/*  打开  google 
*/
#g::Run https://www.google.com
/*  打开  百度 
*/
#b::Run https://www.baidu.com

/*  打开豆瓣  
*/
#s::Run https://www.douban.com/people/SQ_SZY/

/*  打开简书 
*/
#i::Run http://www.jianshu.com







