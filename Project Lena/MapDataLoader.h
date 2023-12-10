#pragma once

#include <vector>
#include "Vector2D.h"
#include "ObjectType.h"
#include "C_PlatformProperty.h"

// rapidjson
#include "document.h"
using namespace rapidjson;

// 오브젝트 데이터
//	
//	(형식)				(명칭)				(용도)
// 
//	ObjectType			objtype				오브젝트 종류
//	Vector2D			Pos1				좌표1
// ///
//	Vector2D			Pos2				좌표2
// ///
//	int					platformsizeindex	플랫폼 사이즈 인덱스
//	int					movespeed			이동 속도
//	Passable			passable			플랫폼 통과 타입
//	int					rotatedirection		회전 방향
//	int					digtmoveamount		디긎 이동시 이동정도
struct ObjectData
{
	ObjectType objtype;			// 오브젝트 종류		

	// ot_Monster1			(0) = 고정 몬스터
	// ot_Monster2			(1) = 이동 몬스터
	// ot_Monster3			(2) = 비행 몬스터 , 좌우
	// ot_Monster4			(3) = 비행 몬스터 , 상하

	// ot_PlatformStatic	(4) = 플랫폼 고정
	// ot_PlatformLine		(5) = 플랫폼 선이동
	// ot_PlatformCircle	(6) = 플랫폼 원이동
	// ot_PlatformDigt		(7) = 플랫폼 디긎이동

	//-----------------------------------------------------

	Vector2D Pos1;				// 좌표1
	Vector2D Pos2;				// 좌표2

	//-----------------------------------------------------

	int platformsizeindex;		// 플랫폼 사이즈 인덱스

	//-----------------------------------------------------

	int movespeed;				// 이동속도 ( 1 ~ 100 )

	//-----------------------------------------------------

	Passable passable;			// 플랫폼 통과타입

	// Default	(0) = 모두 막음			2	모두 불가
	// Up		(1) = 상향 통과 가능		1	아래 점프 불가
	// Down		(2) = 하향 통과 가능
	// All		(3) = 모두 통과 가능		0	모두 가능
	// Box		(4) = 플랫폼 위에 놓일 박스

	//-----------------------------------------------------

	int rotatedirection;		// 회전 방향 

	// 0 : 시계 방향		|| 가로 방향 
	// 1 : 반시계 방향	|| 세로 방향

	//-----------------------------------------------------

	int digtmoveamount;			// 디긎 이동시 이동정도

	//-----------------------------------------------------
};


using namespace rapidjson;

class MapDataLoader
{
public:
	MapDataLoader();

	Document CJsonParser(const char* filepath);
	// Json 데이터로 부터 오브젝트 데이터 로드
	std::vector<ObjectData> LoadMapData(const char* filepath);
};

enum JsonFileType
{
	// 이 순서대로 배열에 저장
	_Type,
	_PosX_1,
	_PosY_1,
	_PosX_2,
	_PosY_2,
	_PlatformSize,
	_MoveSpeed,
	_Passable,
	_Clockwise,
	_MoveArea
};