#include "fBullCowGame.h"

#define TMap std::map;

FBullCowGame::FBullCowGame() { Reset(); }

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }
EGameDifficulty FBullCowGame::GetDifficulty() const { return GameDifficulty; }
FString FBullCowGame::GetHiddenWord() const { return MyHiddenWord; }


// takes the enum value and returns as a string for use in text outputs (ex. title screen)
// could be a better way to do this
FString FBullCowGame::GetDifficultyText(EGameDifficulty GameDifficulty) const
{
	// setup a map of the difficulty enums to a text of the value for return
	std::map <EGameDifficulty, FString> GameDifficultyToTextMap
	{ 
		{ EGameDifficulty::Beginner,"Beginner" },
		{ EGameDifficulty::Novice,"Novice" },
		{ EGameDifficulty::Hard,"Hard" },
		{ EGameDifficulty::Expert,"Expert" },
		{ EGameDifficulty::Mooster,"Mooster" },
	};

	return GameDifficultyToTextMap[GameDifficulty];
}

// takes an integer value of difficulty level and sets difficulty in the class as an enum
// resets the game based on the newly initialized difficulty level
void FBullCowGame::SetDifficulty(int32 NewDifficultyLevel)
{
	//kjchange
	switch (NewDifficultyLevel)
	{
	case(1):
		GameDifficulty = EGameDifficulty::Beginner;
		break;
	case(2):
		GameDifficulty = EGameDifficulty::Novice;
		break;
	case(3):
		GameDifficulty = EGameDifficulty::Hard;
		break;
	case(4):
		GameDifficulty = EGameDifficulty::Expert;
		break;
	case(5):
		GameDifficulty = EGameDifficulty::Mooster;
		break;
	default:
		GameDifficulty = EGameDifficulty::Novice;
		break;

	}
	
	Reset();
	return;

}

int32 FBullCowGame::GetMaxTries() const 
{ 
	// map the difficulty level to the amount of tries allowed
	std::map <int32, int32> WordLengthToMaxTries{ {3,10}, {4,10}, {5,10}, {6,16}, {7,20} };
	return WordLengthToMaxTries[GetHiddenWordLength()];
}

void FBullCowGame::Reset()
{
	// used to initialize any values on the first run
	if (bFirstRun) {
		GameDifficulty = DEFAULT_DIFFICULTY; // set default difficulty value
		bFirstRun = false;
	}

	bGameIsWon = false;

	constexpr int32 MAX_TRIES = 8;
	MyMaxTries = MAX_TRIES;

	const FString HIDDEN_WORD = GetNewHiddenWord();
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess)
{
	
	if (Guess.length() != GetHiddenWordLength()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else if (!IsLowercase(Guess)) // if the guess isnt all lowercase 
	{
		return EGuessStatus::Not_Lowercase; // TODO write function
	}
	else if (!IsIsogram(Guess)) // if guess isnt an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else // else
	{
		return EGuessStatus::OK;
	}

	return EGuessStatus::OK;
}

// receives a valid guess and increments the turn and return the count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{

	MyCurrentTry++;

	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length();

	// compare letters against the hidden word
	for (int32 HWChar = 0; HWChar < WordLength; HWChar++) {
		for (int32 GChar = 0; GChar < WordLength; GChar++) {
			//if they match then
			if (Guess[HWChar] == MyHiddenWord[GChar]){
				//if they are in the same place
				if (HWChar == GChar) {
					BullCowCount.Bulls++; // incrememnt bulls
				}else{
					BullCowCount.Cows++; // incrememnt cows
				}
			}
		}
	}

	// check to see if game has been won
	// game has been won if the bull count is the same as the word length
	if (BullCowCount.Bulls == WordLength) 
	{
		bGameIsWon = true;
	}
	else 
	{
		bGameIsWon = false;
	}

	return BullCowCount;
}

FString FBullCowGame::GetNewHiddenWord() const
{
	// setup a map of difficulty level to string vector containing hidden words
	std::map<EGameDifficulty, std::vector<FString>> HiddenWordListMap
	{ 
		{ EGameDifficulty::Beginner,{ "ant", "top", "wag", "pun", "set", "bag" }}, 
		{ EGameDifficulty::Novice,{ "hole", "plot", "brag", "fine", "zest" , "newt", "junk", "city", "rock"}}, 
		{ EGameDifficulty::Hard,{ "steal", "snake", "lemon", "media", "ghost", "radio" }}, 
		{ EGameDifficulty::Expert,{ "planet", "drones", "travel", "dragon", "rocket" } }, 
		{ EGameDifficulty::Mooster,{ "swinger", "flaming", "special", "quality", "gunshot" }}
	};
	
	// seed the random number generator
	srand(time(0)); 

	// generate a random number based on the size of the vector at the current difficulty level
	// ex. if there are 6 Hard words choose a random number between 1 and 6
	int32 RandomNumber = rand() % HiddenWordListMap[GetDifficulty()].capacity();

	// return a hidden word from the hidden word map using the difficulty level and the random number 
	return HiddenWordListMap[GetDifficulty()][RandomNumber];

}

bool FBullCowGame::IsIsogram(FString Word) const
{

	if (Word.length() <= 1) { return true; };

	std::map<char, bool> LetterSeen; // Setup Map

	for (auto Letter : Word) // For all the letters in the word
	{

		Letter = tolower(Letter);

		if (!LetterSeen[Letter]) // if we have not seen the letter yet
		{
			LetterSeen[Letter] = true; // add the letter to the map
		}
		else
		{
			return false; // otherwyse if we have seen the letter we do not have an isogram
		}
		
	}

	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{

	if (Word.length() == 0) { return true; }; // if the string is empty return true

	for (auto Letter : Word) // for each letter of the word
	{
		if (!islower(Letter)) { // if the character is not lower case
			return false; // return it is NOT all lowercase
		}
	}

	return true; // if we reach here all characters are lowercase
}
