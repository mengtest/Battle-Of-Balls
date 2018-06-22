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
auto scene = Scene::create();  //����һ������

auto layer = MenuScene::create();

scene->addChild(layer);

return scene;
}
auto audio = SimpleAudioEngine::getInstance();

bool MenuScene::init()
{
	if (!Layer::init())   //��ʼ��
		return false;

	audio->preloadBackgroundMusic("music/bg1.mp3");
	if (audio->isBackgroundMusicPlaying() == true);
	else
		audio->playBackgroundMusic("music/bg1.mp3", true);


	Size visibleSize = Director::getInstance()->getVisibleSize(); //������ͼ�Ŀ��ӻ��ߴ�
	Vec2 origin = Director::getInstance()->getVisibleOrigin();   //������ͼ�Ŀ��ӻ�ԭ��

	auto bg = Sprite::create("menuScene/menu_background.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	this->addChild(bg);

	//��ʼ����
	Sprite * startSpriteNormal = Sprite::create("menuScene/menu_start_btn.png");
	Sprite * startSpriteSelected = Sprite::create("menuScene/menu_start_btn.png");
	MenuItemSprite * startMenuItem = MenuItemSprite::create(startSpriteNormal,
		startSpriteSelected, CC_CALLBACK_1(MenuScene::menuGameStartCallback, this));
	startMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	//�˳���ϷͼƬ�˵���
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png", CC_CALLBACK_1(MenuScene::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	auto menu1 = Menu::create(startMenuItem, closeItem, NULL);   //�����˵�����
	menu1->setPosition(Vec2::ZERO);             //����˵�����λ��
	this->addChild(menu1);    //�˵�������ӵ�����


	//��սģʽ�˵���
	auto teamModeItem = MenuItemImage::create(
		"menuScene/menu_teamMode_btn.png",
		"menuScene/menu_teamMode_btn.png",
		CC_CALLBACK_1(MenuScene::menuTeamModeCallback, this));
	teamModeItem->setPosition(580, 300);

	//����ģʽ�˵���
	auto survivalModeItem = MenuItemImage::create(
		"menuScene/menu_survivalMode_btn.png",
		"menuScene/menu_survivalMode_btn.png",
		CC_CALLBACK_1(MenuScene::menuSurvivalModeCallback, this));
	survivalModeItem->setPosition(580, 150);

	//�Զ���ģʽ�˵���
	auto customModeItem = MenuItemImage::create(
		"menuScene/menu_customMode_btn.png",
		"menuScene/menu_customMode_btn.png",
		CC_CALLBACK_1(MenuScene::menuCustomModeCallback, this));
	customModeItem->setPosition(580, 221);

	auto menu2 = Menu::create(teamModeItem, survivalModeItem, customModeItem, NULL);   //�����˵�����
	menu2->setPosition(Vec2::ZERO);             //����˵�����λ��
	this->addChild(menu2);    //�˵�������ӵ�����
	//���ò˵���
	MenuItemImage * settingMenuItem = MenuItemImage::create(
		"menuScene/menu_setting_btn.png",
		"menuScene/menu_setting_btn.png", CC_CALLBACK_1(MenuScene::menuSettingCallback, this));
	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 50)));

	//������²˵���
	MenuItemImage * storyMenuItem = MenuItemImage::create(
		"menuScene/menu_story_btn.png",
		"menuScene/menu_story_btn.png", CC_CALLBACK_1(MenuScene::menuStoryCallback, this));
	storyMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 150)));

	//�ۿ������˵���
	auto watchGameItem = MenuItemImage::create(
		"menuScene/menu_watchGame_btn.png",
		"menuScene/menu_watchGame_btn.png",
		CC_CALLBACK_1(MenuScene::menuWatchGameCallback, this));
	watchGameItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 250)));
	//���Բ˵���
	auto strategyItem = MenuItemImage::create(
		"menuScene/menu_strategy_btn.png",
		"menuScene/menu_strategy_btn.png",
		CC_CALLBACK_1(MenuScene::menuStrategyCallback, this));
	strategyItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 350)));

	auto menu3 = Menu::create(storyMenuItem, settingMenuItem, watchGameItem, strategyItem, NULL);   //�����˵�����
	menu3->setPosition(Vec2::ZERO);             //����˵�����λ��
	this->addChild(menu3);    //�˵�������ӵ�����

	//ս�Ӳ˵���
	auto teamItem = MenuItemImage::create(
		"menuScene/menu_team_btn0.png",
		"menuScene/menu_team_btn1.png",
		CC_CALLBACK_1(MenuScene::menuTeamCallback, this));
	teamItem->setPosition(Director::getInstance()->convertToGL(Vec2(100, 400)));

	//��ϵ�˵���
	auto relationItem = MenuItemImage::create(
		"menuScene/menu_relation_btn0.png",
		"menuScene/menu_relation_btn1.png",
		CC_CALLBACK_1(MenuScene::menuRelationCallback, this));
	relationItem->setPosition(Director::getInstance()->convertToGL(Vec2(250, 400)));

	//���а�˵���
	MenuItemImage * rankMenuItem = MenuItemImage::create(
		"menuScene/menu_rank_btn0.png",
		"menuScene/menu_rank_btn1.png", CC_CALLBACK_1(MenuScene::menuRankCallback, this));
	rankMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(400, 400)));

	//�̵�˵���
	auto storeItem = MenuItemImage::create(
		"menuScene/menu_store_btn0.png",
		"menuScene/menu_store_btn1.png",
		CC_CALLBACK_1(MenuScene::menuStoreCallback, this));
	storeItem->setPosition(Director::getInstance()->convertToGL(Vec2(550, 400)));

	auto menu4 = Menu::create(teamItem, relationItem, rankMenuItem, storeItem, NULL);   //�����˵�����
	menu4->setPosition(Vec2::ZERO);             //����˵�����λ��
	this->addChild(menu4);    //�˵�������ӵ�����

	//���ͷ��
	auto playerItem = MenuItemImage::create(
		"menuScene/menu_player_btn.png",
		"menuScene/menu_player_btn.png",
		CC_CALLBACK_1(MenuScene::menuPlayerCallback, this));
	playerItem->setPosition(82, 412);

	//����ǳ�


	auto menu5 = Menu::create(playerItem, NULL);   //�����˵�����
	menu5->setPosition(Vec2::ZERO);             //����˵�����λ��
	this->addChild(menu5);    //�˵�������ӵ�����

	return true;
}

void MenuScene::menuGameStartCallback(Ref * pSender)  //��Ϸ��ʼ��ť�ص�
{

	auto sc = GameScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->replaceScene(reScene);
};


void MenuScene::menuSettingCallback(Ref * pSender)   //���ð�ť�ص�
{
	auto sc = SettingScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};

void MenuScene::menuStoryCallback(Ref * pSender)  //������°�ť�ص�
{
	auto sc=StoryScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};

void MenuScene::menuWatchGameCallback(Ref * pSender)  //�ۿ�������ť�ص�
{
	//auto sc = WatchGameScene::createScene();
//	auto reScene = TransitionCrossFade::create(0.5f, sc);
	//Director::getInstance()->pushScene(reScene);
}

void MenuScene::menuStrategyCallback(Ref * pSender)  //���԰�ť�ص�
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
void MenuScene::menuTeamCallback(Ref * pSender)  //ս�Ӱ�ť�ص�
{
	auto sc = TeamScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};

void MenuScene::menuRelationCallback(Ref * pSender)  //��ϵ��ť�ص�
{
	auto sc = RelationScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};
void MenuScene::menuRankCallback(Ref * pSender)  //���а�ť�ص�
{
	auto sc = RankScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
};

void MenuScene::menuStoreCallback(Ref * pSender)    //�̵갴ť�ص�
{
	auto sc = StoreScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
}


void MenuScene::menuPlayerCallback(Ref * pSender)   //�����Ϣ��ť�ص�
{
	auto sc = PlayerScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->pushScene(reScene);
}


void MenuScene::menuCloseCallback(Ref*pSender)   //�˳���Ϸ��ť�ص�
{
	Director::getInstance()->end();

#if(CC_TARGET_PLATFORM==CC_PLANTFORM_IOS)
	exit(0);
#endif
};

void MenuScene::onExit()
{

};
