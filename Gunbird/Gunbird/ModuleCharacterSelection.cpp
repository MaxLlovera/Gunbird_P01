#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCharacterSelection.h"
#include "ModuleFadeToBlack.h"
#include "ModuleInput.h"

//TODO include only the random maps?
#include "ModuleSceneSea.h"

ModuleCharacterSelection::ModuleCharacterSelection()
{
	// Background
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;
}

ModuleCharacterSelection::~ModuleCharacterSelection()
{}

// Load assets
bool ModuleCharacterSelection::Start()
{
	LOG("Loading Character Selection assets");
	bool ret = true;
	graphics = App->textures->Load("Assets/characterSelection/exampleCharacterSelection.png");
	if (graphics == nullptr) {
		LOG("Cannot load the texture in Character Selection");
		ret = false;
	}

	if (!App->audio->PlayMusic("Assets/audio/gunbird_characterSelection_music.wav"))
		ret = false;

	return ret;
}

// Update: draw background
update_status ModuleCharacterSelection::Update()
{

	update_status status = UPDATE_CONTINUE;

	if (App->input->keyboard[SDL_SCANCODE_RETURN] && App->fade->FadeIsOver()) {
		App->fade->FadeToBlack(this, App->sceneSea);
	}

	// Draw everything --------------------------------------
	if (!App->render->Blit(graphics, 0, 0, &background, 1.0f)) {
		LOG("Cannot blit the texture in Character Selection %s\n", SDL_GetError());
		status = UPDATE_ERROR;
	}

	return status;

}

bool ModuleCharacterSelection::CleanUp()
{
	//TODO check if we can control this
	LOG("Unloading character selection");

	graphics = nullptr;
	App->textures->Unload();
	App->audio->StopMusic();

	return true;
}