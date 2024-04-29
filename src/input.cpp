#include"input.h"
#include<iostream>
int inputDifficulty() {
	int needErasedGrids = 0;
	int difficultyLevel = 0;
	while (true) {
		message(I18n::getSingleInstance()->getValue(I18n::Key::ASK_DIFFICULTY));
		std::cin >> difficultyLevel;
		try {
			Difficulty difficulty = static_cast<Difficulty>(difficultyLevel);
			switch (difficulty) {
			case Difficulty::SIMPLE:
				needErasedGrids = 20;
				break;
			case Difficulty::NORMAL:
				needErasedGrids = 35;
				break;
			case Difficulty::HARD:
				needErasedGrids = 50;
				break;
			}
		}
		catch (...) {
			needErasedGrids = 0;
		}
		if (needErasedGrids > 0) {
			break;
		}
		message(I18n::getSingleInstance()->getValue(I18n::Key::INPUT_ERROR));
	}
	
	return needErasedGrids;
}

KeyMode inputKeyMode(){
	int keyMode;
	for (;;) {
		message(I18n::getSingleInstance()->getValue(I18n::Key::ASK_KEY_MAP));
		std::cin >> keyMode;
		try {
			KeyMode kmd = static_cast<KeyMode>(keyMode);
			return kmd;
		}catch (...) {

		}

		message(I18n::getSingleInstance()->getValue(I18n::Key::INPUT_ERROR));
	}
}

void inputLanguage() {
	unsigned long language;
	for (;;) {
		message("1 English  2 中文");
		std::cin >> language;
		try {
			auto langMode = static_cast<Language>(language);
			if (langMode < Language::MAX) {
				I18n::getSingleInstance()->setLanguage(langMode);
				return;
			}
		}catch(...){

		}
		message(I18n::getSingleInstance()->getValue(I18n::Key::INPUT_ERROR));
	}
}


