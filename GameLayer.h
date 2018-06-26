#ifndef _GameLayer_H_
#define _GameLayer_H_

#include "cocos2d.h"
#include "AppMacros.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class Joystick;
class Player;
class Bean;
class Spore;
class Prick;

class GameLayer : public Layer {
public:
	GameLayer();
	~GameLayer();

	virtual bool init();

	CREATE_FUNC(GameLayer);

	virtual void onExit();

	bool onTouchBegan(Touch * touch, Event * event);
	void onTouchMoved(Touch * touch, Event * event);
	void onTouchEnded(Touch * touch, Event * event);
	void onTouchCancelled(Touch * touch, Event * event);

	void keyPressed(EventKeyboard::KeyCode keyCode, Event*event);
	void keyReleased(EventKeyboard::KeyCode keyCode, Event*event);

	virtual void update(float dt);		//��Ϸ��ÿ֡����

													/*�������Ժ���*/
	void initDataDefault();
	void initRival();
	void initPlayer();
	void initBean();

	void updateView();		//������Ϸ��ͼ
	void updateBean();		//���¶���
	void updateSpore();		//��������
	void updatePrick();		//�����̴�
	void updateRival();		//���¶���

	void updateScore(float dt);		//���·���

									/*�������Ժ���*/
	void startAddPrick(float dt);
	void addPrick(float dt);

	void collideBean(Player * player);		//�붹�ӵ���ײ���
	void collide();			//��ײ���

	void resetBean(Node * node);		//���ö���
	void resetPlayer();

private:
	Joystick * _joystick;		//����ҡ��
	Node * _map;				//��ͼ
	Player * _player;			//���

	Player * _player1;			//���
	Map<std::string, Player *> _rivalMap;		//�����б�
	Vector<Bean *> _beanList;			//�����б�
	Map<int, Prick *> _prickMap;			//�̴�
	Map<int, Spore *> _sporeMap;			//����
	std::vector<int> _vecSporeNeedUpdate;			//��Ҫ���µ�����
	float _mapScale;			//��ͼ��������
	float _timeCount;
	int _mode;			//��Ϸģʽ
	Label * _scoreLabel;

};

#endif
