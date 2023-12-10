#pragma once
#ifndef _CMOTION
#define _CMOTION

#include <vector>
#include <unordered_map>

#include "Component.h"
#include "Renderer.h"

//class Renderer;
class Animation;

using namespace std;

/// <summary>
/// Animater
/// </summary>
class C_Animator : public Component
{
public:
    C_Animator(Actor* owner);
    const static int sortOrder = static_cast<int>(ComponentSortOrder::Animation);

public:
    // 애니메이션 추가
    // 입력값 : 애니메이션 포인터 
    void AddAnimation(Animation* input_animation,float input_delayframe);

    // 애니메이션 맵 대입 함수
    void SetAnimationVector(vector<Animation*> input_animation, float input_delayframe);

    // 업데이트
    virtual void Update(float deltatime) override;
    void C_Update(float deltatime);

    // 렌더
    virtual void Render(Renderer* renderer) override;

    virtual int GetSortOrder() override;

    // 위치 조정 여부 설정
    // 입력값 : 위치 조정 여부 , 위치 조정 정도 ( 조정 여부 true 시에만 반영됨 )
    void SetAdjust(bool input_set, Vector2D input_amount = Vector2D(0, 0));

    // 애니메이션 방향 설정
    // 입력값 : 방향 왼쪽 여부
    void SetAnimationDirection(bool input_isLeft);

    // 애니메이션 사용 여부 설정
    // 입력값 : 애니메이션 사용 여부
    void SetAnimationUse(bool input_isanimation);

    // 현재 애니메이션 설정
    void SetAnimationState(int animation_index);

    // 설정된 애니메이션 중 특정 애니메이션 플레임 딜레이 입력값으로 수정
    // 입력값 : 대상 애니메이션 인덱스 , 프레임 딜레이
    void SetFrameDilay(int input_animationindex,int input_delay);

    // 현재 애니매이션 내 프레임 설정
    void SetFrameState(int input_frame);

    // 스크린 출력 여부 설정
    void SetIsOnScreen(bool input_isonscreen);

    // 화면 출력시 위치
    void SetToScreenPos(Vector2D input_pos);

    // 스프라이트 월드 사이즈 변경
    // 입력값 : 스프라이트 가로길이 , 스프라이트 세로길이
    void SetWorldSize(float input_width, float input_height);
    void SetWorldSize(Vector2D input_size);

    void RenderOn();

    void RenderOff();

protected:

    bool isRender;

	int ID;                                             // 애니메이터 아이디

	int CurrentAnimation_index;                         // 현재 애니메이션 키값
	int CurrentSprite_index;                            // 현재 애니메이션 내 스프라이트 인덱스

	int FrameCheck = 0;                                 // Update에서 매 프레임마다 1씩 더해지는 수

	int SpriteNum;                                      // 스프라이트 번호

	//float DelayFrame;                                 // 애니메이션 재생 속도 ( 프레임 딜레이 )

	float World_width;                                  // 스프라이트 가로길이
	float World_height;                                 // 스프라이트 세로길이

	bool is_Animation;                                  // 애니메이션 여부
    bool is_Left;                                       // 애니메이션 방향 왼쪽 여부
    bool is_AdjustOn;                                   // 위치 조정 여부
    bool is_OnScreen;                                   // 스크린에 출력 여부

    Vector2D ToScreenPos;                               // 화면 출력시 위치

    Vector2D AdjustAmount;                              // 위치 조정 정도

    vector<Animation*> m_Animations;                    // 애니메이션 
};

#endif