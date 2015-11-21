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
	double ManhattanDist;
    
    Point(int id, double* data){
        ID = id;
		name = id;
        Data = data;
    }
	Point(int id, double* data, int dim, double* coor0){
		ID = id;
		Data = data;
		ManhattanDist = GetManhattanDist(dim, coor0, data);
	}

	static double GetManhattanDist(int dim, double* coor1, double* coor2){
		double dist = 0;
		for (int i = 0; i < dim; i++)
		{
			dist += fabs(coor2[i]-coor1[i]);
		}
		return dist;
	}

	static double* CopyCoor(int dim, double* coor){
		double* newCoor = new double[dim];
		for (int i = 0; i < dim; i++)
		{
			newCoor[i]=coor[i];
		}
		return newCoor;
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