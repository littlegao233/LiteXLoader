## 📦 方块相关事件

#### `"onBlockInteracted"` - 方块接受玩家互动

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    与方块互动的玩家对象

  - block : `Block`  
    被互动的方块对象

- 拦截事件：函数返回`false`

只有可以被互动的方块才会触发此事件，如木桶、信标、制图台、磨石等  
拦截事件对箱子、潜影盒、工作台无效

<br>

#### `"onBlockExploded"` - 方块被爆炸破坏

- 监听函数原型
  `function(block,source)`
- 参数：
  - block : `Block`  
    被爆炸破坏的方块对象
  - source : `Entity`  
    爆炸来源的实体对象
- 拦截事件：不可以拦截

<br>

#### `"onCmdBlockExecute"` - 命令方块执行命令

- 监听函数原型
  `function(cmd,pos)`
- 参数：
  - cmd : `String`  
    执行的命令

  - pos : `IntPos`  
    执行命令的命令方块坐标

- 拦截事件：函数返回`false`

<br>

#### `"onContainerChange"` - 容器内容改变

- 监听函数原型
  `function(player,container,slotNum,oldItem,newItem)`
- 参数：
  - player : `Player`  
    操作容器的玩家对象
  - container : `Block`  
    被操作的容器的方块对象
  - slotNum : `Integer`  
    操作容器的格子位置（第slotNum个格子）
  - oldItem : `Item`  
    格子中的原来旧物品对象
  - newItem : `Item`  
    格子中新的物品对象
- 拦截事件：不可以拦截

此处的 **容器** 为宽泛容器的概念，包括箱子、桶等多种可以储存物品的容器都可以触发此事件

对回调参数的解释：  
旧物品对象与新物品对象有多种不同的组合情况，表示格子内不同的变化情况

- 放入物品：旧物品对象为空，新物品对象不为空
- 取出物品：旧物品对象不为空，新物品对象不为空
- 物品增加堆叠：旧物品对象的`type` == 新物品对象的`type`，且旧物品对象的`count` < 新物品对象的`count`
- 物品减少堆叠：旧物品对象的`type` == 新物品对象的`type`，且旧物品对象的`count` > 新物品对象的`count`
- 替换物品：旧物品对象的`type` 不等于 新物品对象的`type`，且两物品对象均不为空

注意：当某个容器被打开时，物品栏的每一格都会先触发一次这个事件

<br>

#### `"onProjectileHitBlock"` - 方块被弹射物击中

- 监听函数原型
  `function(block,pos,source)`
- 参数：
  - block : `Block`  
    被击中的方块对象
  - source : `Entity`  
    发射的弹射物实体（如箭）
- 拦截事件：不可以拦截

<br>

#### `"onRedStoneUpdate"` - 发生红石更新

- 监听函数原型
  `function(block,level,isActive)`
- 参数：
  - block : `Block`  
    发生红石更新的方块对象
  - level : `Integer`  
    更新的红石能量等级（0-15）
  - isActive : `Boolean`  
    表示红石更新是激活还是熄灭  
    - 为`true`表示红石变为激活状态
    - 为`false`表示红石变为熄灭状态
- 拦截事件：函数返回`false`

目前可以监测红石更新的方块种类有：红石线、红石火把、红石中继器、红石比较器

<br>

#### `"onHopperSearchItem"` - 漏斗（漏斗矿车）检测可否吸取物品

- 监听函数原型
  `function(pos)`
- 参数：
  - pos : `FloatPos`  
    漏斗（漏斗矿车）所在的位置
- 拦截事件：函数返回`false`

注：在放置漏斗之后，会在服务端反复不断触发这个事件  
当你拦截事件之后，漏斗就会无法吸取这个物品

<br>

#### `"onHopperPushOut"` - 漏斗（漏斗矿车）输出物品

- 监听函数原型
  `function(pos)`
- 参数：
  - pos : `FloatPos`  
    漏斗（漏斗矿车）所在的位置
- 拦截事件：函数返回`false`

<br>

#### `"onPistonPush"` - 活塞推动

- 监听函数原型
  `function(pistonPos,block)`
- 参数：
  - pistonPos : `IntPos`  
    正在工作的活塞坐标
  - block : `Block`  
    被推动的方块对象
- 拦截事件：函数返回`false`

<br>

#### `"onFarmLandDecay"` - 耕地退化

- 监听函数原型
  `function(pos,entity)`
- 参数：
  - pos : `IntPos`  
    退化的耕地的坐标
  - entity : `Entity`  
    造成耕地退化的实体
- 拦截事件：函数返回`false`

<br>

#### "onUseFrameBlock" - 操作物品展示框

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    操作物品展示框的玩家对象
  - block : `Block`  
    被操作的物品展示框方块对象
- 拦截事件：函数返回`false`

注：操作包括放置物品、取下物品、旋转物品。