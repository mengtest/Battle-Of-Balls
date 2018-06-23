
#include"MenuScene.h"
#include "SettingScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
Scene * MenuScene::createScene()
{
auto scene = Scene::create();  //创建一个场景

auto layer = MenuScene::create();

scene->addChild(layer);

return scene;
}
auto audio = SimpleAudioEngine::getInstance();

bool MenuScene::init()
{
	if (!Layer::init())   //初始化
		return false;

	audio->preloadBackgroundMusic("music/bg1.mp3");
	if (audio->isBackgroundMusicPlaying() == true);
	else
		audio->playBackgroundMusic("music/bg1.mp3", true);


	Size visibleSize = Director::getInstance()->getVisibleSize(); //定义视图的可视化尺寸
	Vec2 origin = Director::getInstance()->getVisibleOrigin();   //定义视图的可视化原点

	auto bg = Sprite::create("menuScene/menu_background.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);

	//开始精灵
	Sprite * startSpriteNormal = Sprite::create("menuScene/menu_start_btn.png");
	Sprite * startSpriteSelected = Sprite::create("menuScene/menu_start_btn.png");
	MenuItemSprite * startMenuItem = MenuItemSprite::create(startSpriteNormal,
		startSpriteSelected, CC_CALLBACK_1(MenuScene::menuGameStartCallback, this));
	startMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	//退出游戏图片菜单项
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png", CC_CALLBACK_1(MenuScene::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	auto menu1 = Menu::create(startMenuItem, closeItem, NULL);   //创建菜单对象
	menu1->setPosition(Vec2::ZERO);             //定义菜单对象位置
	this->addChild(menu1);    //菜单对象添加到层中

	//设置菜单项
	MenuItemImage * settingMenuItem = MenuItemImage::create(
		"menuScene/menu_setting_btn.png",
		"menuScene/menu_setting_btn.png", CC_CALLBACK_1(MenuScene::menuSettingCallback, this));
	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 50)));

	auto menu3 = Menu::create(settingMenuItem, NULL);   //创建菜单对象
	menu3->setPosition(Vec2::ZERO);             //定义菜单对象位置
	this->addChild(menu3);    //菜单对象添加到层中

	return true;
}

void MenuScene::menuGameStartCallback(Ref * pSender)  //游戏开始按钮回调
{

	auto sc = GameScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->replaceScene(reScene);
};


void MenuScene::menuSettingCallback(Ref * pSender)   //设置按钮回调
{
	auto sc = SettingScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};


void MenuScene::menuCloseCallback(Ref*pSender)   //退出游戏按钮回调
{
	Director::getInstance()->end();

#if(CC_TARGET_PLATFORM==CC_PLANTFORM_IOS)
	exit(0);
#endif
};

void MenuScene::onExit()
{

};
