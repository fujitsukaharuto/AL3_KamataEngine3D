#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"

#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete titleSrite_;
	delete blackSrite_;
	delete gameClearSrite_;
	delete gameOverSrite_;

}

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

	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	/*playerTextureHandle_ = TextureManager::Load("human.png");*/
	modelFighterBody_.reset(Model::CreateFromOBJ("playerbody", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("playerhead", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("playerlefthand", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("playerrighthand", true));
	modelPlayerWeapon_.reset(Model::CreateFromOBJ("playerweapon", true));
	modelPlayerBullet_.reset(Model::CreateSphere());
	std::vector<Model*> playerModels = {modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(), modelFighterR_arm_.get(), modelPlayerWeapon_.get(), modelPlayerBullet_.get()};

	enemyModelfightBody_.reset(Model::CreateFromOBJ("enemybody", true));
	enemyModelfightWeapon_.reset(Model::CreateFromOBJ("enemyweapon", true));
	enemyModelfightHead_.reset(Model::CreateFromOBJ("enemyhead", true));
	enemyModelfightL_arm_.reset(Model::CreateFromOBJ("enemylefthand", true));
	enemyModelfightR_arm_.reset(Model::CreateFromOBJ("enemyrighthand", true));
	std::vector<Model*> enemyModels = {enemyModelfightBody_.get(), enemyModelfightHead_.get(), enemyModelfightL_arm_.get(), enemyModelfightR_arm_.get(), enemyModelfightWeapon_.get()};

	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	groundModel_.reset(Model::CreateFromOBJ("ground", true));

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	player_->SetLockOn(lockOn_.get());
	player_->SetLittleEnemy(enemyModels);

	followCamera_->SetTarget(&player_->GetWorldTransform());
	followCamera_->SetLockOn(lockOn_.get());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	for (int i = 0; i < 1; i++) {
		enemies_.push_back(std::make_unique<Enemy>());
	}
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Initialize(enemyModels);
	}

	effect_ = Effect::GetInstance();
	effect_->Initialize();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());


	sceneTypeRequest_ = SceneType::title;

	titleHandle_ = TextureManager::Load("title.png");
	titleSrite_ = Sprite::Create(titleHandle_, {0.0f, 0.0f});

	blackHandle_ = TextureManager::Load("white1x1.png");
	blackSrite_ = Sprite::Create(blackHandle_, {-2560.0f, 0.0f});
	blackSrite_->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
	blackSrite_->SetSize({2560.0f, 720.0f});

	gameClearHandle_ = TextureManager::Load("gameclear.png");
	gameClearSrite_ = Sprite::Create(gameClearHandle_, {0.0f, 0.0f});

	gameOverHandle_ = TextureManager::Load("gameover.png");
	gameOverSrite_ = Sprite::Create(gameOverHandle_, {0.0f, 0.0f});


	AButtonSound_ = audio_->LoadWave("ASound.mp3");
	clearSound_ = audio_->LoadWave("clearSE.mp3");
	overSound_ = audio_->LoadWave("overSE.mp3");
	bgmSound_ = audio_->LoadWave("BGM.mp3");;
}

void GameScene::Update() {

	BlackMove();

	if (sceneTypeRequest_) {
		sceneType_ = sceneTypeRequest_.value();
		switch (sceneType_) {
		case SceneType::title:
			audio_->StopWave(roopStopHandle_);

			for (std::unique_ptr<Enemy>& enemy : enemies_) {
				enemy->SceneReset();
			}
			player_->SceneReset();
			effect_->SceneReset();
			collisionManager_->Reset();
			followCamera_->Reset();
			lockOn_->SceneReset();

			isClear_ = false;
			isGameover_ = false;

			break;
		case SceneType::tutorial:

			break;
		case SceneType::game:
			for (std::unique_ptr<Enemy>& enemy : enemies_) {
				enemy->SceneReset();
			}
			player_->SceneReset();
			effect_->SceneReset();
			collisionManager_->Reset();
			followCamera_->Reset();

			roopStopHandle_ = audio_->PlayWave(bgmSound_, true, 0.3f);

			break;
		case SceneType::gameClear:

			audio_->StopWave(roopStopHandle_);
			roopStopHandle_ = audio_->PlayWave(clearSound_, false, 0.4f);

			break;
		case SceneType::gameOver:

			audio_->StopWave(roopStopHandle_);
			roopStopHandle_ = audio_->PlayWave(overSound_, false, 0.4f);

			break;
		default:
			break;
		}
		sceneTypeRequest_ = std::nullopt;
	}

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	switch (sceneType_) {
	case SceneType::title:

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				if (stiffnessTimer_ == 0) {
					sceneChangeTimer_ = 40;
					stiffnessTimer_ = 60;
					isSceneChange_ = true;
					audio_->PlayWave(AButtonSound_, false, 0.2f);
				}
			}
		}
		if (isSceneChange_) {
			if (sceneChangeTimer_ > 0) {
				sceneChangeTimer_--;
			} else {
				sceneTypeRequest_ = SceneType::tutorial;
				isSceneChange_ = false;
			}
		} else {
		
			followCamera_->Update();

		}

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

		break;
	case SceneType::tutorial:

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
				if (stiffnessTimer_ == 0) {
					sceneChangeTimer_ = 40;
					stiffnessTimer_ = 60;
					isSceneChange_ = true;
					audio_->PlayWave(AButtonSound_, false, 0.2f);
				}
			}
		}
		if (isSceneChange_) {
			if (sceneChangeTimer_ > 0) {
				sceneChangeTimer_--;
			} else {
				sceneTypeRequest_ = SceneType::game;
				isSceneChange_ = false;
			}
		} else {
			player_->Update();

			followCamera_->Update();

			effect_->Update();

		}

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

		break;
	case SceneType::game:

		if (isSceneChange_) {
			if (sceneChangeTimer_ > 0) {
				sceneChangeTimer_--;
			} else {
				if (isGameover_) {
					sceneTypeRequest_ = SceneType::gameOver;
					isSceneChange_ = false;
				} else if (isClear_) {
					sceneTypeRequest_ = SceneType::gameClear;
					isSceneChange_ = false;
				}
			}
		} else {

#pragma region Game

			player_->Update();

			followCamera_->Update();

			Vector3 littleEnemyTarget = player_->GetCenterPosisionOrigine();
			for (std::unique_ptr<Enemy>& enemy : enemies_) {
				enemy->Update();
				enemy->SetLittleEnemyTarget(littleEnemyTarget);
				if (Input::GetInstance()->TriggerKey(DIK_DELETE)) {
					enemy->SettingLittles();
				}
			}

			lockOn_->Update(enemies_, viewProject_);

			CheckAllCollisions();
			collisionManager_->UpdateWorldTransform();

#ifdef _DEBUG

			if (input_->TriggerKey(DIK_F12)) {
				if (isDebugCameraMode_) {
					isDebugCameraMode_ = false;
				} else {
					isDebugCameraMode_ = true;
				}
			}

#endif // _DEBUG

			effect_->Update();

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

			if (player_->GetLifeCount() == 0 && player_->IsEndPlayer()) {
				isSceneChange_ = true;
				isGameover_ = true;
				sceneChangeTimer_ = 40;
				stiffnessTimer_ = 60;
			}
			for (std::unique_ptr<Enemy>& enemy : enemies_) {
				if (enemy->GetLifeCount() == 0 && enemy->IsEndEnemy()) {
					isSceneChange_ = true;
					isClear_ = true;
					sceneChangeTimer_ = 40;
					stiffnessTimer_ = 60;
				}
			}

#pragma endregion

		}
		break;
	case SceneType::gameClear:

		if ((Input::GetInstance()->GetJoystickState(0, joyState))&&(Input::GetInstance()->GetJoystickStatePrevious(0,joyStatePre))) {
			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				if (stiffnessTimer_ == 0) {
					sceneChangeTimer_ = 40;
					stiffnessTimer_ = 60;
					isSceneChange_ = true;
					audio_->PlayWave(AButtonSound_, false, 0.2f);
				}
			}
		}
		if (isSceneChange_) {
			if (sceneChangeTimer_ > 0) {
				sceneChangeTimer_--;
			} else {
				sceneTypeRequest_ = SceneType::title;
				isSceneChange_ = false;
			}
		}

		break;
	case SceneType::gameOver:

		if ((Input::GetInstance()->GetJoystickState(0, joyState)) && (Input::GetInstance()->GetJoystickStatePrevious(0, joyStatePre))) {
			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				if (stiffnessTimer_ == 0) {
					sceneChangeTimer_ = 40;
					stiffnessTimer_ = 60;
					isSceneChange_ = true;
					audio_->PlayWave(AButtonSound_, false, 0.2f);
				}
			}
		}
		if (isSceneChange_) {
			if (sceneChangeTimer_ > 0) {
				sceneChangeTimer_--;
			} else {
				sceneTypeRequest_ = SceneType::title;
				isSceneChange_ = false;
			}
		}

		break;
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

	switch (sceneType_) {
	case SceneType::title:

		skydome_->Draw(viewProject_);
		ground_->Draw(viewProject_);

		break;
	case SceneType::tutorial:

		skydome_->Draw(viewProject_);
		ground_->Draw(viewProject_);
		player_->Draw(viewProject_);
		effect_->Draw(viewProject_);

		break;
	case SceneType::game:

		skydome_->Draw(viewProject_);
		ground_->Draw(viewProject_);
		for (std::unique_ptr<Enemy>& enemy : enemies_) {
			enemy->Draw(viewProject_);
		}
		player_->Draw(viewProject_);
		collisionManager_->Draw(viewProject_);

		effect_->Draw(viewProject_);

		break;
	case SceneType::gameClear:



		break;
	case SceneType::gameOver:



		break;
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

	switch (sceneType_) {
	case SceneType::title:

		titleSrite_->Draw();
		blackSrite_->Draw();

		break;
	case SceneType::tutorial:

		player_->DrawTutolialSprite();
		blackSrite_->Draw();

		break;
	case SceneType::game:

		lockOn_->Draw();

		for (std::unique_ptr<Enemy>& enemy : enemies_) {
			enemy->DrawSprite();
		}

		player_->DrawSprite();

		blackSrite_->Draw();

		break;
	case SceneType::gameClear:
		
		gameClearSrite_->Draw();
		blackSrite_->Draw();

		break;
	case SceneType::gameOver:

		gameOverSrite_->Draw();
		blackSrite_->Draw();

		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

	collisionManager_->Reset();

	collisionManager_->AddCollider(player_.get());
	if (player_->GetIsAttack()) {
		/*collisionManager_->AddCollider(player_->GetWeaponCollider());*/
	}
	for (auto& i : player_->GetBallCollider()) {
		collisionManager_->AddCollider(i);
	}

	// 敵について
	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
		for (auto& i : enemy->GetLittleEnemyCollider()) {
			collisionManager_->AddCollider(i);
		}
		for (auto& i : enemy->GetAttackZoneCollider()) {
			collisionManager_->AddCollider(i);
		}
	}

	collisionManager_->CheckAllCollisions();
}

void GameScene::BlackMove() {
	if (stiffnessTimer_ > 0) {
		float move = blackSrite_->GetPosition().x + 64.0f;
		blackSrite_->SetPosition({move, 0.0f});

		stiffnessTimer_--;

	} else {
		blackSrite_->SetPosition({-2560.0f, 0.0f});
	}
}
