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

PlayerDivision * PlayerDivision::create(const std::string& filename)
{
	PlayerDivision * playerDivision = new PlayerDivision();
	if (playerDivision && playerDivision->init(filename))
	{
		playerDivision->autorelease();
		return playerDivision;
	}
	CC_SAFE_DELETE(playerDivision);
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

PlayerDivision * PlayerDivision::create(const std::string &name,int keywordID, int score)
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

bool PlayerDivision::collideBean(Bean * bean)
{
	Vec2 position = this->getPosition();
	Vec2 beanPosition = bean->getPosition();
	if (beanPosition.distance(position) <= _radius)
	{
		eatBean(bean);
		return true;
	}

	return false;
}

float PlayerDivision::getSpeed()
{
	return _speed;
}

void PlayerDivision::eatBean(Bean * bean)
{
	_score += bean->getScore();
	calculateData();
}

void PlayerDivision::eatRival(int score)
{
	_score += score;
	calculateData();
}

void PlayerDivision::eatPrick(int score)
{
	_score = score;
	calculateData();
}

void PlayerDivision::scaleSprite()
{


    std::string path = StringUtils::format("public/keyword_%d.png", _keywordID);
	this->setTexture(path);

    Size size = this->getContentSize();
	float scale = float(_radius * 2) / size.width;

    this->setScale(scale);

}

void PlayerDivision::spitSpore()
{
	_score -= SPORE_SCORE;
	calculateData();
}

bool PlayerDivision::collideSpore(Spore * spore)
{
	if (_score<PLAYER_MIN_EAT_SPORE_SCORE)
	{
		return false;
	}

	Vec2 position = this->getPosition();
	Vec2 sporePosition = spore->getPosition();
	if (position.distance(sporePosition) <= _radius)
	{
		eatSpore(spore);
		return true;
	}
	return false;
}

void PlayerDivision::eatSpore(Spore * spore)
{
	_score += spore->getScore();
	calculateData();
}

void PlayerDivision::divide()
{
	_score /= 2;
	calculateData();

}

void PlayerDivision::addWeight(float w)
{
	_weight += w;

}

void PlayerDivision::setWeight(float w)
{
	_weight = w;

}

float PlayerDivision::getWeight()
{
	return _weight;
}

void PlayerDivision::setVelocity(Vec2 v)
{
	_velocity = v;
}

Vec2 PlayerDivision::getVelocity()
{
	return _velocity;
}

bool PlayerDivision::collidePrick(Prick * prick)
{
	float prickScore = prick->getScore();

	if (_score>prickScore*MIN_EAT_MULTIPLE)
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

void PlayerDivision::setPlayerName(const std::string name)
{
	_nameLabel = Label::createWithTTF(name.c_str(), "fonts/STSONG.TTF", 22);
	Size size = this->getContentSize();
	_nameLabel->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(_nameLabel);
}

void PlayerDivision::calculateData()
{
	_radius = sqrt(_score * PLAYER_INITIAL_RADIUS*4/ PLAYER_INITIAL_SCORE);
	_speed = (PLAYER_INITIAL_RADIUS / _radius) * (PLAYER_INITIAL_SPEED - PLAYER_MIN_SPEED) + PLAYER_MIN_SPEED;

	this->setLocalZOrder(_score);
	scaleSprite();
}

void PlayerDivision::setScore(int score)
{
	_score = score;
	calculateData();
}

void PlayerDivision::setPrePosition(const Vec2 & position)
{
	_prePosition = position;
}

Vec2 PlayerDivision::getPrePosition()
{
	return _prePosition;
}