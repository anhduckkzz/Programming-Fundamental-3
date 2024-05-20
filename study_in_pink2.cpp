#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

int distance(const Position& char1,const Position& char2){
    return(abs(char1.getRow()-char2.getRow())+abs(char1.getCol()-char2.getCol()));
}
// TASK 3.1 - MapElement
// Requirement 1
MapElement::MapElement(ElementType in_type){
    this->type = in_type;
}
MapElement::~MapElement(){};
ElementType MapElement::getType() const {
    return type;
}

//Requirement 2
Path::Path() : MapElement(type) {
    type = PATH;
}
Wall::Wall() : MapElement(type) {
    type = WALL;
}
FakeWall::FakeWall(int in_req_exp) : MapElement(type){
    this->type = FAKE_WALL;
    this->req_exp = in_req_exp;
}
int FakeWall::getreq_exp() const{
    return req_exp;
}

//TASK 3.2 - Map

Map::Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls) {
    this->num_cols = num_cols;
    this->num_rows = num_rows;

    // Allocate memory for the map
    map = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; i++) {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; j++) {
            map[i][j] = new Path(); // Initialize the map with blank    
        }
    }

    for(int i = 0;i < num_rows;i++){
        for(int j = 0;j < num_cols;j++){
            for (int k = 0; k < num_walls; k++) {
                int wall_row = array_walls[k].getRow();
                int wall_col = array_walls[k].getCol();
                int fake_wall_col = array_fake_walls[k].getCol();
                int fake_wall_row = array_fake_walls[k].getRow();
                
                if((map[i][j] == map[wall_row][wall_col]) || (map[i][j] == map[fake_wall_row][fake_wall_col])){
                    if(map[i][j] == map[wall_row][wall_col] && map[i][j] == map[fake_wall_row][fake_wall_col]){
                        delete map[wall_row][wall_col];
                        map[wall_row][wall_col] = new Wall(); // If Wall and FakeWall are at the same position, replace with Wall
                    }
                    
                    if(map[i][j] == map[wall_row][wall_col]){
                        delete map[wall_row][wall_col];
                        map[wall_row][wall_col] = new Wall(); // Replace with Wall
                    }

                    if(map[i][j] == map[fake_wall_row][fake_wall_col]){
                        delete map[fake_wall_row][fake_wall_col];
                        map[fake_wall_row][fake_wall_col] = new FakeWall((i * 257 + j * 139 + 89) % 900 + 1); // Replace with FakeWall
                    }
                }
            }
        }
    }
}
Map::~Map(){
    for(int i = 0;i < num_rows;i++){
        for(int j = 0;j < num_cols;j++){
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}
bool Map::isValid(const Position &pos, MovingObject *mv_obj) const {
    int r = pos.getRow();
    int c = pos.getCol();
    if (r < 0 || r >= num_rows || c < 0 || c >= num_cols) {
        return false; // Out of bounds
    }
    MapElement *element = map[r][c];
   
    if (element->getType() == WALL) {
        return false; // Wall
    }
    if (element->getType() == FAKE_WALL) {
        if(mv_obj->getname() == "Watson"){
            if(Watson * watson = dynamic_cast<Watson*>(mv_obj)){
                if(watson->getExp() <= dynamic_cast<FakeWall*>(element)->getreq_exp()){
                    return false;
                }
            }
        }
        
    }
    return true; // Default case
}

//Task 3.3 - Position

const Position Position::npos(-1, -1);
Position::Position(int r, int c){
    this->r = r;
    this->c = c;
}
Position::Position(const string & str_pos){
    if(str_pos.find(',') != string::npos){
        this->r = stoi(str_pos.substr(1,str_pos.find(',') - 1));
        this->c = stoi(str_pos.substr(str_pos.find(',') + 1,str_pos.find(')')  - str_pos.find(',')));
    }
}
void Position::setRow(int r){
    this->r = r;
}
int Position::getRow() const{
    return r;
}
void Position::setCol(int c){
    this->c = c;
}
int Position::getCol() const{
    return c;
}
string Position::str() const{
    return "(" + to_string(r) + "," + to_string(c) + ")";
};
bool Position::isEqual(Position pos) const{
    if(r == pos.getRow() && c == pos.getCol()) return true;
    else return false;
};

//Task 3.4 - MovingObject

MovingObject::MovingObject(int index,const Position pos, Map * map,const string & name){
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = "";
} 
MovingObject::~MovingObject(){};
Position MovingObject::getCurrentPosition() const{
    return pos; 
}
string MovingObject::getname() const{
    return name;
}

//Task Character

Character::Character(int index, const Position &pos, Map *map, const string &name) : MovingObject(index, pos, map, name) {
}
Character::~Character() {
}
Position Character::getNextPosition() {
    num++;
    Position next_pos = pos;
    if (num == moving_rule.length())
        num = 0;
    if (num < moving_rule.length())
    {
        switch (moving_rule[num])
        {
        case 'U':
            next_pos.setRow(pos.getRow() - 1);
            break;
        case 'D':
            next_pos.setRow(next_pos.getRow() + 1);
            break;
        case 'R':
            next_pos.setCol(next_pos.getCol() + 1);
            break;
        case 'L':
            next_pos.setCol(next_pos.getCol() - 1);
            break;
        default:
            break;
        }
        if (map->isValid(next_pos, this))
            return next_pos;
        else
        {
            next_pos = Position::npos;
        }
    }
    return Position::npos;
}
Position Character::getCurrentPosition() const{
    return pos;
}
int Character::getCol(){
    return pos.getCol();
}
int Character::getRow(){
    return pos.getRow();
}
string Character::str() const{
    return 0;
};
void Character::move(){};

int Character::getExp(){
    return exp;
}

int Character::getHp(){
    return hp;
}

void Character::setName(string name){
    this->name = name;
}


//Task 3.5 - Sherlock

Sherlock::Sherlock(int index, const string &moving_rule,const  Position &pos, Map *map, int hp, int exp) : Character(index, pos, map, "Sherlock"){
    this->moving_rule = moving_rule;
    this->exp = exp;
    this->hp = hp;
    if (hp < 0) {
        hp = 0;
    } else if (hp > 900) {
        hp = 900;
    }

    if (exp < 0) {
        exp = 0;
    } else if (exp > 900) {
        exp = 900;
    }
}
void Sherlock::move() {
    Position next_pos = getNextPosition();
    if (!next_pos.isEqual(Position::npos))
        pos = next_pos;
}
string Sherlock::str() const {
    return "Sherlock[index=" + to_string(index) + ",pos=" + pos.str() + ",moving_rule=" + moving_rule +"]";
}

//Task 3.6 - Watson

Watson::Watson(int index, const string &moving_rule, const Position &pos, Map *map, int hp, int exp) : Character(index, pos, map, "Watson"){
    this->moving_rule = moving_rule;
    this->exp = exp;
    this->hp = hp; 
    if (hp < 0) {
        hp = 0;
    } else if (hp > 500) {
        hp = 500;
    }

    if (exp < 0) {
        exp = 0;
    } else if (exp > 900) {
        exp = 900;
    }   
    
}
void Watson::move(){
    Position next_pos = this->getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
        pos = next_pos;
    
}
string Watson::str() const {
    return "Watson[index=" + to_string(index) + ",pos=" + pos.str() + ",moving_rule=" + moving_rule + "]";
}

//Task 3.7 - Criminal

Criminal::Criminal(int index, const Position &pos, Map *map, Sherlock * sherlock, Watson * watson) : Character(index, pos, map, "Criminal") {
    this->sherlock = sherlock;
    this->watson = watson;
    count = 0;
}
void Criminal::move(){
    Position next_pos = this->getNextPosition();
    if (next_pos.isEqual(Position::npos)) return;
    pos = next_pos;
    count++;
}
void Criminal::setCount(int count){
    this->count = count;
}
int Criminal::getCount() const{
    return count;
}
string Criminal::str() const {
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}
Position Criminal::getNextPosition(){
    Position next_pos = pos; 
    int sherlock_row = sherlock->getCurrentPosition().getRow();
    int sherlock_col = sherlock->getCurrentPosition().getCol();
    int watson_row = watson->getCurrentPosition().getRow();
    int watson_col = watson->getCurrentPosition().getCol();
    int crim_col = this->getCurrentPosition().getCol();
    int crim_row = this->getCurrentPosition().getRow();

    Position arr[4] = {0};  // arr of Position of location of Criminal
    arr[0] = Position(crim_row - 1, crim_col);
    arr[1] = Position(crim_row,crim_col - 1);
    arr[2] = Position(crim_row + 1,crim_col);
    arr[3] = Position(crim_row,crim_col + 1);
    
    int distance;
    int max = -1;
    for (int i = 0; i < 4; i++) {
        if(map->isValid(arr[i],this)){
            distance = abs(arr[i].getRow() - sherlock_row) + abs(arr[i].getCol() - sherlock_col) + abs(arr[i].getRow() - watson_row) + abs(arr[i].getCol() - watson_col);
            if (distance > max) {
                max = distance;
                next_pos = arr[i];
            }else if(distance == max){
                continue;
            }
        }
    }

    if (next_pos.isEqual(pos)) {
        next_pos = Position::npos;
    }
    return next_pos;
}

//Task 3.8 - ArrayMovingObject

ArrayMovingObject::ArrayMovingObject(int capacity){
    this->capacity = capacity;
    this->count = 0;
    arr_mv_objs = new MovingObject *[capacity];
}
ArrayMovingObject::~ArrayMovingObject(){
    delete arr_mv_objs;
};
bool ArrayMovingObject::isFull() const {
    if (count == capacity) {
        return true;
    }
    return false;
}
bool ArrayMovingObject::add(MovingObject *mv_obj) {
    if (this->isFull()) {
        return false;
    }
    arr_mv_objs[count] = mv_obj;
    count++;
    return true; 
}
MovingObject * ArrayMovingObject::get(int index) const {
    if (index < 0 || index > count) {
        return nullptr;
    }
    return arr_mv_objs[index];
}
int ArrayMovingObject::size() const {
    return count;
}   
string ArrayMovingObject::str() const {
    string res = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity);
    for (int i = 0; i < count; i++) {
        res+= ";" + arr_mv_objs[i]->str();
    }
    return res;
}

//Task 3.9 - Configuration

Configuration::Configuration(const string &filepath){
    ifstream file(filepath);
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream target(line);
            string key, value;  
            getline(target, key, '=');
            getline(target, value);

            if (key == "MAP_NUM_ROWS") {
                map_num_rows = stoi(value);
            }else if(key == "MAP_NUM_COLS"){
                map_num_cols = stoi(value);
            }else if (key == "MAX_NUM_MOVING_OBJECTS"){
                max_num_moving_objects = stoi(value);
            }else if (key == "NUM_WALLS"){
                int count = 1;
                for(int i = 0; i < key.length(); i++){
                    if(key[i] == ';'){
                        count++;
                    }
                }
            num_walls = count;
            arr_walls = new Position[num_walls];
            }else if (key == "ARRAY_WALLS"){
                parsePositions(value, arr_walls, num_walls);
            }else if (key == "NUM_FAKE_WALLS"){
                arr_fake_walls = new Position[num_fake_walls];
            }else if (key == "ARRAY_FAKE_WALLS"){
                parsePositions(value, arr_fake_walls, num_fake_walls);
            }else if (key == "SHERLOCK_MOVING_RULE"){
                sherlock_moving_rule = value;
            }else if (key == "SHERLOCK_INIT_POS"){
                parsePosition(value, sherlock_pos);
            }else if (key == "SHERLOCK_INIT_HP"){
                sherlock_hp = stoi(value);
            }else if (key == "SHERLOCK_INIT_EXP"){
                sherlock_exp = stoi(value);
            }else if (key == "WATSON_MOVING_RULE"){
                watson_moving_rule = value;
            }else if (key == "WATSON_INIT_POS"){
                parsePosition(value, watson_pos);
            }else if (key == "WATSON_INIT_HP"){
                watson_hp = stoi(value);
            }else if (key == "WATSON_INIT_EXP"){
                watson_exp = stoi(value);
            }else if (key == "CRIMINAL_INIT_POS"){
                parsePosition(value, criminal_pos);
            }else if (key == "NUM_STEPS"){
                num_steps = stoi(value);
            }
        }
        file.close();
    }
}
Configuration::~Configuration(){
    delete[] arr_walls;
    delete[] arr_fake_walls;
}
string Configuration::str() const {
    stringstream ss;
    ss << "Configuration[" << endl;
    ss << "MAP_NUM_ROWS=" << map_num_rows << endl;
    ss << "MAP_NUM_COLS=" << map_num_cols << endl;
    ss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << endl;
    ss << "NUM_WALLS=" << num_walls << endl;
    ss << "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; ++i) {
        ss << "(" << arr_walls[i].getRow() << "," << arr_walls[i].getCol() << ")";
        if (i < num_walls - 1) ss << ";";
    }
    ss << "]" << endl;
    ss << "NUM_FAKE_WALLS=" << num_fake_walls << endl;
    ss << "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; ++i) {
        ss << "(" << arr_fake_walls[i].getRow() << "," << arr_fake_walls[i].getCol() << ")";
        if (i < num_fake_walls - 1) ss << ";";
    }
    ss << "]" << endl;  
    ss << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << endl;
    ss << "SHERLOCK_INIT_POS=" << sherlock_pos.str() << endl;
    ss << "SHERLOCK_INIT_HP=" << sherlock_hp << endl;
    ss << "SHERLOCK_INIT_EXP=" << sherlock_exp << endl;
    ss << "WATSON_MOVING_RULE=" << watson_moving_rule << endl;
    ss << "WATSON_INIT_POS=" << watson_pos.str() << endl;
    ss << "WATSON_INIT_HP=" << watson_hp << endl;
    ss << "WATSON_INIT_EXP=" << watson_exp << endl;
    ss << "CRIMINAL_INIT_POS=" << criminal_pos.str() << endl;
    ss << "NUM_STEPS=" << num_steps << endl;
    ss << "]";
    return ss.str();
}
void Configuration::parsePositions(const string& input, Position*& positions, int& num_positions){
    int count = 1;
    for (int i = 0; i < input.length(); i++){
        if (input[i] == ';') count++;
    }
    num_positions = count;
    positions = new Position[num_positions];
    int start = input.find("[") + 1;
    int end = input.find("]");
    string substr = input.substr(start, end - start);
    int pos = 0;
    for (int i=0; i<num_positions - 1; i++){
        pos = substr.find(";");
        string pair = substr.substr(0, pos);
        int comma_pos = pair.find(",");
        int row = stoi(pair.substr(1, comma_pos - 1));
        int col = stoi(pair.substr(comma_pos + 1, pair.size() - comma_pos - 2));
        positions[i] = Position(row,col);
        substr.erase(0,pos+1);
    }
    //the last pair
    int comma_pos = substr.find(",");
    int row = stoi(substr.substr(1, comma_pos - 1));
    int col = stoi(substr.substr(comma_pos + 1, substr.size() - comma_pos - 2));    
    positions[num_positions - 1] = Position(row,col);

    delete[] positions;
    return;
}

void Configuration::parsePosition(const string& input, Position& position){
    int start = input.find("(") + 1;
    int comma_pos = input.find(",");
    position.setRow(stoi(input.substr(start, comma_pos - start)));
    int end = input.find(")");
    position.setCol(stoi(input.substr(comma_pos + 1, end - comma_pos - 1)));
}
//Task 3.10 - Robot
Robot::Robot(int index, const Position & pos, Map * map, Criminal* criminal) : MovingObject(index, pos, map,"Robot"){
    this->criminal = criminal;
}

Position getNextPosition(){
return 0;
}
void Robot::move(){
}
string Robot::str() const{
    return 0;
}
int Robot::getDistance()const{
    return 0;
}


Robot* Robot::create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson){
    int disCS = distance(criminal->getCurrentPosition(),sherlock->getCurrentPosition());
    int disCW = distance(criminal->getCurrentPosition(),watson->getCurrentPosition());
    Position robot_pos = criminal->getCurrentPosition();

    int p = robot_pos.getCol() * robot_pos.getRow();
    int s = two_into_one(p);

    if(s == 1 || s == 0){
        setItem(MAGIC_BOOK, criminal);
    }else if(s == 2 || s == 3){
        setItem(ENERGY_DRINK,criminal);
    }else if(s == 4 || s == 5){
        setItem(EXCEMPTION_CARD,criminal);
    }else if(s == 6 || s == 7){
        setItem(FIRST_AID,criminal);
    }else if(s == 8 || s == 9){
        setItem(PASSING_CARD,criminal);
    }
        
    if(criminal->getCount() % 3 == 0){
        if (criminal->getCount() == 3){
        RobotC* robotc =  new RobotC(index, robot_pos, map, criminal);
        return robotc;
    }else if(disCS > disCW){    
        RobotS* robots =  new RobotS(index, robot_pos, map, criminal, sherlock);
        return robots;
    }
    else if(disCS < disCW){
        RobotW* robotw =  new RobotW(index, robot_pos, map, criminal, watson);
        return robotw;
    }
    else if(disCS == disCW){
        RobotSW* robotsw = new RobotSW(index, robot_pos, map, criminal, sherlock, watson);
        return robotsw;
    }
}
return nullptr;
}

void Robot::setItem(ItemType itemtype, Criminal* criminal)
{
    switch (itemtype)
    {
    case MAGIC_BOOK:
        item = new MagicBook();
        break;
    case ENERGY_DRINK:
        item = new EnergyDrink();
        break;
    case FIRST_AID:
        item = new FirstAid();
        break;
    case EXCEMPTION_CARD:
        item = new ExcemptionCard();
        break;
    case PASSING_CARD:
        int t = (criminal->getRow()*11 + criminal->getCol()) % 4;
        if(t == 0){
            item = new PassingCard("RobotS");
        }else if(t == 1){
            item = new PassingCard("RobotC");
        }else if(t == 2){
            item = new PassingCard("RobotSW");
        }else if(t == 3){
            item = new PassingCard("all");
        }
        break;
    }
};

int Robot::two_into_one(int p){
    int result;
    string temp = to_string(p);
    if(temp.length() == 1){
        result = p;
    }else{
        result = (p % 10) + (p / 10);
        if(result >= 10){
            result = two_into_one(result);
        }
    }
    return result;
}
//RobotC

RobotC::RobotC ( int index , const Position & pos , Map * map , Criminal * criminal ): Robot(index,pos,map,criminal){
    this->criminal = criminal;
}
Position RobotC::getNextPosition(){
    Position next_pos = criminal->getCurrentPosition();
    if (map->isValid(next_pos,this)){
        return next_pos;
    }else{
        return Position::npos;
    
    }
}
void RobotC::move(){
    Position next_pos = getCurrentPosition();
    if (next_pos.isEqual(Position::npos));
        pos = next_pos;
}
int RobotC::getDistance(Sherlock* sherlock){
    int distance = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
    return distance;
}
int RobotC::getDistance(Watson* watson){
    int distance = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
    return distance;
}
int RobotC::getDistance() const{
    return 0;
};
string RobotC::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(C)+",dist="+""+"]";
};

RobotType RobotC::getType() const{
    return C;
}
//RobotS

RobotS::RobotS ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * Sherlock ): Robot(index,pos,map,criminal){
    this->sherlock = sherlock;
    this->criminal = criminal;
}
Position RobotS::getNextPosition() {
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[4];
    arr[0] = Position(pos.getRow(), pos.getCol()+1);  // Up
    arr[1] = Position(pos.getRow()+1, pos.getCol());  // Right
    arr[2] = Position(pos.getRow(), pos.getCol()-1);  // Down
    arr[3] = Position(pos.getRow()-1, pos.getCol());  // Left
    for(int i = 0; i < 4; i++){
        if(map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol());
            if(distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }
        }
        else continue;
    }
    return next_pos;
}
int RobotS::getDistance() const{
    int distance = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
    return distance;
}
void RobotS::move(){
    Position next_pos = getNextPosition();
    if (next_pos.isEqual(Position::npos));
    pos = next_pos;
}
string RobotS::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(S)+"dist="+to_string(getDistance())+"]";
}

RobotType RobotS::getType() const{
    return S;
}
//RobotW
Position RobotW::getNextPosition() {
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[4];
    arr[0] = Position(pos.getRow(), pos.getCol()+1);
    arr[1] = Position(pos.getRow()+1, pos.getCol());
    arr[2] = Position(pos.getRow(), pos.getCol()-1);
    arr[3] = Position(pos.getRow()-1, pos.getCol());
    for (int i = 0; i < 4; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }
        }
        else continue;
    }
    return next_pos;
};
RobotW::RobotW ( int index , const Position & pos , Map * map , Criminal * criminal , Watson * watson ): Robot(index,pos,map,criminal){
    this->watson = watson;
    this->criminal = criminal;
}
int RobotW::getDistance() const{
    int distance = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());
    return distance;
}
string RobotW::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(SW)+"dist="+to_string(getDistance())+"]";
};

RobotType RobotW::getType() const{
    return W;
}
//RobotSW

RobotSW::RobotSW ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson ) : Robot(index,pos,map,criminal){
    this->sherlock = sherlock;
    this->criminal = criminal;
    this->watson = watson;
}
Position RobotSW::getNextPosition() {
    Position next_pos = pos;
    int min_distance = 100;
    Position arr[8];
    arr[0] = Position(pos.getRow(), pos.getCol()+2);
    arr[1] = Position(pos.getRow()-1,pos.getCol()+1);
    arr[2] = Position(pos.getRow()+2, pos.getCol());
    arr[3] = Position(pos.getRow()+1, pos.getCol()+1);
    arr[4] = Position(pos.getRow(), pos.getCol()-2);
    arr[5] = Position(pos.getRow()+1, pos.getCol()-1);
    arr[6] = Position(pos.getRow()-2, pos.getCol());
    arr[7] = Position(pos.getRow()-1, pos.getCol()-1);
    for (int i = 0; i < 8; i++){
        if (map->isValid(arr[i], this)){
            int distance = abs(arr[i].getRow() - sherlock->getCurrentPosition().getRow()) + abs(arr[i].getCol() - sherlock->getCurrentPosition().getCol()) + abs(arr[i].getRow() - watson->getCurrentPosition().getRow()) + abs(arr[i].getCol() - watson->getCurrentPosition().getCol());
            if (distance < min_distance){
                min_distance = distance;
                next_pos = arr[i];
            }
            else if (distance == min_distance){
                continue;
            }   
        }
        else continue;
    }
    return next_pos;
};
int RobotSW::getDistance() const{
    int sherlock_dis,watson_dis;
    sherlock_dis = abs(pos.getRow() - sherlock->getCurrentPosition().getRow()) + abs(pos.getCol() - sherlock->getCurrentPosition().getCol());
    watson_dis = abs(pos.getRow() - watson->getCurrentPosition().getRow()) + abs(pos.getCol() - watson->getCurrentPosition().getCol());

    return sherlock_dis + watson_dis;
}
string RobotSW::str() const{
    return "Robot[pos="+pos.str()+"type="+to_string(SW)+"dist="+to_string(getDistance())+"]";
}

RobotType RobotSW::getType() const{
    return SW;
}

//Task 3.11 - BaseItem
bool BaseItem::canUse(Character * obj, Robot * robot){
    return 0;
}

void Character::setExp(int exp){
    this->exp = exp;
}

void Character::setHp(int hp){
    this->hp = hp;
}

// Class for each item
// MagicBook
ItemType MagicBook::getitemtype() const{ 
    return MAGIC_BOOK;
}

bool MagicBook::canUse(Character * obj, Robot * robot){
    if(obj->getExp() < 350 && robot == nullptr){
        return true;
    }
    return false;
}

string MagicBook::str() const{
    return "MAGIC_BOOK";
}

void MagicBook::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setExp(ceil(obj->getExp()*125/100));
    }
}

//EnergyDrink
ItemType EnergyDrink::getitemtype() const{
    return ENERGY_DRINK;
}
bool EnergyDrink::canUse(Character * obj, Robot * robot){
    if(obj->getHp() < 100 && robot == nullptr){
        return true;
    }
    return false;
}

void EnergyDrink::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setHp(ceil(obj->getHp() * 120/100));
    }
}

string EnergyDrink::str() const{
    return "ENERGGY_DRINK";
}

//FirstAid
ItemType FirstAid::getitemtype() const{
    return FIRST_AID;
}
bool FirstAid::canUse(Character * obj, Robot * robot){
    if((obj->getHp() <= 100 || obj->getExp() <= 350)&& robot == nullptr){
        return true;
    }
    return false;
}

void FirstAid::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setHp(obj->getHp() * 150/100);
    }
}

string FirstAid::str() const{
    return "FIRST_AID";
}

//ExcemptionCard
ItemType ExcemptionCard::getitemtype() const{
    return EXCEMPTION_CARD;
}
bool ExcemptionCard::canUse(Character * obj, Robot * robot){
    if(obj->getExp() % 2 == 1 && obj->getname() == "Sherlock"){
        return true;
    }
    return false;
}

void ExcemptionCard::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setExp(obj->getExp());
        obj->setHp(obj->getHp());
    }
}

string ExcemptionCard::str() const{
    return "EXCEMPTION_CARD";
}

//PassingCard
PassingCard::PassingCard(int a,int b){
    int t = (a * 11 + b) % 4;
    if(t == 0){
        challenge == "RobotS";
    }
    else if(t==1){
        challenge == "RobotC";
    }
    else if(t==2){
        challenge == "RobotSW";
    }
    else if(t==3){
        challenge == "all";
    }
}
PassingCard::PassingCard(string challenge){
    this->challenge = challenge;
}
ItemType PassingCard::getitemtype() const{
    return PASSING_CARD;
}

bool PassingCard::canUse(Character * obj, Robot * robot){
    if(obj->getExp() % 2 == 0 && obj->getname() == "Watson"){
        return true;
    }
    return false;
}
void PassingCard::use(Character * obj, Robot * robot){
    if(canUse(obj, robot)){
        if(challenge == "RobotS"){
            if(robot->getType() == S){
                obj->setHp(obj->getHp());
                obj->setExp(obj->getExp());
            }
        }
        else if(challenge == "RobotC"){
            if(robot->getType() == C){
                obj->setExp(obj->getExp());
                obj->setHp(obj->getHp());
            }
        }
        else if(challenge == "RobotSW"){
            if(robot->getType() == SW){
                obj->setExp(obj->getExp());
                obj->setHp(obj->getHp());
            }
        }
        else if(challenge == "all"){
            obj->setExp(obj->getExp());
            obj->setHp(obj->getHp());
        }
        else 
            obj->setHp(obj->getHp() - 50);
    }
}

string PassingCard::str() const{
    return "PASSING_CARD";
}


//Task 3.12 - BaseBag
BaseBag::Node::Node(BaseItem* item, Node* next = nullptr){
    this->item = item;
    this->next = next;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
///////////////////////////////////////////////