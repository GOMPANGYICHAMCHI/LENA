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
/// Player 클래스
/// Player를 조립한 후 GamaManager에서 생성한다. 이후에는 ObjectManager에서 생성할 예정
/// 단, Transform과 ActorID는 액터 생성 시 Defult로 생성
/// 2022.02.07
/// </summary>

class O_Player
{
public:
	// 플레이어 생성자
	O_Player(InputHandler*);

	// 플레이어 초기화 (Actor 생성, currentScene 추가, AddComponents, SetComponents를 호출)
	void Initialize(std::shared_ptr<GameScene> currentScene,std::vector<Animation*> input_animation, std::shared_ptr<Actor> cursor);

	// 플레이어 Actor Getter
	std::shared_ptr<Actor> GetPlayerActor();

	// 플레이어 VA_Collision Getter
	AABBStruct GetPlayerCollision();
	void SetPlayerMoveSpeed(float moveSpeed);
	void SetPlayerJumpSpeed(float jumpSpeed);
	void SetBossStage();

	void SetWeapon(std::shared_ptr<Actor> weapon);

private:
	// 필요한 Components 추가 및 Components Initialize
	void AddComponents();

	// 추가된 Components 초기화한 멤버 변수로 Set
	void SetComponents();

private:
	// 플레이어 생성 Position
	Vector2D position;

	// 플레이어 MoveSpeed
	float moveSpeed;

	// 플레이어 JumpSpeed
	float jumpSpeed;

	// Camera가 플레이어를 따라오는 속도
	float cameraSpeed;

	// 플레이어 ActorID
	std::string playerActorID;

	// 플레이어 AABB VA_Collision
	AABBStruct playerCollision;

private:
	// 플레이어 Actor
	std::shared_ptr<Actor> playerActor;

	std::shared_ptr<Actor> playerGunActor;

	std::shared_ptr<Actor> CursorActor;


private:
	// 플레이어의 입력 Component를 중재하는 Handler
	InputHandler* inputHandler;
};

