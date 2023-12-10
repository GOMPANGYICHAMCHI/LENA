#pragma once

#include "Sprite.h"
#include "Vector2D.h"

#include "Component.h"
#include "Renderer.h"

// 스프라이트 컴포넌트
// 애니메이션이 들어가지 않는 고정형 물체를 위한 이미지 렌더용 컴포넌트
class C_Sprite : public Component
{
public:
	C_Sprite(Actor* owner);
	const static int sortOrder = static_cast<int>(ComponentSortOrder::Sprite);

	// 업데이트 
	virtual void Update(float deltaTime) override;
	// 렌더
	virtual void Render(Renderer* renderer) override;

	virtual int GetSortOrder() override;

	// 위치 보정 정도 설정
	// 입력값 : 위치 보정 정도
	void SetAdjustAmount(Vector2D input_amount);

	// 위치 보정 여부 설정 ( 호출시 자동으로 보정이 ON 됩니다 )
	// 입력값 : 위치 보정 여부 설정
	void SetAdjustPos(bool input_set);

	// 피봇 반환 함수
	Vector2D GetPivot();

	virtual void SetSource(Sprite*);

	// 이미지 사이즈 설정
	void SetImageSize(Vector2D input_size);

	// 스프라이트 렌더 활성화
	void RenderOn();

	void RenderOff();

protected:
	// 렌더 여부 
	bool isrender;
	// 포지션 보정 여부
	bool isadjustpos;

	// 포지션 보정 정도
	Vector2D adjustamount;

	// 이미지 가로길이
	float World_width;
	// 이미지 세로길이
	float World_height;

	// 원본 소스 주소를 가지고있는 스프라이트 객체
	Sprite* Source;
};

