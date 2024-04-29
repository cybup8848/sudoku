#include"i18n.h"
#include<cassert>
I18n* I18n::_singleInstance= new I18n();

I18n::DictType english = {
    {I18n::Key::ASK_KEY_MAP, "Keymap mode: 1 WASD 2 VIM 3 ARROW"},
    {I18n::Key::INPUT_ERROR, "Input error!"},
    {I18n::Key::LOAD_PROGRESS_FAIL, "Load progress failed!"},
    {I18n::Key::ASK_QUIT, "Quit game? [Y/N]"},
    {I18n::Key::ASK_SAVE, "Do you want to save the game progress? [Y/N]"},
    {I18n::Key::ASK_SAVE_PATH, "Input path of the progress file:"},
    {I18n::Key::FILE_EXIST_ERROR, "This file is already exist."},
    {I18n::Key::CONTINUE, "Continue."},
    {I18n::Key::UNDO_ERROR, "No more action to undo."},
    {I18n::Key::CONGRATULATION, "Congratulation! You Win!"},
    {I18n::Key::NOT_COMPLETED, "Sorry, not completed."},
    {I18n::Key::ASK_DIFFICULTY, "Select difficulty: 1 Easy 2 Normal 3 Hard"},
};

I18n::DictType chinese = {
    {I18n::Key::ASK_KEY_MAP, "设置按键模式: 1 WASD 2 VIM 3 箭头模式"},
    {I18n::Key::INPUT_ERROR, "输入错误!"},
    {I18n::Key::LOAD_PROGRESS_FAIL, "加载存档失败!"},
    {I18n::Key::ASK_QUIT, "退出游戏? [Y/N]"},
    {I18n::Key::ASK_SAVE, "保存进度? [Y/N]"},
    {I18n::Key::ASK_SAVE_PATH, "输入存档文件路径:"},
    {I18n::Key::FILE_EXIST_ERROR, "文件已存在"},
    {I18n::Key::CONTINUE, "继续"},
    {I18n::Key::UNDO_ERROR, "没有可以撤销的动作"},
    {I18n::Key::CONGRATULATION, "恭喜! 你解开了谜题!"},
    {I18n::Key::NOT_COMPLETED, "对不起, 还未完成"},
    {I18n::Key::ASK_DIFFICULTY, "设置难度: 1简单 2普通 3困难"},
};

I18n* I18n::getSingleInstance() {
	return _singleInstance;
}

void I18n::setLanguage(Language language) {
    _language = language;
    switch (language) {
    case Language::CHINESE:
        _dict = &chinese;
        break;
    case Language::ENGLISH:
        _dict = &english;
        break;
    case Language::MAX:
    defualt:
        assert(false);
   }
}

Language I18n::getLanguage() const {
    return _language;
}

std::string I18n::getValue(Key key) const {
    assert(_dict->find(key) != _dict->end());
	return (*_dict)[key];
}	

I18n::I18n() {
    _dict = &english;
    _language = Language::ENGLISH;
}

I18n::~I18n() {

}



