#ifndef _SceneManager_H_
#define _SceneManager_H_

#include "cocos2d.h"
USING_NS_CC;

/*���������������������������ת�����ֳ���δʵ��*/

class SceneManager : public Ref
{
public:
	enum SceneType
	{
		en_MenuScene,		//��������
		en_GameScene,		//��Ϸ����
		en_SettingScene,    //���ó��� 
		en_PlayerScene,		//�����Ϣ����
		en_ExitScene,		//�˳�����
		en_EnterScene,		//��Ϸ���볡��
		en_GameOverScene,	//��Ϸ��������	
		en_RankScene,		//���а񳡾�
	};

	static SceneManager* getInstance();
	virtual bool init();

	void changeScene(SceneType enSceneType);
private:
	static SceneManager * s_SceneManager;

};


#endif


