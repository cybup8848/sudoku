#include"scene.h"
#include"input.h"
#include"system_env.h"
#include<cstring>
static void printHelp() {
    std::cout << std::endl;
    std::cout << "sudoku - a little game in command line" << std::endl
        << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t sudoku [-l <progressFile>]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t -l <path> \t specify path of progress file to load, optional." << std::endl
        << std::endl;
}

int main(int argc, char** argv)
{
    //SetSystemEnv();
    //std::cout<<"please input th"

    CScene scene("1.txt");


    if (argc == 1) {
        inputLanguage();
        int eraseGridNumber = inputDifficulty();
        scene.generate();
        scene.eraseRandomGrids(eraseGridNumber);
    }
    else if (argc == 3 && !strcmp(argv[1], "-l")) {
        // load saved game progress
        if (!scene.load(argv[2])) {
            message(I18n::getSingleInstance()->getValue(I18n::Key::LOAD_PROGRESS_FAIL));
            return 0;
        }
        inputLanguage();
    }
    else {
        printHelp();
        return 0;
    }

    scene.setMode(inputKeyMode());

    scene.play();

    return 0;
}


