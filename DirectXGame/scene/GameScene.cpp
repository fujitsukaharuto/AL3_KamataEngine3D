#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"

#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);

	viewProject_.Initialize();
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProject_);
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	/*playerTextureHandle_ = TextureManager::Load("human.png");*/
	modelFighterBody_.reset(Model::CreateFromOBJ("playerbody", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("playerhead", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("playerlefthand", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("playerrighthand", true));
	std::vector<Model*> playerModels = {
		modelFighterBody_.get(), modelFighterHead_.get(),
		modelFighterL_arm_.get(), modelFighterR_arm_.get()};

	enemyModelfightBody_.reset(Model::CreateFromOBJ("enemybody", true));
	enemyModelfightWeapon_.reset(Model::CreateFromOBJ("enemyweapon", true));
	std::vector<Model*> enemyModels = {
		enemyModelfightBody_.get(), enemyModelfightWeapon_.get()};

	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	groundModel_.reset(Model::CreateFromOBJ("ground", true));


	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());


	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);


	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());


	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

}

void GameScene::Update()
{
	
	player_->Update();

	followCamera_->Update();

	enemy_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_F12)) {
		if (isDebugCameraMode_) {
			isDebugCameraMode_ = false;
		} else {
			isDebugCameraMode_ = true;
		}
	}

#endif // _DEBUG

	if (isDebugCameraMode_) {
		debugCamera_->Update();
		viewProject_.matView = debugCamera_->GetViewProjection().matView;
		viewProject_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProject_.TransferMatrix();
	} else {
		viewProject_.UpdateMatrix();
		viewProject_.matView = followCamera_->GetViewProjection().matView;
		viewProject_.matProjection = followCamera_->GetViewProjection().matProjection;
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

	skydome_->Draw(viewProject_);
	ground_->Draw(viewProject_);
	enemy_->Draw(viewProject_);
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
