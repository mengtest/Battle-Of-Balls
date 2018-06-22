#include "SettingScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* SettingScene::createScene()
{
	return SettingScene::create();
}

bool SettingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//background
	Sprite* backGround = Sprite::create("public/background.png");
	backGround->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(backGround, 0);

	//ÒôÀÖÍ¼Æ¬²Ëµ¥Ïî
	auto soundOnMenuItem = MenuItemImage::create("music/soundOn.jpg", "music/soundOn.jpg");
	auto soundOffMenuItem = MenuItemImage::create("music/soundOff.jpg", "music/soundOff.jpg");

	//ÉèÖÃÒôÀÖ
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
	{
		auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingScene::menuSoundToggleCallback, this),
			soundOnMenuItem, soundOffMenuItem, NULL);
		soundToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width / 2, visibleSize.height / 2)));
		auto menu = Menu::create(soundToggleMenuItem, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 0);
	}
	else
	{
		auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingScene::menuSoundToggleCallback, this),
			soundOffMenuItem, soundOnMenuItem, NULL);
		soundToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width / 2, visibleSize.height / 2)));
		auto menu1 = Menu::create(soundToggleMenuItem, NULL);
		menu1->setPosition(Vec2::ZERO);
		this->addChild(menu1, 0);
	}

	//·µ»Ø²Ëµ¥Ïî
	auto backMenuItem = MenuItemImage::create("public/return_btn0.png", "public/return_btn1.png",//Norma instead of Normal for a strange bug
		CC_CALLBACK_1(SettingScene::menuBackCallback, this));
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 121 / 2, visibleSize.height - 38 / 2)));//Size 121*38
	auto menu2 = Menu::create(backMenuItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 0);
	return true;
}

void SettingScene::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void SettingScene::menuSoundToggleCallback(Ref* pSender)
{
	MenuItem* item = (MenuItem*)pSender;
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == true)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg.mp3", true);

	}
}
