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
	player_->Initialize(playerModel_, playerTextureHandle_);

	enemyTextureHandle_ = TextureManager::Load("virus_character.png");
	enemy_ = new Enemy();
	enemy_->Initialize(playerModel_, enemyTextureHandle_);
	enemy_->SetPlayer(player_);
	enemy_->InitApproach();

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

	std::list<Collider*> colliders_;
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);
	for (PlayerBullet* bullet : playerBullets) {
		colliders_.push_back(bullet);
	}
	for (EnemyBullet* enemyBullet : enemyBullets) {
		colliders_.push_back(enemyBullet);
	}

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end();++itrB) {
			Collider* colliderB = *itrB;
			CheckCollisionPair(colliderA, colliderB);
		}
	}

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

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB)
{
	if (0xffffffff == (colliderA->GetAttribute() ^ colliderB->GetMask()) || 0xffffffff == (colliderB->GetAttribute() ^ colliderA->GetMask()))
	{
		return;
	}

	Vector3 posA, posB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	Vector3 leng = posB - posA;
	float length = leng.Lenght();
	if (length <= 3.0f) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
