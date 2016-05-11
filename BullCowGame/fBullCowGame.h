#pragma once
#include <string>
#include <iostream> // to remove
#include <map>
#include <vector>
#include <ctime>

using FString = std::string; // use std::string as FString to read like Unreal Engine
using int32 = int; // use int as int32 to read like Unreal Engine - with can define 32 and 64 bit integers?

// holds the results of the bull cow count
struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

// gives a status of the word guess
enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Wrong_Length,
	Not_Lowercase
};

enum class EGameDifficulty
{
	Beginner,
	Novice,
	Hard,
	Expert,
	Mooster
};

class FBullCowGame {

public:

	FBullCowGame();

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetHiddenWordLength() const;
	FString GetHiddenWord() const;
	EGameDifficulty GetDifficulty() const;
	FString GetDifficultyText(EGameDifficulty) const;

	void Reset();
	bool IsGameWon() const;
	void SetDifficulty(int32);
	EGuessStatus CheckGuessValidity(FString);

	// counts bulls and cows and increases try # assuming valid guess
	FBullCowCount SubmitValidGuess(FString);
	
	FString GetNewHiddenWord() const;

private:
	
	FString MyHiddenWord;

	int32 MyCurrentTry;
	int32 MyMaxTries;
	EGameDifficulty GameDifficulty;
	const EGameDifficulty DEFAULT_DIFFICULTY = EGameDifficulty::Novice;

	bool bGameIsWon;
	bool IsIsogram(FString) const;
	bool IsLowercase(FString) const;
	bool bFirstRun = true;
	
};