#include "Board.h"
#include <QPainter>
#include <QMouseEvent>

#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col

Board::Board(QWidget *parent) : QWidget(parent)
{
    _r = 20;
    for(int i=0; i<32; ++i)
    {
        _s[i].init(i);
    }
    _selectId = -1;
    _bRedTurn = false;
}


void Board::paintEvent(QPaintEvent *)
{
    int d = 40;
    QPainter painter(this);
    //画10横线
    for(int i=1;i<=10;i++)    {
        painter.drawLine(QPoint(d,i*d), QPoint(9*d,i*d));
    }

    for(int i=1;i<=9;i++){
        if(i==1 || i ==9)
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        else{
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }

    //九宫格
    painter.drawLine(QPoint(4*d,1*d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,1*d),QPoint(4*d,3*d));

    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(6*d,8*d),QPoint(4*d,10*d));

    for(int i=0;i<32;i++){
        drawStone(painter,i);
    }
}

void Board::mouseReleaseEvent(QMouseEvent *qMouse)
{
    QPoint pt = qMouse->pos();//鼠标点位置
    int row, col,clickid;
    bool bRet = getRowCol(pt, row, col);
    if(bRet == false) // 点到棋盘外
        return;
    clickid = -1;
    for(int i=0;i<32;i++){
        if(!_s[i].IsDead() && _s[i]._col == col && _s[i]._row == row){
            clickid = i;
            break;
        }
    }
    if(_selectId == -1){
        if(clickid != -1 &&_bRedTurn == _s[clickid]._red){
            _selectId = clickid;
            update();
        }
    }else{
        if(canMove(_selectId,col,row,clickid)){
            _s[_selectId]._col = col;
            _s[_selectId]._row = row;
            if(clickid != -1)
                _s[clickid]._dead = true;

            setTurnFlag();
            _selectId = -1;
            update();
        }

    }


}

void Board::drawStone(QPainter &painter, int id)
{
    if(_s[id].IsDead())
        return;
    QPoint cPoint = center(id);
    QRect rect = QRect(cPoint.x()-2*_r, cPoint.y()-2*_r, _r*4, _r*4);
    painter.setPen(Qt::black);
    if(id == _selectId)
        painter.setBrush(Qt::lightGray);
    else
        painter.setBrush(Qt::yellow);
    painter.drawEllipse(cPoint,_r,_r);
    if(_s[id]._red)
        painter.setPen(Qt::red);
    painter.setFont(QFont("System",_r,20));
    painter.drawText(rect, _s[id].getStoneName(),QTextOption(Qt::AlignCenter));

}

QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col+1)* _r*2;
    ret.ry() = (row+1)* _r*2;
    return ret;
}

QPoint Board::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}

bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    for(row=0; row<=9; row++)
    {
        for(col=0; col<=8; col++)
        {
            QPoint c = center(row, col);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx*dx+dy*dy;
            if(dist < _r*_r)
                return true;
        }
    }
    return false;
}

void Board::setTurnFlag()
{
    _bRedTurn = !_bRedTurn;
    _selectId = -1;
}

bool Board::canMove(int moveid, int col, int row, int killid)
{
    if(killid != -1 && _s[moveid]._red == _s[killid]._red){
        _selectId = killid;
        update();
        return false;
    }
    switch (_s[moveid]._type) {
    case Stone::JIANG:
        return canMoveJiang(moveid,col,row,killid);
        break;
    case Stone::CHE:
        return canMoveChe(moveid,col,row,killid);
        break;
    case Stone::PAO:
        return canMovePao(moveid,col,row,killid);
        break;
    case Stone::MA:
        return canMoveMa(moveid,col,row,killid);
        break;
    case Stone::BING:
        return canMoveBing(moveid,col,row,killid);
        break;
    case Stone::SHI:
        return canMoveShi(moveid,col,row,killid);
        break;
    case Stone::XIANG:
        return canMoveXiang(moveid,col,row,killid);
        break;
    default:
        break;
    }
    return false;
}

bool Board::canMoveJiang(int moveid, int col, int row, int killid)
{
    //将
    if(_s[moveid]._red){
        if(row > 2)
            return false;
    }else{
        if(row < 7)
            return false;
    }
    if(col < 3) return false;
    if(col > 5) return false;
    GetRowCol(row1,col1,moveid);
    int rid =  relation(row1,col1,row,col);
    if(rid == 1 || rid == 10)
        return true;
    return false;
}

bool Board::canMoveChe(int moveid, int col, int row, int killid)
{
    //车
   int ret = getStoneCountAtLine(_s[moveid]._row,_s[moveid]._col,row,col);
   if( ret == 0)
       return true;
    return false;
}

bool Board::canMovePao(int moveid, int col, int row, int killid)
{
    //炮
    int ret = getStoneCountAtLine(_s[moveid]._row,_s[moveid]._col,row,col);
    if(killid == -1){
        if(ret == 0)
            return true;
    }
    else{
        if(ret == 1)
            return true;
    }
    return false;
}

bool Board::canMoveMa(int moveid, int col, int row, int killid)
{
    //马
    GetRowCol(row1,col1,moveid);
    int rnum = relation(row1,col1,row,col);
    if(rnum != 12 && rnum != 21)
        return false;
    if(rnum == 12){
        if(getStoneId(row1,(col1+col)/2) != -1)
            return false;
    }else {
        if(getStoneId((row1+row)/2,col1) != -1)
            return false;
    }

    return true;
}

bool Board::canMoveBing(int moveid, int col, int row, int killid)
{
    //兵
    GetRowCol(row1,col1, moveid);
    int ret = relation(row1, col1, row, col);
    if(ret != 1 && ret != 10)
        return false;
    if(_s[moveid]._red){
        if(row < row1)
            return false;
        if(row1 < 6 && ret != 10)
            return false;
    }else{
        if(row > row1)
            return false;
        if(row1 > 5 && ret != 10)
            return false;
    }
    return true;
}

bool Board::canMoveShi(int moveid, int col, int row, int killid)
{
    //士
    if(_s[moveid]._red){
        if(row > 2)
            return false;
    }else{
        if(row < 7)
            return false;
    }
    if(col < 3) return false;
    if(col > 5) return false;
    GetRowCol(row1, col1, moveid);
    int ret = relation(row1,col1, row, col);
    if( ret != 11 )
        return false;
    return true;
}

bool Board::canMoveXiang(int moveid, int col, int row, int killid)
{
    GetRowCol(row1, col1, moveid);
    int ret = relation(row1, col1, row, col);
    if(ret != 22)
        return false;
    if(_s[moveid]._red){
        if(row > 5)
            return false;
    }else{
        if(row < 6)
            return false;
    }
    if(getStoneId((row+row1)/2, (col+col1)/2) != -1)
        return false;
    return true;
}

int Board::relation(int row1, int col1, int row2, int col2)
{
    return (abs(row1-row2) *10 + abs(col1-col2));
}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
//        for(int col = min+1; col<max; ++col)
//        {
//            if(getStoneId(row1, col) != -1) ++ret;
//        }
        for(int i=0;i<32;i++){
            if(!_s[i].IsDead()){
                if(_s[i]._row == row1 && _s[i]._col > min && _s[i]._col < max)
                    ++ret;
            }
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
//        for(int row = min+1; row<max; ++row)
//        {
//            if(getStoneId(row, col1) != -1) ++ret;
//        }
        for(int i=0;i<32;i++){
            if(!_s[i].IsDead()){
                if(_s[i]._col == col1 && _s[i]._row > min && _s[i]._row < max)
                    ++ret;
            }
        }
    }

    return ret;
}

int Board::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(_s[i]._row == row && _s[i]._col == col && !_s[i].IsDead())
            return i;
    }
    return -1;
}
