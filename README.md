# wxbot

提供HTTP API操作电脑微信:

* 发送文本消息
* 发起语音通话
* 发起视频通话


# 用法

将目标联系人在独立窗口中打开, ~并置顶窗口~, 然后启动本程序

API:
* 发送文本消息
```
GET /?passwd=test&msg={msg}
msg需要使用Base64编码, 再使用URI编码
```
* 发起语音通话
```
GET /?passwd=test&audio=
```
* 发起视频通话
```
GET /?passwd=test&video=
```


# 使远程会话保持登录状态

使用远程桌面连接到远程计算机时，关闭远程桌面会锁定计算机并显示登录屏幕。在锁定模式下，计算机没有 GUI，因此任何当前运行或计划的 GUI 测试都将失败。

为避免 GUI 测试出现问题，请使用tscon实用程序断开与远程桌面的连接。tscon将控制权返回到远程计算机上的原始本地会话，绕过登录屏幕。远程计算机上的所有程序继续正常运行，包括 GUI 测试。

```
for /f "skip=1 tokens=3" %%s in ('query user %USERNAME%') do (
  %windir%\System32\tscon.exe %%s /dest:console
)
```
