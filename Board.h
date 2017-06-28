#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "Stone.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
private:
    void drawStone(QPainter& painter, int id);

    /* 返回象棋棋盘行列对应的像素坐标 */
    QPoint center(int row, int col);
    QPoint center(int id);
    bool getRowCol(QPoint pt, int& row, int& col);

    void setTurnFlag();
    bool canMove(int moveid, int col, int row, int killid);
    bool canMoveJiang(int moveid, int col, int row, int killid);
    bool canMoveChe(int moveid, int col, int row, int killid);
    bool canMovePao(int moveid, int col, int row, int killid);
    bool canMoveMa(int moveid, int col, int row, int killid);
    bool canMoveBing(int moveid, int col, int row, int killid);
    bool canMoveShi(int moveid, int col, int row, int killid);
    bool canMoveXiang(int moveid, int col, int row, int killid);

    int relation(int row1,int col1,int row2,int col2);
    int getStoneCountAtLine(int row1, int col1, int row2, int col2);
    int getStoneId(int row, int col);
private:
    Stone _s[32];
    int _r; /* 棋子的半径 */
    int _selectId; //当前选中的棋子id
    bool _bRedTurn;//是否轮到红方
};


#endif // BOARD_H
