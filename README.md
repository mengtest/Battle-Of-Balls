# Battle-Of-Balls
#### C++大项目设计
******
## 参与人员及分工

 - **YXY**

负责游戏界面（加载、菜单、设置）的设计和实现

负责音乐设计

工作量：25%  


- **LM**

负责游戏实体功能的实现

工作量：25%


- **PX**

负责游戏场景的功能实现

负责地图设计

工作量：30%

- **PSY**

负责网络通信的实现（未达成）

部分界面视觉效果设计

工作量：20%

## 实现的功能


- **基础功能** 

- [x] 支持图形显示

- [x] 支持鼠标和键盘操作

- [x] 支持agar.io中的吞噬操作

- [x] 支持agar.io中的分裂操作

- [x] 支持动画

- [ ] 实现服务器端：支持局域网联机对战（自由模式），支持多人在同一个地图上游戏

- **拓展功能**

- [x] 支持绿刺

- [x] 支持吐孢子

- [x] 支持玩家合并

- [x] 支持背景音乐


## 用到的C++特性

- [x] 初始化列表

- [x] 类型推断

- [x] 基于范围的for循环

- [x] 智能指针

- [x] STL容器，如vector、map

- [x] 迭代器

- [x] 指针空值nullptr


## 游戏系统介绍

#### 场景
- 进入场景（EnterScene）：
- - 图片淡入淡出动作
- - 1、加载资源
    这里放图！！！！！！！！！
- - 2、实现细节
``` C++
auto fadeIn = FadeIn::create(0.5);   //创建一个FadeIn动作，执行该动作的节点在0.5秒内淡入
auto fadeOut = FadeOut::create(1.0);  //创建一个FadeIn动作，执行该动作的节点在0.5秒内淡出
auto callFunc = CallFunc::create(CC_CALLBACK_0(EnterScene::startLoding, this));
auto seq = Sequence::create(fadeIn, fadeOut, callFunc, NULL);  //创建一个顺序动作
```
- - 加载资源



 


