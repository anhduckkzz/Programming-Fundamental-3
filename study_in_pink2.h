/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Sherlock;
class Watson;
class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType { 
    MAGIC_BOOK,
    ENERGY_DRINK, 
    FIRST_AID, 
    EXCEMPTION_CARD, 
    PASSING_CARD 
};

enum ElementType { 
    PATH, 
    WALL, 
    FAKE_WALL 
};

enum RobotType { C, S, W, SW };

class MapElement {
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
public:
    Path();
};

class Wall : public MapElement {
public: 
    Wall();
};

class FakeWall : public MapElement {
    private:
        int req_exp;
    public:
        FakeWall(int in_req_exp);
        int getreq_exp() const;
}
;

class Map {
private:
    MapElement*** map;
    int num_rows, num_cols;
    int num_walls;
    Position * array_walls;
    int num_fake_walls;
    Position * array_fake_walls;
public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();
    bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
private:
    int r, c;
public:
    static const Position npos; 

    Position(int r = 0, int c = 0);

    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(const Position &pos) const;
};

class MovingObject {
friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map* map;
    string name;

public:
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    string getname() const;
};

class Character : public MovingObject {
protected:
    int hp;
    int exp;
    int num;
    string moving_rule;   

public:
    Character(int index, const Position &pos, Map * map, const string & name);
    virtual ~Character();
    virtual Position getNextPosition();
    Position getCurrentPosition() const;
    int getRow();
    int getCol();
    virtual void setExp(int exp);
    virtual void setHp(int hp);
    virtual int getExp();
    virtual int getHp();
    virtual void setName(string name);
    virtual string getName();
    virtual string str() const;
    virtual void move();
};

class Sherlock : public Character {
    friend class TestStudyInPink;
private:
    
public:
    Sherlock(int index, const string & moving_rule,const Position & pos, Map * map, int hp, int exp);
    void move() override;
    string str() const override;
};

class Watson : public Character {
    friend class TestStudyInPink;
public:
    Watson(int index, const string & moving_rule, const Position & pos, Map * map, int hp, int exp);
    void move() override;
    string str() const override;
    int getExp() const;
};

class Criminal : public Character {
private:
    Sherlock* sherlock;
    Watson * watson;
    int count;
public:
    Criminal(int index, const Position & pos, Map * map, Sherlock * sherlock, Watson * watson);
    Position getNextPosition() override;
    void move() override;
    string str() const override;
    void setCount(int count);
    int getCount() const;
};

class ArrayMovingObject {   
private:
    MovingObject **arr_mv_objs;
    int capacity;
    int count;
public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration {
    friend class StudyPinkProgram;
private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position * arr_walls;
    int num_fake_walls;
    Position * arr_fake_walls;
    int num_steps;
    string sherlock_moving_rule;
    Position sherlock_pos;
    int sherlock_hp;
    int sherlock_exp;
    string watson_moving_rule;
    Position watson_pos;
    int watson_hp;
    int watson_exp;
    Position criminal_pos;

    
public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
    void parsePosition(const string& input, Position& position);
    void parsePositions(const string& input, Position*& positions, int& num_positions);
};  

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject{
private:
    RobotType robot_type;
    Criminal * criminal;
    BaseItem * baseitem;

public:
Robot(int index, const Position & pos, Map * map, RobotType robot_type, Criminal* criminal);
static Robot* create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
virtual Position getNextPosition() = 0;
virtual void move();
virtual string str() const = 0;
virtual int getDistance() const = 0;

};

class RobotC : public Robot {
private:
    Criminal * criminal;
public:
    RobotC(int index , const Position & pos , Map * map , Criminal * criminal ) ;
    
    Position getNextPosition();
    void move();
    int getDistance(Sherlock* sherlock);
    int getDistance(Watson* watson);
    int getDistance() const;
    string str() const;
};

class RobotS : public Robot {
private:
    Sherlock * sherlock;
    Criminal * criminal;
public:
    RobotS ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * Sherlock );
    Position getNextPosition();
    int getDistance() const;
    void move();
    string str() const;
};

class RobotW : public Robot {
private:
    Watson * watson;
    Criminal * criminal;
public:
RobotW ( int index , const Position & pos , Map * map , Criminal * criminal , Watson * watson );
int getDistance() const;
Position getNextPosition();
string str() const;
};

class RobotSW : public Robot {
private:
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
public:
RobotSW ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson );
int getDistance() const;
Position getNextPosition();
string str() const;
};

class BaseItem{
    private:

public:
virtual bool canUse ( Character * obj , Robot * robot ) = 0;
virtual void use ( Character * obj , Robot * robot ) = 0;
};

class MagicBook : public BaseItem {
    public:
    MagicBook(ItemType itemtype);
    bool canUse ( Character * obj , Robot * robot );
    void use ( Character * obj , Robot * robot );
};

class EnergyDrink:public BaseItem {
    public:
    EnergyDrink(ItemType itemtype);
    bool canUse ( Character * obj , Robot * robot );
    void use ( Character * obj , Robot * robot );
};

class FirstAid : public BaseItem{
    public:
    FirstAid(ItemType itemtype);
    bool canUse ( Character * obj , Robot * robot );
    void use ( Character * obj , Robot * robot );
};

class ExcemptionCard: public BaseItem{
    public:
    ExcemptionCard(ItemType itemtype);
    bool canUse ( Character * obj , Robot * robot );
    void use ( Character * obj , Robot * robot );
};

class PassingCard: public BaseItem{
    public:
    PassingCard(ItemType itemtype);
    bool canUse ( Character * obj , Robot * robot );
    void use ( Character * obj , Robot * robot );
};
class StudyPinkProgram {
private:
    // Sample attributes
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;
public:
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~StudyPinkProgram();
};


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
