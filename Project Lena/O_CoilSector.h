#pragma once
#include "Actor.h"
#include "MapObjectPoolSize.h"
#include "CC_CoilAnimator.h"

class Sprite;
class GameScene;
class GameStateManager;

class O_CoilSector
{
public:
	O_CoilSector();

	// ���� ���� �ʱ�ȭ (���� ����, ������� �߰�, ������Ʈ �߰�, ������Ʈ ����)
	void Initialize(
		std::shared_ptr<GameScene> currentScene, 
		std::vector<Animation*> input_animation,
		GameStateManager* input_gamestate,
		Sprite* input_coilplatform);

	// ���� ���� ���� ��ȯ
	std::shared_ptr<Actor> GetActor();

	// ���� ���� ����
	// �Է°� : ���� ����
	void SetCoil(bool input_set);

	// �� ���� ���� ���Ϸμ���
	void SetVoidSecCoil(bool input_set);

	// ��ġ ����
	void SetPosition(Vector2D input_lefttop);

	// ���� ��ġ���� ���߱�
	// �Է°� : �����
	void SetDown(float input_amount);

private:
	// �ʿ��� ������Ʈ �߰� �� �ʱ�ȭ
	void AddComponents();

	// �߰��� ������Ʈ ����
	void SetComponents(GameStateManager* input_gamestate);

	// �÷��� ActorID
	std::string m_PlatformActorID;

private:
	// ���ϼ��� �÷��� AABB �ݸ���
	AABBStruct PlatformCollision;
	// ���� ���� ����
	std::shared_ptr<Actor> CoilSectorActor;
};