#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SkyDome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include <sstream>

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
	void CheckAllCollision();

	void AddEnemyBullet(EnemyBullet* enemybullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopDate();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	void EnemyPop(Vector3 pos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	bool isDEbugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	ViewProjection viewProject_;

	uint32_t playerTextureHandle_ = 0;
	Model* playerModel_ = nullptr;

	Player* player_ = nullptr;

	uint32_t enemyTextureHandle_ = 0;
	std::list<Enemy*> enemy_;
	std::list<EnemyBullet*> enemybullets_;

	Model* skyDomeModel_ = nullptr;
	SkyDome* skyDome_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	CollisionManager* collisionManager_ = nullptr;

	std::stringstream enemyPopCommands;

	bool isWait_;
	int32_t waitTimer_ = 0;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
