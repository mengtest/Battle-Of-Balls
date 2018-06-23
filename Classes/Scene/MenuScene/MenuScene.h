#ifndef _MenuScene_H_
#define _MenuScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;

class MenuScene : Layer {


public:

	static Scene* createScene();  	// 创建对象

	virtual bool init();   	// 对象初始化

	virtual void onExit();

	CREATE_FUNC(MenuScene);

	void menuGameStartCallback(Ref * pSender);    //游戏开始按钮回调

	void menuSettingCallback(Ref * pSender);    //设置按钮回调

	void menuCloseCallback(Ref*pSender);    //退出游戏按钮回调




};

#endif

