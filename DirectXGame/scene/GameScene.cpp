#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete playerModel_; 
	delete player_;
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

}

void GameScene::Update() {

	player_->Update();
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
