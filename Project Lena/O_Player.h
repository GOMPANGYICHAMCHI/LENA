#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Vector2D.h"
#include "C_AABBCollision.h"
#include "CC_Sprite.h"


class Actor;
class GameScene;
class InputHandler;
class Animation;

/// <summary>
/// Player Ŭ����
/// Player�� ������ �� GamaManager���� �����Ѵ�. ���Ŀ��� ObjectManager���� ������ ����
/// ��, Transform�� ActorID�� ���� ���� �� Defult�� ����
/// 2022.02.07
/// </summary>

class O_Player
{
public:
	// �÷��̾� ������
	O_Player(InputHandler*);

	// �÷��̾� �ʱ�ȭ (Actor ����, currentScene �߰�, AddComponents, SetComponents�� ȣ��)
	void Initialize(std::shared_ptr<GameScene> currentScene,std::vector<Animation*> input_animation, std::shared_ptr<Actor> cursor);

	// �÷��̾� Actor Getter
	std::shared_ptr<Actor> GetPlayerActor();

	// �÷��̾� VA_Collision Getter
	AABBStruct GetPlayerCollision();
	void SetPlayerMoveSpeed(float moveSpeed);
	void SetPlayerJumpSpeed(float jumpSpeed);
	void SetBossStage();

	void SetWeapon(std::shared_ptr<Actor> weapon);

private:
	// �ʿ��� Components �߰� �� Components Initialize
	void AddComponents();

	// �߰��� Components �ʱ�ȭ�� ��� ������ Set
	void SetComponents();

private:
	// �÷��̾� ���� Position
	Vector2D position;

	// �÷��̾� MoveSpeed
	float moveSpeed;

	// �÷��̾� JumpSpeed
	float jumpSpeed;

	// Camera�� �÷��̾ ������� �ӵ�
	float cameraSpeed;

	// �÷��̾� ActorID
	std::string playerActorID;

	// �÷��̾� AABB VA_Collision
	AABBStruct playerCollision;

private:
	// �÷��̾� Actor
	std::shared_ptr<Actor> playerActor;

	std::shared_ptr<Actor> playerGunActor;

	std::shared_ptr<Actor> CursorActor;


private:
	// �÷��̾��� �Է� Component�� �����ϴ� Handler
	InputHandler* inputHandler;
};

