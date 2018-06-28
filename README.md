# Battle-Of-Balls
#### C++大项目设计
******
## 参与人员及分工

-  **LM**

负责游戏实体功能的实现

工作量：25%

 - **YXY**

负责游戏界面（加载、菜单、设置）的设计和实现

负责音乐设计

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


## 球球

- **属性**
>名字（“Ball”、“AI”）

>位置

>速度

>分数

>半径


- **功能**
>吃: 豆子、孢子、对手

>分裂

>吐孢子



#### 实体类

- **Entity类**: 半径和分数的设置和返回

- **豆子：Bean类**: 创建和初始化豆子

- **刺：Prick类**: 创建和初始化刺

- **孢子：Spore类**: 创建和初始化孢子

- **玩家分身：PlayerDivision类** :分身的创建和初始化


- **玩家操作的球球：Player类**


- **球球实体**：

    - 玩家：贴图

    - AI（对手）：贴图

- 创建时根据keywordID（关键词）的不同区分玩家和AI


- **实现细节**

```
if (keywordID == 1)
{
	_playerName = "Ball";
	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);
}
Else
{
	_playerName = "AI";
	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE * 10);
	_map->addChild(division, PLAYER_INITIAL_SCORE * 10);
}
```

## 球球移动

- **判断**：
玩家根据速度改变每个分身的位置

 AI根据随机数改变每个分身的位置

- **实现细节**

```
if (_keywordID == 1)//玩家
{
	float speed = division->getSpeed();
	if (_state == State::CONCENTRATE)
	{
		speed = PLAYER_CONCENTRATE_SPEED;
	}
	dx = velocity.x*speed;
	dy = velocity.y*speed;
}
else if (_keywordID == 2)//AI随机返回坐标
{
	dx = 0.01*(rand() % 100);
	dy = 0.01*(rand() % 100);
}
else if (_keywordID == 3)//AI随机返回坐标
{
	dx = -0.01*(rand() % 100);
	dy = 0.01*(rand() % 100);
}
Vec2 divisionVec = Vec2(dx, dy); //分身移动方向
Vec2 oldPosition = division->getPosition();
Vec2 newPosition = Vec2(oldPosition.x + divisionVec.x, oldPosition.y + divisionVec.y);
```

## 碰撞检测

- **相关函数**:

根据两者中心点间的距离是否小于两者半径之和判定是否碰撞。

碰到刺就使对象分裂。

碰到豆子就吃掉。

>bool collideBean(Bean * bean);		//与豆子的碰撞检测
>bool collideSpore(Spore * spore);		//与孢子的碰撞检测	
>bool collidePrick(Prick *prick);		//与绿刺的碰撞检测
>bool collideRival(Player *rival);		//与对手的碰撞检测
>int collideDivision(PlayerDivision * division);		//与对手分身的碰撞

## 场景

- **进入场景（EnterScene)**

  1、图片淡入淡出动作
    
    ![] (https://github.com/Psymiao/Battle-Of-Balls/tree/master/截图/1.png)
    
    - 实现细节
    
``` C++
auto fadeIn = FadeIn::create(0.5);   //创建一个FadeIn动作，执行该动作的节点在0.5秒内淡入
auto fadeOut = FadeOut::create(1.0);  //创建一个FadeIn动作，执行该动作的节点在0.5秒内淡出
auto callFunc = CallFunc::create(CC_CALLBACK_0(EnterScene::startLoding, this));
auto seq = Sequence::create(fadeIn, fadeOut, callFunc, NULL);  //创建一个顺序动作
```

   2、资源加载

 ![] (https://github.com/Psymiao/Battle-Of-Balls/tree/master/截图/2.png)

   - 实现细节
   
```
void EnterScene::startLoding()    //设置进度条
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("public/bean.plist", "public/bean.png");  //加载资源
	this->scheduleUpdate();       //开始计时器
void EnterScene::update(float delta)  //更新进度条百分比
}
void EnterScene::startLoding()    //设置进度条
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("public/bean.plist", "public/bean.png");  //加载资源
	this->scheduleUpdate();       //开始计时器
void EnterScene::update(float delta)  //更新进度条百分比
}
```

- **主界面（MenuScene）**

- 开始菜单
贴图

- 实现细节

```
if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
	{
		Auto soundToggleMenuItem=MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingScene::menuSoundToggleCallback, this),
			soundOnMenuItem, soundOffMenuItem, NULL);
		soundToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width / 2, visibleSize.height / 2)));
		auto menu = Menu::create(soundToggleMenuItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 0);
	}
	else
	{
		auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingScene::menuSoundToggleCallback, this),
			soundOffMenuItem, soundOnMenuItem, NULL);
		soundToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width / 2, visibleSize.height / 2)));
		auto menu1 = Menu::create(soundToggleMenuItem, NULL);
		menu1->setPosition(Vec2::ZERO);
		this->addChild(menu1, 0);
	}
```

- **游戏结束界面（GameOverScene）**

- 功能：①显示输赢         ②返回主界面
  贴图
  
- 实现细节

```
MenuItemFont::setFontName("arial");
	MenuItemFont::setFontSize(40);
	MenuItemFont*item = MenuItemFont::create("Game Over! You Lose!");
	Menu*mn = Menu::create(item, NULL);
	mn->alignItemsVertically();
	this->addChild(mn);
```

- **游戏场景（GameScene）**

- 功能：实时显示游戏进程，主要功能在GameLayer里实现
贴图


## 游戏具体场景

- **游戏层**

1.添加精灵，图层，或其他node到子节点上

```
auto colorLayer = LayerColor::create(Color4B(49, 49, 49, 255), DESIGN_SCREEN_WIDTH, DESIGN_SCREEN_HEIGHT);
	this->addChild(colorLayer, GAME_LAYER_BACKGROUND_Z);//添加游戏背景层


	_map = Node::create();//添加地图
	_map->setContentSize(Size(MAP_WIDTH, MAP_HEIGHT));
	this->addChild(_map, GAME_LAYER_MAP_Z);
```
2、设置虚拟摇杆
贴图1 2

```
auto listener = EventListenerTouchOneByOne::create();
bool GameLayer::onTouchBegan(Touch * touch, Event * event)
void GameLayer::onTouchMoved(Touch * touch, Event * event)
void GameLayer::onTouchEnded(Touch * touch, Event * event)
void GameLayer::onTouchCancelled(Touch * touch, Event * event)
```

3、键盘事件

```
auto keyboardListener = EventListenerKeyboard::create();
void GameLayer::keyPressed(EventKeyboard::KeyCode keyCode, Event*event)
void GameLayer::keyReleased(EventKeyboard::KeyCode keyCode, Event*event)
if (keyCode == EventKeyboard::KeyCode::KEY_S)  //S键控制球球吐孢子
	{
		int sporeCount = _player->countSpitSporeNum();
		_player->spitSpore(_map, _sporeMap);
	} 
	else if (keyCode == EventKeyboard::KeyCode::KEY_F)  //F键控制球球分裂
	{
		_player->dividePlayer();
	}
```

4、更新

>void updateView();		//更新游戏视图
>void updateBean();		//更新豆子
>void updateSpore();		//更新孢子
>void updatePrick();		//更新绿刺
>void updateRival();		//更新对手
>void updateScore(float dt)； //更新分数

5、吐孢子&分身

>void spitSpore();		//吐孢子操作
>void dividePlayer();		//分身操作

- **游戏场景(GameScene)**

```
Scene * GameScene::createScene()  //创建场景
{
auto scene = Scene::create();
auto layer = GameScene::create();
scene->addChild(layer);  //添加层到子节点
return scene;
}
```

- **场景管理器（SceneManager）**

```
SceneManager* SceneManager::getInstance()  //创建场景管理器
void SceneManager::changeScene(SceneType enSceneType)  //变换场景auto pDirector = Director::getInstance();
switch (enSceneType)
	{
	case en_EnterScene:
		scene = EnterScene::createScene();
		break;
	case en_MenuScene:
		scene = MenuScene::createScene();
		break;
	/*case en_GameScene:
		scene = GameScene::createScene();
		break;
	case en_PlayerScene:
		scene = PlayerScene::createScene();
		break;
	case en_TeamScene:
		scene = TeamScene::createScene();
		break;
	case en_RankScene:
		scene = RankScene::createScene();
		break;*/
	}
	auto curScene = pDirector->getRunningScene();   //运行当前场景
```

## 网络通信

使用了Windows Socket进行多线程(thread)运行和网络通信
目前只实现了服务端与客户端的简单多线程通信

- **服务端结构**
贴图

- **客户端结构**
贴图

- **效果**
贴图

- **实现细节**

将数据的发送和接受单独作为两个线程

```
//创建发送和接收数据线程
BOOL CClient::StartRuning(void)
{
	m_bConning = TRUE;//设置连接状态

	//创建接收数据线程
	unsigned long ulThreadId;
	m_hThreadRecv = CreateThread(NULL, 0, RecvDataThread, this, 0, &ulThreadId);
	if(NULL == m_hThreadRecv)
	{
		return FALSE;
	}else{
		CloseHandle(m_hThreadRecv);
	}

	//创建发送据的线程
	m_hThreadSend =  CreateThread(NULL, 0, SendDataThread, this, 0, &ulThreadId);
	if(NULL == m_hThreadSend)
	{
		return FALSE;
	}else{
		CloseHandle(m_hThreadSend);
	}

	return TRUE;
}
```


 


