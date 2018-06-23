
#include"MenuScene.h"
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

	//���ò˵���
	MenuItemImage * settingMenuItem = MenuItemImage::create(
		"menuScene/menu_setting_btn.png",
		"menuScene/menu_setting_btn.png", CC_CALLBACK_1(MenuScene::menuSettingCallback, this));
	settingMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(760, 50)));

	auto menu3 = Menu::create(settingMenuItem, NULL);   //�����˵�����
	menu3->setPosition(Vec2::ZERO);             //����˵�����λ��
	this->addChild(menu3);    //�˵�������ӵ�����

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
