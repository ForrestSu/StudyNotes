/*☆不知道就来这：------我的知识备忘录
*0.google ip 更新http://laod.cn/
*1.ppt中有一个情景模式提要很多logo
*2.shutdown -s -t 3600    1小时后关机 shutdown  -a  取消
*3.比如你的电脑要在22:00关机，可以选择"开始→运行"，输入"at 22:00 shutdown -s"，
*  这样到了22点电脑, 就会出现“系统关机”对话框，默认有30秒钟的倒计时并提示你保存工作.
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
    FormatTime, CurrentDateTime,, yyyy-MM-dd HH:mm:ss
    Clipboard := CurrentDateTime
    SendInput ^v  ; It seems that "^V{Return}" not works as expected.
Return

^E::
    FormatTime, CurrentDateTime,, yyyy-MM-ddTHH:mm:ss+08:00
    Clipboard := CurrentDateTime
    SendInput ^v  ; It seems that "^V{Return}" not works as expected.
Return

/*运行cmd
*/
^!t::run   D:\Portable\cmder\Cmder.exe


/*运行截图程序
*/
#a::run   C:\windows\system32\SnippingTool.exe


/*运行visio
*/
#v::Run  C:\Program Files\Microsoft Office\root\Office16\VISIO.EXE

/* notepad++
*/
#n::run   D:\Portable\Notepad++v7.7.1\Notepadplus.exe


/* eclipse java  比如： D:\eclipse\eclipse.exe -data  E:\workspace
*/
#j::run   D:\eclipse\eclipse.exe 


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
