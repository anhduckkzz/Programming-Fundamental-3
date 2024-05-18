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
bool Position::isEqual(const Position &pos) const{
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

string Character::getName(){
    return name;
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
int Watson::getExp() const{
   return exp; 
};

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

            cout << key << " " << value << endl;
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
                arr_walls = parsePositionArray(value);
                num_walls = (value.length()/6);
            }else if (key == "NUM_FAKE_WALLS"){
                num_fake_walls = stoi(value);
                arr_fake_walls = new Position[num_fake_walls];
            }else if (key == "ARRAY_FAKE_WALLS"){
                arr_fake_walls = parsePositionArray(value);
            }else if (key == "SHERLOCK_MOVING_RULE"){
                sherlock_moving_rule = value;
            }else if (key == "SHERLOCK_pos"){
                sherlock_pos = parsePosition(value);
            }else if (key == "SHERLOCK_hp"){
                sherlock_hp = stoi(value);
            }else if (key == "SHERLOCK_exp"){
                sherlock_exp = stoi(value);
            }else if (key == "WATSON_MOVING_RULE"){
                watson_moving_rule = value;
            }else if (key == "WATSON_pos"){
                watson_pos = parsePosition(value);
            }else if (key == "WATSON_hp"){
                watson_hp = stoi(value);
            }else if (key == "WATSON_exp"){
                watson_exp = stoi(value);
            }else if (key == "CRIMINAL_pos"){
                criminal_pos = parsePosition(value);
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
    ss << "SHERLOCK_pos=" << sherlock_pos.str() << endl;
    ss << "SHERLOCK_hp=" << sherlock_hp << endl;
    ss << "SHERLOCK_exp=" << sherlock_exp << endl;
    ss << "WATSON_MOVING_RULE=" << watson_moving_rule << endl;
    ss << "WATSON_pos=" << watson_pos.str() << endl;
    ss << "WATSON_hp=" << watson_hp << endl;
    ss << "WATSON_exp=" << watson_exp << endl;
    ss << "CRIMINAL_pos=" << criminal_pos.str() << endl;
    ss << "NUM_STEPS=" << num_steps << endl;
    ss << "]";
    return ss.str();
}
Position* Configuration::parsePositionArray(const string& positionArrayStr){
    int num_positions = 1;
    for(int i = 0; i < positionArrayStr.length(); i++){
        if(positionArrayStr[i] == ';'){
            num_positions++;
        }
    }
    Position* positions = new Position[num_positions];
    for (int i = 0; i < num_positions; i++) {
        positions[i] = parsePosition(positionArrayStr.substr(i * 6, 6));
    }
    return positions;
}
Position Configuration::parsePosition(const string& positionStr){
    int commaPos = positionStr.find(",");
    int row = stoi(positionStr.substr(1, commaPos - 1));
    int col = stoi(positionStr.substr(commaPos + 1, positionStr.size() - commaPos - 2));
    return Position(row, col);
}

//Task 3.10 - Robot
Robot::Robot(int index, const Position & pos, Map * map, RobotType robot_type, Criminal* criminal) : MovingObject(index, pos, map,"Robot"){
    this->criminal = criminal;
    this->robot_type = robot_type;
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
    if(criminal->getCount() == 2 && criminal->getCount() > 0){
        return new RobotC(index, criminal->getCurrentPosition(), map, criminal);
    }else if(disCS > disCW){
        RobotS* robots =  new RobotS(index, criminal->getCurrentPosition(), map, criminal, sherlock);
        return robots;
    }
    else if(disCS < disCW){
        RobotW* robotw =  new RobotW(index, criminal->getCurrentPosition(), map, criminal, watson);
        return robotw;
    }
    else if(disCS == disCW){
        RobotSW* robotsw = new RobotSW(index, criminal->getCurrentPosition(), map, criminal, sherlock, watson);
        return robotsw;
    }
    return 0;
};
//RobotC

RobotC::RobotC ( int index , const Position & pos , Map * map , Criminal * criminal ): Robot(index,pos,map,C,criminal){
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
    return "Robot[pos="+pos.str()+"type="+to_string(C)+"dist="+""+"]";
};
//RobotS

RobotS::RobotS ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * Sherlock ): Robot(index,pos,map,S,criminal){
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
RobotW::RobotW ( int index , const Position & pos , Map * map , Criminal * criminal , Watson * watson ): Robot(index,pos,map,W,criminal){
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

//RobotSW

RobotSW::RobotSW ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson ) : Robot(index,pos,map,SW,criminal){
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

bool BaseItem::canUse(Character * obj, Robot * robot){
    return 0;
}

int two_into_one(int p){
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

void Character::setExp(int exp){
    this->exp = exp;
}

void Character::setHp(int hp){
    this->hp = hp;
}

// Class for each item
// MagicBook
MagicBook::MagicBook(ItemType itemtype){
    itemtype = MAGIC_BOOK;
}

bool MagicBook::canUse(Character * obj, Robot * robot){
    if(obj->getExp() < 350 && robot == nullptr){
        return true;
    }
    return false;
}

void MagicBook::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setExp(obj->getExp()*125/100);
    }
}

//EnergyDrink
EnergyDrink::EnergyDrink(ItemType itemtype){
    itemtype = ENERGY_DRINK;
}
bool EnergyDrink::canUse(Character * obj, Robot * robot){
    if(obj->getHp() < 100 && robot == nullptr){
        return true;
    }
    return false;
}

void EnergyDrink::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
        obj->setHp(obj->getHp() * 120/100);
    }
}

//FirstAid
FirstAid::FirstAid(ItemType itemtype){
    itemtype = FIRST_AID;
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

//ExcemptionCard
ExcemptionCard::ExcemptionCard(ItemType itemtype){
    itemtype = EXCEMPTION_CARD;
}
bool ExcemptionCard::canUse(Character * obj, Robot * robot){
    if(obj->getExp() % 2 == 1 && obj->getName() == "Sherlock"){
        return true;
    }
    return false;
}

void ExcemptionCard::use(Character * obj, Robot * robot){
    if(canUse(obj,robot)){
       
    }
}



////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
///////////////////////////////////////////////