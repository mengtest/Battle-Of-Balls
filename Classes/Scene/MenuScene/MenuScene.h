#ifndef _MenuScene_H_
#define _MenuScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;

class MenuScene : Layer {

private:

	//	TextField * _gameName;	//游戏名输入框

public:

	static Scene* createScene();  	// 创建对象

	virtual bool init();   	// 对象初始化

	virtual void onExit();

	CREATE_FUNC(MenuScene);

	void menuGameStartCallback(Ref * pSender);    //游戏开始按钮回调

	void menuSettingCallback(Ref * pSender);    //设置按钮回调

	void menuStoryCallback(Ref * pSender);    //球球故事按钮回调

	void menuWatchGameCallback(Ref * pSender);    //观看比赛按钮回调

	void menuStrategyCallback(Ref * pSender);    //攻略按钮回调


	void menuSurvivalModeCallback(Ref * pSender);	//生存模式按钮回调

	void menuTeamModeCallback(Ref * pSender);		//团战模式按钮回调

	void menuCustomModeCallback(Ref * pSender);		//自定义模式按钮回调


	void menuTeamCallback(Ref * pSender);      //战队按钮回调

	void menuRelationCallback(Ref * pSender);      //关系按钮回调

	void menuRankCallback(Ref * pSender);    //排行榜按钮回调

	void menuStoreCallback(Ref * pSender);    //商店按钮回调



	void menuPlayerCallback(Ref * pSender);  //玩家头像按钮回调

											 //void gameNameEvent(Ref * pSender, TextField::EventType type);

	void menuCloseCallback(Ref*pSender);    //退出游戏按钮回调




};

#endif

