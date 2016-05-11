/*
This is the console executable that makes use of the BullCow Class
This acts as the view in a MVC pattern and is responsible for all user interactions.
For game logic see the FBullCowGame class
*/

#include <iostream>
#include <string>
#include "fBullCowGame.h"

using FText = std::string;
using int32 = int;

int32 PrintTitleScreen();
FText GetValidGuess();
void PrintGuess(FText);
void PlayGame();
bool SetDifficulty();
bool AskToPlayAgain();
bool bIsMenuSelectValid(FText);
void PrintGameSummary();

// Instantiate game
FBullCowGame BCGame;

// Entry Point
int main()
{
	
	int32 MainMenuSelection;
	bool bRunProgram = true;
	bool bPlayAgain = true;
	bool bValidDifficultSetting = true;

	do 
	{

		// prints the title screen and return the users selection
		MainMenuSelection = PrintTitleScreen();

		switch (MainMenuSelection) 
		{
		case(1): // Start Game

			bPlayAgain = true;
			do {
				PlayGame();
				bPlayAgain = AskToPlayAgain();
			} while (bPlayAgain);
			break;

		case(2): // Set Difficulty
			do {
				bValidDifficultSetting = SetDifficulty();
			} while (!bValidDifficultSetting);
			break;

		case(3): // Exit
			bRunProgram = false;
			break;

		default:
			std::cout << "how'd you get here?!";
			break;
		}
		
	} while (bRunProgram);
	
	return 0; // Exit the application
}

// plays a single round of the game and prints summary
void PlayGame()
{
	int32 MaxTries = BCGame.GetMaxTries();
	FBullCowCount BullCowCount;

	system("CLS"); // clear screen
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram I'm thinking of?" << std::endl;
	

	// Loop for the number of turns asking for guesses while the game is NOT won
	while ((BCGame.GetCurrentTry() <= BCGame.GetMaxTries()) && !BCGame.IsGameWon()) {

		FText Guess;

		Guess = GetValidGuess();

		// Submit valid guess to the game
		BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls: " << BullCowCount.Bulls;
		std::cout << ". Cows: " << BullCowCount.Cows << std::endl;

	}

	PrintGameSummary();

}

// prints a set difficulty menu and loops until a valid input is taken
bool SetDifficulty()
{

	bool bValidDifficulty = false;
	FText DifficultySelectionText;
	int32 DifficultySelection;

	do {
		system("CLS"); // clear screen
		std::cout << "Difficulty Settings\n\n";
		std::cout << "     1. Beginner\n";
		std::cout << "     2. Novice\n";
		std::cout << "     3. Hard\n";
		std::cout << "     4. Expert\n";
		std::cout << "     5. Mooster\n\n";
		std::cout << "Select your difficulty level (1-5):";

		std::getline(std::cin, DifficultySelectionText); // Get Manu Selection

		// checks to make sure the selection is valid TODO find a cleaner way to do this?
		if (DifficultySelectionText == "1" || DifficultySelectionText == "2" || DifficultySelectionText == "3" || DifficultySelectionText == "4" || DifficultySelectionText == "5") { //TODO write method to clean this up
			bValidDifficulty = true;
		}

	} while (!bValidDifficulty); // loop until a valid difficult setting is input
			 
	std::size_t lastChar; // Convert string to integer
	DifficultySelection = std::stoi(DifficultySelectionText, &lastChar, 10);

	// set the new difficulty
	BCGame.SetDifficulty(DifficultySelection);

	return true;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again at this difficulty? (y/n)";
	FText Response = "";
	getline(std::cin, Response);
	bool PlayAgain = false;
	if (Response[0] == 'y' || Response[0] == 'Y') {
		PlayAgain = true;
		BCGame.Reset();
	}
	return PlayAgain;
}

bool bIsMenuSelectValid(FText MenuSelectText)
{
	bool bInputValid = false;

	if (MenuSelectText == "1" || MenuSelectText == "2" || MenuSelectText == "3") 
	{
		bInputValid = true;
	}
	else
	{
		bInputValid = false;
	}

	return bInputValid;
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon()) {
		std::cout << "You Got It! :D\n\n";
	}
	else
	{
		std::cout << "\nOut Of Turns :(\n";
		std::cout << "The Word Was: " << BCGame.GetHiddenWord() <<"\n\n";
	}
	
}

int32 PrintTitleScreen()
{
	// Introduce the game
	int32 HiddenWordLength = BCGame.GetHiddenWordLength();
	FText MainMenuSelctionText;
	int32 MainMenuSelection;

	bool bValidAnswer = false;

	while (!bValidAnswer) 
	{
		system("CLS");
		std::cout << "              #################################################\n";
		std::cout << "               ###############################################\n";
		std::cout << "                #############################################\n";
		std::cout << "                ##                                         ##\n";
		std::cout << "                ##         Welcome to Bulls and Cows       ##\n";
		std::cout << "                ##                                         ##\n";
		std::cout << "                ##              (___)                      ##\n";
		std::cout << "                ##      moo()   (o o)__________/           ##\n";
		std::cout << "                ##           \\  (||)      O   \\            ##\n";
		std::cout << "                ##                 \\ ___o____  /           ##\n";
		std::cout << "                ##                 //       //             ##\n";
		std::cout << "                ##                 ^^       ^^             ##\n\n";

		std::cout << "                         1. Start Game (Difficulty: " << BCGame.GetDifficultyText(BCGame.GetDifficulty()) << ")\n";
		std::cout << "                         2. Set Difficulty\n";
		std::cout << "                         3. Exit\n\n";

		std::cout << "Enter a selection(1-3): ";
		std::getline(std::cin, MainMenuSelctionText); // Get Manu Selection

		bValidAnswer = bIsMenuSelectValid(MainMenuSelctionText);

	}
	
	// Convert string to integer
	std::size_t lastChar;
	MainMenuSelection = std::stoi(MainMenuSelctionText, &lastChar, 10); 
	
	return MainMenuSelection;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{

	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";
	
	do {

		int32 CurrentTry = BCGame.GetCurrentTry();
		int32 MaxTries = BCGame.GetMaxTries();

		std::cout << std::endl << "Guess " << CurrentTry << "/" << MaxTries << ": ";
		std::getline(std::cin, Guess);

		Status = BCGame.CheckGuessValidity(Guess);

		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter your guess in lower case letters.\n";
			break;
		case EGuessStatus::Invalid_Status:
			std::cout << "The Guess is Invalid.\n";
			break;
		case EGuessStatus::OK:
			return Guess;
			break;
		default:
			return Guess;
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors

	return Guess; // should never get here
}

void PrintGuess(FText Guess)
{
	int32 CurrentTry = BCGame.GetCurrentTry();

	//repeat the guess back to them
	std::cout << "Try " << CurrentTry << ": " << Guess << std::endl;
	return;
}