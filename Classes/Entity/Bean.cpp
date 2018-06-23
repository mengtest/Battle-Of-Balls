#include "Bean.h"
#include "AppMacros.h"

Bean::Bean()
{

}

Bean::~Bean()
{

}

Bean * Bean::create(const std::string& filename)//创建豆子
{
	Bean * bean = new Bean();
	if (bean && bean->init(filename))
	{
		bean->autorelease();
		return bean;
	}
	CC_SAFE_DELETE(bean);
	return nullptr;
}

bool Bean::init(const std::string& filename)
{
	if (!Entity::initWithSpriteFrameName(filename))
	{
		return false;
	}

	_score = BEAN_SCORE;
	_radius = BEAN_RADIUS;

	Size size = this->getContentSize();
	float scale = (_radius * 2) / size.width;
	this->setScale(scale);//根据半径按比例增大

	return true;
}
