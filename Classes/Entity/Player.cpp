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

Player * Player::create(Vec2 position, int keywordID, Node * map)//�������
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

bool Player::init(Vec2 position, int keywordID, Node * map)//��ʼ��
{
	if (!Node::init())
	{
		return false;
	}
	_keywordID = keywordID;
	_map = map;
	_state = State::NORMAL;//��ʼ��״̬
	_combineEnable = true;
	if (keywordID == 1)
	{
		_playerName = "Ball";
		auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
		_map->addChild(division, PLAYER_INITIAL_SCORE);
	}
	else
	{
		_playerName = "AI";
		auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE * 10);
		_map->addChild(division, PLAYER_INITIAL_SCORE * 10);

	}

	return true;
}

PlayerDivision * Player::createDivision(Vec2 position, Vec2 velocity, int score)
{
	/*����һ����ҷ���*/
	auto division = PlayerDivision::create(_playerName, _keywordID, score);
	division->setPosition(position);
	division->setPlayerName(_playerName);
	division->setVelocity(velocity);

	_divisionList.pushBack(division);
	_divisionNum++;

	return division;
}

void Player::setVelocity(Vec2 v)//����Velocity
{
	_state = State::NORMAL;//�ı�״̬

	for (auto division : _divisionList)//���������б�
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
		if (score >= PLAYER_MIN_DIVIDE_SCORE)//����������С���ѷ���
		{
			if (_divisionNum + 1 > PLAYER_MAX_DIVISION_NUM)//���������������ֵ
			{
				break;
			}
			this->unscheduleAllCallbacks();
			_state = State::DIVIDE;//�ı�״̬
			_combineEnable = false;

			divideFlag = true;

			division->divide();//����
			float radius = division->getRadius();
			int score = division->getScore();
			Vec2 position = division->getPosition();
			auto newDivision = this->createDivision(position, _velocity, score);//��������
			_map->addChild(newDivision);

			float angle = _velocity.getAngle();
			float divideDistance = radius + 0.5*PLAYER_MIN_DIVIDE_DISTANCE;//�������֮�����֮��ľ���
			Vec2 newPosition = Vec2(divideDistance*cosf(angle), divideDistance*sinf(angle));//���ݾ��������µ�λ��

			auto sequence = Sequence::create(
				EaseOut::create(MoveBy::create(0.5f, newPosition), 1.8f),//�ƶ�����
				CallFunc::create(CC_CALLBACK_0(Player::divideFinish, this)),
				NULL);
			newDivision->runAction(sequence);
		}
	}

	if (divideFlag)
	{
		this->scheduleOnce(schedule_selector(Player::setCombine), 10);//��ʱ����
	}

}

Rect Player::getPlayerRect()//�����Ұ�Χ��
{
	float minX, minY, maxX, maxY;

	auto division = _divisionList.at(0);//���������б�
	if (division != NULL)
	{
		Vec2 divisionPosition = division->getPosition();
		float radius = division->getRadius();
		minX = divisionPosition.x - radius;
		maxX = divisionPosition.x + radius;
		minY = divisionPosition.y - radius;
		maxY = divisionPosition.y + radius;
	}

	for (auto item : _divisionList)//���������б�
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

bool Player::collideBean(Bean * bean)//�붹����ײ���
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

void Player::updateDivision()//����λ��
{
	if (_state == State::CONCENTRATE)
	{
		this->concentrate();
	}

	auto rect = this->getPlayerRect();

	/*�����ٶȸı�ÿ��������λ��*/
	for (auto division : _divisionList)
	{
		if (division != NULL)
		{
			float dx;
			float dy;
			Vec2 velocity = division->getVelocity();
			if (_keywordID == 1)//���
			{
				float speed = division->getSpeed();
				if (_state == State::CONCENTRATE)
				{
					speed = PLAYER_CONCENTRATE_SPEED;
				}
				dx = velocity.x*speed;
				dy = velocity.y*speed;
			}
			else if (_keywordID == 2)//�����������
			{
				dx = 0.01*(rand() % 100);
				dy = 0.01*(rand() % 100);
			}

			else if (_keywordID == 3)
			{
				dx = -0.01*(rand() % 100);
				dy = 0.01*(rand() % 100);
			}
			Vec2 divisionVec = Vec2(dx, dy); //�����ƶ�����
			Vec2 oldPosition = division->getPosition();
			Vec2 newPosition = Vec2(oldPosition.x + divisionVec.x, oldPosition.y + divisionVec.y);
			
			if (_state != State::CONCENTRATE && _state != State::DIVIDE) //������������״̬��Ҳ���ڷ���״̬�������������Ŀ�£
			{
				Vec2 centerVec = rect.origin - oldPosition; // ָ�����ķ���
				centerVec.normalize();
				centerVec.x = centerVec.x*PLAYER_CONCENTRATE_SPEED*0.2;
				centerVec.y = centerVec.y*PLAYER_CONCENTRATE_SPEED*0.2;
				Vec2 moveVec = Vec2(centerVec.x + divisionVec.x, centerVec.y + divisionVec.y);
				float angle = Vec2::angle(centerVec, divisionVec);
				float cosAngle = cosf(angle);

				if (cosAngle > 0 && cosAngle < 1) //ֻ�����н�С��90�ȵ����
					newPosition = Vec2(oldPosition.x + moveVec.x, oldPosition.y + moveVec.y);
			}

			float radius1 = division->getRadius();
			if (!_combineEnable)//���ں��塢����״̬�������������ײ���
			{
				for (auto division2 : _divisionList)
				{
					if (division2 != NULL&&division2 != division)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float oldDistance = position2.distance(oldPosition);
						if (oldDistance <= radius1 + radius2)//��������ʱ���ܲ�����ײ�����
						{
							/*
							������Բ��Բ�������߷ֱ��������ƶ�
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
			/*�߽���*/
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
			division->setPrePosition(oldPosition);//����֮ǰ��λ��
			division->setPosition(newPosition);//������λ��
		}
	}
	if (!_combineEnable)//�����������ײ��⣬�ƶ������������ײ��ȡ���ƶ�
	{
		for (auto division1 : _divisionList)//���������б�
		{
			if (division1 != NULL)
			{
				for (auto division2 : _divisionList)//����
				{
					if (division2 != NULL&&division2 != division1)
					{
						Vec2 position1 = division1->getPosition();
						Vec2 position2 = division2->getPosition();
						float radius1 = division1->getRadius();
						float radius2 = division2->getRadius();
						float distance = position1.distance(position2);//�������߾���
						if (distance < radius1 + radius2)//����С�����߰뾶֮��
						{
							Vec2 oldPosition1 = division1->getPrePosition();
							Vec2 oldPosition2 = division2->getPrePosition();
							float oldDistance = oldPosition1.distance(oldPosition2);//����֮ǰ�ľ���
							float fixDistance = 0;//������õ���λ�õĲ���
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
							Vec2 newPosition1 = Vec2(position1.x - cosf(angle)*fixDistance, position1.y - sinf(angle)*fixDistance);//���ݲ���������λ��
							Vec2 newPosition2 = Vec2(position2.x + sinf(angle)*fixDistance, position2.y + sinf(angle)*fixDistance);
							division1->setPosition(newPosition1);//������λ��
							division2->setPosition(newPosition2);
							//break;
						}
					}
				}
			}
		}
	}

	if (_combineEnable)  //������������
	{
		for (int i = 0; i < _divisionList.size(); i++)
		{
			auto division1 = _divisionList.at(i);//����
			if (division1 != NULL)
			{
				Vec2 position1 = division1->getPosition();
				float radius1 = division1->getRadius();
				for (int j = i + 1; j < _divisionList.size(); j++)
				{
					auto division2 = _divisionList.at(j);//����
					if (division2 != NULL)
					{
						Vec2 position2 = division2->getPosition();
						float radius2 = division2->getRadius();
						float distance = position2.distance(position1);//�������߾���
						if (distance<abs(radius2 - radius1))     //�����������Ժ���
						{
							_divisionNum--;

							_combineEnable = false;
							this->scheduleOnce(schedule_selector(Player::setCombine), 8);//��ʱ����

							if (radius1>radius2)//˭��ͳ��˶Է�
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

void Player::spitSpore(Node * map, Map<int, Spore *> & sporeMap)//������
{
	for (auto division : _divisionList)//����
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
				Vec2 sporePosition = Vec2(position.x + radius*cosf(angle) * 2, position.y + radius*sinf(angle) * 2);//���ݲ�������λ��
				spore->setPosition(sporePosition);

				Vec2 newPosition = Vec2(PLAYER_MIN_SPIT_DISTANCE*cosf(angle), PLAYER_MIN_SPIT_DISTANCE*sinf(angle));
				auto action = MoveBy::create(0.5, newPosition);//�ƶ�
				auto action2 = EaseOut::create(action, 1.8f);
				spore->runAction(action2);
				spore->setLocalZOrder(spore->getScore());
				map->addChild(spore);

			}
		}
	}
}

bool Player::collideSpore(Spore * spore)//��������ײ���
{
	for (auto division : _divisionList)//����
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

void Player::setCombine(float dt)//�����־=true
{
	_combineEnable = true;
}

void Player::divideFinish()
{
	_state = State::NORMAL;//���ѽ�����ı�״̬
}

void Player::concentrate() //���������Ŀ�£
{
	auto rect = this->getPlayerRect();
	for (auto division : _divisionList)//����
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

bool Player::collidePrick(Prick *prick)//�����ײ���
{
	bool collideFlag = false;
	for (auto division : _divisionList)//����
	{
		if (division != NULL)
		{
			Vec2 prickPosition = prick->getPosition();
			if (division->collidePrick(prick))
			{
				int prickScore = prick->getScore();
				int divisionScore = division->getScore();

				/*���������ײ������С��Ĵ�С*/
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

				/*ѭ������С��*/
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

					_state = State::DIVIDE;//�ı�״̬
					_combineEnable = false;

					Vec2 velocity = Vec2(cosf(angle), sinf(angle));
					auto splitDivision = this->createDivision(divisionPosition, velocity, splitScore);
					_map->addChild(splitDivision, splitScore);

					Vec2 newPosition = Vec2(PRICK_SPLIT_DISTANCE*cosf(angle), PRICK_SPLIT_DISTANCE*sinf(angle));//������λ��
					auto sequence = Sequence::create(
						EaseOut::create(MoveBy::create(0.3f, newPosition), 1.8f),//�ƶ�
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
	_state = State::CONCENTRATE;//�ı�״̬
}

bool Player::collideRival(Player * rival)//�������ײ���
{
	bool bRet = false;
	for (int i = 0; i < _divisionList.size(); i++)
	{
		auto division1 = _divisionList.at(i);
		int result = rival->collideDivision(division1);
		if (result == 2) // ��ҷ�������
		{
			if (result == 2) // ��ҷ�������
			{
				SceneManager::getInstance()->changeScene(SceneManager::en_GameOverScene);
			}

		}
		else if (result == 1) // ��ҷ������˵���
		{
			bRet = true;
		}
	}

	return bRet;
}

int Player::collideDivision(PlayerDivision * division)//�������ײ���
{
	int flag = 0;
	Vec2 playerPosition = division->getPosition();
	float playerRadius = division->getRadius();

	for (int j = 0; j < _divisionList.size(); j++)
	{
		auto division2 = _divisionList.at(j);
		Vec2 rivalPosition = division2->getPosition();
		float rivalRadius = division2->getRadius();
		float distance = playerPosition.distance(rivalPosition);//�������߾���
		if (distance< abs(playerRadius - rivalRadius))
		{
			int playerScore = division->getScore();
			int rivalScore = division2->getScore();
			if (playerScore>rivalScore*MIN_EAT_MULTIPLE)  //��ҷ������˶���
			{
				division->eatRival(rivalScore);
				return 1;
			}
			else if (rivalScore > playerScore*MIN_EAT_MULTIPLE)  //��ҷ�������
			{
				division2->eatRival(playerScore);
				return 2;
			}
		}
	}
	return flag;
}

float Player::getTotalWeight()//�����������
{
	float weight = 0;
	for (auto division : _divisionList)//����
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

void Player::resetPlayer()//�������
{
	int xPosition = rand() % MAP_WIDTH;
	int yPosition = rand() % MAP_HEIGHT;
	Vec2 position = Vec2(xPosition, yPosition);

	auto division = this->createDivision(position, Vec2::ZERO, PLAYER_INITIAL_SCORE);
	_map->addChild(division, PLAYER_INITIAL_SCORE);

	_velocity = Vec2::ZERO;
}

int Player::getTotalScore()//�õ������ܺ�
{
	int score = 0;

	for (auto division : _divisionList)//����
	{
		if (division != NULL)
		{
			score += division->getScore();
		}
	}
	return score;
}

int Player::getDivisionNum()//��÷�������
{
	return _divisionNum;
}

Vector<PlayerDivision *> & Player::getDivisionList()//��÷����б�
{
	return _divisionList;
}

void Player::onExit()
{
	Node::onExit();
}

void Player::clearDivisionList()//��շ����б�
{
	for (auto division : _divisionList)//����
	{
		division->removeFromParentAndCleanup(true);
	}
	_divisionList.clear();
	_divisionNum = 0;
}

int Player::countSpitSporeNum()//�����³�����������
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
	_state = State::CONCENTRATE;//�ı�״̬
}