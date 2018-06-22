#include "GameScene.h"
#include "GameLayer.h"
//#include"DataLayer.h"
#include"SceneManager.h"

enum GameZOrder  //设置场景子节点
{
	GAME_BACKGROUND_Z,
	GAME_LAYER_Z,
	GAME_DATA_Z,
	GAME_MENU_Z,
	GAME_SETTING_LAYER_Z
};

Scene * GameScene::createScene()  //创建场景
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}


bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	/*添加层到子节点*/
	auto spitItem = CheckBox::create(
		"gameScene/spit_btn.png",
		"gameScene/spit_btn.png");
	spitItem->setPosition(Vec2(650, 60));
	spitItem->setZoomScale(-0.1f);
	spitItem->addEventListener(CC_CALLBACK_2(GameScene::menuSpitCallback, this));
	this->addChild(spitItem, GAME_MENU_Z);

	auto divideItem = CheckBox::create(
		"gameScene/divide_btn.png",
		"gameScene/divide_btn.png");
	divideItem->setPosition(Vec2(720, 60));
	divideItem->setZoomScale(-0.1f);
	divideItem->addEventListener(CC_CALLBACK_2(GameScene::menuDivideCallback, this));
	this->addChild(divideItem, GAME_MENU_Z);


	//auto dataLayer = DataLayer::create();
	//this->addChild(dataLayer, GAME_DATA_Z);

	auto gameLayer = GameLayer::create();
	this->addChild(gameLayer, GAME_LAYER_Z);

	return true;
}

void GameScene::menuSpitCallback(Ref * pSender, CheckBox::EventType type)  //吐孢子菜单回调
{
	_eventDispatcher->dispatchCustomEvent("Spit");
}

void GameScene::menuDivideCallback(Ref * pSender, CheckBox::EventType type)  //分身菜单回调
{
	_eventDispatcher->dispatchCustomEvent("Divide");
}
