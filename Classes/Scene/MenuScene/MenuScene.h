#ifndef _MenuScene_H_
#define _MenuScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;

class MenuScene : Layer {


public:

	static Scene* createScene();  	// ��������

	virtual bool init();   	// �����ʼ��

	virtual void onExit();

	CREATE_FUNC(MenuScene);

	void menuGameStartCallback(Ref * pSender);    //��Ϸ��ʼ��ť�ص�

	void menuSettingCallback(Ref * pSender);    //���ð�ť�ص�

	void menuCloseCallback(Ref*pSender);    //�˳���Ϸ��ť�ص�




};

#endif

