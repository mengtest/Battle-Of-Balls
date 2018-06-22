#include "PlayerScene.h"
#include "HomeLayer.h"
#include "PictureLayer.h"
#include "PlayerDataLayer.h"
#include "PlayerSkinLayer.h"
#include "MessageLayer.h"

enum PlayerZOrder
{
	PLAYER_BACKGROUND_Z,
	PLAYER_SPRITE_Z,
	PLAYER_LAYER_Z,
	PLAYER_MENU_Z
};

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

	_homeItem = CheckBox::create("playerScene/player_home_btn0.png",
		"playerScene/player_home_btn1.png");
	_homeItem->setPosition(Vec2(53, 378));
	_homeItem->setZoomScale(0);
	_homeItem->setSelected(true);
	_homeItem->addEventListener(CC_CALLBACK_2(PlayerScene::menuHomeCallback, this));
	this->addChild(_homeItem, PLAYER_MENU_Z);

	_pictureItem = CheckBox::create("playerScene/player_picture_btn0.png",
		"playerScene/player_picture_btn1.png");
	_pictureItem->setPosition(Vec2(53, 300));
	_pictureItem->setZoomScale(0);
	_pictureItem->addEventListener(CC_CALLBACK_2(PlayerScene::menuPictureCallback, this));
	this->addChild(_pictureItem, PLAYER_MENU_Z);

	_gameItem = CheckBox::create("playerScene/player_game_btn0.png",
		"playerScene/player_game_btn1.png");
	_gameItem->setPosition(Vec2(53, 222));
	_gameItem->setZoomScale(0);
	_gameItem->addEventListener(CC_CALLBACK_2(PlayerScene::menuGameCallback, this));
	this->addChild(_gameItem, PLAYER_MENU_Z);

	_skinItem = CheckBox::create("playerScene/player_skin_btn0.png",
		"playerScene/player_skin_btn1.png");
	_skinItem->setPosition(Vec2(53, 144));
	_skinItem->setZoomScale(0);
	_skinItem->addEventListener(CC_CALLBACK_2(PlayerScene::menuSkinCallback, this));
	this->addChild(_skinItem, PLAYER_MENU_Z);

	_messageItem = CheckBox::create("playerScene/player_message_btn0.png",
		"playerScene/player_message_btn1.png");
	_messageItem->setPosition(Vec2(53, 66));
	_messageItem->setZoomScale(0);
	_messageItem->addEventListener(CC_CALLBACK_2(PlayerScene::menuMessageCallback, this));
	this->addChild(_messageItem, PLAYER_MENU_Z);

	auto returnItem = MenuItemImage::create(
		"public/return_btn0.png",
		"public/return_btn1.png",
		CC_CALLBACK_1(PlayerScene::menuReturnCallback, this));
	returnItem->setPosition(753, 15);

	auto closeItem = MenuItemImage::create(
		"public/close.png",
		"public/close.png",
		CC_CALLBACK_1(PlayerScene::menuReturnCallback, this));
	closeItem->setPosition(777, 429);

	auto menu = Menu::create(returnItem, closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, PLAYER_MENU_Z);

	auto background1 = Sprite::create("public/background.png");
	background1->setPosition(400, 225);
	this->addChild(background1, PLAYER_BACKGROUND_Z);

	auto background2 = Sprite::create("playerScene/player_background.png");
	background2->setPosition(426, 221);
	this->addChild(background2, PLAYER_SPRITE_Z);

	_homeLayer = HomeLayer::create();
	this->addChild(_homeLayer, PLAYER_LAYER_Z);

	_pictureLayer = PictureLayer::create();
	_pictureLayer->setVisible(false);
	this->addChild(_pictureLayer, PLAYER_LAYER_Z);

	_playerDataLayer = PlayerDataLayer::create();
	_playerDataLayer->setVisible(false);
	this->addChild(_playerDataLayer, PLAYER_LAYER_Z);

	_playerSkinLayer = PlayerSkinLayer::create();
	_playerSkinLayer->setVisible(false);
	this->addChild(_playerSkinLayer, PLAYER_LAYER_Z);

	_messageLayer = MessageLayer::create();
	_messageLayer->setVisible(false);
	this->addChild(_messageLayer, PLAYER_LAYER_Z);

	return true;
}

void PlayerScene::menuHomeCallback(Ref * pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_pictureItem->setSelected(false);
		_gameItem->setSelected(false);
		_skinItem->setSelected(false);
		_messageItem->setSelected(false);

		_homeLayer->setVisible(true);
		_pictureLayer->setVisible(false);
		_playerDataLayer->setVisible(false);
		_playerSkinLayer->setVisible(false);
		_messageLayer->setVisible(false);
		break;
	case CheckBox::EventType::UNSELECTED:
		_homeItem->setSelected(true);
		break;
	default:
		break;
	}
}

void PlayerScene::menuPictureCallback(Ref * pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_homeItem->setSelected(false);
		_gameItem->setSelected(false);
		_skinItem->setSelected(false);
		_messageItem->setSelected(false);

		_homeLayer->setVisible(false);
		_pictureLayer->setVisible(true);
		_playerDataLayer->setVisible(false);
		_playerSkinLayer->setVisible(false);
		_messageLayer->setVisible(false);
		break;
	case CheckBox::EventType::UNSELECTED:
		_pictureItem->setSelected(true);
		break;
	default:
		break;
	}
}

void PlayerScene::menuGameCallback(Ref * pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_homeItem->setSelected(false);
		_pictureItem->setSelected(false);
		_skinItem->setSelected(false);
		_messageItem->setSelected(false);

		_homeLayer->setVisible(false);
		_pictureLayer->setVisible(false);
		_playerDataLayer->setVisible(true);
		_playerSkinLayer->setVisible(false);
		_messageLayer->setVisible(false);
		break;
	case CheckBox::EventType::UNSELECTED:
		_gameItem->setSelected(true);
		break;
	default:
		break;
	}
}

void PlayerScene::menuSkinCallback(Ref * pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_homeItem->setSelected(false);
		_pictureItem->setSelected(false);
		_gameItem->setSelected(false);
		_messageItem->setSelected(false);

		_homeLayer->setVisible(false);
		_pictureLayer->setVisible(false);
		_playerDataLayer->setVisible(false);
		_playerSkinLayer->setVisible(true);
		_messageLayer->setVisible(false);
		break;
	case CheckBox::EventType::UNSELECTED:
		_skinItem->setScale(true);
		break;
	default:
		break;
	}
}

void PlayerScene::menuMessageCallback(Ref * pSender, CheckBox::EventType type)
{
	switch (type)
	{
	case CheckBox::EventType::SELECTED:
		_homeItem->setSelected(false);
		_pictureItem->setSelected(false);
		_gameItem->setSelected(false);
		_skinItem->setSelected(false);

		_homeLayer->setVisible(false);
		_pictureLayer->setVisible(false);
		_playerDataLayer->setVisible(false);
		_playerSkinLayer->setVisible(false);
		_messageLayer->setVisible(true);
		break;
	case CheckBox::EventType::UNSELECTED:
		_messageItem->setSelected(true);
		break;
	default:
		break;
	}
}

void PlayerScene::menuReturnCallback(Ref * pSender)
{
	Director::getInstance()->popScene();
}