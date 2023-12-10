#pragma once
#include "Sprite.h"


/// <summary>
/// Animation
/// </summary>
class Animation
{
public :
	Animation();
	// 업데이트
	void Update(float deltatime);
	// 스프라이트 추가
	void AddSprite(Sprite* sprite);
	// 프레임 딜레이 설정
	void SetDelayFrame(int input_delay);

public :
	// 애니메이션 루프 여부
	// ( 루프가 아닐경우 애니메이션 재생후 마지막 프레임에 고정됨 )
	bool isloop;

	// -----------------------------------------------------------------------
	// 파티클 애니메이션 에서 타입이 SpriteTranslate 일경우, 
	// 해당 변수는 애니메이션 재생 프레임 수로 쓰입니다
	// -----------------------------------------------------------------------
	int delayframe;				// 애니메이션 재생 속도 ( 프레임 딜레이 ) 프레임 단위
	// -----------------------------------------------------------------------

	int AnimationID;			// 애니메이션 아이디
	vector<Sprite*> sprites;	// 애니메이션 스프라이트
};

