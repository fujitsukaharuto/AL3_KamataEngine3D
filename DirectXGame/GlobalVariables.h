#pragma once
#include <iostream>
#include <fstream>
#include <variant>
#include <map>
#include <json.hpp>

#include "Vector3.h"

class GlobalVariables {
public:
	static GlobalVariables* GetInstance();

	struct Item {
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	// 値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	// 値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, Vector3 value);

	void SaveFile(const std::string& groupName);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();


	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";


private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;


	using json = nlohmann::json;

};
