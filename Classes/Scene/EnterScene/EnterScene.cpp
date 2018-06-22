#include "EnterScene.h"
#include "MenuScene.h"

Scene * EnterScene::createScene()
{
	auto scene = Scene::create();  //创建一个场景

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

bool EnterScene::init()  //初始化enterscene层
{
	if (!Layer::init())  //初始化layer
	{
		return false;
	}

	Size size = CCDirector::getInstance()->getVisibleSize(); //定义视图的可视化尺寸

	auto title = Sprite::create("enterScene/enter_background0.png");  //创建精灵对象title
	title->setPosition(Vec2(size.width / 2, size.height / 2));   //将title设置在屏幕中间
	title->setOpacity(0);   //设置透明度

	auto fadeIn = FadeIn::create(0.5);   //创建一个FadeIn动作，执行该动作的节点在0.5秒内淡入
	auto fadeOut = FadeOut::create(1.0);  //创建一个FadeIn动作，执行该动作的节点在0.5秒内淡出
	auto callFunc = CallFunc::create(CC_CALLBACK_0(EnterScene::startLoding, this));
	auto seq = Sequence::create(fadeIn, fadeOut, callFunc, NULL);  //创建一个顺序动作

	title->runAction(seq);
	this->addChild(title);

	return true;
}

void EnterScene::startLoding()    //开始加载资源
{
	auto visiblesize = CCDirector::getInstance()->getVisibleSize();   //定义视图的可视化尺寸

	auto background = Sprite::create("enterScene/enter_background1.png"); //设置背景图片
	background->setPosition(visiblesize.width / 2, visiblesize.height / 2);   //设置位置
	this->addChild(background);
	//设置进度条
	auto loadingBackground = Sprite::create("enterScene/enter_loadingBar0.png");
	loadingBackground->setPosition(400, 40);
	this->addChild(loadingBackground);

	_loadingLabel = Label::createWithTTF("loding:0%%", "fonts/arial.ttf", 20);
	_loadingLabel->setColor(Color3B(0, 0, 0));
	_loadingLabel->setPosition(400, 55);
	this->addChild(_loadingLabel, 1);

	auto loadingSprite = Sprite::create("enterScene/enter_loadingBar1.png");
	_timer = ProgressTimer::create(loadingSprite);
	_timer->setPosition(400, 40);               //位置
	_timer->setType(ProgressTimerType::BAR);   //设置条形进度条
	_timer->setMidpoint(Vec2(0, 1));         //设置进度条为从左到右
	_timer->setBarChangeRate(Vec2(1, 0));    //设置进度条加载进度为横方向
	_timer->setPercentage(0);              //开始百分比
	this->addChild(_timer, 1);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("public/bean.plist", "public/bean.png");
	this->scheduleUpdate();       //开始计时器
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
		this->unscheduleUpdate();     //停止计时器
		this->scheduleOnce(schedule_selector(EnterScene::enterMenuScene), 1.0f);  //一秒后执行entermenuscene函数

	}
}

void EnterScene::enterMenuScene(float dt)   //进入主菜单
{
	auto sc = MenuScene::createScene();
	auto reScene = TransitionCrossFade::create(0.5f, sc);
	Director::getInstance()->replaceScene(reScene); //转换到主菜单场景
}

void EnterScene::onExit()
{
	this->unscheduleAllCallbacks();  //删除计时器
	Layer::onExit();
}