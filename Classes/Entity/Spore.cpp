#include "Spore.h"
#include "AppMacros.h"

Spore::Spore()
{

}

Spore::~Spore()
{

}

Spore * Spore::create(const std::string& filename)//创建
{
	Spore * spore = new Spore();
	if (spore && spore->init(filename))
	{
		spore->autorelease();
		return spore;
	}
	CC_SAFE_DELETE(spore);
	return nullptr;
}

bool Spore::init(const std::string& filename)//初始化
{
	_skinID = 1;
	if (!Entity::initWithFile(filename))
	{
		return false;
	}

	_radius = SPORE_RADIUS;
	_score = SPORE_SCORE;

	Size size = this->getContentSize();
	float scale = (_radius * 2) / size.width;
	this->setScale(scale);

	return true;
}

Spore * Spore::create(int id)//创建
{
	Spore * spore = new Spore();
	if (spore && spore->init(id))
	{
		spore->autorelease();
		return spore;
	}
	CC_SAFE_DELETE(spore);
	return nullptr;
}

bool Spore::init(int id)//初始化
{
	_skinID = id;
	std::string filename = StringUtils::format("public/keyword_%d.png", id);
	if (!Entity::initWithFile(filename))
	{
		return false;
	}

	_radius = SPORE_RADIUS;
	_score = SPORE_SCORE;

	Size size = this->getContentSize();
	float scale = (_radius * 2) / size.width;
	this->setScale(scale);

	return true;
}

int Spore::getID()
{
	return _skinID;
}
