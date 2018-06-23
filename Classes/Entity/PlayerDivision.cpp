#include "PlayerDivision.h"
#include "AppMacros.h"
#include "Bean.h"
#include "Prick.h"
#include "Spore.h"

PlayerDivision::PlayerDivision()
{

}

PlayerDivision::~PlayerDivision()
{

}

PlayerDivision * PlayerDivision::create(const std::string& filename)//创建分身
{
	PlayerDivision * playerDivision = new PlayerDivision();
	if (playerDivision && playerDivision->init(filename))
	{
		playerDivision->autorelease();
		return playerDivision;
	}
	CC_SAFE_DELETE(playerDivision);//清内存
	return nullptr;
}

bool PlayerDivision::init(const std::string& filename)
{
	if (!Entity::initWithFile(filename))
	{
		return false;
	}

	return true;
}

PlayerDivision * PlayerDivision::create(const std::string &name,int keywordID, int score)//创建分身
{
	PlayerDivision * playerDivision = new PlayerDivision();
	if (playerDivision && playerDivision->init(name, keywordID, score))
	{
		playerDivision->autorelease();
		return playerDivision;
	}
	CC_SAFE_DELETE(playerDivision);
	return nullptr;
}

bool PlayerDivision::init(const std::string &name, int keywordID, int score)
{
	_name = name;
	_keywordID = keywordID;

	if (!Entity::initWithFile(StringUtils::format("public/keyword_%d.png", _keywordID)))
	{
		return false;
	}

	_score = score;
	calculateData();

	return true;
}

bool PlayerDivision::collideBean(Bean * bean)//分身与豆子碰撞检测
{
	Vec2 position = this->getPosition();
	Vec2 beanPosition = bean->getPosition();
	if (beanPosition.distance(position) <= _radius)//距离小于半径就吃掉豆子
	{
		eatBean(bean);
		return true;
	}

	return false;
}

float PlayerDivision::getSpeed()//获得速度
{
	return _speed;
}

void PlayerDivision::eatBean(Bean * bean)//吃豆子
{
	_score += bean->getScore();
	calculateData();
}

void PlayerDivision::eatRival(int score)//吃对手
{
	_score += score;
	calculateData();
}

void PlayerDivision::eatPrick(int score)//吃刺
{
	_score = score;
	calculateData();
}

void PlayerDivision::scaleSprite()//按比例增大
{


    std::string path = StringUtils::format("public/keyword_%d.png", _keywordID);
	this->setTexture(path);

    Size size = this->getContentSize();
	float scale = float(_radius * 2) / size.width;

    this->setScale(scale);

}

void PlayerDivision::spitSpore()//吐孢子
{
	_score -= SPORE_SCORE;
	calculateData();
}

bool PlayerDivision::collideSpore(Spore * spore)//与孢子碰撞检测
{
	if (_score<PLAYER_MIN_EAT_SPORE_SCORE)//分数小于能吃掉孢子最小的最小分数
	{
		return false;
	}

	Vec2 position = this->getPosition();
	Vec2 sporePosition = spore->getPosition();
	if (position.distance(sporePosition) <= _radius)//距离小于分身半径
	{
		eatSpore(spore);
		return true;
	}
	return false;
}

void PlayerDivision::eatSpore(Spore * spore)//吃掉孢子
{
	_score += spore->getScore();
	calculateData();
}

void PlayerDivision::divide()//分裂
{
	_score /= 2;
	calculateData();

}

void PlayerDivision::addWeight(float w)//增加体重
{
	_weight += w;

}

void PlayerDivision::setWeight(float w)//设置体重
{
	_weight = w;

}

float PlayerDivision::getWeight()//得到体重
{
	return _weight;
}

void PlayerDivision::setVelocity(Vec2 v)//设置velocity
{
	_velocity = v;
}

Vec2 PlayerDivision::getVelocity()
{
	return _velocity;
}

bool PlayerDivision::collidePrick(Prick * prick)//分身与刺碰撞检测
{
	float prickScore = prick->getScore();

	if (_score>prickScore*MIN_EAT_MULTIPLE)//分身分数大于能吃掉刺的分数
	{
		Vec2 prickPosition = prick->getPosition();
		Vec2 divisionPostion = this->getPosition();
		float distance = prickPosition.distance(divisionPostion);
		if (distance <= abs(_radius - prick->getRadius()))
		{
			return true;
		}
	}
	return false;
}

void PlayerDivision::setPlayerName(const std::string name)//设置名字
{
	_nameLabel = Label::createWithTTF(name.c_str(), "fonts/STSONG.TTF", 22);
	Size size = this->getContentSize();
	_nameLabel->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(_nameLabel);
}

void PlayerDivision::calculateData()//重新计算分数和半径
{
	_radius = sqrt(_score * PLAYER_INITIAL_RADIUS*4/ PLAYER_INITIAL_SCORE);
	_speed = (PLAYER_INITIAL_RADIUS / _radius) * (PLAYER_INITIAL_SPEED - PLAYER_MIN_SPEED) + PLAYER_MIN_SPEED;

	this->setLocalZOrder(_score);
	scaleSprite();
}

void PlayerDivision::setScore(int score)//设置分数
{
	_score = score;
	calculateData();
}

void PlayerDivision::setPrePosition(const Vec2 & position)//设置之前的位置
{
	_prePosition = position;
}

Vec2 PlayerDivision::getPrePosition()//得到之前的位置
{
	return _prePosition;
}
