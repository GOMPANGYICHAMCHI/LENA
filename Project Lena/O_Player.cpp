#include "O_Player.h"
#include "Actor.h"

#include "C_Transform.h"
#include "C_Velocity.h"
#include "CC_Camera.h"
#include "CC_CheckPlayerInput.h"
#include "C_Physics.h"
#include "CC_PlayerAnimator.h"
#include "CC_Debugger.h"

#include "C_CursorPosition.h"

#include "InputHandler.h"
#include "GameScene.h"

Vector2D PlayerSize(161, 200);

/// <summary>
///  플레이어 생성자
/// </summary>
O_Player::O_Player(InputHandler* inputHandler) :
	inputHandler(inputHandler),
	position(1000, 1800),
	moveSpeed(7), jumpSpeed(230),
	cameraSpeed(10), playerActorID("Player"),
	playerCollision{ position, PlayerSize.X / 5, PlayerSize.Y / 2 - 20 }
{
}

/// <summary>
/// 플레이어 초기화 (Actor 생성, currentScene 추가, AddComponents, SetComponents를 호출)
/// </summary>
void O_Player::Initialize(std::shared_ptr<GameScene> currentScene, std::vector<Animation*> input_animation, std::shared_ptr<Actor> cursor)
{
	// PlayerActor 생성
	this->playerActor = std::make_shared<Actor>();

	this->playerActor->SetRenderOrder(static_cast<int>(RenderOrder::Player));

	this->CursorActor = cursor;

	// 필요한 Components 추가 및 Components Initialize
	this->AddComponents();

	// 추가한 Components 초기화한 멤버 변수로 Set
	this->SetComponents();

	// 현재 씬에 Player Actor
	currentScene->Actors.Add(playerActor);

	// VA_Collision 설정 후 CollisionManager 에게 전달
	currentScene->m_CollisionManager.Add(playerActor);

	std::shared_ptr<CC_PlayerAnimator> Animator = this->playerActor->GetComponent<CC_PlayerAnimator>();
	Animator->SetAnimationState(0);
	Animator->SetWorldSize(PlayerSize);
	Animator->SetAnimationVector(input_animation, 5);
	Animator->SetFrameDilay(1, 2);		// 점프 애니메이션 플레임 딜레이 수정
	Animator->SetFrameDilay(2, 5);		// 이동 애니메이션 플레임 딜레이 수정
	Animator->SetAnimationUse(true);
}

/// <summary>
/// // 플레이어 Actor Getter
/// </summary>
std::shared_ptr<Actor> O_Player::GetPlayerActor()
{
	return playerActor;
}

/// <summary>
/// // 플레이어 VA_Collision Getter
/// </summary>
AABBStruct O_Player::GetPlayerCollision()
{
	return this->playerCollision;
}

void O_Player::SetPlayerMoveSpeed(float moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

void O_Player::SetPlayerJumpSpeed(float jumpSpeed)
{
	this->jumpSpeed = jumpSpeed;
}

void O_Player::SetBossStage()
{
	// 보스전 시 해야 할 것들
	playerActor->GetComponent<CC_Camera>()->SetCameraScaleXhalf();
	// playerActor->GetComponent<CC_Camera>()->isBoss = true;
	playerActor->GetComponent<CC_Camera>()->CursorActor->GetComponent<C_CursorPosition>()->isBoss = true;
	playerActor->GetComponent<CC_Camera>()->SetCameraScaleXhalf();
	// .....
}

void O_Player::SetWeapon(std::shared_ptr<Actor> weapon)
{
	this->playerGunActor = weapon;
}

/// <summary>
/// 필요한 Components 추가
/// </summary>
void O_Player::AddComponents()
{
	playerActor->AddComponent<CC_Camera>()->SetCameraMoveState(true);
	playerActor->AddComponent<C_Velocity>();
	playerActor->AddComponent<CC_CheckPlayerInput>()->SetInput(inputHandler);
	playerActor->AddComponent<C_State>();
	playerActor->AddComponent<C_Physics>()->UseGravity();
	playerActor->AddComponent<C_AABBCollision>()->SetLayer(CollisionLayer::Player);
	playerActor->AddComponent<CC_PlayerAnimator>();
	playerActor->AddComponent<CC_Debugger>();
}

/// <summary>
/// 추가된 Components 초기화한 멤버 변수로 Set
/// </summary>
void O_Player::SetComponents() //SetCameraScaleX1();
{
	playerActor->ActorID->SetStringActorID(playerActorID);
	playerActor->GetComponent<CC_CheckPlayerInput>()->SetMoveSpeed(moveSpeed);
	playerActor->GetComponent<CC_CheckPlayerInput>()->SetJompSpeed(jumpSpeed);
	playerActor->GetComponent<CC_Camera>()->SetMoveSpeed(cameraSpeed);
	playerActor->GetComponent<CC_Camera>()->SetCursorActor(this->CursorActor);

	playerActor->GetComponent<CC_Camera>()->SetCameraScaleX1();

	playerActor->GetComponent<C_AABBCollision>()->SetRect(playerCollision);
	playerActor->transform->SetPosition(position);
	playerActor->transform->SetPlayer();
	//playerActor->GetComponent<C_Animator>()->AddAnimation();
}
