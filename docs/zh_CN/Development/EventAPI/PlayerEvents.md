## 📜 监听事件列表

这里给出了LXL支持监听的各种事件的列表。

提示：你可以根据监听得到的游戏对象来获取他们的相关信息，比如说方块的坐标、实体的名字等等。  
同时，这些对象的成员函数也都可以被调用。

> 接口设计哲学：只要可以提供完善的对象，就进行封装，而非将事件内容作为参数单独传输 

## 🏃‍♂️ 玩家相关事件

#### `"onPreJoin"` - 玩家开始连接服务器

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    正在连接服务器的玩家对象
- 拦截事件：不可以拦截一些

注：在这个监听函数中只能获取一些玩家的基础信息，比如名字、IP、xuid等。因为此时玩家尚未完全进服

<br>

#### `"onJoin"` - 玩家进入游戏（加载世界完成）

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    进入游戏的玩家对象
- 拦截事件：不可以拦截

<br>

#### `"onLeft"` - 玩家离开游戏

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    离开游戏的玩家对象

- 拦截事件：不可以拦截

<br>

#### `"onRespawn"` - 玩家重生

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    重生的玩家对象
- 拦截事件：不可以拦截

<br>

#### `"onPlayerDie"` - 玩家死亡

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    死亡的玩家对象

- 拦截事件：不可以拦截

<br>

#### `"onPlayerCmd"` - 玩家执行命令

- 监听函数原型
  `function(player,cmd)`
- 参数：
  - player : `Player`  
    执行命令的玩家对象

  - cmd : `String`  
    执行的命令

- 拦截事件：函数返回`false`

<br>

#### `"onChat"` - 玩家发送聊天信息

- 监听函数原型
  `function(player,msg)`
- 参数：
  - player : `Player`  
    发送聊天信息的玩家对象

  - msg : `String`  
    发送的聊天消息

- 拦截事件：函数返回`false`

<br>

#### `"onChangeDim"` - 玩家切换维度

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    切换维度的玩家对象
- 拦截事件：不可以拦截

注：在切换维度的过程中，会在服务端反复多次激发这个事件

<br>

#### `"onJump"` - 玩家跳跃

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    跳跃的玩家对象

- 拦截事件：不可以拦截

<br>

#### `"onSneak"` - 玩家切换潜行状态

- 监听函数原型
  `function(player,isSneaking)`
- 参数：
  - player : `Player`  
    切换潜行状态的玩家对象
  - isSneaking : `Boolean`  
    `True`表示玩家进入潜行状态，`False`表示玩家退出潜行状态

- 拦截事件：不可以拦截

<br>

#### `"onAttack"` - 玩家攻击实体

- 监听函数原型
  `function(player,entity)`
- 参数：
  - player : `Player`  
    攻击实体的玩家对象

  - entity : `Entity`  
    被攻击的实体对象

- 拦截事件：函数返回`false`

<br>

#### `"onUseItem"` - 玩家使用物品 

- 监听函数原型
  `function(player,item)`
- 参数：
  - player : `Player`  
    使用物品的玩家对象
  - item : `Item`  
    被使用的物品对象
- 拦截事件：函数返回`false`

<br>

#### `"onUseItemOn"` - 玩家对方块使用物品（点击右键）

- 监听函数原型
  `function(player,item,block)`
- 参数：
  - player : `Player`  
    使用物品的玩家对象
  - item : `Item`  
    被使用的物品对象
  - block : `Block`  
    被点击到的方块对象
- 拦截事件：函数返回`false`

注：Win10客户端玩家右键会在服务端连续多次激发这个事件

<br>

#### `"onTakeItem"` - 玩家捡起物品

- 监听函数原型
  `function(player,entity)`
- 参数：

  - player : `Player`  
    捡起物品的玩家对象

  - entity: `Entity`  
    被捡起的物品的掉落物实体

- 拦截事件：函数返回`false`

<br>

#### `"onDropItem"` - 玩家丢出物品

- 监听函数原型
  `function(player,item)`
- 参数：
  - player : `Player`  
    丢出物品的玩家对象

  - item : `Item`  
    被丢出的物品对象

- 拦截事件：函数返回`false`

<br>

#### `"onEat"` - 玩家食用食物

- 监听函数原型
  `function(player,item)`
- 参数：
  - player : `Player`  
    正在吃的玩家对象
  - item : `Item`  
    被吃的物品对象

- 拦截事件：不可以拦截

<br>

#### `"onStartDestroyBlock"` - 玩家开始破坏方块  / 点击左键

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    正在破坏方块的玩家对象

  - block : `Block`  
    正在被破坏的方块对象

- 拦截事件：不可以拦截

<br>

#### `"onDestroyBlock"` - 玩家破坏方块完成

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    破坏方块的玩家对象

  - block : `Block`  
    被破坏的方块对象

- 拦截事件：函数返回`false`

<br>

#### `"onPlaceBlock"` - 玩家放置方块

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    放置方块的玩家对象

  - block : `Block`  
    被放置的方块对象

- 拦截事件：函数返回`false`

<br>

#### `"onOpenContainer"` - 玩家打开容器方块

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    打开容器方块的玩家对象

  - block : `Block`  
    被打开的容器方块对象
- 拦截事件：函数返回`false`

目前已支持监听的容器有：箱子（`minecraft:chest`）、木桶（`minecraft:barrel`）

<br>

#### `"onCloseContainer"` - 玩家关闭容器方块

- 监听函数原型
  `function(player,block)`
- 参数：
  - player : `Player`  
    关闭容器方块的玩家对象

  - block : `Block`  
    被关闭的容器方块对象
- 拦截事件：函数返回`false`

目前已支持监听的容器有：箱子（`minecraft:chest`）、木桶（`minecraft:barrel`）

<br>

#### `"onInventoryChange"` - 玩家物品栏变化

- 监听函数原型
  `function(player,slotNum,oldItem,newItem)`
- 参数：
  - player : `Player`  
    操作物品栏的玩家对象
  - slotNum : `Integer`  
    操作物品栏的格子位置（第slotNum个格子）
  - oldItem : `Item`  
    格子中的原来旧物品对象
  - newItem : `Item`  
    格子中新的物品对象
- 拦截事件：不可以拦截

对回调参数的解释：  
旧物品对象与新物品对象有多种不同的组合情况，表示格子内不同的变化情况

- 放入物品：旧物品对象为空，新物品对象不为空
- 取出物品：旧物品对象不为空，新物品对象不为空
- 物品增加堆叠：旧物品对象的`type` == 新物品对象的`type`，且旧物品对象的`count` < 新物品对象的`count`
- 物品减少堆叠：旧物品对象的`type` == 新物品对象的`type`，且旧物品对象的`count` > 新物品对象的`count`
- 替换物品：旧物品对象的`type` 不等于 新物品对象的`type`，且两物品对象均不为空

注意：当某个玩家进服时，在进服前的某个时刻，物品栏的每一格都会先触发一次这个事件

<br>

#### `"onMove"` - 玩家移动

- 监听函数原型
  `function(player,pos)`
- 参数：
  - player : `Player`  
    正在移动的玩家对象
  - pos : `FloatPos`  
    这个玩家当前的位置
- 拦截事件：不可以拦截

注：在玩家移动的过程中，会在服务端反复不断触发这个事件

<br>

#### `"onSetArmor"` - 玩家改变盔甲栏

- 监听函数原型
  `function(player,slotNum,item)`
- 参数：
  - player : `Player`  
    改变盔甲栏的玩家对象
  - slotNum : `Integer`  
    盔甲栏序号，范围0-3
  - item : `Item`  
    盔甲栏中的物品对象
- 拦截事件：不可以拦截

注意：当某个玩家进服时，在进服前的某个时刻，物品栏的每一格都会先触发一次这个事件

<br>

#### `"onUseRespawnAnchor"` - 玩家使用重生锚

- 监听函数原型
  `function(player,pos)`
- 参数：
  - player : `Player`  
    使用重生锚的玩家指针
  - pos : `IntPos`  
    被使用的重生锚的位置
- 拦截事件：函数返回`false`

<br>

#### "onFishingHookRetrieve" - 玩家收鱼钩

- 监听函数原型
  `function(player,fishingHook)`
- 参数：
  - player : `Player`  
    收起该鱼钩的玩家对象
  - fishingHook : `Entity`  
    被收起的鱼钩的实体对象
- 拦截事件：函数返回`false`

注：拦截此事件的实际效果是鱼钩勾不起任何东西，但客户端依然有假收勾的场景。

<br>

#### "onOpenContainerScreen" - 玩家打开容器类GUI

- 监听函数原型
  `function(player)`
- 参数：
  - player : `Player`  
    尝试骑乘的玩家对象
- 拦截事件：函数返回`false`

注：此事件非常强力，甚至可以拦截打开背包。





