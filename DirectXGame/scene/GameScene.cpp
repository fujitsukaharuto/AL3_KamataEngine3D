#include "GameScene.h"
#include "TextureManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("cube/cube.jpg");
	sprite_ = Sprite::Create(textureHandle_, {10, 20});

	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	soundDataHandle_ = audio_->LoadWave("fanfare.wav");
	//audio_->PlayWave(soundDataHandle_);
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

	debugCamera_ = new DebugCamera(1280, 720);
	isDebugCamera_ = true;
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {

#ifdef _DEBUG

	Vector2 position = sprite_->GetPosition();

	ImGui::Begin("Debug1");
	ImGui::Text("Fujituka Haruto %d.%d.%d", 2050, 12, 31);
	ImGui::Checkbox("debug Camera", &isDebugCamera_);
	// ImGui::InputFloat("Sprite Position x", &position.x);
	// ImGui::InputFloat("Sprite Position y", &position.y);
	ImGui::SliderFloat("Sprite Position x", &position.x, 0.0f, 300.0f);
	ImGui::SliderFloat("Sprite Position y", &position.y, 0.0f, 300.0f);
	ImGui::End();

	sprite_->SetPosition(position);

	ImGui::ShowDemoWindow();

#endif // DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
	
		audio_->StopWave(voiceHandle_);

	}

	if (isDebugCamera_) {
		debugCamera_->Update();
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

	sprite_->Draw();

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

	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	for (int x = -50; x <= 50;) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({static_cast<float>(x), 0, -50}, {static_cast<float>(x), 0, 50}, {1.0f, 0.0f, 0.0f, 1.0f});
		PrimitiveDrawer::GetInstance()->DrawLine3d({-50, 0, static_cast<float>(x)}, {50, 0, static_cast<float>(x)}, {0.0f, 0.0f, 1.0f, 1.0f});
		x += 10;
	}

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
