#include "RelationScene.h"

Scene *RelationScene::createScene()
{
	auto scene = Scene::create();

	auto layer = RelationScene::create();

	scene->addChild(layer);

	return scene;
}

bool RelationScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize(); //������ͼ�Ŀ��ӻ��ߴ�
	Vec2 origin = Director::getInstance()->getVisibleOrigin();   //������ͼ�Ŀ��ӻ�ԭ��

	auto bg = Sprite::create("public/background.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);

	//���ز˵���
	auto backMenuItem = MenuItemImage::create("public/return_btn0.png", "public/return_btn1.png",//Norma instead of Normal for a strange bug
		CC_CALLBACK_1(RelationScene::menuBackCallback, this));
	backMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width - 121 / 2, visibleSize.height - 38 / 2)));//Size 121*38

	auto menu2 = Menu::create(backMenuItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 0);

	return true;
}

void RelationScene::menuBackCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void RelationScene::onExit()
{

};