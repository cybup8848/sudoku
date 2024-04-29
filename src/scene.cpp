#include"scene.h"
#include"log.h"

CScene::CScene(const std::string logFile, int index) {
    _maxColumn = (int)std::pow(index,2);
    _curPoint.x = 0;
    _curPoint.y = 0;
    init();

    //初始化日志
    Log::getSingleInstance()->init(logFile);
}

CScene::~CScene() {
    if (_pKeyMap) {
        delete _pKeyMap;
        _pKeyMap = nullptr;
    }
}

void CScene::generate() {//一个可以多次被初始化
    static char mapPattern[9][10] = {
        "ighcabfde",
        "cabfdeigh",
        "fdeighcab",
        "ghiabcdef",
        "abcdefghi",
        "defghiabc",
        "higbcaefd",
        "bcaefdhig",
        "efdhigbca"
    };
    
    int size = 0;
    int index = 0;

    std::vector<char>charSet{ 'a','b','c','d','e','f','g','h','i' };
    std::unordered_map<char, int>charValue;
    srand(time(nullptr));
    for (int i = 1;i <= 9;i++) {
        size = charSet.size();
        index = rand() % size;
        charValue.insert(std::make_pair(charSet.at(index),i));
        charSet.erase(charSet.begin()+index);
    }

    /*
    point_t point{ 0,0 };
    char key = '\0';
    int row = 0;
    int col = 0;
    for (int i = 0;i < 81;i++) {
        row = i / 9;
        col = i % 9;
        point.x = row;
        point.y = col;
        key = charValue[mapPattern[row][col]];
        setValue(point, key);
    }
    */

    for (int row = 0;row < 9;row++) {
        for (int col = 0;col < 9;col++) {
            point_t point{ row,col };
            char key = mapPattern[row][col];
            setValue(point,charValue[key]);
        }
    }

    assert(isComplete());//检查是否有效
}

void CScene::show() const {
    cls();
    printUnderline();
    int lineNo = -1;
    for (int i = 0;i < 9;i++) {
        _rowBlock[i].print();
        printUnderline(i);
    }
}

void CScene::printUnderline(int line_no) const {
    bool flag = line_no == _curPoint.y;
    for (int i = 0;i < _maxColumn;i++) {
        std::cout << CORNER << LINE;
        if (flag&&_curPoint.x == i) {//判断列
            std::cout << Modifier(ColorCode::FG_RED) <<ARROW<<Modifier(ColorCode::RESET);
        }
        else {
            std::cout << LINE;
        }
        std::cout << LINE;
    }
    std::cout << CORNER << std::endl;
}

bool CScene::setCurValue(const int curValue, int& lastValue) {
    point_value_t point = _map[_curPoint.y * 9 + _curPoint.x];
    if (State::ERASED == point.state) {
        lastValue = point.val;
        setValue(curValue);
        return true;
    }
    return false;
}

bool CScene::setPointValue(const point_t& p, const int v) {
    point_value_t point = _map[p.x + p.y * 9];
    if (State::ERASED == point.state) {
        _curPoint = p;
        setValue(v);
        return true;
    }
    return false;
}

point_t CScene::getCurPoint() {
    return _curPoint;
}

void CScene::eraseRandomGrids(const int count) {
    point_value_t point{UNSELECTED,State::ERASED};
    std::vector<int>e(81,0);
    for (int i = 0;i < 81;i++) {
        e.at(i) = i;
    }

    srand(time(nullptr));//设置随机数种子

    for (int i = 0;i < count;i++) {
        int size = e.size();
        int index = rand() % size;
        _map[e.at(index)] = point;
        e.erase(e.begin()+index);
    }
}

bool CScene::isComplete() {
    for (const point_value_t &x : _map) {
        if (UNSELECTED == x.val) {//是否都已经填充
            return false;
       }
    }

    //同时block里的数字还要符合规则
    for (const CBlock& x : _rowBlock) {
        if (!x.isValid()) {
            return false;
        }
    }

    for (const CBlock &x : _columnBlock) {
        if (!x.isValid()) {
            return false;
        }
    }

    for (int block = 0;block < _maxColumn;++block) {
        if (!_xyBlock[block / 3][block % 3].isValid()) {
            return false;
        }
    }

    return true;
}

void CScene::play() {
    //获取语言
    Language language = I18n::getSingleInstance()->getLanguage();

    show();
    char key = '\0';
    while (true) {
        key = _pKeyMap->getchar();
        if (key >= '0' && key <= '9')
        {
            CCommand oCommand(this);
            if (!oCommand.execute(key - '0'))
            {
                std::cout << "this number can't be modified." << std::endl;

                LOG_INFO("%s %s:this number %d can't be modified at (%d,%d)", specialCTime(),language == Language::ENGLISH ? "execute" : "执行",
                    key-'0',_curPoint.x,_curPoint.y);
            }
            else
            {
                _commands.push_back(std::move(oCommand));  // XXX: move without move constructor
                show();

                LOG_INFO("%s %s:this position is set as %d at (%d,%d)", specialCTime(), language == Language::ENGLISH ? "execute" : "执行",
                    key - '0', _curPoint.x, _curPoint.y);
                continue;
            }
        } 
        else if (key == _pKeyMap->Q)
        {
            message(I18n::getSingleInstance()->getValue(I18n::Key::ASK_QUIT));
            std::string strInput;
            std::cin >> strInput;
            if (strInput[0] == 'y' || strInput[0] == 'Y')
            {
                message(I18n::getSingleInstance()->getValue(I18n::Key::ASK_SAVE));
                std::cin >> strInput;
                if (strInput[0] == 'y' || strInput[0] == 'Y') {
                    do {
                        message(I18n::getSingleInstance()->getValue(I18n::Key::ASK_SAVE_PATH));
                        std::cin >> strInput;
                        if (!save(strInput.c_str())) {
                            message(I18n::getSingleInstance()->getValue(I18n::Key::FILE_EXIST_ERROR));

                            LOG_INFO("%s %s:%s", specialCTime(), language == Language::ENGLISH ? "save" : "保存",
                                I18n::getSingleInstance()->getValue(I18n::Key::FILE_EXIST_ERROR));
                        }
                        else {
                            break;
                        }
                    } while (true);
                }

                LOG_INFO("%s %s:%s", specialCTime(),language == Language::ENGLISH ? "save" : "保存",
                    language ==Language::ENGLISH?"exit game":"退出游戏");
                exit(0);
            }
            else {
                message(I18n::getSingleInstance()->getValue(I18n::Key::CONTINUE));
            }
        }
        else if (key == _pKeyMap->U)
        {
            if (_commands.empty()) {
                message(I18n::getSingleInstance()->getValue(I18n::Key::UNDO_ERROR));

                LOG_INFO("%s %s:%s", specialCTime(),language == Language::ENGLISH ? "undo" : "撤销",
                    I18n::getSingleInstance()->getValue(I18n::Key::UNDO_ERROR));
            }
            else {
                CCommand& oCommand = _commands.back();
                bool flag=oCommand.undo();
                _commands.pop_back();
                show();

                if (flag) {
                    LOG_INFO("%s %s:(%d,%d)-->(%d,%d) point_value=%d", specialCTime(), language == Language::ENGLISH ? "undo" : "撤销",
                        _curPoint.x, _curPoint.y,
                        oCommand.getPoint().x, oCommand.getPoint().y, oCommand.getPreValue());
                }
                else {
                    LOG_INFO("%s %s:%s", specialCTime(),language == Language::ENGLISH ? "undo" : "撤销",
                        I18n::getSingleInstance()->getLanguage()==Language::ENGLISH?"undo successfully":"撤销成功");
                }
            }
        }
        else if (key == _pKeyMap->LEFT)
        {
            int oldx = _curPoint.x;
            _curPoint.x = (_curPoint.x - 1) < 0 ? 0 : _curPoint.x - 1;
            show();

            LOG_INFO("%s %s:(%d,%d)-->(%d,%d)", specialCTime(), language == Language::ENGLISH ? "move to left" : "左移",
                oldx, _curPoint.y,_curPoint.x,_curPoint.y);
        }
        else if (key == _pKeyMap->RIGHT)
        {
            int oldx = _curPoint.x;
            _curPoint.x = (_curPoint.x + 1) > 8 ? 8 : _curPoint.x + 1;
            show();

            time_t timer = time(nullptr);
            LOG_INFO("%s %s:(%d,%d)-->(%d,%d)", specialCTime(),language == Language::ENGLISH ? "move to right" : "右移",
                oldx, _curPoint.y, _curPoint.x, _curPoint.y);
        }
        else if (key == _pKeyMap->DOWN)
        {
            int oldy = _curPoint.y;
            _curPoint.y = (_curPoint.y + 1) > 8 ? 8 : _curPoint.y + 1;
            show();

            time_t timer = time(nullptr);
            LOG_INFO("%s %s:(%d,%d)-->(%d,%d)", specialCTime(),language == Language::ENGLISH ? "move to down" : "下移",
                _curPoint.x, oldy, _curPoint.x, _curPoint.y);
        }
        else if (key == _pKeyMap->UP)
        {
            int oldy = _curPoint.y;
            _curPoint.y = (_curPoint.y - 1) < 0 ? 0 : _curPoint.y - 1;
            show();
            time_t timer = time(nullptr);
            LOG_INFO("%s %s:(%d,%d)-->(%d,%d)", specialCTime(),language == Language::ENGLISH ? "move to up" : "上移",
                _curPoint.x, oldy, _curPoint.x, _curPoint.y);
        }
        else if (key == _pKeyMap->ENTER)
        {
            if (isComplete()) {
                message(I18n::getSingleInstance()->getValue(I18n::Key::CONGRATULATION));

                time_t timer = time(nullptr);
                LOG_INFO("%s %s:%s", specialCTime(), language == Language::ENGLISH ? "check completed" : "检查游戏结束",
                    I18n::getSingleInstance()->getValue(I18n::Key::CONGRATULATION));

                getchar();
                exit(0);
            }
            else {
                message(I18n::getSingleInstance()->getValue(I18n::Key::NOT_COMPLETED));

                time_t timer = time(nullptr);
                LOG_INFO("%s %s:%s", specialCTime(), language == Language::ENGLISH ? "check completed" : "检查游戏结束",
                    I18n::getSingleInstance()->getValue(I18n::Key::NOT_COMPLETED));
            }
        }
    }

}

bool CScene::save(const char* filename) {
    /*
    std::filesystem::path filePath = std::filesystem::path(filename);
    if (!std::filesystem::exists(filePath)) {
        return false;
    }
    */

    Language language = I18n::getSingleInstance()->getLanguage();

    std::ofstream ofs(filename,std::ios::app);
    if (!ofs.is_open()) {
        time_t timer = time(nullptr);
        LOG_INFO("%s %s:%s", ctime(&timer), language == Language::ENGLISH ? "save file" : "保存文件",
            language == Language::ENGLISH ? "save failed" : "保存失败");
        return false;
    }
    ofs << RECORD_SEG_FLAG << std::endl;//输入文件按分割标志
    
    ofs << specialCTime() << std::endl;//写入时间

    //save map
    for (int i = 0;i < 81;i++) {
        ofs << _map[i].val << " " << static_cast<int>(_map[i].state) << std::endl;
    }

    //save cur point
    ofs << _curPoint.x << " " << _curPoint.y << std::endl;

    //save commands
    ofs << _commands.size() << std::endl;
    for (CCommand& command : _commands) {
        point_t point = command.getPoint();
        ofs << point.x << " " << point.y << " " << command.getPreValue() << " " << command.getCurValue() << std::endl;
    }

    ofs.close();

    LOG_INFO("%s %s:%s", specialCTime(), language == Language::ENGLISH ? "save game" : "保存游戏",
        language == Language::ENGLISH ? "save successfully" : "保存成功");

    return true;
}

bool CScene::load(const char* filename) {
    /*
    std::filesystem::path filepath = std::filesystem::path(filename);
    if (!std::filesystem::exist(filepath)) {
        return false;
    }
    */

    Language language = I18n::getSingleInstance()->getLanguage();

    std::ifstream ifs(filename,std::ios::in);
    if (!ifs.is_open()) {
        time_t timer = time(nullptr);
        LOG_INFO("%s %s:%s", ctime(&timer), language == Language::ENGLISH ? "load gane" : "加载游戏",
            language == Language::ENGLISH ? "load failed" : "加载失败");
        return false;
    }
    std::string str((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    ifs.close();

    std::size_t prePos = std::string::npos;
    std::size_t curPos = 0;
    while ((curPos = str.find(RECORD_SEG_FLAG,curPos)) != std::string::npos) {
        prePos = curPos;
        curPos+=strlen(RECORD_SEG_FLAG);
    }

    if (std::string::npos == prePos) {
        return false;
    }

    std::string data(str.substr(prePos));
    std::istringstream iss(data);
    std::string tmp("");
    std::getline(iss,tmp);//读取记录分隔符
    std::getline(iss, tmp);//读取时间
    
    //read map
    int cn = 0;
    int val = 0;
    int state = 0;
    while ((cn<81)&&(iss >> val >> state)) {
        _map[cn].val = val;
        _map[cn].state = static_cast<State>(state);
        cn++;
    }
    if (cn < 81) {
        return false;
    }

    //read cur point
    if (iss.eof()) {
        return false;
    }
    iss >> _curPoint.x >> _curPoint.y;

    //read commands
    int commandsSize;
    iss >> commandsSize;
    point_t point{0,0};
    int preValue = 0, curValue = 0;
    for (int i = 0;i < commandsSize;i++) {
        iss >> point.x >> point.y >> preValue >> curValue;
        CCommand command(this,point,preValue,curValue);
        _commands.emplace_back(command);
    }

    time_t timer = time(nullptr);
    LOG_INFO("%s %s:%s", ctime(&timer), language == Language::ENGLISH ? "load game" : "加载游戏",
        language == Language::ENGLISH ? "load successfully" : "加载成功");
    std::cout << "加载完成" << std::endl;
    return true;
}

void CScene::setMode(KeyMode mode) {
    switch (mode) {
    case KeyMode::NORMAL:
        _pKeyMap = new Normal;
        break;
    case KeyMode::VIM:
        _pKeyMap = new Vim;
        break;
    case KeyMode::ARROW:
        _pKeyMap = new Arrow;
        break;
    }
}

//private:
void CScene::init() { // 将每个格子的指针放到block里面

    point_value_t* pmap = _map.data();
    memset(pmap, UNSELECTED, sizeof _map);
    
    //生成row block
    for (int i = 0;i < _maxColumn;i++) {
        CBlock rowBlock;//更有利于流水执行
        for (int j = 0;j < _maxColumn;j++) {
           rowBlock.pushBack(pmap + i * 9+j);
        }
        _rowBlock[i] = std::move(rowBlock);
    }

    //生成 column block
    for (int i = 0;i < _maxColumn;i++) {
        CBlock colBlock;
        for (int j = 0;j < _maxColumn;j++) {
            colBlock.pushBack(pmap+i+j*9);
        }
        _columnBlock[i] = std::move(colBlock);
    }

    //生成 xy block
    for (int block = 0;block < _maxColumn;++block) {
        point_value_t *p = pmap + block / 3 * 27 + block % 3 * 3;
        CBlock xyBlock;
        for (int i = 0;i < 3;i++) {
            for (int j = 0;j < 3;j++) {
                xyBlock.pushBack(p + i * 9 + j);
            }
        }
        _xyBlock[block / 3][block % 3] = std::move(xyBlock);
    }
}

void CScene::setValue(const int value) {
    point_t point = _curPoint;
    setValue(point,value);
}

void CScene::setValue(const point_t &p, const int value) {
    _map[p.y * 9 + p.x].val = value;
}









