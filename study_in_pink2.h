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
class Character;

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

enum CharacterType{
    SHERLOCK,
    WATSON,
    CRIMINAL,
    ROBOT
};

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
    friend class TestStudyInPink;
    protected:
        ElementType type;
    public:
        MapElement(ElementType in_type);
        virtual ~MapElement();
        virtual ElementType getType() const;
};

class Path : public MapElement {
    friend class TestStudyInPink;
    public:
        Path();
    };

class Wall : public MapElement {
    friend class TestStudyInPink;
    public: 
        Wall();
    };

class FakeWall : public MapElement {
    friend class TestStudyInPink;
    private:
        int req_exp;
    public:
        FakeWall(int in_req_exp);
        int getreq_exp() const;
}
;

class Map {
    friend class TestStudyInPink;
    private:
        MapElement*** map;
        int num_rows, num_cols;
    public:
        Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
        ~Map();
        bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
    friend class TestStudyInPink;
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

        bool isEqual(int r, int c) const;
        bool isEqual(Position pos) const;
};

class MovingObject {
friend class TestStudyInPink;
    protected:
        int index;
        Position pos;
        Map* map;
        string name;
        CharacterType character_type;

    public:
        MovingObject(int index, const Position pos, Map * map, const string & name="");
        virtual ~MovingObject();
        virtual Position getNextPosition() = 0;
        virtual Position getCurrentPosition() const;
        virtual void setCurrentPosition(Position pos);

        virtual void move() = 0;
        virtual string str() const = 0;

        virtual CharacterType getCharacterType() const;
        virtual void setCharacterType(CharacterType character_type);
};

class Character : public MovingObject {
    friend class TestStudyInPink;
    protected:
        int hp;
        int exp;
        int num;
        Position next_pos;
        string moving_rule;   
    public:
        Character(int index, const Position &pos, Map * map, const string & name);
        virtual ~Character();
        virtual Position getNextPosition();
        Position getCurrentPosition() const;
        virtual string str() const = 0;
        virtual void move() = 0;
        virtual void setCharacterType(CharacterType character_type);
        CharacterType getCharacterType() const = 0;
        
        int getRow();
        int getCol();
        virtual void setExp(int exp);
        virtual void setHp(int hp);
        virtual int getExp();
        virtual int getHp();
};

class Sherlock : public Character {
    friend class TestStudyInPink;
    private:
    SherlockBag* bag;
    public:
        Sherlock(int index, const string & moving_rule,const Position & pos, Map * map, int hp, int exp);
        ~Sherlock();
        void move() override;
        string str() const override;
        void setCharacterType(CharacterType character_type) override;
        CharacterType getCharacterType() const override;
        bool meet(RobotC* robotc);
        bool meet(RobotS* robots);
        bool meet(RobotW* robotw);
        bool meet(RobotSW* robotsw);
        bool meet(Watson* watson);
        SherlockBag* getBag() const;

    };

    class Watson : public Character {
        friend class TestStudyInPink;
        public:     //illegal
        WatsonBag* bag;
    public:
        Watson(int index, const string & moving_rule, const Position & pos, Map * map, int hp, int exp);
        ~Watson();
        void move();
        string str() const;
        string getName();
        void setName(string name);
        CharacterType getCharacterType() const;
        WatsonBag* getBag() const;

        bool meet(RobotC* robotc);
        bool meet(RobotS* robots);
        bool meet(RobotW* robotw);
        bool meet(RobotSW* robotsw);
        bool meet(Sherlock* sherlock);
        
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
        CharacterType getCharacterType() const;
};

class ArrayMovingObject {   
    private:
        MovingObject **arr_mv_objs;
        int capacity;
        int count;
    public:
        ArrayMovingObject(int capacity);
        ~ArrayMovingObject();
        bool isFull() const;
        bool checkMeet(int index) const;
        bool add(MovingObject * mv_obj);
        MovingObject * get(int index) const;
        int size() const; // return current number of elements in the array
        string str() const;
};

class Configuration {
    friend class StudyPinkProgram;
    friend class TestStudyInPink;
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
    friend class TestStudyInPink;
    private:
        RobotType robot_type;
        Criminal * criminal;
        BaseItem* item;

    public:
        Robot(int index, const Position & pos, Map * map, Criminal* criminal);
        Robot* create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
        virtual Position getNextPosition() = 0;
        virtual void move();
        virtual string str() const = 0;
        virtual int getDistance() const = 0;
        virtual RobotType getRobotType() const = 0;
        virtual void setRobotType(RobotType robot_type);
        void setItem(ItemType itemtype, Criminal* criminal);
        int two_into_one(int p);
        virtual BaseItem* getItem();
    };

class RobotC : public Robot {
    friend class TestStudyInPink;
    private:
        Criminal * criminal;
    public:
        RobotC(int index , const Position & pos , Map * map , Criminal * criminal) ;
        Position getNextPosition();
        void move();
        int getDistance(Sherlock* sherlock);
        int getDistance(Watson* watson);
        int getDistance() const;
        string str() const;
        RobotType getRobotType() const override;
        BaseItem* get();
};

class RobotS : public Robot {
    friend class TestStudyInPink;
    private:
        Sherlock * sherlock;
        Criminal * criminal;
    public:
        RobotS ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * Sherlock );
        Position getNextPosition();
        int getDistance() const;
        void move();
        string str() const;
        RobotType getRobotType() const;
        
};

class RobotW : public Robot {
    friend class TestStudyInPink;
    private:
        Watson * watson;
        Criminal * criminal;
    public:
    RobotW ( int index , const Position & pos , Map * map , Criminal * criminal , Watson * watson );
    int getDistance() const;
    Position getNextPosition();
    string str() const;
    RobotType getRobotType() const;
   
};

class RobotSW : public Robot {
    friend class TestStudyInPink;
    private:
        Sherlock * sherlock;
        Watson * watson;
        Criminal * criminal;
    public:
        RobotSW ( int index , const Position & pos , Map * map , Criminal * criminal , Sherlock * sherlock , Watson* watson );
        int getDistance() const;
        Position getNextPosition();
        string str() const;
        RobotType getRobotType() const;
};

class BaseItem{
    private:
    ItemType item;
    public:
        virtual bool canUse ( Character * obj , Robot * robot ) = 0;
        virtual void use ( Character * obj , Robot * robot ) = 0;
        virtual ItemType getItemType() const = 0;
        virtual void setItemType(ItemType item);
};

class MagicBook : public BaseItem {
    public:
        bool canUse ( Character * obj , Robot * robot );
        void use ( Character * obj , Robot * robot );
        
        ItemType getItemType() const;
};

class EnergyDrink:public BaseItem {
    public:
        bool canUse ( Character * obj , Robot * robot ) override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
};

class FirstAid : public BaseItem{
    public:
        bool canUse ( Character * obj , Robot * robot )override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
};

class ExcemptionCard: public BaseItem{
    public:
        bool canUse ( Character * obj , Robot * robot ) override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
};

class PassingCard: public BaseItem{
    private:
        string challenge;
    public:
        PassingCard(int a,int b);
        PassingCard(string challenge);
        bool canUse ( Character * obj , Robot * robot ) override;
        void use ( Character * obj , Robot * robot ) override;
        ItemType getItemType() const;
        string getChallenge() const {
            return challenge;
        };
};

//BaseBag
class BaseBag{
    public:
        class Node{
            public:
                BaseItem* item;
                Node* next;
            public:
                Node(BaseItem* item,Node* next = nullptr);
        };
    protected:
        Node* head;
        Character* obj;
    public:
        virtual bool insert(BaseItem* item);
        virtual BaseItem* get() = 0;
        virtual BaseItem* get(ItemType itemtype);
        virtual string str() const;
        BaseBag(int capacity);
        virtual ~BaseBag();
        bool checkItem(ItemType itemtype);
        bool isFull() const;
        virtual int getCapacity() = 0;
        virtual void setCapacity(int capacity);
    protected:
        int size;
        int capacity;
};

class SherlockBag : public BaseBag{
    friend class TestStudyInPink;
    private:
        Sherlock* sherlock;
    public:
        SherlockBag(Sherlock* sherlock);
        virtual BaseItem* get();
        virtual BaseItem* get(ItemType itemtype);
        int getCapacity();
        void setCapacity(int capacity);
};

class WatsonBag : public BaseBag{
    friend class TestStudyInPink;
    private:
        Watson* watson;
    public:
        WatsonBag(Watson* watson);
        virtual BaseItem* get();
        virtual BaseItem* get(ItemType itemtype);
        int getCapacity();
        void setCapacity(int capacity);
};
class StudyPinkProgram {
    friend class TestStudyInPink;
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

        void printInfo(int si, int i, ofstream &OUT);
        void run(bool verbose,ofstream &OUTPUT);
            // Note: This is a sample code. You can change the implementation as you like.
            // TODO
        ~StudyPinkProgram();
};


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
