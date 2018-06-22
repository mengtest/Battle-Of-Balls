#ifndef _MenuScene_H_
#define _MenuScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;

class MenuScene : Layer {

private:

	//	TextField * _gameName;	//��Ϸ�������

public:

	static Scene* createScene();  	// ��������

	virtual bool init();   	// �����ʼ��

	virtual void onExit();

	CREATE_FUNC(MenuScene);

	void menuGameStartCallback(Ref * pSender);    //��Ϸ��ʼ��ť�ص�

	void menuSettingCallback(Ref * pSender);    //���ð�ť�ص�

	void menuStoryCallback(Ref * pSender);    //������°�ť�ص�

	void menuWatchGameCallback(Ref * pSender);    //�ۿ�������ť�ص�

	void menuStrategyCallback(Ref * pSender);    //���԰�ť�ص�


	void menuSurvivalModeCallback(Ref * pSender);	//����ģʽ��ť�ص�

	void menuTeamModeCallback(Ref * pSender);		//��սģʽ��ť�ص�

	void menuCustomModeCallback(Ref * pSender);		//�Զ���ģʽ��ť�ص�


	void menuTeamCallback(Ref * pSender);      //ս�Ӱ�ť�ص�

	void menuRelationCallback(Ref * pSender);      //��ϵ��ť�ص�

	void menuRankCallback(Ref * pSender);    //���а�ť�ص�

	void menuStoreCallback(Ref * pSender);    //�̵갴ť�ص�



	void menuPlayerCallback(Ref * pSender);  //���ͷ��ť�ص�

											 //void gameNameEvent(Ref * pSender, TextField::EventType type);

	void menuCloseCallback(Ref*pSender);    //�˳���Ϸ��ť�ص�




};

#endif

