 #include "GameLayer.h"
#include "Joystick.h"
#include "Entity.h"
#include "Bean.h"
#include "Spore.h"
#include "Player.h"
#include "Prick.h"
#include "PlayerDivision.h"
#include "SceneManager.h"
enum GameLayerZOrder   //游戏图层子节点
{
	GAME_LAYER_BACKGROUND_Z,
	GAME_LAYER_MAP_Z,
	GAME_LAYER_SPRITE_Z,
	GAME_LAYER_JOYSTICK_Z
};

GameLayer::GameLayer()
{
	
}

GameLayer::~GameLayer()
{
	_rivalMap.clear();  //清除对手
	_beanList.clear();  //清除豆子
	_sporeMap.clear();  //清除孢子
	_prickMap.clear();  //清除刺
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	/*添加精灵，图层，或其他node到子节点上*/
	auto colorLayer = LayerColor::create(Color4B(49, 49, 49, 255), DESIGN_SCREEN_WIDTH, DESIGN_SCREEN_HEIGHT);
	this->addChild(colorLayer, GAME_LAYER_BACKGROUND_Z);  //添加游戏背景层

	_map = Node::create();
	_map->setContentSize(Size(MAP_WIDTH, MAP_HEIGHT));
	this->addChild(_map, GAME_LAYER_MAP_Z);  //添加地图

	initDataDefault();

	_joystick = Joystick::create("gameScene/base.png", "gameScene/joystick.png");
	_joystick->setPosition(Vec2::ZERO);
	_joystick->setVisible(false);
	this->addChild(_joystick, GAME_LAYER_JOYSTICK_Z);  //添加虚拟摇杆


	auto listener = EventListenerTouchOneByOne::create(); //触摸事件
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  //添加监听器接收自定义事件
	
	 //键盘控制分裂和吐孢子
        auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameLayer::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameLayer::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	
	this->scheduleUpdate();
	this->schedule(schedule_selector(GameLayer::updateScore), 1);

	return true;
}

bool GameLayer::onTouchBegan(Touch * touch, Event * event)  //触摸事件开始
{
	auto position = touch->getLocation();

	_joystick->setPosition(position);
	_joystick->setVisible(true);
	_joystick->onTouchBegan(touch, event);

	return true;
}

void GameLayer::onTouchMoved(Touch * touch, Event * event)  //单点触摸移动
{
	_joystick->onTouchMoved(touch, event);
	_player->setVelocity(_joystick->getVelocity());
}

void GameLayer::onTouchEnded(Touch * touch, Event * event)  //触摸事件结束
{
	_joystick->onTouchEnded(touch, event);
	_joystick->setVisible(false);
	Vec2 velocity = _joystick->getVelocity();
	if (velocity.x == 0 && velocity.y == 0)
	{
		_player->setConcentrate();
	}
}

void GameLayer::onTouchCancelled(Touch * touch, Event * event)  //触摸事件取消
{
	_joystick->onTouchCancelled(touch, event);
}

void GameLayer::keyPressed(EventKeyboard::KeyCode keyCode, Event*event) //按下键盘时
{
	if (keyCode == EventKeyboard::KeyCode::KEY_S)           //S控制吐孢子
	{
		int sporeCount = _player->countSpitSporeNum();
		_player->spitSpore(_map,_sporeMap);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_F)       //F控制分裂
	{
		_player->dividePlayer();
	}
}

void GameLayer::keyReleased(EventKeyboard::KeyCode keyCode, Event*event)  //键盘事件取消
{

	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		CCLOG("S is keyreleased");
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_F)
	{
		CCLOG("F is keyreleased");
	}
}

void GameLayer::update(float dt) //更新
{
	updateBean();  //更新豆子
	updateSpore();  //更新孢子
	updatePrick();  //更新刺
	_player->updateDivision();  //更新分身
	updateRival();        //每个玩家信息由玩家客户端自己更新
	updateView();  //更新游戏视图
	collide();  //碰撞检测

}

void GameLayer::initRival()  //初始化对手
{
	int div[MAP_DIVISION_X * MAP_DIVISIOIN_Y] = { 0 };
	int mapDivision = MAP_DIVISION_X * MAP_DIVISIOIN_Y;
	int max_enemy = ceil(double(MAX_RIVAL_NUM) / mapDivision);
	for (int i = 0; i < MAX_RIVAL_NUM; i++)
	{
		int index = rand() % mapDivision;
		if (div[index] + 1 <= max_enemy)
		{
			div[index]++;
		}
		else
		{
			while (true)
			{
				if (++index >= mapDivision)
				{
					index = 0;
				}
				if (div[index] + 1 <= max_enemy)
				{
					div[index]++;
					break;
				}
			}
		}
	}

	for (int i = 0; i < mapDivision; i++)
	{
		int divisionX = i%MAP_DIVISION_X;
		int divisionY = i / MAP_DIVISION_X;

		for (int j = 0; j < div[i]; j++)
		{
			float positionX = rand() % DESIGN_SCREEN_WIDTH + divisionX*DESIGN_SCREEN_WIDTH;
			float positionY = rand() % DESIGN_SCREEN_HEIGHT + divisionY*DESIGN_SCREEN_HEIGHT;
			auto rival = Player::create(Vec2(positionX, positionY), _map);
			rival->setLocalZOrder(rival->getTotalScore());
			_map->addChild(rival);
			_rivalMap.insert(StringUtils::format("%d", i * 100 + j), rival);

		}
	}
}
void GameLayer::initPlayer()  //初始化玩家
{
	float xPosition = rand() % MAP_WIDTH;
	float yPosition = rand() % MAP_HEIGHT;
	_player = Player::create(Vec2(xPosition, yPosition), _map);
	_player->setLocalZOrder(_player->getTotalScore());
	_map->addChild(_player);
}
void GameLayer::initBean()  //初始化豆子
{
	for (int i = 0; i < MAP_DIVISIOIN_Y; i++)
	{
		for (int j = 0; j < MAP_DIVISION_X; j++)
		{
			for (int m = 0; m < MAP_DIVISION_BEAN_NUM; m++)
			{
				int type = rand() % 4 + 3;
				int color = rand() % 6 + 1;
				float r1 = rand_0_1();
				float r2 = rand_0_1();

				std::string path = StringUtils::format("bean_polygon%d_%d.png", type, color);
				auto bean = Bean::create(path.c_str());

				bean->setPosition(Vec2(DESIGN_SCREEN_WIDTH*(j + r1), DESIGN_SCREEN_HEIGHT*(i + r2)));
				bean->setLocalZOrder(bean->getScore());
				_map->addChild(bean);
				_beanList.pushBack(bean);
			}

		}
	}
}

void GameLayer::startAddPrick(float dt)   
{
	this->schedule(schedule_selector(GameLayer::addPrick), 5);
}

void GameLayer::addPrick(float dt)  //增加绿刺
{
	static int id = 0;
	Prick * prick = Prick::create("gameScene/prick.png");
	int xPosition = rand() % MAP_WIDTH;
	int yPosition = rand() % MAP_HEIGHT;
	prick->setPosition(Vec2(xPosition, yPosition));  //放置位置
	prick->setLocalZOrder(prick->getScore());
	_map->addChild(prick);  //添加刺到子节点
	_prickMap.insert(id, prick);
	id++;
}

void GameLayer::updateView()  //更新游戏视图
{
	auto rect = _player->getPlayerRect();

	float scaleX = DESIGN_SCREEN_WIDTH / (DESIGN_SCREEN_WIDTH + rect.size.width);
	float scaleY = DESIGN_SCREEN_HEIGHT / (DESIGN_SCREEN_HEIGHT + rect.size.height);
	_mapScale = (scaleX < scaleY) ? scaleX : scaleY;

	float dx = rect.origin.x*_mapScale - DESIGN_SCREEN_WIDTH / 2;
	float dy = rect.origin.y*_mapScale - DESIGN_SCREEN_HEIGHT / 2;

	Vec2 newPosition = Vec2(-dx, -dy);

	_map->setScale(_mapScale);  //更新尺寸
	_map->setPosition(newPosition); //更新位置
}

void GameLayer::updateBean()    //更新豆子
{
	collideBean(_player);

	for (auto item : _rivalMap)
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			collideBean(rival);
		}
	}
}

void GameLayer::collideBean(Player * player)  //与豆子的碰撞检测
{
	Rect rect = player->getPlayerRect();
	Vec2 position = player->getPosition();

	Vec2 point1 = Vec2(rect.origin.x - rect.size.width / 2, rect.origin.y - rect.size.height / 2);//玩家左下角
	int divisionX1 = floor(point1.x / DESIGN_SCREEN_WIDTH);
	int divisionY1 = floor(point1.y / DESIGN_SCREEN_HEIGHT);
	int division1 = divisionY1*MAP_DIVISION_X + divisionX1;
	Vec2 point2 = Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2);//玩家右上角
	int divisionX2 = floor(point2.x / DESIGN_SCREEN_WIDTH);
	int divisionY2 = floor(point2.y / DESIGN_SCREEN_HEIGHT);
	int division2 = divisionY2*MAP_DIVISION_X + divisionX2;

	//处理超出界限问题
	if (divisionX1 < 0)
	{
		divisionX1 = 0;
	}
	if (divisionY1 < 0)
	{
		divisionY1 = 0;
	}
	if (divisionX2 == MAP_DIVISION_X)
	{
		divisionX2 = MAP_DIVISION_X - 1;
	}
	if (divisionY2 == MAP_DIVISIOIN_Y)
	{
		divisionY2 = MAP_DIVISIOIN_Y - 1;
	}
	int xSpan = divisionX2 - divisionX1;
	int ySpan = divisionY2 - divisionY1;
	int max_ball = MAP_DIVISION_BEAN_NUM;

	for (int i = 0; i <= ySpan; i++)
	{
		for (int j = 0; j <= xSpan; j++)
		{
			division1 = (divisionY1 + i)*MAP_DIVISION_X + divisionX1 + j;
			for (int index = division1*max_ball, m = 0; m < max_ball; m++)
			{
				auto bean = _beanList.at(index);
				if (bean->isVisible())
				{
					if (player->collideBean(bean))
					{
						bean->setVisible(false);
						float time = rand() % 10 + 10;

						auto sequence = Sequence::create(
							DelayTime::create(time),
							CallFuncN::create(CC_CALLBACK_0(GameLayer::resetBean, this, bean)),
							NULL);
						bean->runAction(sequence);
						//return;
					}
				}
				index++;
			}
		}
	}
}

void GameLayer::updateSpore()  //更新孢子
{
	std::vector<int> vecDel;
	for (auto sporeItem : _sporeMap)
	{
		auto spore = sporeItem.second;
		if (spore != NULL)
		{
			Vec2 position = spore->getPosition();

			//处理超出界限问题
			if (position.x<SPORE_RADIUS)
			{
				position.x = SPORE_RADIUS;
			}
			if (position.x>MAP_WIDTH - SPORE_RADIUS)
			{
				position.x = MAP_WIDTH - SPORE_RADIUS;
			}
			if (position.y<SPORE_RADIUS)
			{
				position.y = SPORE_RADIUS;
			}
			if (position.y>MAP_HEIGHT - SPORE_RADIUS)
			{
				position.y = MAP_HEIGHT - SPORE_RADIUS;
			}
			spore->setPosition(position);
			if (_player->collideSpore(spore))
			{
				vecDel.push_back(sporeItem.first);
			}
			else
			{
				for (auto item : _rivalMap)
				{
					auto rival = item.second;
					if (rival != NULL)
					{
						if (rival->collideSpore(spore))
						{
							vecDel.push_back(sporeItem.first);
							break;
						}
					}
				}
			}
		}
	}

	for (auto key : vecDel)
	{
		auto spore = _sporeMap.at(key);
		_sporeMap.erase(key);
		spore->removeFromParentAndCleanup(true);   //清除孢子
	}

	vecDel.clear();

}

void GameLayer::updatePrick()  //更新刺
{
	std::vector<int> vecDel;
	for (auto prickItem : _prickMap)
	{
		auto prick = prickItem.second;
		if (prick != NULL)
		{

			if (_player->collidePrick(prick))
			{
				vecDel.push_back(prickItem.first);
			}
		}
	}

	for (auto key : vecDel)
	{
		auto prick = _prickMap.at(key);
		_prickMap.erase(key);
		prick->removeFromParentAndCleanup(true);  //清除刺
	}
	vecDel.clear();
}

void GameLayer::updateRival()  //更新对手
{
	for (auto item : _rivalMap)
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			rival->updateDivision();
		}
	}
}

void GameLayer::updateScore(float dt)
{
	int score = _player->getTotalScore();

	if(score>=100000)
		SceneManager::getInstance()->changeScene(SceneManager::en_GameOverScene);  //返回到主菜单场景

}

void GameLayer::collide()
{
	for (auto item : _rivalMap)        //检测玩家与其他对手的碰撞
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			if (_player->collideRival(rival))
			{
				float playerDivisionCount = _player->getDivisionNum();
				if (playerDivisionCount == 0)
				{
					_player->resetPlayer();
					break;
				}
			}
		}

	}
}

void GameLayer::dividePlayer()  //玩家分身
{
	_player->dividePlayer();
}

void GameLayer::resetBean(Node * node)  //重置豆子
{
	node->setVisible(true);
}

void GameLayer::resetPlayer()  //重置玩家
{

}


void GameLayer::onExit()  //退出游戏图层
{
	this->unscheduleAllCallbacks();
	Layer::onExit();
}

void GameLayer::initDataDefault()
{
	initRival();
	initPlayer();
	initBean();
}
