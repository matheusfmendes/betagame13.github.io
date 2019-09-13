#include <stdafx.h>
#include <string>
#include <game/Global.h>
#include <game/Input.h>
#include <game/SDKHooks.h>
#include <mod/Mod.h>
#include <HookLoaderInternal.h>
#include <mod/ModFunctions.h>
#include <mod/MathFunctions.h>
#include <util/JsonConfig.h>
#include <assets/AssetFunctions.h>
#include <assets/FObjectSpawnParameters.h>
#include "../Detours/src/detours.h"
#include "../SatisfactorySDK/SDK.hpp"
#include <memory/MemoryObject.h>
#include <memory/MemoryFunctions.h>


using namespace SML::Mod;
using namespace SML::Objects;

#define SML_VERSION "1.0.0-pr7"
#define MOD_NAME "ExampleMod"
#define LOG(msg) SML::Utility::infoMod(MOD_NAME, msg)
#define INFO(msg) LOG(msg)
#define WARN(msg) SML::Utility::warningMod(MOD_NAME, msg)
#define ERR(msg) SML::Utility::errorMod(MOD_NAME, msg)

// Config
json config = SML::Utility::JsonConfig::load(MOD_NAME, {
	{"Float", 1.0}
});

// Global variables required by the mod
AFGPlayerController* player;

// A custom event handler for when ExampleMod's post setup is complete.
// Other mods can also make a handler for ExampleMod_PostSetupComplete if they want to do something when the event is broadcast.
void postSetupComplete() {
	LOG("ExampleMod's post setup has been completed!");
}

// information about the mod
Mod::Info modInfo {
	// Target sml version
	SML_VERSION,

	// Name
	MOD_NAME,

	// Version
	"1.0.0",

	// Description
	"A basic mod created to showcase SML's functionality.",

	// Authors
	"SuperCoder79, Nomnom, anttirt",

	// Dependencies
	// Place mod names that you want to ensure is loaded in this vector. If you place an asterisk (*) before the mod name, it will be loaded as an optional dependency instead.
	{}
};

// The mod's class, put all functions inside here
class ExampleMod : public Mod {


public:
	// Constructor for SML usage. Do not put anything in here, use setup() instead.
	ExampleMod() : Mod(modInfo) {
	}

	// The setup function is the heart of the mod, where you hook your functions and register your commands and API functions. Do not rename!
	void setup() override {
		// Use the placeholders namespace
		using namespace std::placeholders;

		SDK::InitSDK(); //Initialize the SDK in ExampleMod so the functions work properly

		// More on namespaces:
		// * The functions that will be of use to you are in the SML::Mods::Functions namespace. A tip is to type Functions:: and see what functions are available for you to use. 

		// Hook a member function as handler
		::subscribe<&AFGPlayerController::BeginPlay>(std::bind(&ExampleMod::beginPlay, this, _1, _2)); //bind the beginPlay function, with placeholder variables
		// Because there are two inputs to the function, we use _1 and _2. If there were 3 inputs, we would use _1, _2, and _3, and so forth.

		// Hook a lambda with captured this-ptr as handler
		::subscribe<&PlayerInput::InputKey>([this](Functions::ModReturns* modReturns, PlayerInput* playerInput, FKey key, InputEvent event, float amount, bool gamePad) {
			if (GetAsyncKeyState('K')) {
				SDK::AFGWeapon::GetMagSize(40);

				std::string message = "test " + std::to_string(size);
				LOG(message);

			return false;
		});



	//The postSetup function is where you do things based on other mods' setup functions
	void postSetup() override {
	
		//Broadcast the event for ExampleMod and other mods that do something for ExampleMod_PostSetupComplete.
		Functions::broadcastEvent("ExampleMod_PostSetupComplete");
	}

	~ExampleMod() {
		// Cleanup
		LOG("ExampleMod finished cleanup!");
	}
};

// Required function to create the mod, do not rename!
MOD_API Mod* ModCreate() {
	return new ExampleMod();
}