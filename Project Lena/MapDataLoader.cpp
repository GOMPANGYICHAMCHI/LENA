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

	// json 으로 읽을 수 있도록 변환
	IStreamWrapper isw{ ifs };

	// json doc에다가 내용 저장
	Document doc{};
	doc.ParseStream(isw);

	return doc;
}

// Json 데이터로 부터 오브젝트 데이터 로드
std::vector<ObjectData> MapDataLoader::LoadMapData(const char* filepath)
{
	Document doc = CJsonParser(filepath);
	std::vector<ObjectData> temp;

	Value& attributes = doc["array"];

	// 오브젝트 카운트 수만큼만 가져온다.
	Value& O_count = doc["ObjectCount"];
	int ObjectCount = O_count.GetInt();
	int InputCount = 0;

	assert(attributes.IsArray());
	for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
	{
		// 각 요소들을 임시로 저장함
		int arr[10] = { 0, };	int count = 0;
		const rapidjson::Value& attribute = *itr;
		// 배열 속 배열의 값을 하나씩 가져오기.
		assert(attribute.IsArray());
		for (rapidjson::Value::ConstValueIterator itr2 = attribute.Begin(); itr2 != attribute.End(); ++itr2)
		{
			const rapidjson::Value& num = *itr2;
			arr[count++] = num.GetInt();
		}
		// json의 배열 하나가 arr에 담겼음.
		ObjectData tmp;

		// 오브젝트 종류 저장
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
				// 잘못되면 몬스터 소환
				tmp.objtype = ot_Monster1;
				break;
			}
		}
		// 좌표 설정.
		Vector2D v1(arr[_PosX_1], arr[_PosY_1]);
		tmp.Pos1 = v1;
		Vector2D v2(arr[_PosX_2], arr[_PosY_2]);
		tmp.Pos2 = v2;
		//플랫폼 사이즈 인덱스
		tmp.platformsizeindex = arr[_PlatformSize];
		// 이동 속도
		tmp.movespeed = arr[_MoveSpeed];
		// 플랫폼 통과타입
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
				// 잘못되면 그냥 다 통과
				tmp.passable = Passable::All;
				break;
		}
		// 회전 방향
		tmp.rotatedirection = arr[_Clockwise];
		// 디긎 이동시 이동 정도
		tmp.digtmoveamount = arr[_MoveArea];
		// 빼온 데이터를 벡터에다가 저장
		temp.push_back(tmp);

		InputCount++;
		if (InputCount >= ObjectCount)
			break;
	}

	return temp;
}
