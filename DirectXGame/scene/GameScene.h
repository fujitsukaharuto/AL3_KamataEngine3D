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
#include "LockOn.h"
#include "CollisionManager.h"
#include "Effect.h"

#include <memory>

enum class SceneType : uint32_t {
	title,
	tutorial,
	game,
	gameClear,
	gameOver,
};

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

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	void BlackMove();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Effect* effect_;

	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCameraMode_ = false;

	ViewProjection viewProject_;

	SceneType sceneType_ = SceneType::title;
	std::optional<SceneType> sceneTypeRequest_ = std::nullopt;

	uint32_t sceneChangeTimer_ = 0;
	uint32_t stiffnessTimer_ = 0;
	bool isSceneChange_ = false;
	bool isClear_ = false;
	bool isGameover_ = false;

	uint32_t titleHandle_ = 0;
	Sprite* titleSrite_;

	uint32_t blackHandle_ = 0;
	Sprite* blackSrite_;

	uint32_t gameClearHandle_ = 0;
	Sprite* gameClearSrite_;

	uint32_t gameOverHandle_ = 0;
	Sprite* gameOverSrite_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	std::unique_ptr<LockOn> lockOn_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<CollisionManager> collisionManager_;

	uint32_t playerTextureHandle_ = 0;
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	std::unique_ptr<Player> player_;

	std::unique_ptr<Model> modelPlayerWeapon_;
	std::unique_ptr<Model> modelPlayerBullet_;

	std::unique_ptr<Model> enemyModelfightBody_;
	std::unique_ptr<Model> enemyModelfightHead_;
	std::unique_ptr<Model> enemyModelfightL_arm_;
	std::unique_ptr<Model> enemyModelfightR_arm_;
	std::unique_ptr<Model> enemyModelfightWeapon_;
	std::list<std::unique_ptr<Enemy>> enemies_;

	std::unique_ptr<Model> skydomeModel_ = nullptr;
	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> groundModel_ = nullptr;
	std::unique_ptr<Ground> ground_;


	uint32_t AButtonSound_ = 0;
	uint32_t clearSound_ = 0;
	uint32_t overSound_ = 0;
	uint32_t bgmSound_ = 0;

	uint32_t roopStopHandle_ = 0;

};
