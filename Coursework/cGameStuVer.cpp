/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);

	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	theAreaClicked = { 0, 0 };
	// Store the textures
	textureName = { "bottle", "ship","theBackground", "OpeningScreen", "ClosingScreen", "HScoreScreen", "character_idle", "character_up", "character_down", "character_left", "character_right", "character_shoot_up", "character_shoot_down", "character_shoot_left", "character_shoot_right", "character_dead", "enemy_dead", "enemy", "arrow" };
	texturesToUse = { "Images/Sprites/Bottle64x64.png", "Images/Sprites/shipGreen64x64.png","Images/Bkg/grass.png", "Images/Bkg/menuBckg.png", "Images/Bkg/endBckg.png","Images/Bkg/hsBckg.png", "Images/Sprites/character_idle.png", "Images\\Sprites\\character_up.png", "Images\\Sprites\\character_down.png", "Images\\Sprites\\character_left.png", "Images\\Sprites\\character_right.png", "Images\\Sprites\\character_shoot_up.png", "Images\\Sprites\\character_shoot_down.png", "Images\\Sprites\\character_shoot_left.png", "Images\\Sprites\\character_shoot_right.png", "Images\\Sprites\\character_dead.png", "Images\\Sprites\\enemy_dead.png", "Images\\Sprites\\enemy_walking.png", "Images\\Sprites\\arrow.png" };
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	//tempTextTexture = theTextureMgr->getTexture("sea");
	//aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	//aColour = { 228, 213, 238, 255 };
	// Store the textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn", "hs_btn" };
	btnTexturesToUse = { "Images/Buttons/quitButton.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/menuButton.png", "Images/Buttons/playButton.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png", "Images/Buttons/hSButton.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 }, { 400, 500 } };
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (unsigned int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = gameState::menu;
	theBtnType = btnTypes::exit;
	// Create textures for Game Dialogue (text)
	fontList = { "pirate", "skeleton" };
	fontsToUse = { "Fonts/8bitWonder.ttf", "Fonts/OldGameFatty.ttf" };
	for (unsigned int fonts = 0; fonts < fontList.size(); fonts++)
	{
		if (fonts == 0)
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 48);
		}
		else
		{
			theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
		}
	}
	// Create text Textures
	gameTextNames = { "TitleTxt", "CollectTxt", "InstructTxt", "ThanksTxt", "SeeYouTxt","BottleCount","HSTable","HScore" };
	//gameTextList = { "Pirate  Treasure", "Collect the bottles of Rum!", "Use the arrow keys to navigate the map.", "Thanks for playing!", "See you again soon!", "Collected: ", "", "High Score"};
	gameTextList = { "Skeleton Shooter 5000", "Shoot the skeletons before they reach you!", "Use the arrow keys to navigate the map.", "Thanks for playing!", "See you again soon!", "Score = ", "", "High Score" };

	for (unsigned int text = 0; text < gameTextNames.size(); text++)
	{
		if (text == 0 || text == gameTextNames.size() - 1)
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("pirate")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 0, 160, 70, 255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("skeleton")->createTextTexture(theRenderer, gameTextList[text], textType::solid, { 0, 160, 70, 255 }, { 0, 0, 0, 0 }));
		}
	}
	// Load game sounds
	//soundList = { "theme", "click" };
	//soundTypes = { soundType::music, soundType::sfx};
	//soundsToUse = { "Audio/Theme/Kevin_MacLeod_-_Winter_Reflections.wav", "Audio/SFX/ClickOn.wav"};

	soundList = { "menuMusic", "gameMusic", "click", "shot", "hit_enemy" };
	soundTypes = { soundType::music, soundType::music, soundType::sfx, soundType::sfx, soundType::sfx };
	soundsToUse = { "Audio/Theme/mainTheme.mp3", "Audio/Theme/gameTheme.mp3", "Audio/SFX/basic-click-wooden.wav", "Audio/SFX/arrow.wav", "Audio/SFX/hit-03.wav" };

	for (unsigned int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	//theSoundMgr->getSnd("theme")->play(-1);
	theSoundMgr->getSnd("menuMusic")->play(-1);

	thePlayer.setRocketVelocity(200);
	thePlayer.setSpriteTranslation({ 50,50 });
	//thePlayer.setSpritePos({ (WINDOW_WIDTH) / 2, (WINDOW_HEIGHT / 2) });
	//thePlayer.setTexture(theTextureMgr->getTexture("character_idle"));
	//thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_idle")->getTWidth(), theTextureMgr->getTexture("character_idle")->getTHeight());


	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());

	// Create vector array of textures

	for (int astro = 0; astro < 10; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos({ 30 * AsteroidDis(gen), (10 * AsteroidDis(gen))-300 });
		theAsteroids[astro]->setAsteroidVelocity(75);
		theAsteroids[astro]->setSpriteTranslation({ 0 * theAsteroids[astro]->getAsteroidVelocity(), -1 * theAsteroids[astro]->getAsteroidVelocity() });
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture("enemy"));
		theAsteroids[astro]->setNoFrames(9);
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth() / theAsteroids[astro]->getNoFrames(), theTextureMgr->getTexture("enemy")->getTHeight());
		theAsteroids[astro]->setActive(true);
	}

	//theTileMap.setMapStartXY({ 150, 100 });

	theScore = 0;
	strScore = gameTextList[5];
	strScore += to_string(theScore).c_str();
	theTextureMgr->deleteTexture("BottleCount");

	numTableItems = 0;
	theHSTable.loadFromFile("Data/HighScore.dat");

	theHighScoreTable = gameTextList[6];

	theHSTableSize = theHSTable.getTableSize();
	highScoreTextures = { "score1","score2","score3","score4","score5","score6","score7","score8","score9","score10" };

	for (int item = 0; item < theHSTableSize; item++)
	{
		string entry = "";
		entry += theHSTable.getItem(item).Name + " " + to_string(theHSTable.getItem(item).score);
		theTextureMgr->addTexture(highScoreTextures[item], theFontMgr->getFont("skeleton")->createTextTexture(theRenderer, entry.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
	}

	gameOver = false;

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case gameState::menu:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("OpeningScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("OpeningScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("CollectTxt");
		pos = { 50, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("InstructTxt");
		pos = { 50, 175, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		thePlayer.setSpritePos({ (WINDOW_WIDTH / 2) + 200, (WINDOW_HEIGHT / 2) + 215 });
		thePlayer.setTexture(theTextureMgr->getTexture("character_shoot_left"));
		thePlayer.setNoFrames(13);
		thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_shoot_left")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_shoot_left")->getTHeight());
		thePlayer.render(theRenderer, &thePlayer.getSpriteDimensions(), &thePlayer.getSpritePos(), thePlayer.getSpriteRotAngle(), &thePlayer.getSpriteCentre(), thePlayer.getSpriteScale());

		// Render Button
		//theButtonMgr->getBtn("play_btn")->setSpritePos({ 400, 375});
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 200, 300 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("hs_btn")->setSpritePos({ 200, 400 });
		theButtonMgr->getBtn("hs_btn")->render(theRenderer, &theButtonMgr->getBtn("hs_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("hs_btn")->getSpritePos(), theButtonMgr->getBtn("hs_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 200, 500 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case gameState::playing:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render each enemy in the vector array
		for (int draw = 0; draw < (int)theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSourceRect(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
		}

		//tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		//pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		theTextureMgr->addTexture("BottleCount", theFontMgr->getFont("skeleton")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 255, 255, 155, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("BottleCount");
		pos = { 800, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		//theTileMap.renderGridLines(theRenderer, aRect, aColour);
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 850, 600 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());

		// render the player
		thePlayer.render(theRenderer, &thePlayer.getSpriteDimensions(), &thePlayer.getSpritePos(), thePlayer.getSpriteRotAngle(), &thePlayer.getSpriteCentre(), thePlayer.getSpriteScale());
		SDL_RenderPresent(theRenderer);

		// Render each bullet in the vector array
		for (int draw = 0; draw < (int)theBullets.size(); draw++)
		{
			theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
		}

		// Render each enemy death in the vector array
		for (int draw = 0; draw < (int)theEnemyDead.size(); draw++)
		{
			theEnemyDead[draw]->render(theRenderer, &theEnemyDead[draw]->getSourceRect(), &theEnemyDead[draw]->getSpritePos(), theEnemyDead[draw]->getSpriteScale());
		}

	}
	break;
	case gameState::end:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("ClosingScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("ClosingScreen")->getTWidth(), theTextureMgr->getTexture("ClosingScreen")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		//pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		//tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
		pos = { 50, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ (WINDOW_WIDTH)-200, 400 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ (WINDOW_WIDTH)-200, 500 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case gameState::highscore:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("HScore");
		pos = { 220, 125, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		pos = { 220, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };

		for (int item = 0; item < theHSTableSize; item++)
		{
			tempTextTexture = theTextureMgr->getTexture(highScoreTextures[item]);
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			pos = { 220, 200 + (50 * item), tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		}

		// Render Button
		theButtonMgr->getBtn("play_btn")->setSpritePos({ (WINDOW_WIDTH)-200, 300 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ (WINDOW_WIDTH)-200, 400 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ (WINDOW_WIDTH)-200, 500 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case gameState::quit:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// Check Button clicked and change state
	if (theGameState == gameState::menu || theGameState == gameState::end)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::quit, theAreaClicked);

	}
	else
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::end, theAreaClicked);
	}
	if (theGameState == gameState::highscore)
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("HScoreScreen"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("HScoreScreen")->getTWidth(), theTextureMgr->getTexture("OpeningScreen")->getTHeight());
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
		if (theGameState == gameState::menu)
		{
			theSoundMgr->getSnd("menuMusic")->play(-1); 			//Plays the menu Music
		}
		if (theGameState == gameState::quit)
		{
			theSoundMgr->getSnd("menuMusic")->play(-1); 			//Plays the menu Music
		}
		if (theGameState == gameState::playing)
		{
			theSoundMgr->getSnd("gameMusic")->play(-1); 			//Plays the game Music
			thePlayer.setSpritePos({ (WINDOW_WIDTH) / 2, (WINDOW_HEIGHT / 2) });
			thePlayer.setTexture(theTextureMgr->getTexture("character_idle"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_idle")->getTWidth(), theTextureMgr->getTexture("character_idle")->getTHeight());
		}
	}

	if (theGameState == gameState::menu)
	{
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, gameState::playing, theAreaClicked);
		gameOver = false;
		if (theGameState == gameState::playing && gameOver == false)
		{
			//theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			//theTileMap.update(theBottle.getMapPosition(), 1, 0.0f);
			//theTileMap.update(thePirate.getMapPosition(), 1, 0.0f);
			//theShip.setMapPosition(spriteRandom(gen), spriteRandom(gen));
			//theBottle.genRandomPos(theShip.getMapPosition().R, theShip.getMapPosition().C);
			//theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			//theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
			theScore = 0;
			theSoundMgr->getSnd("gameMusic")->play(-1); 			//Plays the game Music
			thePlayer.setSpritePos({ (WINDOW_WIDTH) / 2, (WINDOW_HEIGHT / 2) });
			thePlayer.setTexture(theTextureMgr->getTexture("character_idle"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_idle")->getTWidth(), theTextureMgr->getTexture("character_idle")->getTHeight());
		}
	}

	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, gameState::menu, theAreaClicked);
	theGameState = theButtonMgr->getBtn("hs_btn")->update(theGameState, gameState::highscore, theAreaClicked);

	if (theGameState == gameState::playing)
	{
		thePlayer.update(deltaTime);

		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
		while (asteroidIterator != theAsteroids.end())
		{
			if ((*asteroidIterator)->isActive() == false)
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
				cout << "Asteroid iterator is not active!!" << endl;
			}
			else
			{
				(*asteroidIterator)->update(deltaTime);
				(*asteroidIterator)->animate(deltaTime);
				++asteroidIterator;
			}
		}

		//cout << "X = " << thePlayer.getSpritePos().x << "and Y = " << thePlayer.getSpritePos().y << endl;

		// Enemies //

		if (frames % 360 == 0)
		{
			int currentSize = theAsteroids.size();
			cout << currentSize << endl;
			int end = currentSize + 6;
			int count = 1;
			for (int astro = currentSize; astro < end; astro++)
			{
				theAsteroids.push_back(new cAsteroid);
				theAsteroids[astro]->setSpritePos({ 30 * AsteroidDis(gen), (10 * AsteroidDis(gen)) - 300 });
				theAsteroids[astro]->setAsteroidVelocity(75);
				theAsteroids[astro]->setSpriteTranslation({ 0 * theAsteroids[astro]->getAsteroidVelocity(), -1 * theAsteroids[astro]->getAsteroidVelocity() });
				theAsteroids[astro]->setTexture(theTextureMgr->getTexture("enemy"));
				theAsteroids[astro]->setNoFrames(9);
				theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture("enemy")->getTWidth() / theAsteroids[astro]->getNoFrames(), theTextureMgr->getTexture("enemy")->getTHeight());
				theAsteroids[astro]->setActive(true);
				count++;
				currentSize += 4;
			}
		}
		frames++;

		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
		{
			//(*bulletIterartor)->update(deltaTime);
			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
				{
					// if a collision set the bullet and asteroid to false
					(*asteroidIterator)->setActive(false);
					(*bulletIterartor)->setActive(false);

					theEnemyDead.push_back(new cSprite);
					int index = theEnemyDead.size() - 1;
					theEnemyDead[index]->setSpriteTranslation({ 0, 0 });
					theEnemyDead[index]->setActive(true);
					theEnemyDead[index]->setNoFrames(6);
					theEnemyDead[index]->setTexture(theTextureMgr->getTexture("enemy_dead"));
					theEnemyDead[index]->setSpriteDimensions(theTextureMgr->getTexture("enemy_dead")->getTWidth() / theEnemyDead[index]->getNoFrames(), theTextureMgr->getTexture("enemy_dead")->getTHeight());
					theEnemyDead[index]->setSpritePos({ (*asteroidIterator)->getSpritePos().x /*+ (int)((*asteroidIterator)->getSpritePos().w/2)*/, (*asteroidIterator)->getSpritePos().y /*+ (int)((*asteroidIterator)->getSpritePos().h / 2)*/ });

					theSoundMgr->getSnd("hit_enemy")->play(0);
					theScore += 10;
					strScore = gameTextList[5];
					strScore += to_string(theScore).c_str();
					theTextureMgr->deleteTexture("BottleCount");
					cout << "Score: " << strScore << endl;
				}
			}
		}


		// Shooting //

		vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
		while (bulletIterartor != theBullets.end())
		{
			if ((*bulletIterartor)->isActive() == false)
			{
				bulletIterartor = theBullets.erase(bulletIterartor);
			}
			else
			{
				(*bulletIterartor)->update(deltaTime);
				++bulletIterartor;
			}
		}

		// Update the visibility and position of each enemy death
		vector<cSprite*>::iterator expIterartor = theEnemyDead.begin();
		while (expIterartor != theEnemyDead.end())
		{
			if ((*expIterartor)->isActive() == false)
			{
				expIterartor = theEnemyDead.erase(expIterartor);
			}
			else
			{
				(*expIterartor)->animateDeath(deltaTime);
				++expIterartor;
			}
		}


		// Movement //

		if (thePlayer.getSpritePos().x <= 0)
		{
			thePlayer.setSpritePos({ 0 , thePlayer.getSpritePos().y });
		}

		if (thePlayer.getSpritePos().x > (WINDOW_WIDTH - thePlayer.getSpriteDimensions().w))
		{
			thePlayer.setSpritePos({ (WINDOW_WIDTH - thePlayer.getSpriteDimensions().w) , thePlayer.getSpritePos().y });
		}

		if (thePlayer.getSpritePos().y <= 0)
		{
			thePlayer.setSpritePos({ thePlayer.getSpritePos().x , 0 });
		}

		if (thePlayer.getSpritePos().y > (WINDOW_HEIGHT - thePlayer.getSpriteDimensions().h))
		{
			thePlayer.setSpritePos({ thePlayer.getSpritePos().x, (WINDOW_HEIGHT - thePlayer.getSpriteDimensions().h) });
		}

		if (thePlayer.isMovingLeft)
		{
			thePlayer.isMovingDown = false;
			thePlayer.isMovingUp = false;
			thePlayer.isMovingRight = false;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_left"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_left")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_left")->getTHeight());
			thePlayer.isFacingLeft = true;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingDown = false;
			//thePlayerAnimated.push_back(new cSprite);
			//int index = thePlayerAnimated.size() - 1;
			//thePlayerAnimated[index]->setSpriteTranslation({ 0, 0 });
			//thePlayerAnimated[index]->setNoFrames(9);
			//thePlayerAnimated[index]->setTexture(theTextureMgr->getTexture("character_left"));
			//thePlayerAnimated[index]->setSpriteDimensions(theTextureMgr->getTexture("character_left")->getTWidth() / thePlayerAnimated[index]->getNoFrames(), theTextureMgr->getTexture("character_left")->getTHeight());
			//thePlayerAnimated[index]->setSpritePos({ 400, 400 });
		}

		if (thePlayer.isMovingRight)
		{
			thePlayer.isMovingDown = false;
			thePlayer.isMovingUp = false;
			thePlayer.isMovingLeft = false;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_right"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_right")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_right")->getTHeight());
			//thePlayer.animate(deltaTime);
			thePlayer.isFacingRight = true;
			thePlayer.isFacingLeft = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingDown = false;
		}

		if (thePlayer.isMovingUp)
		{
			thePlayer.isMovingDown = false;
			thePlayer.isMovingRight = false;
			thePlayer.isMovingLeft = false;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_up"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_up")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_up")->getTHeight());
			//thePlayer.animate(deltaTime);
			thePlayer.isFacingUp = true;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingLeft = false;
			thePlayer.isFacingDown = false;
		}

		if (thePlayer.isMovingDown)
		{
			thePlayer.isMovingUp = false;
			thePlayer.isMovingRight = false;
			thePlayer.isMovingLeft = false;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_down"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_down")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_down")->getTHeight());
			//thePlayer.animate(deltaTime);
			thePlayer.isFacingDown = true;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingLeft = false;
		}

		if (thePlayer.isMovingUL)
		{
			thePlayer.isMovingUp = false;
			thePlayer.isMovingRight = false;
			thePlayer.isMovingLeft = false;
			thePlayer.isMovingDown = false;
			thePlayer.isMovingDL = false;
			thePlayer.isMovingUR = false;
			thePlayer.isMovingDR = false;
			thePlayer.isMovingUL = true;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_left"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_left")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_left")->getTHeight());
			//thePlayer.animate(deltaTime);
			thePlayer.isFacingDown = false;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingLeft = false;
		}

		if (thePlayer.isMovingUR)
		{
			thePlayer.isMovingUp = false;
			thePlayer.isMovingRight = false;
			thePlayer.isMovingLeft = false;
			thePlayer.isMovingDown = false;
			thePlayer.isMovingDL = false;
			thePlayer.isMovingUR = true;
			thePlayer.isMovingDR = false;
			thePlayer.isMovingUL = false;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_right"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_right")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_right")->getTHeight());
			//thePlayer.animate(deltaTime);
			thePlayer.isFacingDown = false;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingLeft = false;
		}

		if (thePlayer.isMovingDR)
		{
			thePlayer.isMovingUp = false;
			thePlayer.isMovingRight = false;
			thePlayer.isMovingLeft = false;
			thePlayer.isMovingDown = false;
			thePlayer.isMovingDL = false;
			thePlayer.isMovingUR = false;
			thePlayer.isMovingDR = true;
			thePlayer.isMovingUL = false;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_right"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_right")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_right")->getTHeight());
			//thePlayer.animate(deltaTime);
			thePlayer.isFacingDown = false;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingLeft = false;
		}

		if (thePlayer.isMovingDL)
		{
			thePlayer.isMovingUp = false;
			thePlayer.isMovingRight = false;
			thePlayer.isMovingLeft = false;
			thePlayer.isMovingDown = false;
			thePlayer.isMovingDL = true;
			thePlayer.isMovingUR = false;
			thePlayer.isMovingDR = false;
			thePlayer.isMovingUL = false;
			thePlayer.setNoFrames(9);
			thePlayer.setTexture(theTextureMgr->getTexture("character_left"));
			thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_left")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_left")->getTHeight());
			//thePlayer.animate(deltaTime);
			thePlayer.isFacingDown = false;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingLeft = false;
		}

		if (!thePlayer.canMove)
		{
			thePlayer.isMovingUp = false;
			thePlayer.isMovingRight = false;
			thePlayer.isMovingLeft = false;
			thePlayer.isMovingDown = false;
			//thePlayer.setNoFrames(1);
			//thePlayer.setTexture(theTextureMgr->getTexture("character_idle"));
			//thePlayer.setSpriteDimensions(theTextureMgr->getTexture("character_idle")->getTWidth() / thePlayer.getNoFrames(), theTextureMgr->getTexture("character_idle")->getTHeight());
			thePlayer.isFacingDown = true;
			thePlayer.isFacingRight = false;
			thePlayer.isFacingUp = false;
			thePlayer.isFacingLeft = false;
		}

		// Check if ship has collided with the bottle
		//if (theShip.getMapPosition() == theBottle.getMapPosition())
		//{
		//	bottlesCollected++;
		//	theBottle.genRandomPos(theShip.getMapPosition().R, theShip.getMapPosition().C);
		//	theTileMap.update(theBottle.getMapPosition(), 2, theBottle.getBottleRotation());
		//	//Create Updated bottle count
		//	strScore = gameTextList[5];
		//	strScore += to_string(bottlesCollected).c_str();
		//	theTextureMgr->deleteTexture("BottleCount");
		//}

		// Check if Pirate has collided with the ship

		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, gameState::end, theAreaClicked);

		if (theGameState == gameState::end)
		{
			theSoundMgr->getSnd("menuMusic")->play(-1); 			//Plays the game Music
		}

		if (gameOver)
		{
			theGameState = gameState::end;
		}
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		//The movement code 

		const Uint8 *keystate = SDL_GetKeyboardState(NULL);

		if (theGameState == gameState::playing)
		{
			if (keystate[SDL_SCANCODE_LEFT] && (!keystate[SDL_SCANCODE_UP] || !keystate[SDL_SCANCODE_DOWN]))
			{
				thePlayer.setRocketMoveH(-1);
				thePlayer.setRocketMoveV(0);
				thePlayer.isMovingLeft = true;

			}

			if (keystate[SDL_SCANCODE_RIGHT] && (!keystate[SDL_SCANCODE_UP] || !keystate[SDL_SCANCODE_DOWN]))
			{
				thePlayer.setRocketMoveH(1);
				thePlayer.setRocketMoveV(0);
				thePlayer.isMovingRight = true;
			}

			if (keystate[SDL_SCANCODE_UP] && (!keystate[SDL_SCANCODE_LEFT] || !keystate[SDL_SCANCODE_RIGHT]))
			{
				thePlayer.setRocketMoveH(0);
				thePlayer.setRocketMoveV(-1);
				thePlayer.isMovingUp = true;
			}

			if (keystate[SDL_SCANCODE_DOWN] && (!keystate[SDL_SCANCODE_LEFT] || !keystate[SDL_SCANCODE_RIGHT]))
			{
				thePlayer.setRocketMoveH(0);
				thePlayer.setRocketMoveV(1);
				thePlayer.isMovingDown = true;

			}

			if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT])
			{
				thePlayer.setRocketMoveH(1);
				thePlayer.setRocketMoveV(-1);
				thePlayer.isMovingUR = true;
			}

			if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_RIGHT])
			{
				thePlayer.setRocketMoveH(1);
				thePlayer.setRocketMoveV(1);
				thePlayer.isMovingDR = true;
			}

			if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT])
			{
				thePlayer.setRocketMoveH(-1);
				thePlayer.setRocketMoveV(-1);
				thePlayer.isMovingUL = true;
				thePlayer.isMovingUp = false;
				thePlayer.isMovingLeft = false;
			}

			if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_LEFT])
			{
				thePlayer.setRocketMoveH(-1);
				thePlayer.setRocketMoveV(1);
				thePlayer.isMovingDL = true;
			}

			if ((keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_DOWN]) || (keystate[SDL_SCANCODE_RIGHT] && keystate[SDL_SCANCODE_LEFT]))
			{
				thePlayer.setRocketMoveH(0);
				thePlayer.setRocketMoveV(0);
				thePlayer.canMove = false;
			}
			else
			{
				thePlayer.canMove = true;
			}
		}




		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			//{
			//	dragTile.setSpritePos({ event.motion.x, event.motion.y });
			//}
			break;
			//case SDL_KEYDOWN:
			//	switch (event.key.keysym.sym)
			//	{
			//	case SDLK_ESCAPE:
			//		theLoop = false;
			//		break;
			//	case SDLK_DOWN:
			//	{
			//		if (theGameState == gameState::playing && theShip.getMapPosition().R < 9)
			//		{
			//			theShip.setShipRotation(0.0f);
			//			theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			//			theShip.update(theShip.getMapPosition().C, theShip.getMapPosition().R + 1);
			//			theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			//			// Lab Code goes here
			//		}
			//	}
			//	break;

			//case SDLK_UP:
			//{
			//	if (theGameState == gameState::playing && theShip.getMapPosition().R > 0)
			//	{
			//		theShip.setShipRotation(180.0f);
			//		theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			//		theShip.update(theShip.getMapPosition().C, theShip.getMapPosition().R - 1);
			//		theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			//		// Lab Code goes here
			//	}
			//}
			//break;
			//case SDLK_RIGHT:
			//{
			//	if (theGameState == gameState::playing && theShip.getMapPosition().C < 9)
			//	{
			//		theShip.setShipRotation(270.0f);
			//		theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			//		theShip.update(theShip.getMapPosition().C + 1, theShip.getMapPosition().R);
			//		theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			//		// Lab Code goes here
			//	}
			//}
			//break;

			//case SDLK_LEFT:
			//{
			//	if (theGameState == gameState::playing && theShip.getMapPosition().C > 0)
			//	{
			//		theShip.setShipRotation(90.0f);
			//		theTileMap.update(theShip.getMapPosition(), 1, 0.0f);
			//		theShip.update(theShip.getMapPosition().C - 1, theShip.getMapPosition().R);
			//		theTileMap.update(theShip.getMapPosition(), 3, theShip.getShipRotation());
			//		// Lab Code goes here
			//	}
			//}
			//break;
			//case SDLK_SPACE:
			//{
			//}
			//break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				theLoop = false;
				break;
			case SDLK_DOWN:
			{
				if (SDL_SCANCODE_LEFT || SDL_SCANCODE_RIGHT)
				{
					thePlayer.setRocketMoveV(0);
				}
				else
				{
					thePlayer.setRocketMoveV(0);
					thePlayer.setRocketMoveH(0);
				}
				thePlayer.canMove = true;
				thePlayer.isMovingDown = false;
				thePlayer.isMovingDL = false;
				thePlayer.isMovingDR = false;

			}
			break;

			case SDLK_UP:
			{
				if (SDL_SCANCODE_LEFT || SDL_SCANCODE_RIGHT)
				{
					thePlayer.setRocketMoveV(0);
				}
				else
				{
					thePlayer.setRocketMoveV(0);
					thePlayer.setRocketMoveH(0);
				}
				thePlayer.canMove = true;
				thePlayer.isMovingUp = false;
				thePlayer.isMovingUL = false;
				thePlayer.isMovingUR = false;

			}
			break;
			case SDLK_RIGHT:
			{
				if (SDL_SCANCODE_UP || SDL_SCANCODE_DOWN)
				{
					thePlayer.setRocketMoveH(0);
				}
				else
				{
					thePlayer.setRocketMoveV(0);
					thePlayer.setRocketMoveH(0);
				}
				thePlayer.canMove = true;
				thePlayer.isMovingRight = false;
				thePlayer.isMovingUR = false;
				thePlayer.isMovingDR = false;

			}
			break;

			case SDLK_LEFT:
			{
				if (SDL_SCANCODE_UP || SDL_SCANCODE_DOWN)
				{
					thePlayer.setRocketMoveH(0);
				}
				else
				{
					thePlayer.setRocketMoveV(0);
					thePlayer.setRocketMoveH(0);
				}
				thePlayer.canMove = true;
				thePlayer.isMovingLeft = false;
				thePlayer.isMovingUL = false;
				thePlayer.isMovingDL = false;
			}
			break;
			case SDLK_SPACE:
			{
				theBullets.push_back(new cBullet);
				int numBullets = theBullets.size() - 1;
				//theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x + thePlayer.getSpriteCentre().x, thePlayer.getBoundingRect().y + thePlayer.getSpriteCentre().y });

				if (thePlayer.isFacingLeft)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x - 30, thePlayer.getBoundingRect().y + 20 });
					theBullets[numBullets]->setSpriteRotAngle(-180);

				}

				if (thePlayer.isFacingRight)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x + 30, thePlayer.getBoundingRect().y + 20 });
				}

				if (thePlayer.isFacingUp)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x + 15, thePlayer.getBoundingRect().y - 25 });
					theBullets[numBullets]->setSpriteRotAngle(-90);
				}

				if (thePlayer.isFacingDown)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x + 15, thePlayer.getBoundingRect().y + 25 });
					theBullets[numBullets]->setSpriteRotAngle(90);
				}

				if (thePlayer.isMovingUR)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x + 30, thePlayer.getBoundingRect().y + 30 });
					theBullets[numBullets]->setSpriteRotAngle(-45);

				}

				if (thePlayer.isMovingUL)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x - 10, thePlayer.getBoundingRect().y + 10 });
					theBullets[numBullets]->setSpriteRotAngle(-135);
				}

				if (thePlayer.isMovingDR)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x + 30, thePlayer.getBoundingRect().y + 30 });
					theBullets[numBullets]->setSpriteRotAngle(45);

				}

				if (thePlayer.isMovingDL)
				{
					theBullets[numBullets]->setSpritePos({ thePlayer.getBoundingRect().x - 10, thePlayer.getBoundingRect().y + 30 });
					theBullets[numBullets]->setSpriteRotAngle(135);
				}

				theBullets[numBullets]->setSpriteTranslation({ 50, 50 });
				theBullets[numBullets]->setTexture(theTextureMgr->getTexture("arrow"));
				theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("arrow")->getTWidth(), theTextureMgr->getTexture("arrow")->getTHeight());
				theBullets[numBullets]->setBulletVelocity(10);
				theBullets[numBullets]->setActive(true);
			}
			theSoundMgr->getSnd("shot")->play(0);
			break;
			default:
				break;
			}
		default:
			break;
		}
		//default:
		//	break;
		//}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	//Quit FONT system
	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

