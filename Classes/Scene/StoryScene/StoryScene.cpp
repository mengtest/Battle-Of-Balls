#include "StoryScene.h"
//#include "../SceneManager.h"

enum StoryTag
{
	TAG_SPRITE_MIN = 1,
	TAG_SPRITE_MAX = 7
};

enum StoryZOrder
{
	STORY_BACKGROUND_Z,
	STORY_MENU_Z
};

const int MIN_TOUCH_DISTANCE = 20;

Scene * StoryScene::createScene()
{
	auto scene = Scene::create();

	auto layer = StoryScene::create();

	scene->addChild(layer);

	return scene;
}

bool StoryScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(StoryScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(StoryScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(StoryScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_nextItem = MenuItemImage::create(
		"storyScene/story_next.png",
		"storyScene/story_next.png",
		CC_CALLBACK_1(StoryScene::menuNextCallback, this));
	_nextItem->setPosition(767, 237);

	_previousItem = MenuItemImage::create(
		"storyScene/story_next.png",
		"storyScene/story_next.png",
		CC_CALLBACK_1(StoryScene::menuPreviousCallback, this));
	_previousItem->setPosition(33, 237);
	_previousItem->setVisible(false);

	auto returnItem = MenuItemImage::create(
		"storyScene/story_return.png",
		"storyScene/story_return.png",
		CC_CALLBACK_1(StoryScene::menuReturnCallback, this));
	returnItem->setPosition(757, 42);

	auto menu = Menu::create(_nextItem, _previousItem, returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, STORY_MENU_Z);

	createStorySprite();

	return true;
}

bool StoryScene::onTouchBegan(Touch * touch, Event * event)
{
	_startPoint = touch->getLocation();

	return true;
}

void StoryScene::onTouchMoved(Touch * touch, Event * event)
{

}

void StoryScene::onTouchEnded(Touch * touch, Event * event)
{
	auto point = touch->getLocation();
	float distance = point.distance(_startPoint);
	if (distance > MIN_TOUCH_DISTANCE)
	{
		if (point.x > _startPoint.x)
		{
			if (_selectedSprite != TAG_SPRITE_MIN)
			{
				menuPreviousCallback(NULL);
			}
		}
		else
		{
			if (_selectedSprite != TAG_SPRITE_MAX)
			{
				menuNextCallback(NULL);
			}
		}
	}

}

void StoryScene::menuReturnCallback(Ref * pSender)
{
	Director::getInstance()->popScene();
}

void StoryScene::menuNextCallback(Ref * pSender)
{
	auto oldSprite = this->getChildByTag(_selectedSprite);
	oldSprite->stopAllActions();
	oldSprite->setOpacity(255);
	oldSprite->runAction(FadeOut::create(0.5));

	_selectedSprite++;
	auto newSprite = this->getChildByTag(_selectedSprite);
	newSprite->stopAllActions();
	newSprite->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.3), NULL));

	if (_selectedSprite == TAG_SPRITE_MAX)
	{
		_nextItem->setVisible(false);
	}
	_previousItem->setVisible(true);
}

void StoryScene::menuPreviousCallback(Ref * pSender)
{
	auto oldSprite = this->getChildByTag(_selectedSprite);
	oldSprite->stopAllActions();
	oldSprite->setOpacity(255);
	oldSprite->runAction(FadeOut::create(0.5));

	_selectedSprite--;
	auto newSprite = this->getChildByTag(_selectedSprite);
	newSprite->stopAllActions();
	newSprite->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.3), NULL));

	if (_selectedSprite == TAG_SPRITE_MIN)
	{
		_previousItem->setVisible(false);
	}
	_nextItem->setVisible(true);
}

void StoryScene::createStorySprite()
{
	for (int i = 1; i <= 7; i++)
	{
		std::string path = StringUtils::format("storyScene/story_%d.png", i);
		auto sprite = Sprite::create(path.c_str());
		sprite->setPosition(400, 225);
		sprite->setTag(i);
		sprite->setOpacity(0);
		this->addChild(sprite, STORY_BACKGROUND_Z);
	}

	_selectedSprite = 1;
	auto sprite = this->getChildByTag(_selectedSprite);
	sprite->setOpacity(255);
}