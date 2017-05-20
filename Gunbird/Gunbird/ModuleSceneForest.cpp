#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleSceneForest.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleEnemies.h"
#include "ModuleScoreRanking.h"
#include "ModuleUI.h"
#include "ModuleCharacterSelection.h"
#include "SDL\include\SDL_timer.h"

//TODO: remove this if not necesary
#include "ModuleInput.h"


ModuleSceneForest::ModuleSceneForest()
{
	background_speed = 0.5f;
	// Background
	background.w = SCREEN_WIDTH;
	background.h = 1717;


	/*soldier_left_wall.loop = true;
	soldier_left_wall.PushBack({ 533, 373, 13, 26 });
	soldier_left_wall.PushBack({ 550, 373, 13, 26 });
	soldier_left_wall.PushBack({ 566, 373, 13, 26 });
	soldier_left_wall.PushBack({ 582, 373, 13, 26 });
	soldier_left_wall.speed = 0.06f;
	soldier_left_wall_y = -145;
	soldier_left_wall_x = 50;*/

}

ModuleSceneForest::~ModuleSceneForest()
{}

// Load assets
bool ModuleSceneForest::Start()
{
	spawned = 0;
	//setting bckground
	background_x = 0;
	background_y = -1717.0f;
	//EXAMPLE
	/*soldier_left_wall_y = -145;
	soldier_left_wall_x = 50;
	soldier_left_wall.Reset();*/

	LOG("Loading SceneForest assets");
	bool ret = true;
	
	// ENABLING SELECTED CHARACTER

	if (App->characterSelection->characterSelected_P1 == CHARACTER_SELECTED::VALNUS_SELECTED)
	{
		App->player->Enable();
		App->ui->Enable();
	}
	else if (App->characterSelection->characterSelected_P1 == CHARACTER_SELECTED::TETSU_SELECTED)
	{
		App->player2->Enable();
		App->ui->Enable();
	}

	if (App->characterSelection->characterSelected_P2 == CHARACTER_SELECTED::VALNUS_SELECTED)
	{
		App->player->Enable();
		App->ui->Enable();
	}
	else if (App->characterSelection->characterSelected_P2 == CHARACTER_SELECTED::TETSU_SELECTED)
	{
		App->player2->Enable();
		App->ui->Enable();
	}

	if (App->player2->playerLives > 0 && App->ui->p2 == true)
		App->player2->Enable();

	App->collision->Enable();
	App->enemies->Enable();
	App->particles->Enable();

	graphics = App->textures->Load("Assets/maps/forest/map_forest_background.png");
	if (graphics == nullptr) {
		LOG("Cannot load the texture in SceneForest");
		ret = false;
	}

	//EXAMPLE:
	/*graphicsSoldier = App->textures->Load("Assets/maps/castle/castle_map_stuff.png");
	if (graphicsSoldier == nullptr) {
		LOG("Cannot load the animations spritesheet in SceneCastle");
		ret = false;
	}*/

	if (!App->audio->PlayMusic("Assets/audio/gunbird_jungle_level_music.ogg"))
		ret = false;

	LOG("Loading audio fx when player 2 joins the game");
	player2joined = App->audio->LoadFx("Assets/audio/effects/Valnus_Start.wav");

	return ret;
}

// Update: draw background
update_status ModuleSceneForest::Update()
{
	if (App->characterSelection->player2_joined)
		App->characterSelection->player2_joined = false;

	update_status status = UPDATE_CONTINUE;

	if (background_y < -SCREEN_HEIGHT) {
		background_y += background_speed;
		//EXAMPLE:
		//soldier_left_y += background_speed;
	}

	// Draw everything --------------------------------------
	if (!App->render->Blit(graphics, (int)background_x, (int)background_y + SCREEN_HEIGHT, &background, 0.75f)) {
		LOG("Cannot blit the texture in SceneJungle %s\n", SDL_GetError());
		status = UPDATE_ERROR;
	}

	//BACKGROUND ANIMATIONS
	{
		//soldier animations EXAMPLE
		/*if (background_y <= -1800.0f && soldier_left_x >= -12 && graphicsSoldier != nullptr) {
			if (!App->render->Blit(graphicsSoldier, (int)soldier_left_x, (int)soldier_left_y + SCREEN_HEIGHT, &(soldier_left.GetCurrentFrame()), 0.75f)) {
				LOG("Cannot blit the texture in SceneCastle %s\n", SDL_GetError());
				status = UPDATE_ERROR;
			}
			if (!App->render->Blit(graphicsSoldier, (int)soldier_left_x - 6, (int)soldier_left_y + SCREEN_HEIGHT + 20, &(soldier_left.GetCurrentFrame()), 0.75f)) {
				LOG("Cannot blit the texture in SceneCastle %s\n", SDL_GetError());
				status = UPDATE_ERROR;
			}
			soldier_left_x -= 0.4f;
		}*/
		
	}

	if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_STATE::KEY_DOWN && App->sceneForest->IsEnabled())
	{
		//DO UNLOAD OF ALL THE TEXTURES ABOUT ANIM IN BACKGROUND LIKE SOLDIERS ETC
		//EXAMPLE:
		/*if (!App->textures->Unload(graphicsSoldier)) {
			LOG("Error unloading graphics in SceneCastle");
			status = UPDATE_ERROR;
		}
		graphicsSoldier = nullptr;
		*/
		App->enemies->Disable();
		App->particles->Disable();
		background_y = -SCREEN_HEIGHT;
	}


	//TODO change the position of the player to private to be more pro
	if ((App->player2->position.y < 0 || App->player->position.y < 0) && App->fade->FadeIsOver() /*delete this in big releases*/ ||(App->input->keyboard[SDL_SCANCODE_RETURN] == KEY_STATE::KEY_DOWN))
		App->fade->FadeToBlack(this, this, 1.0f);

	if (App->player->playerLost) {
		LOG("Player LOST");
		App->fade->FadeToBlack(this, App->scoreRanking);
	}

	if (App->player2->playerLost) {
		LOG("Player2 LOST");
		App->ui->p2 = false;
		App->player2->Disable();
	}

	//ENEMY SPAWN PHASE
	
	{
		if ((int)background_y == -1600 && spawned == 0) {
			spawned = 1;
			App->enemies->AddEnemy(ENEMY_TYPES::TRUMP_RED_MECHA, 20, 20, ENEMY_MOVEMENT::TRUMP_RED_MECHA_PATH);
			
		}
		/*if ((int)background_y == -1600 && spawned == 0) {
			spawned = 1;
			App->enemies->AddEnemy(ENEMY_TYPES::BEE, -10, -10, ENEMY_MOVEMENT::BEE_CORNER_LEFT_PATH);
			App->enemies->AddEnemy(ENEMY_TYPES::BEE, SCREEN_WIDTH, -20, ENEMY_MOVEMENT::BEE_CORNER_RIGHT_PATH);

			App->enemies->AddEnemy(ENEMY_TYPES::BEE, 50, -30, ENEMY_MOVEMENT::BEE_CORNER_LEFT_PATH2);
			App->enemies->AddEnemy(ENEMY_TYPES::BEE, SCREEN_WIDTH /2 + 30, -29, ENEMY_MOVEMENT::BEE_CORNER_RIGHT_PATH2);
		}

		if ((int)background_y == -1500 && spawned == 1)
		{
			spawned = 2;
			App->enemies->AddEnemy(ENEMY_TYPES::BIG_RED_TURRET, 40, -52, ENEMY_MOVEMENT::STAY);

			
		}

		if ((int)background_y == -1450 && spawned == 2)
		{
			spawned = 3;
			App->enemies->AddEnemy(ENEMY_TYPES::BIG_RED_TURRET, 130, -52, ENEMY_MOVEMENT::STAY);
		}

		if ((int)background_y == -1350 && spawned == 3)
		{
			spawned = 4;
			App->enemies->AddEnemy(ENEMY_TYPES::RED_TURRET, 70, -30, ENEMY_MOVEMENT::STAY);
			App->enemies->AddEnemy(ENEMY_TYPES::RED_TURRET, 150, -50, ENEMY_MOVEMENT::STAY);
			App->enemies->AddEnemy(ENEMY_TYPES::RED_TURRET, 200, -90, ENEMY_MOVEMENT::STAY);
		}*/

		
	}
	

	if (App->input->keyboard[SDL_SCANCODE_KP_1] && !App->player2->IsEnabled()) 
	{
		App->ui->p2 = true;
		App->player2->playerLives = 3;
		App->player2->Enable();
		App->characterSelection->player2_joined = true;

		if(App->characterSelection->characterSelected_P1 = CHARACTER_SELECTED::VALNUS_SELECTED)
			App->characterSelection->characterSelected_P2 = CHARACTER_SELECTED::TETSU_SELECTED;

		App->audio->PlayFx(player2joined);
	}


	if (App->input->keyboard[SDL_SCANCODE_KP_1] && !App->player->IsEnabled())
	{
		App->ui->p2 = true;
		App->player->playerLives = 3;
		App->player->Enable();
		App->characterSelection->player2_joined = true;

		if (App->characterSelection->characterSelected_P2 = CHARACTER_SELECTED::VALNUS_SELECTED)
			App->characterSelection->characterSelected_P1 = CHARACTER_SELECTED::TETSU_SELECTED;

		App->audio->PlayFx(player2joined);
	}
	

	/*if (App->input->keyboard[SDL_SCANCODE_F9] == KEY_STATE::KEY_DOWN)
	{
	App->enemies->AddEnemy(ENEMY_TYPES::METALLICBALLOON, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ENEMY_MOVEMENT::STAY);
	}*/

	return status;
}


bool ModuleSceneForest::CleanUp()
{
	LOG("Unloading SceneForest");

	bool ret = true;

	if (!App->textures->Unload(graphics)) {
		LOG("Error unloading graphics in SceneForest");
		ret = false;
	}
	
	App->collision->Disable();
	App->particles->Disable();
	App->enemies->Disable();
	App->ui->Disable();
	App->player->Disable();
	App->player2->Disable();

	return ret;
}