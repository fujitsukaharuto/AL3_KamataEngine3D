#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "Player.h"
#include "Enemy.h"

#include <memory>


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCameraMode_ = false;

	ViewProjection viewProject_;

	std::unique_ptr<FollowCamera> followCamera_;

	uint32_t playerTextureHandle_ = 0;
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	std::unique_ptr<Player> player_;

	std::unique_ptr<Model> enemyModelfightBody_;
	std::unique_ptr<Model> enemyModelfightWeapon_;
	std::unique_ptr<Enemy> enemy_;

	std::unique_ptr<Model> skydomeModel_ = nullptr;
	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> groundModel_ = nullptr;
	std::unique_ptr<Ground> ground_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
