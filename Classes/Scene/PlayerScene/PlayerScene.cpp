
#include "PlayerScene.h"


Scene * PlayerScene::createScene()
{
	auto scene = Scene::create();

	auto layer = PlayerScene::create();

	scene->addChild(layer);

	return scene;
}

bool PlayerScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize(); //定义视图的可视化尺寸
	Vec2 origin = Director::getInstance()->getVisibleOrigin();   //定义视图的可视化原点

	auto bg = Sprite::create("public/background.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);

	//“玩家信息功能尚未开启”
	MenuItemFont::setFontName("arial");
	MenuItemFont::setFontSize(40);
	MenuItemFont*item = MenuItemFont::create("The PlayerScene Function Is Not On.");
	Menu*mn = Menu::create(item, NULL);
	mn->alignItemsVertically();
	this->addChild(mn);

	//返回菜单项
	auto backMenuItem = MenuItemImage::create("public/return_btn0.png", "public/return_btn1.png",//Norma instead of Normal for a strange bug
		CC_CALLBACK_1(PlayerScene::menuBackCallback, this));
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 121 / 2, visibleSize.height - 38 / 2)));//Size 121*38

	auto menu2 = Menu::create(backMenuItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 0);

	return true;
}

void PlayerScene::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void PlayerScene::onExit()
{

};