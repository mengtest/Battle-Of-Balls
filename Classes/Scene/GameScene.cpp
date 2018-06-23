 #include "GameScene.h"
#include "GameLayer.h"
#include"SceneManager.h"

enum GameZOrder  //设置子节点
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

	scene->addChild(layer);  //添加层到子节点

	return scene;
}


bool GameScene::init()  //初始化函数
{
	if (!Layer::init())
	{
		return false;
	}


	auto gameLayer = GameLayer::create();
	this->addChild(gameLayer, GAME_LAYER_Z);  //添加游戏层到子节点

	return true;
}
