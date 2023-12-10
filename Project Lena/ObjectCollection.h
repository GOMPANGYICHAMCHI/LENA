#pragma once
#include <string>
#include <queue>
#include <random>
#include "MapDataLoader.h"
#include "ResourceCollection.h"
#include "MapObjectPoolSize.h"

#include "GameStateManager.h"

#include "C_Transform.h"

// 오브젝트 헤더들
#include "O_PlatformStatic.h"			// 플랫폼 정적
#include "O_PlatformCircle.h"			// 플랫폼 원이동
#include "O_PlatformDigt.h"				// 플랫폼 디긎
#include "O_PlatformLine.h"				// 플랫폼 선이동

#include "O_FixedEnemy.h"				// 고정 적
#include "O_FlyingHorizontalEnemy.h"	// 수평이동비행 적
#include "O_FlyingVerticalEnemy.h"		// 수직이동비행 적
#include "O_NormalEnemy.h"				// 일반 적

#include "O_BackGround.h"				// 백그라운드
#include "O_CoilSector.h"				// 코일섹터 오브젝트

#include "O_FloorDebugger.h"			// 플로어 디버거
#include "O_Player.h"

#include "O_VoidSecNumber.h"

#include "O_Boss.h"

using namespace std;

//index_range(RandomDevice);

struct Sector
{
	// 플로어에 배치되는 오브젝트들의 정보
	vector<ObjectData> SectorObjects;
};

struct VoidSector 
{
	vector<O_PlatformStatic*>	P_static;	// 발판 정적
	O_VoidSecNumber*			O_number;	// 층 번호 오브젝트
};

struct FloorObject
{
	vector<O_PlatformStatic*>			P_static;	// 정적		발판
	vector<O_PlatformCircle*>			P_circle;	// 원이동	발판
	vector<O_PlatformDigt*>				P_digt;		// 디긎		발판
	vector<O_PlatformLine*>				P_line;		// 선이동	발판

	vector<O_FixedEnemy*>				E_fixed;	// 고정		적
	vector<O_FlyingHorizontalEnemy*>	E_flyhor;	// 수평비행	적
	vector<O_FlyingVerticalEnemy*>		E_flyver;	// 수직비행	적
	vector<O_NormalEnemy*>				E_normal;	// 일반		적
};

/// <summary>
/// =오브젝트 콜랙션=
/// 
/// 게임 내에 쓰이는 모든 플랫폼 , 몬스터의 
/// 객체를 생성,재배치,활성화,비활성화,풀링 한다
/// </summary>
class ObjectCollection
{
public:
	ObjectCollection();

	// 초기화
	void Initialize(
		std::shared_ptr<GameScene> input_GameScene,
		ResourceCollection* resourceCollection,
		GameStateManager* input_gamestate,
		O_Player* input_playerobj);

	// 전체 맵정보 로드
	void LoadAllMapData();

	// 다음 층 도달시 ( 활성화 된 오브젝트 비활성화 , 플로어 오브젝트 생성 )
	void NextFloorReached(bool isVoidSecCoil);

	// 섹터 오브젝트 생성 ( 활성화 및 배치 )
	void GenerateSector(int input_index);

	// 섹터 최좌상단 입력값으로 초기화
	void SetSetorPos(Vector2D input_pos);

private:

	// 활성화된 리소스 비활성화 및 일반 오브젝트 벡터로 이동 
	void SetoffAllMapResource();

	// 맵 오브젝트 리소스 생성
	void GenerateMapResources
	(std::shared_ptr<GameScene> input_GameScene, GameStateManager* input_gamestate);

	// 코일 섹터 오브젝트 생성 ( 비활성화 및 재배치 )
	void GenerateCoilSector(bool isfirst);

	// 빈 섹터 생성
	void GenerateVoidSector();

	// 플로어 오브젝트 생성( 활성화 및 배치 )
	void GenerateFloor(bool isfirst);

	// 빈섹터 오브젝트 데이터 로드 및 생성
	void GenerateVoidSectorResources
	(std::shared_ptr<GameScene> input_GameScene, GameStateManager* input_gamestate);

	// 백그라운드 활성화
	void SetBackground();

	// 보스전 백그라운드 활성화
	void SetBossBackground();

	// 빈섹터 백그라운드 활성화
	void SetVoidSecBackground();

	// 전체 백그라운드 비활성화
	void SetOffAllBackground();

	// 인덱스 랜덤 생성
	// 입력값 : 랜덤생성으로 수를 받은 변수의 포인터
	void RandomIndexing(int* num1, int* num2, int* num3);

	// 빈 섹터 위치 조정
	void SetVoidSectorPos();

	void CoilSwap();

	void SetoffAllMapResource_Circling();

	void GenerateFloor_Circling(bool isfirst);

	void AutoBackGround();

	void SetCoilSectorBackground();

	void SetBossStage();

private:
	Vector2D m_sectorpos;						// 섹터 최좌상단 좌표

	MapDataLoader* m_mapdataloader;				// 맵데이터 로드용 클래스

	ResourceCollection* m_ResourceCollection;	// 리소스 콜랙션 주소

	vector<Sector> SectorData;					// 섹터 탬플릿 데이터

	Sector VoidSecData;							// 빈섹터 데이터

	//====================================================================================
	// 배경 오브젝트=======================================================================

	vector<O_BackGround*> m_BackGround;				// 백그라운드 오브젝트 

	vector<O_BackGround*> m_BGVoidSector;			// 빈섹터 백그라운드 오브젝트 

	vector<O_BackGround*> m_BGBossStage;			// 보스 스테이지 백그라운드 오브젝트

	vector<O_BackGround*> m_BGCoilSector;			// 코일섹터 백그라운드 오브젝트

	int currentbg_index;							// 현재 백그라운드 인덱스

	int currentvbg_index;							// 현재 빈섹터 백그라운드  인덱스

	int currentBossStagebg_indxe;					// 현재 보스스테이지 백그라운드 인덱스

	int currentCoilSecbg_index;						// 현재 코일섹터 백그라운드 인덱스

	//====================================================================================
	// 코일섹터 오브젝트 ==================================================================

	O_CoilSector* CoilSector_low;					// 낮은 코일섹터 오브젝트
	O_CoilSector* CoilSector_high;					// 높은 코일섹터 오브젝트

	O_CoilSector* CoilSector_voidsec;				// 빈섹터 코일섹터 오브젝트

	//====================================================================================
	// 비활성 오브젝트 ====================================================================

	FloorObject DactObj;							// 비활성화 된 오브젝트 벡터

	//====================================================================================
	// 활성 오브젝트=======================================================================

	FloorObject ActObj_UNUSE[2];					// 2개 층 분량의 활성화 된 오브젝트 벡터
	FloorObject ActObj;								// 2개 층 분량의 활성화 된 오브젝트 벡터

	bool isup = false;

	//====================================================================================
	// 빈섹터 오브젝트 ====================================================================

	VoidSector VoidSecObj;							// 빈섹터 오브젝트 

	bool isVoidSecUnder;							// 빈섹터가 하부에 배치 되었는지 여부

	//====================================================================================
	// 보스 오브젝트 ======================================================================

	O_Boss* m_BossObject;							// 보스 오브젝트
			
	bool isBossStage;								// 보스 스테이지 여부

	//====================================================================================
		
	GameStateManager* p_gamestate;					// 게임 스테이트 주소

	O_Player* m_playerobj;							// 플레이어 게임 오브젝트

	// 코딩해 코딩!!!! 난 김재우 코딩의 고수지!

	//====================================================================================
	// 플로어 디버거=======================================================================

	int FloorCount;

	bool isDebuggerON;							// 디버거 사용 여부

	O_FloorDebugger* deb1;						// 디버거
	O_FloorDebugger* deb2;						// 디버거
	O_FloorDebugger* deb3;						// 디버거

	O_FloorDebugger* deb4;						// 디버거
	O_FloorDebugger* deb5;						// 디버거
	O_FloorDebugger* deb6;						// 디버거
};