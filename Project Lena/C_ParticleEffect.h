#pragma once

#include "Pch.h"
#include "Component.h"
#include "Renderer.h"

#include "Animation.h"
#include "Actor.h"

// 파티클 애니메이션 방식
enum ParticleType
{
	SpriteSwap,
	SpriteTranslate
};

/// <summary>
/// 파티클 이펙트
/// 
/// 애니메이터와 동일한 방식으로 동작하는 'SpriteSwap' 형식과
/// 스프라이트 여러개가 각각 따로 움직이는 'SpriteTranslate' 형식 이 있다
/// 사용되는 변수는 동일하나 형식에따라 동작이 다르다
/// </summary>
class C_ParticleEffect : public Component
{
public:
	C_ParticleEffect(Actor* owner);
	const static int SortOrder = static_cast<int>(ComponentSortOrder::Animation);

public:
	// 업데이트
	virtual void Update(float deltatime) override;
	// 렌더
	virtual void Render(Renderer* renderer) override;

	// 애니메이션 설정
	// 입력값 : 애니메이션 포인터
	void SetSource(Sprite* input_sprite);

	// 애니메이션 방식 설정
	// 입력값 : 파티클 애니메이션 enum 타입
	void SetAnimationType(ParticleType input_type);

	// 스프라이트 월드 사이즈 변경
	// 입력값 : 스프라이트 가로길이 , 스프라이트 세로길이
	void SetWorldSize(float input_width, float input_height);

	// 딜레이 프레임 설정
	void SetDelayFrame(int input_delayframe);

	// 파티클 수량 설정
	void SetParticleAmount(int input_particleamount);

	// 파티클 애니메이션 루프 설정
	// 입력값 : 루프설정 여부
	void SetLoop(bool input_loop);

	// 파티클 실행
	void Play();

	// 파티클 종료
	void Stop();

private :
	ParticleType ptype;					// 파티클 애니메이션 타입

	int CurrentSprite_index;			// 현재 스프라이트 인덱스
	
	int delayframe;						// 애니메이션 재생 시간
	int FrameCheck;						// 프레임 체크 ( 업데이트 에서 매프레임 마다 1씩 더해짐 )

	int ParticleAmount;					// 파티클 수량

	float World_width;                  // 스프라이트 가로길이
    float World_height;                 // 스프라이트 세로길이

	bool IsActivate;					// 파티클 이펙트 활성화 여부
	bool IsLoop;						// 파티클 애니메이션 루프 여부

	Sprite* m_sprite;				    // 파티클 애니메이션 원본 리소스

	vector<Vector2D> particle_pos;		// 전체 파티클 위치
};

