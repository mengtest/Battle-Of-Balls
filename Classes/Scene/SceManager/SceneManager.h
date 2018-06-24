#ifndef _SceneManager_H_
#define _SceneManager_H_

#include "cocos2d.h"
USING_NS_CC;

/*场景管理器，负责各个场景的跳转，部分场景未实现*/

class SceneManager : public Ref
{
public:
	enum SceneType
	{
		en_MenuScene,		//大厅场景
		en_GameScene,		//游戏场景
		en_SettingScene,    //设置场景 
		en_PlayerScene,		//玩家信息场景
		en_ExitScene,		//退出场景
		en_EnterScene,		//游戏进入场景
		en_GameOverScene,	//游戏结束场景	
		en_RankScene,		//排行榜场景
	};

	static SceneManager* getInstance();
	virtual bool init();

	void changeScene(SceneType enSceneType);
private:
	static SceneManager * s_SceneManager;

};


#endif


