#include "ObjectCollection.h"
#include "atlstr.h"

#include "CC_Camera.h"
#include "C_Camera.h"
#include "O_Player.h"
#include "Animation.h"
#include "Actor.h"

#define ADJUST_X 1.32
#define Y_MOVE_AMOUNT 6720

Vector2D NormalMap_CenterPos(500, 2500);
Vector2D BossMap_CenterPos(1500, 2500);

// �÷��� ���������
vector<Vector2D> PlatformSize =
{
	Vector2D(120,40),		// 0
	Vector2D(180,40),		// 1
	Vector2D(240,40),		// 2
	Vector2D(270,40),		// 3
	Vector2D(300,40),		// 4
};

// �÷��� ��������Ʈ Ű
vector<string> PlatformSpriteKey =
{
	"platform_size1",		// 0
	"platform_size2",		// 1
	"platform_size3",		// 2
	"platform_size4",		// 3
	"platform_size5"		// 4
};

random_device rd;
mt19937 RandomDevice(rd());
uniform_int_distribution<int> index_range_mapdata;
uniform_int_distribution<int> index_range_backgroundimage;

ObjectCollection::ObjectCollection()
{
	FloorCount = 3;
	currentbg_index = 0;
	currentBossStagebg_indxe = 0;
	currentCoilSecbg_index = 0;
	isVoidSecUnder = false;
	isBossStage = false;
}

// �ʱ�ȭ
void ObjectCollection::Initialize(
	std::shared_ptr<GameScene> input_GameScene,
	ResourceCollection* resourceCollection,
	GameStateManager* input_gamestate,
	O_Player* input_playerobj)
{
	//====================================================================================
	// �÷ξ� �����=======================================================================
	//====================================================================================
	/*				*/	isDebuggerON = false; /* <------- ����ġ			���� ---------->*/																								if (isDebuggerON) { deb1 = new O_FloorDebugger; deb2 = new O_FloorDebugger; deb3 = new O_FloorDebugger; deb4 = new O_FloorDebugger; deb5 = new O_FloorDebugger; deb6 = new O_FloorDebugger; deb1->Initialize(input_GameScene); deb2->Initialize(input_GameScene); deb3->Initialize(input_GameScene); deb4->Initialize(input_GameScene); deb5->Initialize(input_GameScene); deb6->Initialize(input_GameScene); }
	//====================================================================================

	// �÷��̾� ������Ʈ ������ ����
	m_playerobj = input_playerobj;

	// �ʵ����� �δ� ����
	m_mapdataloader = new MapDataLoader;

	// ��ü �ʵ����� �ε�
	LoadAllMapData();

	// ���ҽ� �ݷ��� �ּ� �Ҵ�
	m_ResourceCollection = resourceCollection;

	// ���ӽ�����Ʈ �ּ� ����
	p_gamestate = input_gamestate;

	// �� ������Ʈ ���ҽ� ����
	GenerateMapResources(input_GameScene, input_gamestate);

	// ���� �ε��� ���� ����
	if (SectorData.size() > 0)
	{
		index_range_mapdata = uniform_int_distribution<int>(0, SectorData.size() - 1);
	}

	// ��� �̹��� ���� �ε��� ����
	index_range_backgroundimage = uniform_int_distribution<int>(0, 4);

	// �ʱ���ǥ ���� , ù �÷ξ� ����
	m_sectorpos = NormalMap_CenterPos;


	GenerateFloor_Circling(true);
	
	m_playerobj->GetPlayerActor()->transform->SetPosition(Vector2D(900, 2100));
	m_playerobj->GetPlayerActor()->GetComponent<CC_Camera>()->CameraPosSet_Y();
}

// ��ü ������ �ε�
void ObjectCollection::LoadAllMapData()
{
	Sector temp;

	// 1�� ���� n���� �� ������ ����.
	int Max_map_index = 36;

	for (int i = 1; i <= Max_map_index; i++)
	{
		CString temp1 = "../Resource/json/Mapdata/Mapdata (";
		CString temp2;
		temp2.Format(_T("%d"),i );
		CString temp3 = ").json";

		CString temp4 = temp1 + temp2 + temp3;

		CStringA temp5 = (CStringA)temp4;
		const char* temp6 = temp5.GetBuffer();

		temp.SectorObjects = m_mapdataloader->LoadMapData(temp6);
		SectorData.push_back(temp);
	}

}

// Ȱ��ȭ�� ���ҽ� ��Ȱ��ȭ �� �Ϲ� ������Ʈ ���ͷ� �̵� 
void ObjectCollection::SetoffAllMapResource_Circling()
{
	int limit;

	limit = ActObj.P_static.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� �������� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_PlatformStatic* temp_ps = ActObj.P_static.back();
		ActObj.P_static.pop_back();
		temp_ps->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_static.push_back(temp_ps);
	}

	limit = ActObj.P_circle.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ���̵����� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_PlatformCircle* temp_pc = ActObj.P_circle.back();
		ActObj.P_circle.pop_back();
		temp_pc->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_circle.push_back(temp_pc);
	}

	limit = ActObj.P_digt.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ���M�̵����� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_PlatformDigt* temp_pd = ActObj.P_digt.back();
		ActObj.P_digt.pop_back();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_digt.push_back(temp_pd);
	}

	limit = ActObj.P_line.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ���̵����� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_PlatformLine* temp_pl = ActObj.P_line.back();
		ActObj.P_line.pop_back();
		temp_pl->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_line.push_back(temp_pl);
	}



	limit = ActObj.E_fixed.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ���� �� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_FixedEnemy* temp_efix = ActObj.E_fixed.back();
		ActObj.E_fixed.pop_back();
		temp_efix->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.E_fixed.push_back(temp_efix);
	}		

	limit = ActObj.E_flyhor.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ������� �� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_FlyingHorizontalEnemy* temp_efhor = ActObj.E_flyhor.back();
		ActObj.E_flyhor.pop_back();
		temp_efhor->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.E_flyhor.push_back(temp_efhor);
	}

	limit = ActObj.E_flyver.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� �������� �� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_FlyingVerticalEnemy* temp_efver = ActObj.E_flyver.back();
		ActObj.E_flyver.pop_back();
		temp_efver->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.E_flyver.push_back(temp_efver);
	}

	limit = ActObj.E_normal.size();
	// �Ⱦ��̴� ���� Ȱ��ȭ�� �Ϲ� �� ���� ��Ȱ��ȭ
	for (int i = 0; i < limit; i++)
	{
		O_NormalEnemy* temp_enormal = ActObj.E_normal.back();
		ActObj.E_normal.pop_back();
		temp_enormal->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.E_normal.push_back(temp_enormal);
	}

																						/* �÷ξ� ����� -> */						if (isDebuggerON) { if (isup) { deb1->ClearVector(); } else { deb4->ClearVector(); } }
}

// Ȱ��ȭ�� ���ҽ� ��Ȱ��ȭ �� �Ϲ� ������Ʈ ���ͷ� �̵� 
void ObjectCollection::SetoffAllMapResource()
{
	// �Ⱦ��̴� ���� Ȱ��ȭ�� �������� ���� ��Ȱ��ȭ
	for (int i = 0; i < ActObj_UNUSE[isup].P_static.size(); i++)
	{
		O_PlatformStatic* temp_ps = ActObj_UNUSE[isup].P_static.back();
		ActObj_UNUSE[isup].P_static.pop_back();
		temp_ps->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_static.push_back(temp_ps);
	}
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ���̵����� ���� ��Ȱ��ȭ
	for (int i = 0; i < ActObj_UNUSE[isup].P_circle.size(); i++)
	{
		O_PlatformCircle* temp_pc = ActObj_UNUSE[isup].P_circle.back();
		ActObj_UNUSE[isup].P_circle.pop_back();
		temp_pc->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_circle.push_back(temp_pc);
	}
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ���M�̵����� ���� ��Ȱ��ȭ
	for (int i = 0; i < ActObj_UNUSE[isup].P_digt.size(); i++)
	{
		O_PlatformDigt* temp_pd = ActObj_UNUSE[isup].P_digt.back();
		ActObj_UNUSE[isup].P_digt.pop_back();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_digt.push_back(temp_pd);
	}
	// �Ⱦ��̴� ���� Ȱ��ȭ�� ���̵����� ���� ��Ȱ��ȭ
	for (int i = 0; i < ActObj_UNUSE[isup].P_line.size(); i++)
	{
		O_PlatformLine* temp_pl = ActObj_UNUSE[isup].P_line.back();
		ActObj_UNUSE[isup].P_line.pop_back();
		temp_pl->DeActivate();

		// ��Ȱ�� ���Ϳ� ����
		DactObj.P_line.push_back(temp_pl);
	}
																				/* �÷ξ� ����� -> */						//if (isDebuggerON) {  deb1->ClearVector(); deb4->ClearVector();  }
}

// ���� �� ���޽� ( Ȱ��ȭ �� ������Ʈ ��Ȱ��ȭ , �÷ξ� ������Ʈ ���� )
void ObjectCollection::NextFloorReached(bool isVoidSecCoil)
{
	// �� ���� ���� �浹�� 
	if (isVoidSecCoil)
	{
		// ���Ͱ� ��ܿ� �������,
		if (!isVoidSecUnder)
		{
			if (FloorCount == 3)
			{
				SetBossStage();
			}

			else
			{
				float cha =
					m_BossObject->GetBossActor()->transform->GetPosition().Y
					- m_playerobj->GetPlayerActor()->transform->GetPosition().Y;

				m_playerobj->GetPlayerActor()->transform->SetPosition(Vector2D(
					m_playerobj->GetPlayerActor()->transform->GetPosition().X, 2400));

				m_BossObject->GetBossActor()->transform->SetPosition(Vector2D(
					m_BossObject->GetBossActor()->transform->GetPosition().X, 2400 + cha));
			}

			m_sectorpos = NormalMap_CenterPos;

			AutoBackGround();

			SetoffAllMapResource_Circling();
			SetVoidSectorPos();
			GenerateFloor_Circling(false);

			m_playerobj->GetPlayerActor()->GetComponent<C_Camera>()->CameraPosSet_Y();

			isVoidSecUnder = true;
		}
	}

	else
	{
		// ���Ͱ� �ϴܿ� �������.
		if (isVoidSecUnder)
		{
			SetVoidSectorPos();

			isVoidSecUnder = false;
		}
	}
}

// �� ������Ʈ ���ҽ� ����
void ObjectCollection::GenerateMapResources
(std::shared_ptr<GameScene> input_GameScene, GameStateManager* input_gamestate)
{
	// �̵� �÷��� �÷��� �ִϸ��̼� ����
	vector<Animation*> movintPanim_temp;
	movintPanim_temp.push_back(&m_ResourceCollection->m_Animations["lmp_size01"]);
	movintPanim_temp.push_back(&m_ResourceCollection->m_Animations["lmp_size02"]);
	movintPanim_temp.push_back(&m_ResourceCollection->m_Animations["lmp_size03"]);
	movintPanim_temp.push_back(&m_ResourceCollection->m_Animations["lmp_size04"]);

	// ���̵� �÷��� ���� ��������Ʈ ����
	vector<Sprite*> lmprail_tmep;
	lmprail_tmep.push_back(&m_ResourceCollection->m_StaticSprites["lmp_rail_horizontal"]);
	lmprail_tmep.push_back(&m_ResourceCollection->m_StaticSprites["lmp_rail_vertical"]);
	lmprail_tmep.push_back(&m_ResourceCollection->m_StaticSprites["lmp_rail_diagonal"]);

	// ���̵� �÷��� ��Ŀ ��������Ʈ 
	Sprite* cmpanchor_s_temp = &m_ResourceCollection->m_StaticSprites["anchor_short"];
	Sprite* cmpanchor_l_temp = &m_ResourceCollection->m_StaticSprites["anchor_long"];

	// ��� ��������Ʈ ����
	vector<Sprite*> background_temp;
	background_temp.push_back(&m_ResourceCollection->m_StaticSprites["BackgroundNomal_01"]);
	background_temp.push_back(&m_ResourceCollection->m_StaticSprites["BackgroundNomal_02"]);
	background_temp.push_back(&m_ResourceCollection->m_StaticSprites["BackgroundNomal_03"]);
	background_temp.push_back(&m_ResourceCollection->m_StaticSprites["BackgroundNomal_04"]);
	background_temp.push_back(&m_ResourceCollection->m_StaticSprites["BackgroundNomal_05"]);

	// ������ ��� ��������Ʈ
	vector<Sprite*> bossbackground_temp;
	bossbackground_temp.push_back(&m_ResourceCollection->m_StaticSprites["BackgroundBoss_01"]);

	// ���ϼ��� ��׶��� ��������Ʈ
	vector<Sprite*> coilsectorbackground_temp;
	coilsectorbackground_temp.push_back(&m_ResourceCollection->m_StaticSprites["CoilSector_Background"]);

	// ������ �� �ִϸ��̼� ����
	vector<Animation*> fixedEanim_temp;
	fixedEanim_temp.push_back(&m_ResourceCollection->m_Animations["SE_Idle"]);
	fixedEanim_temp.push_back(&m_ResourceCollection->m_Animations["SE_Moving"]);
	fixedEanim_temp.push_back(&m_ResourceCollection->m_Animations["SE_Attack_1"]);
	fixedEanim_temp.push_back(&m_ResourceCollection->m_Animations["SE_Attack_2"]);

	// ������� �� �ִϸ��̼� ����
	vector<Animation*> flyhorEanim_temp;
	flyhorEanim_temp.push_back(&m_ResourceCollection->m_Animations["FH_Idle"]);

	// �������� �� �ִϸ��̼� ����
	vector<Animation*> flyverEanim_temp;
	flyverEanim_temp.push_back(&m_ResourceCollection->m_Animations["FV_Idle"]);

	// �Ϲ� �� �ִϸ��̼� ����
	vector<Animation*> normalEanim_temp;
	normalEanim_temp.push_back(&m_ResourceCollection->m_Animations["NE_Idle"]);
	normalEanim_temp.push_back(&m_ResourceCollection->m_Animations["NE_Moving"]);

	// �� �Ѿ� ��������Ʈ 
	Sprite* bulletEimg_temp = &m_ResourceCollection->m_StaticSprites["bullet_red"];

	// �� ����� �����Ǵ� ��ƼŬ ����Ʈ ��������Ʈ
	Sprite* destroyparticle_temp = &m_ResourceCollection->m_StaticSprites["TestParticle"];

	// =========================================================================================
	// ���� ====================================================================================
	
	// ���� ���� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < PLATFORM_STATIC_AMOUNT; i++)
	{
		O_PlatformStatic* temp_ps = new O_PlatformStatic;
		temp_ps->Initialize(input_GameScene, NULL);
		DactObj.P_static.push_back(temp_ps);
	}
	// ���̵� ���� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < PLATFORM_CIRCLE_AMOUNT; i++)
	{
		O_PlatformCircle* temp_pc = new O_PlatformCircle;
		temp_pc->Initialize(input_GameScene, movintPanim_temp, cmpanchor_s_temp, cmpanchor_l_temp);
		DactObj.P_circle.push_back(temp_pc);
	}
	// ���M�̵� ���� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < PLATFORM_DIGT_AMOUNT; i++)
	{
		O_PlatformDigt* temp_pd = new O_PlatformDigt;
		DactObj.P_digt.push_back(temp_pd);
	}
	// ���̵� ���� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < PLATFORM_LINE_AMOUNT; i++)
	{
		O_PlatformLine* temp_pl = new O_PlatformLine;
		temp_pl->Initialize
		(input_GameScene, movintPanim_temp,
			lmprail_tmep[0], lmprail_tmep[1], lmprail_tmep[2]);

		DactObj.P_line.push_back(temp_pl);
	}

	// =========================================================================================
	// �� ======================================================================================
	
	// ������ �� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < ENEMY_FIXED_AMOUNT; i++)
	{
		O_FixedEnemy* temp_efix = new O_FixedEnemy;
		temp_efix->Initialize
		(input_GameScene, fixedEanim_temp, destroyparticle_temp, bulletEimg_temp);

		DactObj.E_fixed.push_back(temp_efix);
	}

	// ������� �� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < ENEMY_FLYHOR_AMOUNT; i++)
	{
		O_FlyingHorizontalEnemy* temp_efhor = new O_FlyingHorizontalEnemy;
		temp_efhor->Initialize
		(input_GameScene, flyhorEanim_temp, destroyparticle_temp, bulletEimg_temp);

		DactObj.E_flyhor.push_back(temp_efhor);
	}
	
	// �������� �� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < ENEMY_FLYVER_AMOUNT; i++)
	{
		O_FlyingVerticalEnemy* temp_efver = new O_FlyingVerticalEnemy;
		temp_efver->Initialize
		(input_GameScene, flyverEanim_temp, destroyparticle_temp, bulletEimg_temp);

		DactObj.E_flyver.push_back(temp_efver);
	}

	// �Ϲ� �� ������Ʈ ���� �� ���Ϳ� Ǫ��
	for (int i = 0; i < ENEMY_NORMAL_AMOUNT; i++)
	{
		O_NormalEnemy* temp_enor = new O_NormalEnemy;
		temp_enor->Initialize
		(input_GameScene, destroyparticle_temp, normalEanim_temp);

		DactObj.E_normal.push_back(temp_enor);
	}

	// =========================================================================================
	// ��� ====================================================================================

	// ��� ������Ʈ ����
	for (int i = 0; i < 7; i++)
	{
		O_BackGround* bgo_temp = new O_BackGround;
		bgo_temp->Initialize(input_GameScene, background_temp);
		bgo_temp->SetSize(bt_Normal);
		m_BackGround.push_back(bgo_temp);

		// ������ ���
		O_BackGround* bsbgo_temp = new O_BackGround;
		bsbgo_temp->Initialize(input_GameScene, bossbackground_temp);
		bsbgo_temp->SetSize(bt_BossStage);
		m_BGBossStage.push_back(bsbgo_temp);
	}

	// ���ϼ��� ��� ������Ʈ ����
	for (int i = 0; i < 5; i++)
	{
		O_BackGround* bgsc_temp = new O_BackGround;
		bgsc_temp->Initialize(input_GameScene, coilsectorbackground_temp);
		bgsc_temp->SetSize(bt_CoilSector);
		m_BGCoilSector.push_back(bgsc_temp);
	}

	// ���ϼ��� ����
	vector<Animation*> tmep;
	m_ResourceCollection->m_Animations["CoilCoil_Closing"].isloop = false;
	tmep.push_back(&m_ResourceCollection->m_Animations["CoilCoil_Opened"]);
	tmep.push_back(&m_ResourceCollection->m_Animations["CoilCoil_Closing"]);

	CoilSector_low = new O_CoilSector;
	CoilSector_low->Initialize(
		input_GameScene, tmep, input_gamestate,
		&m_ResourceCollection->m_StaticSprites["coil_platform"]);

	CoilSector_high = new O_CoilSector;
	CoilSector_high->Initialize(
		input_GameScene, tmep, input_gamestate,
		&m_ResourceCollection->m_StaticSprites["coil_platform"]);


	// ���� ���� ������Ʈ ============================================================
 
	// ���� ������ �ε�
	VoidSecData.SectorObjects =
		m_mapdataloader->LoadMapData("../Resource/json/Mapdata/VoidSector.json");

	// ���� ���� �� ���� �߰�
	for (int i = 0; i < VoidSecData.SectorObjects.size(); i++)
	{
		O_PlatformStatic* sp_temp = new O_PlatformStatic;
		sp_temp->Initialize(input_GameScene,
			&m_ResourceCollection->m_StaticSprites[PlatformSpriteKey[4]]);
		VoidSecObj.P_static.push_back(sp_temp);
	}

	// ���� ��׶��� ������Ʈ
	for (int i = 0; i < 2; i++)
	{
		O_BackGround* vbgo_temp = new O_BackGround;
		vbgo_temp->Initialize(input_GameScene, bossbackground_temp);
		vbgo_temp->SetSize(bt_BossStage);
		m_BGVoidSector.push_back(vbgo_temp);
	}

	// �� ��ȣ ��������Ʈ
	vector<Sprite*> temp_om;
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num01"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num02"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num03"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num04"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num05"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num06"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num07"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num08"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num09"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num10"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num11"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num12"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num13"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num14"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num15"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num16"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num17"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num18"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num19"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num20"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num21"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num22"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num23"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num24"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num25"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num26"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num27"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num28"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num29"]);
	temp_om.push_back(&m_ResourceCollection->m_StaticSprites["num30"]);

	// �� ǥ�� ������Ʈ ���� �� �ʱ�ȭ
	VoidSecObj.O_number = new O_VoidSecNumber;
	VoidSecObj.O_number->Initialize(input_GameScene, temp_om);

	// ���ϼ��� ����
	CoilSector_voidsec = new O_CoilSector;
	CoilSector_voidsec->Initialize(
		input_GameScene, tmep, input_gamestate,
		&m_ResourceCollection->m_StaticSprites["coil_platform"]);
	CoilSector_voidsec->SetVoidSecCoil(true);



	// ��������
	vector<Animation*> BosE;
	BosE.push_back(&m_ResourceCollection->m_Animations["B_Idle"]);
	BosE.push_back(&m_ResourceCollection->m_Animations["B_Climb"]);
	m_ResourceCollection->m_Animations["B_Climb"].isloop = false;
	BosE.push_back(&m_ResourceCollection->m_Animations["B_horizontalMoving_1"]);
	m_ResourceCollection->m_Animations["B_horizontalMoving_1"].isloop = false;
	BosE.push_back(&m_ResourceCollection->m_Animations["B_horizontalMoving_2"]);
	m_ResourceCollection->m_Animations["B_horizontalMoving_2"].isloop = false;

	vector<Animation*> Laser;
	Laser.push_back(&m_ResourceCollection->m_Animations["Laser"]);
	m_ResourceCollection->m_Animations["Laser"].isloop = false;
	Laser.push_back(&m_ResourceCollection->m_Animations["LaserWarning"]);

	Sprite* sprite
		= (&m_ResourceCollection->m_StaticSprites["Laserpod"]);

	// todo  ���� ��������Ʈ�� ������ ���ϸ��̼� ���͸� ���ڷ� �Ѱ��ֱ�
	m_BossObject = new O_Boss;
	m_BossObject->Initialize(input_GameScene, BosE, Laser, sprite);
}

// ���� ������Ʈ ���� ( Ȱ��ȭ �� ��ġ )
void ObjectCollection::GenerateSector(int input_index)
{
	Vector2D pos1, pos2;
	int SizeIndex;

	// ���� ������Ʈ ���� ��ŭ �ݺ��� ����
	for (int i = 0; i < SectorData[input_index].SectorObjects.size(); i++)
	{
		pos1 = SectorData[input_index].SectorObjects[i].Pos1;
		pos2 = SectorData[input_index].SectorObjects[i].Pos2;
		SizeIndex = SectorData[input_index].SectorObjects[i].platformsizeindex;

		// ����� =====================================================================
		bool isCir;
		isCir = (SectorData[input_index].SectorObjects[i].objtype == ot_PlatformCircle);
		if (true == false)
		{
			if (pos1.X != 0)
			{
				if (isup)
				{
					if (SectorData[input_index].SectorObjects[i].objtype == ot_PlatformStatic)
					{
						deb1->AddVector(
							Vector2D(
								(m_sectorpos.X + pos1.X) / ADJUST_X,
								m_sectorpos.Y + pos1.Y), isCir);
					}
					else
					{
						deb1->AddVector(
							Vector2D(
								(m_sectorpos.X + pos1.X) / ADJUST_X,
								m_sectorpos.Y + pos1.Y), isCir);
						deb1->AddVector(
							Vector2D(
								(m_sectorpos.X + pos2.X) / ADJUST_X,
								m_sectorpos.Y + pos2.Y), isCir);
					}
				}
				else
				{
					if (SectorData[input_index].SectorObjects[i].objtype == ot_PlatformStatic)
					{
						deb4->AddVector(
							Vector2D(
								(m_sectorpos.X + pos1.X) / ADJUST_X,
								m_sectorpos.Y + pos1.Y), isCir);
					}
					else
					{
						deb4->AddVector(
							Vector2D(
								(m_sectorpos.X + pos1.X) / ADJUST_X,
								m_sectorpos.Y + pos1.Y), isCir);
						deb4->AddVector(
							Vector2D(
								(m_sectorpos.X + pos2.X) / ADJUST_X,
								m_sectorpos.Y + pos2.Y), isCir);
					}
				}
			}
		}
		//=============================================================================

		switch (SectorData[input_index].SectorObjects[i].objtype)
		{
			// ���� ��
			case ot_Monster1:
			{
			
					O_FixedEnemy* temp_efix = DactObj.E_fixed.back();
					DactObj.E_fixed.pop_back();
					temp_efix->SetToGameScene(
						Vector2D((m_sectorpos.X + pos1.X) / ADJUST_X, m_sectorpos.Y + pos1.Y));

					ActObj.E_fixed.push_back(temp_efix);
					temp_efix->Activate();
				
				break;
			}
			// �Ϲ� ��
			case ot_Monster2:
			{
				O_NormalEnemy* temp_enor = DactObj.E_normal.back();
				DactObj.E_normal.pop_back();
				temp_enor->SetToGameScene(
					Vector2D((m_sectorpos.X + pos1.X) / ADJUST_X, m_sectorpos.Y + pos1.Y),
					Vector2D((m_sectorpos.X + pos2.X) / ADJUST_X, m_sectorpos.Y + pos2.Y),
					SectorData[input_index].SectorObjects[i].movespeed / 10);

				ActObj.E_normal.push_back(temp_enor);
				temp_enor->Activate();
				break;
			}
			// ���� �� ����
			case ot_Monster3:
			{
				O_FlyingHorizontalEnemy* temp_fhor = DactObj.E_flyhor.back();
				DactObj.E_flyhor.pop_back();
				temp_fhor->SetToGameScene(
					Vector2D((m_sectorpos.X + pos1.X) / ADJUST_X, m_sectorpos.Y + pos1.Y),
					Vector2D((m_sectorpos.X + pos2.X) / ADJUST_X, m_sectorpos.Y + pos2.Y),
					SectorData[input_index].SectorObjects[i].movespeed / 10);

				ActObj.E_flyhor.push_back(temp_fhor);
				temp_fhor->Activate();
				break;
			}
			// ���� �� ����
			case ot_Monster4:
			{
				
					O_FlyingVerticalEnemy* temp_fver = DactObj.E_flyver.back();
					DactObj.E_flyver.pop_back();
					temp_fver->SetToGameScene(
						Vector2D((m_sectorpos.X + pos1.X) / ADJUST_X, m_sectorpos.Y + pos1.Y),
						Vector2D((m_sectorpos.X + pos2.X) / ADJUST_X, m_sectorpos.Y + pos2.Y),
						SectorData[input_index].SectorObjects[i].movespeed / 10);

					ActObj.E_flyver.push_back(temp_fver);
					temp_fver->Activate();
			
				break;
			}
			// �÷��� ����
			case ot_PlatformStatic:
			{
				O_PlatformStatic* temp_ps = DactObj.P_static.back();
				DactObj.P_static.pop_back();
				temp_ps->SetToGameScene(Vector2D(
					(m_sectorpos.X + pos1.X) / ADJUST_X, m_sectorpos.Y + pos1.Y),
					PlatformSize[SizeIndex],
					&m_ResourceCollection->m_StaticSprites
					[PlatformSpriteKey[SizeIndex]]);
				ActObj.P_static.push_back(temp_ps);
				temp_ps->Activate();
				break;
			}
			// �÷��� ���̵�
			case ot_PlatformLine:
			{
				O_PlatformLine* temp_pl = DactObj.P_line.back();
				DactObj.P_line.pop_back();
				temp_pl->SetToGameScene(
					Vector2D(
						(m_sectorpos.X + pos1.X) / ADJUST_X,
						m_sectorpos.Y + pos1.Y),
					Vector2D(
						(m_sectorpos.X + pos2.X) / ADJUST_X,
						m_sectorpos.Y + pos2.Y),
					PlatformSize[SizeIndex],
					SectorData[input_index].SectorObjects[i].movespeed / 10,
					SizeIndex);
				temp_pl->Activate();
				ActObj.P_line.push_back(temp_pl);
				break;
			}
			// �÷��� ���̵�
			case ot_PlatformCircle:
			{
				O_PlatformCircle* temp_pc = DactObj.P_circle.back();
				DactObj.P_circle.pop_back();
				temp_pc->SetToGameScene(
					Vector2D(
						(m_sectorpos.X + pos2.X) / ADJUST_X,
						m_sectorpos.Y + pos2.Y),
					Vector2D(
						(m_sectorpos.X + pos1.X) / ADJUST_X,
						m_sectorpos.Y + pos1.Y),
					PlatformSize[SizeIndex],
					SectorData[input_index].SectorObjects[i].movespeed / 10,
					SectorData[input_index].SectorObjects[i].rotatedirection,
					SizeIndex);
				temp_pc->Activate();
				ActObj.P_circle.push_back(temp_pc);
				break;
			}
			// �÷��� ���M�̵�
			case ot_PlatformDigt:
			{

				break;
			}

		}
	}
}

// �÷ξ� ������Ʈ ����( Ȱ��ȭ �� ��ġ ) 
void ObjectCollection::GenerateFloor(bool isfirst)
{
	// ���� ������ ���� �ε���
	int num1, num2, num3;

	if (isfirst)
	{
		RandomIndexing(&num1, &num2, &num3);

		CoilSector_low->SetCoil(true);
		CoilSector_low->SetPosition
		(Vector2D(m_sectorpos.X, m_sectorpos.Y));
		SetBackground();

		m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																													if (isDebuggerON)deb1->SetCoord(m_sectorpos, num1);
		
		GenerateSector(num1);
		SetBackground();
		m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																													if (isDebuggerON)deb2->SetCoord(m_sectorpos, num2);
		GenerateSector(num2);
		SetBackground();
		m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																													if (isDebuggerON)deb3->SetCoord(m_sectorpos, num3);
		GenerateSector(num3);
		SetBackground();
		m_sectorpos.Y -= COILSECTOR_HEIGHT;
		GenerateCoilSector(isfirst);
	}

	else
	{
		m_sectorpos.Y -= COILSECTOR_HEIGHT;
		GenerateCoilSector(isfirst);
	}

	RandomIndexing(&num1, &num2, &num3);

	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																													if (isDebuggerON) { deb4->SetCoord(m_sectorpos, num1);}
	GenerateSector(num1);
	SetBackground();
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																													if (isDebuggerON) { deb5->SetCoord(m_sectorpos, num2); }
	GenerateSector(num2);
	SetBackground();
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																													if (isDebuggerON) { deb6->SetCoord(m_sectorpos, num3); }
	GenerateSector(num3);
	SetBackground();

	m_sectorpos.Y -= SECTOR_HEIGHT;
	GenerateVoidSector();

	isup = !isup;
}

// ���� ������Ʈ ������ �ε� �� ����
void ObjectCollection::GenerateVoidSectorResources
(std::shared_ptr<GameScene> input_GameScene, GameStateManager* input_gamestate)
{
	// ���� ������ �ε�
	VoidSecData.SectorObjects = 
		m_mapdataloader->LoadMapData("../Resource/json/Mapdata/VoidSector.json");

	// ���� ���� �� ���� �߰�
	for (int i = 0; i < VoidSecData.SectorObjects.size(); i++)
	{
		O_PlatformStatic* sp_temp = new O_PlatformStatic;
		sp_temp->Initialize(input_GameScene, 
			&m_ResourceCollection->m_StaticSprites[PlatformSpriteKey[4]]);
		VoidSecObj.P_static.push_back(sp_temp);
	}
	
	// ���� ���� �ִϸ��̼�
	vector<Animation*> tmep_anim;
	m_ResourceCollection->m_Animations["CoilCoil_Closing"].isloop = false;
	tmep_anim.push_back(&m_ResourceCollection->m_Animations["CoilCoil_Opened"]);
	tmep_anim.push_back(&m_ResourceCollection->m_Animations["CoilCoil_Closing"]);

	// ���ϼ��� ����
	CoilSector_voidsec = new O_CoilSector;
	CoilSector_voidsec->Initialize(
		input_GameScene, tmep_anim,input_gamestate, 
		&m_ResourceCollection->m_StaticSprites["coil_platform"]);
}

// ��׶��� Ȱ��ȭ
void ObjectCollection::SetBackground()
{
	int num = index_range_backgroundimage(RandomDevice);

	m_BackGround[currentbg_index]->
		SetToGameScene(Vector2D(
			m_sectorpos.X + SECTOR_WIDTH / 2,
			m_sectorpos.Y + SECTOR_HEIGHT / 2),
			num);

	currentbg_index++;

	if (currentbg_index == m_BackGround.size())
	{
		currentbg_index = 0;
	}
}

// ������ ��׶��� Ȱ��ȭ
void ObjectCollection::SetBossBackground()
{
	m_BGBossStage[currentBossStagebg_indxe]->
		SetToGameScene(Vector2D(
			m_sectorpos.X + SECTOR_WIDTH / 2,
			m_sectorpos.Y + SECTOR_HEIGHT / 2),0);

	currentBossStagebg_indxe++;

	if (currentBossStagebg_indxe == m_BGBossStage.size())
	{
		currentBossStagebg_indxe = 0;
	}
}

// ���� ��׶��� Ȱ��ȭ
void ObjectCollection::SetVoidSecBackground()
{
	m_BGVoidSector[currentvbg_index]->
		SetToGameScene(Vector2D(
			m_sectorpos.X + SECTOR_WIDTH / 2,
			m_sectorpos.Y + SECTOR_HEIGHT / 2),0);

	currentvbg_index++;

	if (currentvbg_index == m_BGVoidSector.size())
	{
		currentvbg_index = 0;
	}
}

// ��ü ��׶��� ��Ȱ��ȭ
void ObjectCollection::SetOffAllBackground()
{
	for (int i = 0; i < m_BackGround.size(); i++)
	{
		m_BackGround[i]->DeActivate();
	}
}

// ���� ���»�� �Է°����� �ʱ�ȭ
// �Է°� : �ʱ�ȭ �� ��ǥ
void ObjectCollection::SetSetorPos(Vector2D input_pos)
{
	m_sectorpos = input_pos;
}

// �ε��� ���� ����
// �Է°� : ������������ ���� ���� ������ ������
void ObjectCollection::RandomIndexing(int* num1, int* num2, int* num3)
{
	*num1 = index_range_mapdata(RandomDevice);
	while (1)
	{
		*num2 = index_range_mapdata(RandomDevice);
		if (*num2 != *num1)
		{
			break;
		}
	}
	while (1)
	{
		*num3 = index_range_mapdata(RandomDevice);
		if (*num3 != *num1 && *num3 != *num2)
		{
			break;
		}
	}
}

// �� ���� ��ġ ����
void ObjectCollection::SetVoidSectorPos()
{
	if (isVoidSecUnder)
	{
		//-----------------------------------/ COIL
		m_sectorpos.Y -= COILSECTOR_HEIGHT;
		SetCoilSectorBackground();
		CoilSector_voidsec->SetCoil(false);
		CoilSector_voidsec->SetPosition(m_sectorpos);
		//-----------------------------------/

		m_sectorpos.Y -= SECTOR_HEIGHT;
		GenerateVoidSector();
	}
	else
	{
		SetCoilSectorBackground();
		CoilSector_voidsec->SetPosition(m_sectorpos);
		m_sectorpos.Y -= SECTOR_HEIGHT;
		GenerateVoidSector();
	}
}

// ���Ͻ���
void ObjectCollection::CoilSwap()
{
	O_CoilSector* temp;
	temp = CoilSector_high;
	CoilSector_high = CoilSector_low;
	CoilSector_low = temp;
}

void ObjectCollection::GenerateFloor_Circling(bool isfirst)
{
	// ���� ������ ���� �ε���
	int num1, num2, num3;

	RandomIndexing(&num1, &num2, &num3);

	if (isfirst)
	{
		AutoBackGround();
	}
	else
	{
		// �÷ξ� ī��Ʈ ++
		FloorCount++;
	}

	//----------------------------------- / COIL
	CoilSector_low->SetCoil(false);
	CoilSector_low->SetPosition(m_sectorpos);
	SetCoilSectorBackground();
	//----------------------------------- /

	//============================	1
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																				if (isDebuggerON)deb1->SetCoord(m_sectorpos, num1);
	GenerateSector(num1);
	AutoBackGround();
	//============================	2
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																				if (isDebuggerON)deb2->SetCoord(m_sectorpos, num2);
	GenerateSector(num2);
	AutoBackGround();
	//============================	3
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																				if (isDebuggerON)deb3->SetCoord(m_sectorpos, num3);
	GenerateSector(num3);
	AutoBackGround();
	//=============================

	//-----------------------------------/ COIL
	m_sectorpos.Y -= COILSECTOR_HEIGHT;
	CoilSector_high->SetCoil(false);
	CoilSector_high->SetPosition(m_sectorpos);
	SetCoilSectorBackground();
	//-----------------------------------/

	RandomIndexing(&num1, &num2, &num3);

	//=============================	4
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																				if (isDebuggerON) { if (isup)deb1->SetCoord(m_sectorpos, num1); else deb4->SetCoord(m_sectorpos, num1); }
	GenerateSector(num1);
	AutoBackGround();
	//=============================	5
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																				if (isDebuggerON) { if (isup)deb2->SetCoord(m_sectorpos, num2); else deb5->SetCoord(m_sectorpos, num2); }
	GenerateSector(num2);
	AutoBackGround();
	//=============================	6
	m_sectorpos.Y -= SECTOR_HEIGHT;												/* �÷ξ� ����� -> */
																				if (isDebuggerON) { if (isup)deb3->SetCoord(m_sectorpos, num3); else deb6->SetCoord(m_sectorpos, num3); }
	GenerateSector(num3);
	AutoBackGround();
	//=============================

	isup = !isup;

	if (isfirst)
	{
		//-----------------------------------/ COIL
		m_sectorpos.Y -= COILSECTOR_HEIGHT;
		CoilSector_voidsec->SetCoil(false);
		CoilSector_voidsec->SetPosition(m_sectorpos);
		SetCoilSectorBackground();
		//-----------------------------------/

		m_sectorpos.Y -= SECTOR_HEIGHT;
		GenerateVoidSector();
	}
}

void ObjectCollection::AutoBackGround()
{
	if (isBossStage)
	{
		SetBossBackground();
	}
	else
	{
		SetBackground();
	}
}

void ObjectCollection::SetCoilSectorBackground()
{
	m_BGCoilSector[currentCoilSecbg_index]->
		SetToGameScene(Vector2D(
			m_sectorpos.X + SECTOR_WIDTH / 2,
			m_sectorpos.Y + COILSECTOR_HEIGHT / 2), 0);

	currentCoilSecbg_index++;

	if (currentCoilSecbg_index == m_BGCoilSector.size())
	{
		currentCoilSecbg_index = 0;
	}
}

void ObjectCollection::SetBossStage()
{
	isBossStage = true;
	m_playerobj->SetBossStage();
	m_playerobj->GetPlayerActor()->transform->SetPosition(Vector2D(1100, 2000));
	
	m_BossObject->GetBossActor()->transform->SetPosition(Vector2D(1100, 3500));
	m_BossObject->Activate();

	m_playerobj->GetPlayerActor()->GetComponent<CC_Camera>()->SetTarget
	(m_BossObject->GetBossActor()->transform);
	m_playerobj->GetPlayerActor()->GetComponent<CC_Camera>()->
		SetAdjustAmount(Vector2D(0, -450));
}

// ���� ���� ���� ( ��Ȱ��ȭ �� ���ġ )
void ObjectCollection::GenerateCoilSector(bool isfirst)
{
	if (!isfirst)
	{
		CoilSwap();
	}
	CoilSector_high->SetCoil(false);
	CoilSector_high->SetPosition
	(Vector2D(m_sectorpos.X, m_sectorpos.Y));
	return;
}

// �� ���� ����
void ObjectCollection::GenerateVoidSector()
{
	Vector2D pos_temp;

	SetVoidSecBackground();

	for (int i = 0; i < VoidSecData.SectorObjects.size(); i++)
	{
		pos_temp = Vector2D(
			(m_sectorpos.X + VoidSecData.SectorObjects[i].Pos1.X) / ADJUST_X,
			m_sectorpos.Y + VoidSecData.SectorObjects[i].Pos1.Y);

		VoidSecObj.P_static[i]->
			SetToGameScene(pos_temp, PlatformSize[4],
				&m_ResourceCollection->m_StaticSprites[PlatformSpriteKey[4]]);

		VoidSecObj.P_static[i]->Activate();
	}

	if (FloorCount >= 30)
	{
		VoidSecObj.O_number->SetImage(29);
	}
	else
	{
		VoidSecObj.O_number->SetImage(FloorCount - 1);
	}
	VoidSecObj.O_number->SetPosition
	(Vector2D(m_sectorpos.X + SECTOR_WIDTH/2 , m_sectorpos.Y + SECTOR_HEIGHT/2));
}
