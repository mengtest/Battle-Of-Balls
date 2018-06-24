#include "Prick.h"
#include "AppMacros.h"

Prick::Prick()
{

}

Prick::~Prick()
{

}

Prick * Prick::create(const std::string& filename)//创建刺
{
	Prick * prick = new Prick();
	if (prick && prick->init(filename))
	{
		prick->autorelease();
		return prick;
	}
	CC_SAFE_DELETE(prick);
	return nullptr;
}

bool Prick::init(const std::string& filename)//初始化
{
	if (!Entity::initWithFile(filename))
	{
		return false;
	}

	_radius = PRICK_INITIAL_RADIUS;//初始半径
	_score = PRICK_INITIAL_SCORE;//初始分数

	Size size = this->getContentSize();
	float scale = (_radius * 2) / size.width;
	this->setScale(scale);//按比例增大

	return true;
}
