#include "Player.h"
#include "PlayerDivision.h"
#include "Spore.h"
#include "Prick.h"
#include "AppMacros.h"
//#include "I18N.h"
//#include "GlobalEnum.h"

Player::Player()
{
	_divisionNum = 0;
}

Player::~Player()
{
	_divisionList.clear();
}

Player * Player::create(Vec2 position, int keywordID, Node * map)//创建玩家
{
	Player * player = new Player();
	if (player && player->init(position, keywordID, map))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init(Vec2 position, int keywordID, Node * map)//初始化
{
	if (!Node::init())
	{
		return false;
	}
	if (keywordID == 1)
	{
		_playerName = "Ball";
	}
	else
	{
		_playerName = "AI";
	}
	_keywordID = keywordID;
	_map = map;
	_state = State::NORMAL;//初始化状态
	_combineEnable = true;
	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);
	return true;
}

PlayerDivision * Player::createDivision(Vec2 position, Vec2 velocity, int score)
{
	/*创建一个玩家分身*/
	auto division = PlayerDivision::create(_playerName, _keywordID, score);
	division->setPosition(position);
	division->setPlayerName(_playerName);
	division->setVelocity(velocity);

	_divisionList.pushBack(division);
	_divisionNum++;

	return division;
}

void Player::setVelocity(Vec2 v)//设置Velocity
{
	_state = State::NORMAL;//改变状态

	for (auto division : _divisionList)//遍历分身列表
	{
		if (division != NULL)
		{
			division->setVelocity(v);
		}
	}
	_velocity = v;
}

Vec2 Player::getVelocity()
{
	return _velocity;
}

void  Player::dividePlayer()
{
	bool divideFlag = false;
	int size = _divisionList.size();

	for (int i = 0; i<size; i++)
	{
		auto division = _divisionList.at(i);
		int score = division->getScore();
		if (score >= PLAYER_MIN_DIVIDE_SCORE)//分数大于最小分裂分数
		{
			if (_divisionNum + 1 > PLAYER_MAX_DIVISION_NUM)//分身数量到达最大值
			{
				break;
			}
			this->unscheduleAllCallbacks();
			_state = State::DIVIDE;//改变状态
			_combineEnable = false;

			divideFlag = true;

			division->divide();//分裂
			float radius = division->getRadius();
			int score = division->getScore();
			Vec2 position = division->getPosition();
			auto newDivision = this->createDivision(position, _velocity, score);//创建分身
			_map->addChild(newDivision);

			float angle = _velocity.getAngle();
			float divideDistance = radius + 0.5*PLAYER_MIN_DIVIDE_DISTANCE;//计算分裂之后分身之间的距离
			Vec2 newPosition = Vec2(divideDistance*cosf(angle), divideDistance*sinf(angle));//根据距离计算出新的位置

			auto sequence = Sequence::create(
				EaseOut::create(MoveBy::create(0.5f, newPosition), 1.8f),//移动分身
				CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this)),
				NULL);
			newDivision->runAction(sequence);
		}
	}

	if (divideFlag)
	{
		this->scheduleOnce(schedule_selector(Player::setCombine), 10);//定时合体
	}

}

Rect Player::getPlayerRect()//获得玩家包围盒
{
	float minX, minY, maxX, maxY;

	auto division = _divisionList.at(0);//遍历分身列表
	if (division != NULL)
	{
		Vec2 divisionPosition = division->getPosition();
		float radius = division->getRadius();
		minX = divisionPosition.x - radius;
		maxX = divisionPosition.x + radius;
		minY = divisionPosition.y - radius;
		maxY = divisionPosition.y + radius;
	}

	for (auto item : _divisionList)//遍历分身列表
	{

		if (item != NULL)
		{
			Vec2 divisionPosition = item->getPosition();
			float radius = item->getRadius();
			if (minX > divisionPosition.x - radius) { minX = divisionPosition.x - radius; }
			if (maxX <divisionPosition.x + radius) { maxX = divisionPosition.x + radius; }
			if (minY >divisionPosition.y - radius) { minY = divisionPosition.y - radius; }
			if (maxY < divisionPosition.y + radius) { maxY = divisionPosition.y + radius; }
		}
	}

	Rect rect;
	rect.size = Size(maxX - minX, maxY - minY);
	rect.origin = Vec2((minX + maxX) / 2, (minY + maxY) / 2);
	return rect;
}

bool Player::collideBean(Bean * bean)//与豆子碰撞检测
{
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			if (division->collideBean(bean))
			{
				division->setLocalZOrder((int)ceil(division->getScore()));
				return true;
			}
		}
	}
	return false;
}

void Player::updateDivision()//更新位置
{
	if (_state == State::CONCENTRATE)
	{
		this->concentrate();
	}

	auto rect = this->getPlayerRect();

	/*根据速度改变每个分身的位置*/
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			float dx;
			float dy;
			Vec2 velocity = division->getVelocity();
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
			else//AI
			{
				int x, y;
				x = rand() % 100;
				y = rand() % 100;
				if (x > 70)
					dx = 0.01*x;
				else
					dx = -0.01*x;
				if (y >10)
					dy = -0.01*y;
				else
					dy = 0.01*y;

			}

			Vec2 divisionVec = Vec2(dx, dy); //分身移动方向
			Vec2 oldPosition = division->getPosition();
			Vec2 newPosition = Vec2(oldPosition.x + divisionVec.x, oldPosition.y + divisionVec.y);
			if (_state != State::CONCENTRATE && _state != State::DIVIDE) //不在主动合体状态，也不在分身状态，分身会往中心靠拢
			{
				Vec2 centerVec = rect.origin - oldPosition; // 指向中心方向
				centerVec.normalize();
				centerVec.x = centerVec.x*PLAYER_CONCENTRATE_SPEED*0.2;
				centerVec.y = centerVec.y*PLAYER_CONCENTRATE_SPEED*0.2;
				Vec2 moveVec = Vec2(centerVec.x + divisionVec.x, centerVec.y + divisionVec.y);
				float angle = Vec2::angle(centerVec, divisionVec);
				float cosAngle = cosf(angle);

				if (cosAngle > 0 && cosAngle < 1) //只处理夹角小于90度的情况
					newPosition = Vec2(oldPosition.x + moveVec.x, oldPosition.y + moveVec.y);
			}

			float radius1 = division->getRadius();
			if (!_combineEnable)//不在合体、分身状态，分身会进行碰撞检测
			{
				for (auto division2 : _divisionList)
				{
					if (division2 != NULL&&division2 != division)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float oldDistance = position2.distance(oldPosition);
						if (oldDistance <= radius1 + radius2)//处理分身时可能产生碰撞的情况
						{
							/*
							将两个圆以圆心连接线分别向两边移动
							*/
							float distance = ((radius1 + radius2) - oldDistance) / 2;
							Vec2 vec = oldPosition - position2;
							float angle = vec.getAngle();
							newPosition = Vec2(oldPosition.x + cosf(angle), oldPosition.y + sinf(angle));
							division2->setPosition(Vec2(position2.x - cosf(angle), position2.y - sinf(angle)));
							break;
						}
					}
				}
			}
			/*边界检测*/
			if (newPosition.x <= radius1)
			{
				newPosition.x = radius1;
			}
			if (newPosition.x >= MAP_WIDTH - radius1)
			{
				newPosition.x = MAP_WIDTH - radius1;
			}
			if (newPosition.y <= radius1)
			{
				newPosition.y = radius1;
			}
			if (newPosition.y >= MAP_HEIGHT - radius1)
			{
				newPosition.y = MAP_HEIGHT - radius1;
			}
			division->setPrePosition(oldPosition);//设置之前的位置
			division->setPosition(newPosition);//设置新位置
		}
	}
	if (!_combineEnable)//分身会进行碰撞检测，移动后如果产生碰撞则取消移动
	{
		for (auto division1 : _divisionList)//遍历分身列表
		{
			if (division1 != NULL)
			{
				for (auto division2 : _divisionList)//遍历
				{
					if (division2 != NULL&&division2 != division1)
					{
						Vec2 position1 = division1->getPosition();
						Vec2 position2 = division2->getPosition();
						float radius1 = division1->getRadius();
						float radius2 = division2->getRadius();
						float distance = position1.distance(position2);//计算两者距离
						if (distance < radius1 + radius2)//距离小于两者半径之和
						{
							Vec2 oldPosition1 = division1->getPrePosition();
							Vec2 oldPosition2 = division2->getPrePosition();
							float oldDistance = oldPosition1.distance(oldPosition2);//计算之前的距离
							float fixDistance = 0;//计算出得到新位置的参数
							if (oldDistance <= radius1 + radius2)
							{
								fixDistance = 2;
							}
							else
							{
								fixDistance = (radius1 + radius2 - distance);
							}
							Vec2 v = position2 - position1;
							float angle = v.getAngle();
							Vec2 newPosition1 = Vec2(position1.x - cosf(angle)*fixDistance, position1.y - sinf(angle)*fixDistance);//根据参数计算新位置
							Vec2 newPosition2 = Vec2(position2.x + sinf(angle)*fixDistance, position2.y + sinf(angle)*fixDistance);
							division1->setPosition(newPosition1);//设置新位置
							division2->setPosition(newPosition2);
							//break;
						}
					}
				}
			}
		}
	}

	if (_combineEnable)  //处理分身合体
	{
		for (int i = 0; i < _divisionList.size(); i++)
		{
			auto division1 = _divisionList.at(i);//遍历
			if (division1 != NULL)
			{
				Vec2 position1 = division1->getPosition();
				float radius1 = division1->getRadius();
				for (int j = i + 1; j < _divisionList.size(); j++)
				{
					auto division2 = _divisionList.at(j);//遍历
					if (division2 != NULL)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float distance = position2.distance(position1);//计算两者距离
						if (distance<abs(radius2 - radius1))     //两个分身可以合体
						{
							_divisionNum--;

							_combineEnable = false;
							this->scheduleOnce(schedule_selector(Player::setCombine), 8);//定时合体

							if (radius1>radius2)//谁大就吃了对方
							{
								division1->eatRival(division2->getScore());
								_divisionList.eraseObject(division2);
								division2->removeFromParentAndCleanup(true);
								return;
							}
							else
							{
								division2->eatRival(division1->getScore());
								_divisionList.eraseObject(division1);
								division1->removeFromParentAndCleanup(true);
								return;
							}
						}
					}

				}
			}
		}
	}
}

void Player::spitSpore(Node * map, Map<int, Spore *> & sporeMap)//吐孢子
{
	for (auto division : _divisionList)//遍历
	{
		if (division != NULL)
		{
			int score = division->getScore();
			if (score >= PLAYER_MIN_SPIT_SCORE)
			{
				division->spitSpore();
				Vec2 position = division->getPosition();
				float angle = division->getVelocity().getAngle();
				float radius = division->getRadius();
				Spore * spore = Spore::create("public/spore_1.png");
				Vec2 sporePosition = Vec2(position.x + radius*cosf(angle) * 2, position.y + radius*sinf(angle) * 2);//根据参数计算位置
				spore->setPosition(sporePosition);

				Vec2 newPosition = Vec2(PLAYER_MIN_SPIT_DISTANCE*cosf(angle), PLAYER_MIN_SPIT_DISTANCE*sinf(angle));
				auto action = MoveBy::create(0.5, newPosition);//移动
				auto action2 = EaseOut::create(action, 1.8f);
				spore->runAction(action2);
				spore->setLocalZOrder(spore->getScore());
				map->addChild(spore);

			}
		}
	}
}

bool Player::collideSpore(Spore * spore)//与孢子碰撞检测
{
	for (auto division : _divisionList)//遍历
	{

		if (division != NULL)
		{
			if (division->collideSpore(spore))
			{
				division->setLocalZOrder((int)ceil(division->getScore()));
				return true;
			}
		}
	}

	return false;
}

void Player::setCombine(float dt)//合体标志=true
{
	_combineEnable = true;
}

void Player::divideFinish()
{
	_state = State::NORMAL;//分裂结束后改变状态
}

void Player::concentrate() //主动向中心靠拢
{
	auto rect = this->getPlayerRect();
	for (auto division : _divisionList)//遍历
	{
		if (division != NULL)
		{
			Vec2 divisionPosition = division->getPosition();
			Vec2 velocity = rect.origin - divisionPosition;
			velocity.normalize();
			division->setVelocity(velocity);
		}
	}
}

bool Player::collidePrick(Prick *prick)//与刺碰撞检测
{
	bool collideFlag = false;
	for (auto division : _divisionList)//遍历
	{
		if (division != NULL)
		{
			Vec2 prickPosition = prick->getPosition();
			if (division->collidePrick(prick))
			{
				int prickScore = prick->getScore();
				int divisionScore = division->getScore();

				/*计算与刺碰撞后生成小球的大小*/
				int splitScore = (prickScore + divisionScore) / (MAX_EAT_PRICK_DIVISION_NUM + 1);
				if (splitScore >= MAX_EAT_PRICK_SCORE)
				{
					splitScore = MAX_EAT_PRICK_SCORE;
					divisionScore = divisionScore + prickScore - splitScore*(MAX_EAT_PRICK_DIVISION_NUM);
					division->eatPrick(divisionScore);
				}
				else
				{
					division->eatPrick(splitScore);
				}

				/*循环创建小球*/
				float angle = 0;
				Vec2 divisionPosition = division->getPosition();
				float radius = sqrt(splitScore * PLAYER_INITIAL_RADIUS * PLAYER_INITIAL_RADIUS / PLAYER_INITIAL_SCORE);
				float anglePerDivision = PI * 2 / MAX_EAT_PRICK_DIVISION_NUM;
				for (int i = 0; i < MAX_EAT_PRICK_DIVISION_NUM; i++)
				{
					if (_divisionNum + 1 >= PLAYER_MAX_DIVISION_NUM)
					{
						break;
					}

					_state = State::DIVIDE;//改变状态
					_combineEnable = false;

					Vec2 velocity = Vec2(cosf(angle), sinf(angle));
					auto splitDivision = this->createDivision(divisionPosition, velocity, splitScore);
					_map->addChild(splitDivision, splitScore);

					Vec2 newPosition = Vec2(PRICK_SPLIT_DISTANCE*cosf(angle), PRICK_SPLIT_DISTANCE*sinf(angle));//计算新位置
					auto sequence = Sequence::create(
						EaseOut::create(MoveBy::create(0.3f, newPosition), 1.8f),//移动
																				 //CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this),
						CallFunc::create(CC_CALLBACK_0(Player::collidePrickFinish, this)),
						NULL);
					splitDivision->runAction(sequence);

					angle += anglePerDivision;

				}

				this->scheduleOnce(schedule_selector(Player::setCombine), 10);

				return true;
			}
		}
	}

	return false;
}

void Player::setConcentrate()
{
	_state = State::CONCENTRATE;//改变状态
}

bool Player::collideRival(Player * rival)//与对手碰撞检测
{
	bool bRet = false;
	for (int i = 0; i < _divisionList.size(); i++)
	{
		auto division1 = _divisionList.at(i);
		int result = rival->collideDivision(division1);
		if (result == 2) // 玩家分身被吃
		{
			_divisionList.eraseObject(division1);
			division1->removeFromParentAndCleanup(true);
			_divisionNum--;
			i--;
			bRet = true;
		}
		else if (result == 1) // 玩家分身吃了敌人
		{
			bRet = true;
		}
	}

	return bRet;
}

int Player::collideDivision(PlayerDivision * division)//与分身碰撞检测
{
	int flag = 0;
	Vec2 playerPosition = division->getPosition();
	float playerRadius = division->getRadius();

	for (int j = 0; j < _divisionList.size(); j++)
	{
		auto division2 = _divisionList.at(j);
		Vec2 rivalPosition = division2->getPosition();
		float rivalRadius = division2->getRadius();
		float distance = playerPosition.distance(rivalPosition);//计算两者距离
		if (distance< abs(playerRadius - rivalRadius))
		{
			int playerScore = division->getScore();
			int rivalScore = division2->getScore();
			if (playerScore>rivalScore*MIN_EAT_MULTIPLE)  //玩家分身吃了对手
			{
				division->eatRival(rivalScore);
				return 1;
			}
			else if (rivalScore > playerScore*MIN_EAT_MULTIPLE)  //玩家分身被吃
			{
				division2->eatRival(playerScore);
				return 2;
			}
		}
	}
	return flag;
}

float Player::getTotalWeight()//获得整个重量
{
	float weight = 0;
	for (auto division : _divisionList)//遍历
	{
		if (division != NULL)
		{
			weight += division->getWeight();
		}
	}
	return weight;
}

std::string  Player::getPlayerName()
{
	return _playerName;
}

void Player::resetPlayer()//重置玩家
{
	int xPosition = rand() % MAP_WIDTH;
	int yPosition = rand() % MAP_HEIGHT;
	Vec2 position = Vec2(xPosition, yPosition);

	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);

	_velocity = Vec2::ZERO;
}

int Player::getTotalScore()//得到分数总和
{
	int score = 0;

	for (auto division : _divisionList)//遍历
	{
		if (division != NULL)
		{
			score += division->getScore();
		}
	}
	return score;
}

int Player::getDivisionNum()//获得分身数量
{
	return _divisionNum;
}

Vector<PlayerDivision *> & Player::getDivisionList()//获得分身列表
{
	return _divisionList;
}

void Player::onExit()
{
	Node::onExit();
}

void Player::clearDivisionList()//清空分身列表
{
	for (auto division : _divisionList)//遍历
	{
		division->removeFromParentAndCleanup(true);
	}
	_divisionList.clear();
	_divisionNum = 0;
}

int Player::countSpitSporeNum()//计算吐出的孢子数量
{
	int count = 0;
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			int score = division->getScore();
			if (score >= PLAYER_MIN_SPIT_SCORE)
			{
				count++;
			}
		}
	}

	return count;
}

void Player::collidePrickFinish()
{
	_state = State::CONCENTRATE;//改变状态
}