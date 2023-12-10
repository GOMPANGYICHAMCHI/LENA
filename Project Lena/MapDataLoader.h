#pragma once

#include <vector>
#include "Vector2D.h"
#include "ObjectType.h"
#include "C_PlatformProperty.h"

// rapidjson
#include "document.h"
using namespace rapidjson;

// ������Ʈ ������
//	
//	(����)				(��Ī)				(�뵵)
// 
//	ObjectType			objtype				������Ʈ ����
//	Vector2D			Pos1				��ǥ1
// ///
//	Vector2D			Pos2				��ǥ2
// ///
//	int					platformsizeindex	�÷��� ������ �ε���
//	int					movespeed			�̵� �ӵ�
//	Passable			passable			�÷��� ��� Ÿ��
//	int					rotatedirection		ȸ�� ����
//	int					digtmoveamount		���M �̵��� �̵�����
struct ObjectData
{
	ObjectType objtype;			// ������Ʈ ����		

	// ot_Monster1			(0) = ���� ����
	// ot_Monster2			(1) = �̵� ����
	// ot_Monster3			(2) = ���� ���� , �¿�
	// ot_Monster4			(3) = ���� ���� , ����

	// ot_PlatformStatic	(4) = �÷��� ����
	// ot_PlatformLine		(5) = �÷��� ���̵�
	// ot_PlatformCircle	(6) = �÷��� ���̵�
	// ot_PlatformDigt		(7) = �÷��� ���M�̵�

	//-----------------------------------------------------

	Vector2D Pos1;				// ��ǥ1
	Vector2D Pos2;				// ��ǥ2

	//-----------------------------------------------------

	int platformsizeindex;		// �÷��� ������ �ε���

	//-----------------------------------------------------

	int movespeed;				// �̵��ӵ� ( 1 ~ 100 )

	//-----------------------------------------------------

	Passable passable;			// �÷��� ���Ÿ��

	// Default	(0) = ��� ����			2	��� �Ұ�
	// Up		(1) = ���� ��� ����		1	�Ʒ� ���� �Ұ�
	// Down		(2) = ���� ��� ����
	// All		(3) = ��� ��� ����		0	��� ����
	// Box		(4) = �÷��� ���� ���� �ڽ�

	//-----------------------------------------------------

	int rotatedirection;		// ȸ�� ���� 

	// 0 : �ð� ����		|| ���� ���� 
	// 1 : �ݽð� ����	|| ���� ����

	//-----------------------------------------------------

	int digtmoveamount;			// ���M �̵��� �̵�����

	//-----------------------------------------------------
};


using namespace rapidjson;

class MapDataLoader
{
public:
	MapDataLoader();

	Document CJsonParser(const char* filepath);
	// Json �����ͷ� ���� ������Ʈ ������ �ε�
	std::vector<ObjectData> LoadMapData(const char* filepath);
};

enum JsonFileType
{
	// �� ������� �迭�� ����
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