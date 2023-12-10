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
///  �÷��̾� ������
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
/// �÷��̾� �ʱ�ȭ (Actor ����, currentScene �߰�, AddComponents, SetComponents�� ȣ��)
/// </summary>
void O_Player::Initialize(std::shared_ptr<GameScene> currentScene, std::vector<Animation*> input_animation, std::shared_ptr<Actor> cursor)
{
	// PlayerActor ����
	this->playerActor = std::make_shared<Actor>();

	this->playerActor->SetRenderOrder(static_cast<int>(RenderOrder::Player));

	this->CursorActor = cursor;

	// �ʿ��� Components �߰� �� Components Initialize
	this->AddComponents();

	// �߰��� Components �ʱ�ȭ�� ��� ������ Set
	this->SetComponents();

	// ���� ���� Player Actor
	currentScene->Actors.Add(playerActor);

	// VA_Collision ���� �� CollisionManager ���� ����
	currentScene->m_CollisionManager.Add(playerActor);

	std::shared_ptr<CC_PlayerAnimator> Animator = this->playerActor->GetComponent<CC_PlayerAnimator>();
	Animator->SetAnimationState(0);
	Animator->SetWorldSize(PlayerSize);
	Animator->SetAnimationVector(input_animation, 5);
	Animator->SetFrameDilay(1, 2);		// ���� �ִϸ��̼� �÷��� ������ ����
	Animator->SetFrameDilay(2, 5);		// �̵� �ִϸ��̼� �÷��� ������ ����
	Animator->SetAnimationUse(true);
}

/// <summary>
/// // �÷��̾� Actor Getter
/// </summary>
std::shared_ptr<Actor> O_Player::GetPlayerActor()
{
	return playerActor;
}

/// <summary>
/// // �÷��̾� VA_Collision Getter
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
	// ������ �� �ؾ� �� �͵�
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
/// �ʿ��� Components �߰�
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
/// �߰��� Components �ʱ�ȭ�� ��� ������ Set
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
