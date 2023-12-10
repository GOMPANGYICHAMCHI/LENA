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

	// 코일 섹터 초기화 (액터 생성, 현재씬에 추가, 컴포넌트 추가, 컴포넌트 설정)
	void Initialize(
		std::shared_ptr<GameScene> currentScene, 
		std::vector<Animation*> input_animation,
		GameStateManager* input_gamestate,
		Sprite* input_coilplatform);

	// 코일 섹터 액터 반환
	std::shared_ptr<Actor> GetActor();

	// 코일 상태 변경
	// 입력값 : 코일 상태
	void SetCoil(bool input_set);

	// 빈 섹터 종속 코일로설정
	void SetVoidSecCoil(bool input_set);

	// 위치 변경
	void SetPosition(Vector2D input_lefttop);

	// 기존 위치에서 낮추기
	// 입력값 : 낮출양
	void SetDown(float input_amount);

private:
	// 필요한 컴포넌트 추가 및 초기화
	void AddComponents();

	// 추가된 컴포넌트 설정
	void SetComponents(GameStateManager* input_gamestate);

	// 플랫폼 ActorID
	std::string m_PlatformActorID;

private:
	// 코일섹터 플랫폼 AABB 콜리전
	AABBStruct PlatformCollision;
	// 코일 섹터 액터
	std::shared_ptr<Actor> CoilSectorActor;
};