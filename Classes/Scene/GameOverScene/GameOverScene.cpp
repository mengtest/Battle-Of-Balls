#include "GameOverScene.h"
#include"SceneManager.h"

Scene * GameOverScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameOverScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//“Game Over”
	MenuItemFont::setFontName("arial");
	MenuItemFont::setFontSize(40);
	MenuItemFont*item = MenuItemFont::create("Game Over!");
	Menu*mn = Menu::create(item, NULL);
	mn->alignItemsVertically();
	this->addChild(mn);

	//返回菜单项
	auto backMenuItem = MenuItemImage::create("public/return_btn0.png", "public/return_btn1.png",//Norma instead of Normal for a strange bug
		CC_CALLBACK_1(GameOverScene::menuBackCallback, this));
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 121 / 2, visibleSize.height - 38 / 2)));//Size 121*38
	auto menu = Menu::create(backMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 0);

	return true;
}

void GameOverScene::menuBackCallback(Ref* pSender)
{
	SceneManager::getInstance()->changeScene(SceneManager::en_MenuScene);  //返回到主菜单场景
}