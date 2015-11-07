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
		name = id;
        Data = data;
    }
    
    Point(){};
    
	~Point(){
	
		delete Data;
	}
};


/***
**	desc: �������ڶ���vector�е�Point����ID���н�������
**	
*/

bool SortByIDDesc(  Point*v1,  Point *v2)
{
    return v1->ID > v2->ID;
}

/***
**	desc: �������ڶ���vector�е�Point����ID������������
**	
*/
bool SortByIDAsc(  Point*v1,  Point *v2)
{
    return v1->ID < v2->ID;
}

#endif