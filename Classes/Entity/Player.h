#ifndef _Player_H_
#define _Player_H_

#include "cocos2d.h"

USING_NS_CC;

class Prick;
class PlayerDivision;
class Bean;
class Spore;

class Player :public Node {
public:

	enum class State
	{
		NORMAL, //正常状态
		DIVIDE,  //分身状态
		SPIT,  //吐球状态
		CONCENTRATE, //主动中合状态
	};

	Player();
	~Player();

	static Player * create(Vec2  position, int keywordID, Node * map);
	bool init(Vec2  position, int keywordID, Node * map);

	//static Player * create(std::string & name, int keywordID, Vec2 velocity, Node * map);
	//bool init(std::string & name, int keywordID, Vec2 velocity, Node * map);

	virtual void onExit();

	PlayerDivision * createDivision(Vec2 position, Vec2 velocity, int score);

	int countSpitSporeNum();		//计算玩家一次可以吐出几个孢子
	void spitSpore(Node * map, Map<int, Spore *> & sporeMap);		//吐孢子
	void dividePlayer();		//分身

	bool collideBean(Bean * bean);		//与豆子的碰撞检测
	bool collideSpore(Spore * spore);		//与孢子的碰撞检测
	bool collidePrick(Prick *prick);		//与绿刺的碰撞检测
	bool collideRival(Player *rival);		//与对手的碰撞检测
	int collideDivision(PlayerDivision * division);		//与对手分身的碰撞

	void updateDivision();		//玩家信息更新
	void resetPlayer();			//玩家复活

	void divideFinish();		//分身结束回调
	void concentrate();			//玩家中合
	void setConcentrate();		//设置中合状态
	void collidePrickFinish();	//碰到刺回调

	void setVelocity(Vec2 v);	//设置速度
	Vec2 getVelocity();			//获取速度

	Rect getPlayerRect();		//获取玩家包围盒
	void setCombine(float dt);
	int getDivisionNum();		//获取当前分身数量
	float getTotalWeight();
	std::string getPlayerName();
	int getTotalScore();
	Vector<PlayerDivision *> & getDivisionList();


	void clearDivisionList();
private:
	Vector<PlayerDivision *> _divisionList;		//玩家分身列表
	int _divisionNum;							//分身数量

	Node * _map;			//地图指针
	std::string _playerName;		//玩家名

	Vec2 _velocity;			//速度
	int _color;			//颜色,没有关键字时可用
	int _keywordID;
	State _state;		//玩家状态
	bool _combineEnable; //分身是否可以合体
};
#endif