## 🔊 其他事件

#### `"onScoreChanged"` - 玩家计分板数值改变

- 监听函数原型
  `function()`
- 参数：（未完成）
  - 
- 拦截事件：不可以拦截

<br>

#### `"onServerStarted"` - 服务器启动完毕

- 监听函数原型
  `function()`
- 参数：
  - 无
- 拦截事件：不可以拦截

<br>

#### `"onServerCmd"` - 服务端执行后台命令

- 监听函数原型
  `function(cmd)`
- 参数：
  - cmd : `String`  
    执行的后台命令

- 拦截事件：函数返回`false`

<br>

#### `"onConsoleOutput"` - 控制台产生命令输出

- 监听函数原型
  `function(cmd)`
- 参数：
  - cmd : `String`  
    输出的命令结果信息

- 拦截事件：函数返回`false`