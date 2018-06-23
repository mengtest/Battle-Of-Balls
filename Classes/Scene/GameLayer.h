#ifndef _GameLayer_H_
#define _GameLayer_H_

#include "cocos2d.h"
#include "Common.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class Joystick;  //虚拟摇杆
class Player;   //玩家
class Bean;  //豆子
class Spore;  //孢子
class Prick;  //刺

class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	virtual void onExit();  //退出游戏图层

	bool onTouchBegan(Touch * touch, Event * event);
	void onTouchMoved(Touch * touch, Event * event);
	void onTouchEnded(Touch * touch, Event * event);
	void onTouchCancelled(Touch * touch, Event * event);
	
	 //键盘事件
	void keyPressed(EventKeyboard::KeyCode keyCode, Event*event);
	void keyReleased(EventKeyboard::KeyCode keyCode, Event*event);

	virtual void update(float dt);		//游戏中每帧更新


													/*单机调试函数*/
	void initDataDefault();
	void initRival();
	void initPlayer();
	void initBean();

	void updateView();		//更新游戏视图
	void updateBean();		//更新豆子
	void updateSpore();		//更新孢子
	void updatePrick();		//更新绿刺
	void updateRival();		//更新对手

	void updateScore(float dt);		//更新分数

									/*单机调试函数*/
	void startAddPrick(float dt);
	void addPrick(float dt);

	void collideBean(Player * player);		//与豆子的碰撞检测
	void collide();			//碰撞检测

	void spitSpore();		//吐孢子操作
	void dividePlayer();		//分身操作
	void resetBean(Node * node);		//重置豆子
	void resetPlayer();

private:
	Joystick * _joystick;		//虚拟摇杆
	Node * _map;				//地图
	Player * _player;			//玩家
	Map<std::string, Player *> _rivalMap;		//对手列表
	Vector<Bean *> _beanList;			//豆子列表
	Map<int, Prick *> _prickMap;			//绿刺
	Map<int, Spore *> _sporeMap;			//孢子
	std::vector<int> _vecSporeNeedUpdate;			//需要更新的孢子
	float _mapScale;			//地图缩放因子
	float _timeCount;  计时
	int _mode;			//游戏模式

};

#endif
