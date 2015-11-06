#ifndef POINT_H
#define POINT_H

#include <vector>

using namespace std;

class Point{
public:
    int ID;						//Point index
	int name;					//Point real name
    double* Data;				//Point coordinate
    int Layer;					//Point layer
    vector<Point*> Parents;		//all parents of point
    
    Point(int id, double* data){
        ID = id;
        Data = data;
    }
    
    Point(){};
    
  
};

/***
**	desc: 方法用于对于vector中的Point根据ID进行降序排列
**	
*/

bool SortByIDDesc(  Point*v1,  Point *v2)
{
    return v1->ID > v2->ID;
}

/***
**	desc: 方法用于对于vector中的Point根据ID进行升序排列
**	
*/
bool SortByIDAsc(  Point*v1,  Point *v2)
{
    return v1->ID < v2->ID;
}

#endif