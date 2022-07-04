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