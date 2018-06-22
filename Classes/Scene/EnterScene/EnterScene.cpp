#include "EnterScene.h"
#include "MenuScene.h"

Scene * EnterScene::createScene()
{
	auto scene = Scene::create();  //����һ������

	auto layer = EnterScene::create();

	scene->addChild(layer);

	return scene;
}

EnterScene::EnterScene()
{
	_timer = NULL;
	_loadingLabel = NULL;
}

EnterScene::~EnterScene()
{

}

bool EnterScene::init()  //��ʼ��enterscene��
{
	if (!Layer::init())  //��ʼ��layer
	{
		return false;
	}

	Size size = CCDirector::getInstance()->getVisibleSize(); //������ͼ�Ŀ��ӻ��ߴ�

	auto title = Sprite::create("enterScene/enter_background0.png");  //�����������title
	title->setPosition(Vec2(size.width / 2, size.height / 2));   //��title��������Ļ�м�
	title->setOpacity(0);   //����͸����

	auto fadeIn = FadeIn::create(0.5);   //����һ��FadeIn������ִ�иö����Ľڵ���0.5���ڵ���
	auto fadeOut = FadeOut::create(1.0);  //����һ��FadeIn������ִ�иö����Ľڵ���0.5���ڵ���
	auto callFunc = CallFunc::create(CC_CALLBACK_0(EnterScene::startLoding, this));
	auto seq = Sequence::create(fadeIn, fadeOut, callFunc, NULL);  //����һ��˳����

	title->runAction(seq);
	this->addChild(title);

	return true;
}

void EnterScene::startLoding()    //��ʼ������Դ
{
	auto visiblesize = CCDirector::getInstance()->getVisibleSize();   //������ͼ�Ŀ��ӻ��ߴ�

	auto background = Sprite::create("enterScene/enter_background1.png"); //���ñ���ͼƬ
	background->setPosition(visiblesize.width / 2, visiblesize.height / 2);   //����λ��
	this->addChild(background);
	//���ý�����
	auto loadingBackground = Sprite::create("enterScene/enter_loadingBar0.png");
	loadingBackground->setPosition(400, 40);
	this->addChild(loadingBackground);

	_loadingLabel = Label::createWithTTF("loding:0%%", "fonts/arial.ttf", 20);
	_loadingLabel->setColor(Color3B(0, 0, 0));
	_loadingLabel->setPosition(400, 55);
	this->addChild(_loadingLabel, 1);

	auto loadingSprite = Sprite::create("enterScene/enter_loadingBar1.png");
	_timer = ProgressTimer::create(loadingSprite);
	_timer->setPosition(400, 40);               //λ��
	_timer->setType(ProgressTimerType::BAR);   //�������ν�����
	_timer->setMidpoint(Vec2(0, 1));         //���ý�����Ϊ������
	_timer->setBarChangeRate(Vec2(1, 0));    //���ý��������ؽ���Ϊ�᷽��
	_timer->setPercentage(0);              //��ʼ�ٷֱ�
	this->addChild(_timer, 1);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("public/bean.plist", "public/bean.png");
	this->scheduleUpdate();       //��ʼ��ʱ��
}

void EnterScene::update(float delta)
{
	float percent = _timer->getPercentage();
	percent += 0.5f;
	_timer->setPercentage(percent);

	if (percent <= 100)
	{
		auto str1 = String::createWithFormat("loading:%d%%", int(percent));
		_loadingLabel->setString(str1->getCString());
	}


	else
	{
		this->unscheduleUpdate();     //ֹͣ��ʱ��
		this->scheduleOnce(schedule_selector(EnterScene::enterMenuScene), 1.0f);  //һ���ִ��entermenuscene����

	}
}

void EnterScene::enterMenuScene(float dt)   //�������˵�
{
	auto sc = MenuScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->replaceScene(reScene); //ת�������˵�����
}

void EnterScene::onExit()
{
	this->unscheduleAllCallbacks();  //ɾ����ʱ��
	Layer::onExit();
}