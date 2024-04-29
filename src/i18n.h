#pragma once
#ifndef _SUDOKU_I18N_H_
#define _SUDOKU_I18N_H_

#include<unordered_map>
#include<string>

enum class Language {
	ENGLISH=1,
	CHINESE,
	MAX,
};

//使用单例模式
class I18n {
public:
	enum class Key {//这些Key，只有这一个类才会用到，所以放入类里面
        ASK_KEY_MAP,
        INPUT_ERROR,
        LOAD_PROGRESS_FAIL,
        ASK_QUIT,
        ASK_SAVE,
        ASK_SAVE_PATH,
        FILE_EXIST_ERROR,
        CONTINUE,
        UNDO_ERROR,
        CONGRATULATION,
        NOT_COMPLETED,
        ASK_DIFFICULTY,
	};

    using DictType = std::unordered_map<Key, std::string>;

    static I18n* getSingleInstance();

    void setLanguage(Language);

    Language getLanguage() const;

    std::string getValue(Key) const;

    //I18n() = delete;//或者将构造函数设置私有成员

private:
    I18n();
    ~I18n();
    
    Language _language;
    DictType *_dict;

private:
    static I18n* _singleInstance;//单例，饥汉模式
};









#endif
