#ifndef STONE_H
#define STONE_H

#include <QString>

class Stone
{
public:
    Stone();
    ~Stone();

    void init(int id);
    QString getStoneName();

    bool IsDead();

    friend class Board;

private:
   enum TYPE{JIANG, CHE, PAO, MA, BING, SHI, XIANG};

   int _row;
   int _col;
   TYPE _type;


   int _id;
   bool _dead;
   bool _red;


};

#endif // STONE_H
