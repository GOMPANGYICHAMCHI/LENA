#pragma once
#include <string>
#include <queue>
#include <random>
#include "MapDataLoader.h"
#include "ResourceCollection.h"
#include "MapObjectPoolSize.h"

#include "GameStateManager.h"

#include "C_Transform.h"

// ������Ʈ �����
#include "O_PlatformStatic.h"			// �÷��� ����
#include "O_PlatformCircle.h"			// �÷��� ���̵�
#include "O_PlatformDigt.h"				// �÷��� ���M
#include "O_PlatformLine.h"				// �÷��� ���̵�

#include "O_FixedEnemy.h"				// ���� ��
#include "O_FlyingHorizontalEnemy.h"	// �����̵����� ��
#include "O_FlyingVerticalEnemy.h"		// �����̵����� ��
#include "O_NormalEnemy.h"				// �Ϲ� ��

#include "O_BackGround.h"				// ��׶���
#include "O_CoilSector.h"				// ���ϼ��� ������Ʈ

#include "O_FloorDebugger.h"			// �÷ξ� �����
#include "O_Player.h"

#include "O_VoidSecNumber.h"

#include "O_Boss.h"

using namespace std;

//index_range(RandomDevice);

struct Sector
{
	// �÷ξ ��ġ�Ǵ� ������Ʈ���� ����
	vector<ObjectData> SectorObjects;
};

struct VoidSector 
{
	vector<O_PlatformStatic*>	P_static;	// ���� ����
	O_VoidSecNumber*			O_number;	// �� ��ȣ ������Ʈ
};

struct FloorObject
{
	vector<O_PlatformStatic*>			P_static;	// ����		����
	vector<O_PlatformCircle*>			P_circle;	// ���̵�	����
	vector<O_PlatformDigt*>				P_digt;		// ���M		����
	vector<O_PlatformLine*>				P_line;		// ���̵�	����

	vector<O_FixedEnemy*>				E_fixed;	// ����		��
	vector<O_FlyingHorizontalEnemy*>	E_flyhor;	// �������	��
	vector<O_FlyingVerticalEnemy*>		E_flyver;	// ��������	��
	vector<O_NormalEnemy*>				E_normal;	// �Ϲ�		��
};

/// <summary>
/// =������Ʈ �ݷ���=
/// 
/// ���� ���� ���̴� ��� �÷��� , ������ 
/// ��ü�� ����,���ġ,Ȱ��ȭ,��Ȱ��ȭ,Ǯ�� �Ѵ�
/// </summary>
class ObjectCollection
{
public:
	ObjectCollection();

	// �ʱ�ȭ
	void Initialize(
		std::shared_ptr<GameScene> input_GameScene,
		ResourceCollection* resourceCollection,
		GameStateManager* input_gamestate,
		O_Player* input_playerobj);

	// ��ü ������ �ε�
	void LoadAllMapData();

	// ���� �� ���޽� ( Ȱ��ȭ �� ������Ʈ ��Ȱ��ȭ , �÷ξ� ������Ʈ ���� )
	void NextFloorReached(bool isVoidSecCoil);

	// ���� ������Ʈ ���� ( Ȱ��ȭ �� ��ġ )
	void GenerateSector(int input_index);

	// ���� ���»�� �Է°����� �ʱ�ȭ
	void SetSetorPos(Vector2D input_pos);

private:

	// Ȱ��ȭ�� ���ҽ� ��Ȱ��ȭ �� �Ϲ� ������Ʈ ���ͷ� �̵� 
	void SetoffAllMapResource();

	// �� ������Ʈ ���ҽ� ����
	void GenerateMapResources
	(std::shared_ptr<GameScene> input_GameScene, GameStateManager* input_gamestate);

	// ���� ���� ������Ʈ ���� ( ��Ȱ��ȭ �� ���ġ )
	void GenerateCoilSector(bool isfirst);

	// �� ���� ����
	void GenerateVoidSector();

	// �÷ξ� ������Ʈ ����( Ȱ��ȭ �� ��ġ )
	void GenerateFloor(bool isfirst);

	// ���� ������Ʈ ������ �ε� �� ����
	void GenerateVoidSectorResources
	(std::shared_ptr<GameScene> input_GameScene, GameStateManager* input_gamestate);

	// ��׶��� Ȱ��ȭ
	void SetBackground();

	// ������ ��׶��� Ȱ��ȭ
	void SetBossBackground();

	// ���� ��׶��� Ȱ��ȭ
	void SetVoidSecBackground();

	// ��ü ��׶��� ��Ȱ��ȭ
	void SetOffAllBackground();

	// �ε��� ���� ����
	// �Է°� : ������������ ���� ���� ������ ������
	void RandomIndexing(int* num1, int* num2, int* num3);

	// �� ���� ��ġ ����
	void SetVoidSectorPos();

	void CoilSwap();

	void SetoffAllMapResource_Circling();

	void GenerateFloor_Circling(bool isfirst);

	void AutoBackGround();

	void SetCoilSectorBackground();

	void SetBossStage();

private:
	Vector2D m_sectorpos;						// ���� ���»�� ��ǥ

	MapDataLoader* m_mapdataloader;				// �ʵ����� �ε�� Ŭ����

	ResourceCollection* m_ResourceCollection;	// ���ҽ� �ݷ��� �ּ�

	vector<Sector> SectorData;					// ���� ���ø� ������

	Sector VoidSecData;							// ���� ������

	//====================================================================================
	// ��� ������Ʈ=======================================================================

	vector<O_BackGround*> m_BackGround;				// ��׶��� ������Ʈ 

	vector<O_BackGround*> m_BGVoidSector;			// ���� ��׶��� ������Ʈ 

	vector<O_BackGround*> m_BGBossStage;			// ���� �������� ��׶��� ������Ʈ

	vector<O_BackGround*> m_BGCoilSector;			// ���ϼ��� ��׶��� ������Ʈ

	int currentbg_index;							// ���� ��׶��� �ε���

	int currentvbg_index;							// ���� ���� ��׶���  �ε���

	int currentBossStagebg_indxe;					// ���� ������������ ��׶��� �ε���

	int currentCoilSecbg_index;						// ���� ���ϼ��� ��׶��� �ε���

	//====================================================================================
	// ���ϼ��� ������Ʈ ==================================================================

	O_CoilSector* CoilSector_low;					// ���� ���ϼ��� ������Ʈ
	O_CoilSector* CoilSector_high;					// ���� ���ϼ��� ������Ʈ

	O_CoilSector* CoilSector_voidsec;				// ���� ���ϼ��� ������Ʈ

	//====================================================================================
	// ��Ȱ�� ������Ʈ ====================================================================

	FloorObject DactObj;							// ��Ȱ��ȭ �� ������Ʈ ����

	//====================================================================================
	// Ȱ�� ������Ʈ=======================================================================

	FloorObject ActObj_UNUSE[2];					// 2�� �� �з��� Ȱ��ȭ �� ������Ʈ ����
	FloorObject ActObj;								// 2�� �� �з��� Ȱ��ȭ �� ������Ʈ ����

	bool isup = false;

	//====================================================================================
	// ���� ������Ʈ ====================================================================

	VoidSector VoidSecObj;							// ���� ������Ʈ 

	bool isVoidSecUnder;							// ���Ͱ� �Ϻο� ��ġ �Ǿ����� ����

	//====================================================================================
	// ���� ������Ʈ ======================================================================

	O_Boss* m_BossObject;							// ���� ������Ʈ
			
	bool isBossStage;								// ���� �������� ����

	//====================================================================================
		
	GameStateManager* p_gamestate;					// ���� ������Ʈ �ּ�

	O_Player* m_playerobj;							// �÷��̾� ���� ������Ʈ

	// �ڵ��� �ڵ�!!!! �� ����� �ڵ��� �����!

	//====================================================================================
	// �÷ξ� �����=======================================================================

	int FloorCount;

	bool isDebuggerON;							// ����� ��� ����

	O_FloorDebugger* deb1;						// �����
	O_FloorDebugger* deb2;						// �����
	O_FloorDebugger* deb3;						// �����

	O_FloorDebugger* deb4;						// �����
	O_FloorDebugger* deb5;						// �����
	O_FloorDebugger* deb6;						// �����
};