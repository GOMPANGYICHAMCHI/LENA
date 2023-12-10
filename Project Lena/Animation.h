#pragma once
#include "Sprite.h"


/// <summary>
/// Animation
/// </summary>
class Animation
{
public :
	Animation();
	// ������Ʈ
	void Update(float deltatime);
	// ��������Ʈ �߰�
	void AddSprite(Sprite* sprite);
	// ������ ������ ����
	void SetDelayFrame(int input_delay);

public :
	// �ִϸ��̼� ���� ����
	// ( ������ �ƴҰ�� �ִϸ��̼� ����� ������ �����ӿ� ������ )
	bool isloop;

	// -----------------------------------------------------------------------
	// ��ƼŬ �ִϸ��̼� ���� Ÿ���� SpriteTranslate �ϰ��, 
	// �ش� ������ �ִϸ��̼� ��� ������ ���� ���Դϴ�
	// -----------------------------------------------------------------------
	int delayframe;				// �ִϸ��̼� ��� �ӵ� ( ������ ������ ) ������ ����
	// -----------------------------------------------------------------------

	int AnimationID;			// �ִϸ��̼� ���̵�
	vector<Sprite*> sprites;	// �ִϸ��̼� ��������Ʈ
};

