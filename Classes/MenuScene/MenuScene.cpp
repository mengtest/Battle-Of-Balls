#pragma execution_character_set("utf-8")
#include"MenuScene.h"
#include "StoryScene.h"
#include "StoreScene.h"
#include "RankScene.h"
#include "RelationScene.h"
#include "TeamScene.h"
#include "PlayerScene.h"
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


	//团战模式菜单项
	auto teamModeItem = MenuItemImage::create(
		"menuScene/menu_teamMode_btn.png",
		"menuScene/menu_teamMode_btn.png",
		CC_CALLBACK_1(MenuScene::menuTeamModeCallback, this));
	teamModeItem->setPosition(580, 300);

	//生存模式菜单项
	auto survivalModeItem = MenuItemImage::create(
		"menuScene/menu_survivalMode_btn.png",
		"menuScene/menu_survivalMode_btn.png",
		CC_CALLBACK_1(MenuScene::menuSurvivalModeCallback, this));
	survivalModeItem->setPosition(580, 150);

	//自定义模式菜单项
	auto customModeItem = MenuItemImage::create(
		"menuScene/menu_customMode_btn.png",
		"menuScene/menu_customMode_btn.png",
		CC_CALLBACK_1(MenuScene::menuCustomModeCallback, this));
	customModeItem->setPosition(580, 221);

	auto menu2 = Menu::create(teamModeItem, survivalModeItem, customModeItem, NULL);   //创建菜单对象
	menu2->setPosition(Vec2::ZERO);             //定义菜单对象位置
	this->addChild(menu2);    //菜单对象添加到层中
	//设置菜单项
	MenuItemImage * settingMenuItem = MenuItemImage::create(
		"menuScene/menu_setting_btn.png",
		"menuScene/menu_setting_btn.png", CC_CALLBACK_1(MenuScene::menuSettingCallback, this));
	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 50)));

	//球球故事菜单项
	MenuItemImage * storyMenuItem = MenuItemImage::create(
		"menuScene/menu_story_btn.png",
		"menuScene/menu_story_btn.png", CC_CALLBACK_1(MenuScene::menuStoryCallback, this));
	storyMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 150)));

	//观看比赛菜单项
	auto watchGameItem = MenuItemImage::create(
		"menuScene/menu_watchGame_btn.png",
		"menuScene/menu_watchGame_btn.png",
		CC_CALLBACK_1(MenuScene::menuWatchGameCallback, this));
	watchGameItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 250)));
	//攻略菜单项
	auto strategyItem = MenuItemImage::create(
		"menuScene/menu_strategy_btn.png",
		"menuScene/menu_strategy_btn.png",
		CC_CALLBACK_1(MenuScene::menuStrategyCallback, this));
	strategyItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 350)));

	auto menu3 = Menu::create(storyMenuItem, settingMenuItem, watchGameItem, strategyItem, NULL);   //创建菜单对象
	menu3->setPosition(Vec2::ZERO);             //定义菜单对象位置
	this->addChild(menu3);    //菜单对象添加到层中

	//战队菜单项
	auto teamItem = MenuItemImage::create(
		"menuScene/menu_team_btn0.png",
		"menuScene/menu_team_btn1.png",
		CC_CALLBACK_1(MenuScene::menuTeamCallback, this));
	teamItem->setPosition(Director::getInstance()->convertToGL(Vec2(100, 400)));

	//关系菜单项
	auto relationItem = MenuItemImage::create(
		"menuScene/menu_relation_btn0.png",
		"menuScene/menu_relation_btn1.png",
		CC_CALLBACK_1(MenuScene::menuRelationCallback, this));
	relationItem->setPosition(Director::getInstance()->convertToGL(Vec2(250, 400)));

	//排行榜菜单项
	MenuItemImage * rankMenuItem = MenuItemImage::create(
		"menuScene/menu_rank_btn0.png",
		"menuScene/menu_rank_btn1.png", CC_CALLBACK_1(MenuScene::menuRankCallback, this));
	rankMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(400, 400)));

	//商店菜单项
	auto storeItem = MenuItemImage::create(
		"menuScene/menu_store_btn0.png",
		"menuScene/menu_store_btn1.png",
		CC_CALLBACK_1(MenuScene::menuStoreCallback, this));
	storeItem->setPosition(Director::getInstance()->convertToGL(Vec2(550, 400)));

	auto menu4 = Menu::create(teamItem, relationItem, rankMenuItem, storeItem, NULL);   //创建菜单对象
	menu4->setPosition(Vec2::ZERO);             //定义菜单对象位置
	this->addChild(menu4);    //菜单对象添加到层中

	//玩家头像
	auto playerItem = MenuItemImage::create(
		"menuScene/menu_player_btn.png",
		"menuScene/menu_player_btn.png",
		CC_CALLBACK_1(MenuScene::menuPlayerCallback, this));
	playerItem->setPosition(82, 412);

	//随机昵称


	auto menu5 = Menu::create(playerItem, NULL);   //创建菜单对象
	menu5->setPosition(Vec2::ZERO);             //定义菜单对象位置
	this->addChild(menu5);    //菜单对象添加到层中

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

void MenuScene::menuStoryCallback(Ref * pSender)  //球球故事按钮回调
{
	auto sc=StoryScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};

void MenuScene::menuWatchGameCallback(Ref * pSender)  //观看比赛按钮回调
{
	//auto sc = WatchGameScene::createScene();
//	auto reScene = TransitionCrossFade::create(0.5f, sc);
	//Director::getInstance()->pushScene(reScene);
}

void MenuScene::menuStrategyCallback(Ref * pSender)  //攻略按钮回调
{

};

void MenuScene::menuSurvivalModeCallback(Ref * pSender)
{
	/*std::string text = CsvUtils::getInstance()->getMapData(7, 1, "i18n/public.csv");
	auto prompt = PromptBox::getInstance()->createPrompt(text);
	if (prompt)
	{
	this->addChild(prompt, MENU_PROMPT_Z);
	}
	/*auto layer = SurvivalModeLayer::create();
	this->addChild(layer, MENU_LAYER_Z);*/
}

void MenuScene::menuTeamModeCallback(Ref * pSender)
{
	/*std::string text = CsvUtils::getInstance()->getMapData(6, 1, "i18n/public.csv");
	auto prompt = PromptBox::getInstance()->createPrompt(text);
	if (prompt)
	{
	this->addChild(prompt, MENU_PROMPT_Z);
	}
	/*auto layer = TeamModeLayer::create();
	this->addChild(layer, MENU_LAYER_Z);*/
}

void MenuScene::menuCustomModeCallback(Ref * pSender)
{

}
void MenuScene::menuTeamCallback(Ref * pSender)  //战队按钮回调
{
	auto sc = TeamScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};

void MenuScene::menuRelationCallback(Ref * pSender)  //关系按钮回调
{
	auto sc = RelationScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};
void MenuScene::menuRankCallback(Ref * pSender)  //排行榜按钮回调
{
	auto sc = RankScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};

void MenuScene::menuStoreCallback(Ref * pSender)    //商店按钮回调
{
	auto sc = StoreScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
}


void MenuScene::menuPlayerCallback(Ref * pSender)   //玩家信息按钮回调
{
	auto sc = PlayerScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
}


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
