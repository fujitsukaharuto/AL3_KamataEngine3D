#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete playerModel_; 
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete collisionManager_;
	delete skyDome_;
	delete skyDomeModel_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProject_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProject_);

	playerTextureHandle_ = TextureManager::Load("human.png");
	playerModel_ = Model::Create();

	player_ = new Player();
	player_->Initialize(playerModel_, playerTextureHandle_, Vector3{0.0f, 0.0f, 30.0f});

	enemyTextureHandle_ = TextureManager::Load("virus_character.png");
	enemy_ = new Enemy();
	enemy_->Initialize(playerModel_, enemyTextureHandle_);
	enemy_->SetPlayer(player_);
	enemy_->InitApproach();

	skyDome_ = new SkyDome();
	skyDomeModel_ = Model::CreateFromOBJ("skyDome", true);
	skyDome_->Initialize(skyDomeModel_);

	railCamera_ = new RailCamera();
	railCamera_->Initialize(Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 0.0f, 0.0f});

	player_->SetParent(&railCamera_->GetWorldTransform());

	collisionManager_ = new CollisionManager();

}

void GameScene::Update() {

	player_->Update();

	if (enemy_) {
		enemy_->Update();
	}

	CheckAllCollision();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F12)) {
		if (!isDEbugCameraActive_) {
			isDEbugCameraActive_ = true;
		} else if (isDEbugCameraActive_) {
			isDEbugCameraActive_ = false;
		}
	}
#endif // _DEBUG
	if (isDEbugCameraActive_) {
		debugCamera_->Update();
		viewProject_.matView = debugCamera_->GetViewProjection().matView;
		viewProject_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProject_.TransferMatrix();
	} else {
		viewProject_.UpdateMatrix();
		railCamera_->Update();
		viewProject_.matView = railCamera_->GetViewProjection().matView;
		viewProject_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProject_.TransferMatrix();

	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skyDome_->Draw(viewProject_);

	player_->Draw(viewProject_);
	if (enemy_) {
		enemy_->Draw(viewProject_);
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision()
{

	//Vector3 posA, posB;

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	collisionManager_->ClearCollider();

	collisionManager_->SetCollider(player_);
	collisionManager_->SetCollider(enemy_);

	for (PlayerBullet* bullet : playerBullets) {
		collisionManager_->SetCollider(bullet);
	}
	for (EnemyBullet* enemyBullet : enemyBullets) {
		collisionManager_->SetCollider(enemyBullet);
	}

	collisionManager_->CheckCollision();

//#pragma region 自キャラと敵弾の当たり判定
//	/*posA = player_->GetWorldPosition();
//
//	for (EnemyBullet* bullet : enemyBullets) {
//		posB = bullet->GetWorldPosition();
//		Vector3 leng = posB - posA;
//		float length = leng.Lenght();
//		if (length<=3.0f) {
//			player_->OnCollision();
//			bullet->OnCollision();
//		}
//	}*/
//
//	for (EnemyBullet* enemyBullet : enemyBullets) {
//	
//		CheckCollisionPair(player_, enemyBullet);
//	
//	}
//
//#pragma endregion
//
//#pragma region 敵キャラと自弾の当たり判定
//	/*posA = enemy_->GetWorldPosition();
//
//	for (PlayerBullet* bullet : playerBullets) {
//		posB = bullet->GetWorldPosition();
//		Vector3 leng = posB - posA;
//		float length = leng.Lenght();
//		if (length <= 3.0f) {
//			enemy_->OnCollision();
//			bullet->OnCollision();
//		}
//	}*/
//
//	for (PlayerBullet* bullet : playerBullets) {
//
//		CheckCollisionPair(enemy_, bullet);
//	}
//
//#pragma endregion
//
//#pragma region 敵弾と自弾の当たり判定
//
//	/*for (PlayerBullet* bullet : playerBullets) {
//		posA = bullet->GetWorldPosition();
//		for (EnemyBullet* enemybullet : enemyBullets) {
//			posB = enemybullet->GetWorldPosition();
//			Vector3 leng = posB - posA;
//			float length = leng.Lenght();
//			if (length <= 3.0f) {
//				enemybullet->OnCollision();
//				bullet->OnCollision();
//			}
//		}
//	}*/
//
//	for (PlayerBullet* bullet : playerBullets) {
//		for (EnemyBullet* enemyBullet : enemyBullets) {
//			CheckCollisionPair(bullet, enemyBullet);
//		}
//	}
//
//#pragma endregion

}
