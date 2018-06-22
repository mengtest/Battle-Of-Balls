#include "GameLayer.h"
#include "Joystick.h"
#include "Entity.h"
#include "Bean.h"
#include "Spore.h"
#include "Player.h"
#include "Prick.h"
#include "PlayerDivision.h"
#include "SceneManager.h"
enum GameLayerZOrder   //��Ϸͼ���ӽڵ�
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
	_rivalMap.clear();  //�������
	_beanList.clear();  //�������
	_sporeMap.clear();  //�������
	_prickMap.clear();  //�����
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	/*��Ӿ��飬ͼ�㣬������node���ӽڵ���*/
	auto colorLayer = LayerColor::create(Color4B(49, 49, 49, 255), DESIGN_SCREEN_WIDTH, DESIGN_SCREEN_HEIGHT);
	this->addChild(colorLayer, GAME_LAYER_BACKGROUND_Z);  //�����Ϸ������

	_map = Node::create();
	_map->setContentSize(Size(MAP_WIDTH, MAP_HEIGHT));
	this->addChild(_map, GAME_LAYER_MAP_Z);  //��ӵ�ͼ

	//initData();
	initDataDefault();

	_joystick = Joystick::create("gameScene/base.png", "gameScene/joystick.png");
	_joystick->setPosition(Vec2::ZERO);
	_joystick->setVisible(false);
	this->addChild(_joystick, GAME_LAYER_JOYSTICK_Z);  //�������ҡ��


	auto listener = EventListenerTouchOneByOne::create(); //�����¼�
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  //��Ӽ����������Զ����¼�

	this->scheduleUpdate();
	this->schedule(schedule_selector(GameLayer::updateScore), 1);
	this->schedule(schedule_selector(GameLayer::updateRank), 2);

	return true;
}

bool GameLayer::onTouchBegan(Touch * touch, Event * event)  //�����¼���ʼ
{
	auto position = touch->getLocation();

	_joystick->setPosition(position);
	_joystick->setVisible(true);

	_joystick->onTouchBegan(touch, event);

	return true;
}

void GameLayer::onTouchMoved(Touch * touch, Event * event)  //���㴥���ƶ�
{
	_joystick->onTouchMoved(touch, event);
	_player->setVelocity(_joystick->getVelocity());
}

void GameLayer::onTouchEnded(Touch * touch, Event * event)  //�����¼�����
{
	_joystick->onTouchEnded(touch, event);
	_joystick->setVisible(false);
	Vec2 velocity = _joystick->getVelocity();
	if (velocity.x == 0 && velocity.y == 0)
	{
		_player->setConcentrate();
	}
}

void GameLayer::onTouchCancelled(Touch * touch, Event * event)  //�����¼�ȡ��
{
	_joystick->onTouchCancelled(touch, event);
}

void GameLayer::update(float dt) //����
{
	updateBean();  //���¶���
	updateSpore();  //��������
	updatePrick();  //���´�
	_player->updateDivision();  //���·���
	//updateRival();        //ÿ�������Ϣ����ҿͻ����Լ�����
	updateView();  //������Ϸ��ͼ
	collide();  //��ײ���

}

void GameLayer::initRival()  //��ʼ������
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
void GameLayer::initPlayer()  //��ʼ�����
{
	float xPosition = rand() % MAP_WIDTH;
	float yPosition = rand() % MAP_HEIGHT;
	_player = Player::create(Vec2(xPosition, yPosition), _map);
	_player->setLocalZOrder(_player->getTotalScore());
	_map->addChild(_player);
}
void GameLayer::initBean()  //��������
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

void GameLayer::addPrick(float dt)  //�����̴�
{
	static int id = 0;
	Prick * prick = Prick::create("gameScene/prick.png");
	int xPosition = rand() % MAP_WIDTH;
	int yPosition = rand() % MAP_HEIGHT;
	prick->setPosition(Vec2(xPosition, yPosition));  //����λ��
	prick->setLocalZOrder(prick->getScore());
	_map->addChild(prick);  //��Ӵ̵��ӽڵ�
	_prickMap.insert(id, prick);
	id++;
}

void GameLayer::updateView()  //������Ϸ��ͼ
{
	auto rect = _player->getPlayerRect();

	float scaleX = DESIGN_SCREEN_WIDTH / (DESIGN_SCREEN_WIDTH + rect.size.width);
	float scaleY = DESIGN_SCREEN_HEIGHT / (DESIGN_SCREEN_HEIGHT + rect.size.height);
	_mapScale = (scaleX < scaleY) ? scaleX : scaleY;

	float dx = rect.origin.x*_mapScale - DESIGN_SCREEN_WIDTH / 2;
	float dy = rect.origin.y*_mapScale - DESIGN_SCREEN_HEIGHT / 2;

	Vec2 newPosition = Vec2(-dx, -dy);

	_map->setScale(_mapScale);  //���³ߴ�
	_map->setPosition(newPosition); //����λ��
}

void GameLayer::updateBean()
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

void GameLayer::collideBean(Player * player)  //�붹�ӵ���ײ���
{
	Rect rect = player->getPlayerRect();
	Vec2 position = player->getPosition();

	Vec2 point1 = Vec2(rect.origin.x - rect.size.width / 2, rect.origin.y - rect.size.height / 2);//������½�
	int divisionX1 = floor(point1.x / DESIGN_SCREEN_WIDTH);
	int divisionY1 = floor(point1.y / DESIGN_SCREEN_HEIGHT);
	int division1 = divisionY1*MAP_DIVISION_X + divisionX1;
	Vec2 point2 = Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height / 2);//������Ͻ�
	int divisionX2 = floor(point2.x / DESIGN_SCREEN_WIDTH);
	int divisionY2 = floor(point2.y / DESIGN_SCREEN_HEIGHT);
	int division2 = divisionY2*MAP_DIVISION_X + divisionX2;

	//��������������
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

void GameLayer::updateSpore()  //��������
{
	std::vector<int> vecDel;
	for (auto sporeItem : _sporeMap)
	{
		auto spore = sporeItem.second;
		if (spore != NULL)
		{
			Vec2 position = spore->getPosition();

			//��������������
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
		spore->removeFromParentAndCleanup(true);   //�������
	}

	vecDel.clear();

}

void GameLayer::updatePrick()  //���´�
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
			/*else
			{
			for (auto item : _rivalMap)
			{
			auto rival = item.second;
			if (rival != NULL)
			{
			float radius = prick->getRadius();

			if (rival->collidePrick(prick))
			{
			vecDel.push_back(prickItem.first);
			break;
			}
			}
			}
			}*/
		}
	}

	for (auto key : vecDel)
	{
		auto prick = _prickMap.at(key);
		_prickMap.erase(key);
		prick->removeFromParentAndCleanup(true);  //�����
	}
	vecDel.clear();
}

void GameLayer::updateRival()  //���¶���
{
	for (auto item : _rivalMap)
	{
		auto rival = item.second;
		if (rival != NULL)
		{
			//rival->AI(m_map, m_sporeArray);
			rival->updateDivision();
		}
	}
}

void GameLayer::updateRank(float dt)  //��������
{
	Vector<Player *> vec;
	for (auto item : _rivalMap)
	{
		vec.pushBack(item.second);
	}

	vec.pushBack(_player);

	for (int i = 0; i<vec.size(); i++)
	{
		for (int j = vec.size() - 1; j>i; j--)
		{
			Player * player1 = vec.at(j);
			Player * player2 = vec.at(j - 1);
			float score1 = player1->getTotalScore();
			float score2 = player2->getTotalScore();
			if (score1 > score2)
			{
				vec.swap(j, j - 1);
			}
		}
	}

	_eventDispatcher->dispatchCustomEvent("RankChange", &vec);

}

void GameLayer::updateScore(float dt)  //���µ÷�
{
	int score = _player->getTotalScore();

	_eventDispatcher->dispatchCustomEvent("ScoreChange", &score);  //���͡����µ÷֡��¼�
}

void GameLayer::collide()
{
	for (auto item : _rivalMap)        //���������������ֵ���ײ
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

	/*for (auto item1 : _rivalMap)
	{
	auto rival1 = item1.second;

	if (rival1 != NULL)
	{
	for (auto item2 : _rivalMap)
	{
	auto rival2 = item2.second;
	if (rival2 != NULL && item1 != item2)
	{
	if (rival1->collideRival(rival2))
	{
	float rival1DivisionCount = rival1->getDivisionNum();
	float rival2DivisionCount = rival2->getDivisionNum();
	if (rival1DivisionCount == 0)
	{
	rival1->resetPlayer();
	break;
	}

	if (rival2DivisionCount == 0)
	{
	rival2->resetPlayer();
	}
	}
	}
	}
	}

	}*/
}

void GameLayer::spitSpore()  //������
{
	int sporeCount = _player->countSpitSporeNum();

}

void GameLayer::dividePlayer()  //��ҷ���
{
	_player->dividePlayer();
}

void GameLayer::resetBean(Node * node)  //���ö���
{
	node->setVisible(true);
}

void GameLayer::resetPlayer()  //�������
{

}


void GameLayer::onExit()  //�˳���Ϸͼ��
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
