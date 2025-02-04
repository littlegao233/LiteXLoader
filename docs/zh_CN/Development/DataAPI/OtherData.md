## 🧰 其他数据处理 API

提供一些其他的常用数据处理接口。你可以自行使用并拓展。

### 变量转换为Json字符串

`data.toJson(var[,space])`

- 参数：
  - var : `指定类型`  
    要转换为Json字符串的变量。允许进行转换的数据类型有：   
    `Integer` `Float` `String` `Boolean` `Array` `Object `  
    其中，`Array` 和 `Object` 内部仅能嵌套上面出现的这些元素
  - space : `Integer`  
    （可选参数）如果要格式化输出的字符串，则传入此参数  
    代表每个缩进的空格数量，这样输出的Json串更适合人阅读  
    此参数默认为0，即不对输出字符串进行格式化
- 返回值：转换成的Json字符串
- 返回值类型：`String`
  - 如果返回值为`Null`，则表示转换失败

<br>

### Json字符串解析为变量

`data.parseJson(json)`

- 参数：
  - json : `String`  
    要转换为变量的Json字符串
- 返回值：转换成的变量
- 返回值类型：`任意类型`，以Json具体包含的数据类型为准
  - 如返回值为 `Null` 则表示转换失败

<br>