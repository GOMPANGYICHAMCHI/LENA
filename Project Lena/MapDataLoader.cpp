#include "MapDataLoader.h"

// use for json
#include <cstdio>
#include <fstream>
#include "istreamwrapper.h"
#include "stringbuffer.h"
#include "ostreamwrapper.h"

MapDataLoader::MapDataLoader()
{
}

RAPIDJSON_NAMESPACE::Document MapDataLoader::CJsonParser(const char* filepath)
{
	std::ifstream ifs{ filepath };
	if (!ifs.is_open())
	{
		return NULL;
	}

	// json ���� ���� �� �ֵ��� ��ȯ
	IStreamWrapper isw{ ifs };

	// json doc���ٰ� ���� ����
	Document doc{};
	doc.ParseStream(isw);

	return doc;
}

// Json �����ͷ� ���� ������Ʈ ������ �ε�
std::vector<ObjectData> MapDataLoader::LoadMapData(const char* filepath)
{
	Document doc = CJsonParser(filepath);
	std::vector<ObjectData> temp;

	Value& attributes = doc["array"];

	// ������Ʈ ī��Ʈ ����ŭ�� �����´�.
	Value& O_count = doc["ObjectCount"];
	int ObjectCount = O_count.GetInt();
	int InputCount = 0;

	assert(attributes.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
	{
		// �� ��ҵ��� �ӽ÷� ������
		int arr[10] = { 0, };	int count = 0;
		const rapidjson::Value& attribute = *itr;
		// �迭 �� �迭�� ���� �ϳ��� ��������.
		assert(attribute.IsArray());
		for (rapidjson::Value::ConstValueIterator itr2 = attribute.Begin(); itr2 != attribute.End(); ++itr2)
		{
			const rapidjson::Value& num = *itr2;
			arr[count++] = num.GetInt();
		}
		// json�� �迭 �ϳ��� arr�� �����.
		ObjectData tmp;

		// ������Ʈ ���� ����
		switch (arr[_Type])
		{
			case 0:
			{
				tmp.objtype = ot_Monster1;
				break;
			}
			case 1:
			{
				tmp.objtype = ot_Monster2;
				break;
			}
			case 2:
			{
				tmp.objtype = ot_Monster3;
				break;
			}
			case 3:
			{
				tmp.objtype = ot_Monster4;
				break;
			}
			case 4:
			{
				tmp.objtype = ot_PlatformStatic;
				break;
			}
			case 5:
			{
				tmp.objtype = ot_PlatformLine;
				break;
			}
			case 6:
			{
				tmp.objtype = ot_PlatformCircle;
				break;
			}
			case 7:
			{
				tmp.objtype = ot_PlatformDigt;
				break;
			}
			default:
			{
				// �߸��Ǹ� ���� ��ȯ
				tmp.objtype = ot_Monster1;
				break;
			}
		}
		// ��ǥ ����.
		Vector2D v1(arr[_PosX_1], arr[_PosY_1]);
		tmp.Pos1 = v1;
		Vector2D v2(arr[_PosX_2], arr[_PosY_2]);
		tmp.Pos2 = v2;
		//�÷��� ������ �ε���
		tmp.platformsizeindex = arr[_PlatformSize];
		// �̵� �ӵ�
		tmp.movespeed = arr[_MoveSpeed];
		// �÷��� ���Ÿ��
		switch (arr[_Passable])
		{
			case 0:
				tmp.passable = Passable::All;
				break;
			case 1:
				tmp.passable = Passable::Up;
				break;
			case 2:
				tmp.passable = Passable::Default;
				break;
			default:
				// �߸��Ǹ� �׳� �� ���
				tmp.passable = Passable::All;
				break;
		}
		// ȸ�� ����
		tmp.rotatedirection = arr[_Clockwise];
		// ���M �̵��� �̵� ����
		tmp.digtmoveamount = arr[_MoveArea];
		// ���� �����͸� ���Ϳ��ٰ� ����
		temp.push_back(tmp);

		InputCount++;
		if (InputCount >= ObjectCount)
			break;
	}

	return temp;
}
