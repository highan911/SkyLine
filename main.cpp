#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
#include <string>
#include "Windows.h"
#include<time.h>


#include "Point.h"
#include "Unit.h"
#include "Node.h"
#include "SkyLine.h"
using namespace std;

class Unit;

class Point;

class Node;

class Skylines;

int MAX_LAYER = 2;


vector<Unit*> UWiseAddMethod(vector<Point*> AllPoint,int k ){
    
	vector<Unit*> resultsUnit;
    
    vector<Unit*> unitGroups;
    vector<Unit*> unitGroupsHighter;
	int totalCountUnitGroup = 0;
    //preprocessing     O(k)
    for(vector<Point*>::iterator it  = AllPoint.begin(); it != AllPoint.end(); it ++)  {
        
        if((*it)->Parents.size() == k - 1){
            /*cout<<" G-Skyline ";
			cout<< (*it)->name<<" ";
            for(vector<Point*>::iterator it1  = (*it)->Parents.begin(); it1 != (*it)->Parents.end(); it1 ++)  {
                
                cout<< (*it1)->name<<" ";
            }
            cout<<endl;*/
			resultsUnit.push_back(new Unit(*it));
			totalCountUnitGroup++;
        }else if((*it)->Parents.size() < k - 1){
            Unit* unitGroup = new Unit((*it));
            unitGroups.push_back(unitGroup);
        }
    }
	// sort unitGroup by count desc O(klgk)
	std::sort(unitGroups.begin(),unitGroups.end(),SortByCount);
    bool flag = true;
    unitGroupsHighter = unitGroups;
	int iteratorCount  = 0;
    while(flag){
        vector<Unit*> temp;
        for(vector<Unit*>::iterator it  = unitGroupsHighter.begin(); it != unitGroupsHighter.end(); it ++){

			Point* hostPoint = (*it)->Hosts.front();
			Unit* hostUnit = new Unit(hostPoint);
			vector<Unit*>::iterator it2  = unitGroups.begin();

			// O(k)
			for(; it2 != unitGroups.end(); it2 ++)  {

				if((*it2)->equal(hostUnit)){
					it2++;
					break;
				}
			}
			for(; it2 != unitGroups.end(); it2 ++)  {
                

                Unit* u1 = (*it);
                Unit* u2 = (*it2);
				if(!u1->PointInUnit(u2->Hosts)){
                    
					Unit* bigUnitGroup = new Unit(u1,u2);
                    
					if(bigUnitGroup->Count == k){
						//bigUnitGroup->printUnitGroup();
						totalCountUnitGroup++;
						resultsUnit.push_back(bigUnitGroup);
					}
					else if(bigUnitGroup->Count <k)
						temp.push_back(bigUnitGroup);
				}
            }
            
        }
		flag = !temp.empty();
        unitGroupsHighter = temp;
		iteratorCount  ++;
		//cout<<"temp size:"<<temp.size()<<" .iterator count"<< iteratorCount<<endl;
    }
	cout<<"total  G-Skyline :" <<totalCountUnitGroup<<endl;

	return resultsUnit;
}

vector<Point*> getPoints(int Dim ,string PATH){

	int SIZE = 10000;

	double** AllData = new double*[SIZE];
	
	vector<Point*> AllPoint;

	int names[11];
	names[0] = 1;
	names[1] = 6;
	names[2] = 3;
	names[3] = 11;
	names[4] = 8;
	names[5] = 2;
	names[6] = 5;
	names[7] = 10;
	names[8] = 9;
	names[9] = 4;
	names[10] = 7;

	ifstream f;
	f.open(PATH,ios::in);
	int index = 0;
	while (! f.eof() ) {
		AllData[index] = new double[Dim];
		for (int j = 0; j < Dim; j++)
		{
			f>>AllData[index][j];
		}
		index++;
	} 

	f.close();
	
	Skylines skylines = Skylines(Dim, MAX_LAYER);

	for (int i = 0; i < index; i++)
	{
		Point* point = new Point(i,AllData[i]);
		point->name = names[i];
		if(skylines.Insert(point)){
			AllPoint.push_back(point);
		}
	}

	return AllPoint;
}


vector<Point*> getPoints_T(int Dim ,string PATH){

	int SIZE = 10000;
	int names[11];
	names[0] = 1;
	names[1] = 6;
	names[2] = 3;
	names[3] = 11;
	names[4] = 8;
	names[5] = 2;
	names[6] = 5;
	names[7] = 10;
	names[8] = 9;
	names[9] = 4;
	names[10] = 7;
	double** AllData = new double*[SIZE];
	
	vector<Point*> AllPoint;

	ifstream f;
	f.open(PATH,ios::in);
	int index = 0;
	while (! f.eof() ) {
		AllData[index] = new double[Dim];
		for (int j = 0; j < Dim; j++)
		{
			f>>AllData[index][j];
		}
		index++;
	} 

	f.close();
	
	double coor1[] = {-500.0,-500.0,-5.0,-5.0,-5.0,-5.0,-5.0,-5.0};
	double coor2[] = {500.0,500.0,5.0,5.0,5.0,5.0,5.0,5.0};

	Skylines_T skylines = Skylines_T(Dim, MAX_LAYER, coor1, coor2);

	for (int i = 0; i < index; i++)
	{
		Point* point = new Point(i,AllData[i]);
		point->name = names[i];
		if(skylines.Insert(point)){
			AllPoint.push_back(point);
		}
	}

	return AllPoint;
}


vector<Point*> getPoints_R(int Dim ,string PATH){

	int SIZE = 10000;
	int names[11];
	names[0] = 1;
	names[1] = 6;
	names[2] = 3;
	names[3] = 11;
	names[4] = 8;
	names[5] = 2;
	names[6] = 5;
	names[7] = 10;
	names[8] = 9;
	names[9] = 4;
	names[10] = 7;
	double** AllData = new double*[SIZE];
	
	vector<Point*> AllPoint;

	ifstream f;
	f.open(PATH,ios::in);
	int index = 0;
	while (! f.eof() ) {
		AllData[index] = new double[Dim];
		for (int j = 0; j < Dim; j++)
		{
			f>>AllData[index][j];
		}
		index++;
	} 

	f.close();
	
	double coor1[] = {-500.0,-500.0,-5.0,-5.0,-5.0,-5.0,-5.0,-5.0};
	double coor2[] = {500.0,500.0,5.0,5.0,5.0,5.0,5.0,5.0};

	Skylines_R skylines = Skylines_R(Dim, MAX_LAYER, coor1, coor2);

	for (int i = 0; i < index; i++)
	{
		Point* point = new Point(i, AllData[i], Dim, coor1);
		point->name = names[i];
		if(skylines.Insert(point)){
			AllPoint.push_back(point);
		}
	}

	return AllPoint;
}
vector<Point*> getTestPoint(){

	
    Point* p1 = new Point();
    p1->ID = 1; 
	p1->name = 1;
    p1->Layer = 1;
    
    Point* p2 = new Point();
    p2->ID = 2; 
	p2->name = 6;
    p2->Layer = 1;
    
    Point* p3 = new Point();
    p3->ID = 3; 
	p3->name = 11;
    p3->Layer = 1;
    
    Point* p4 = new Point();
    p4->ID = 4; 
	p4->name = 3;
    p4->Layer = 2;
    p4->Parents.push_back(p2);
    
    Point* p5 = new Point();
    p5->ID = 5; 
    p5->Layer = 2;
	p5->name = 8;
    p5->Parents.push_back(p3);
    
    Point* p6 = new Point();
    p6->ID = 6; 
    p6->Layer = 2;
	p6->name = 10;
    p6->Parents.push_back(p3);
    
    Point* p7 = new Point();
    p7->ID = 7; 
	p7->name=2;
    p7->Layer = 3;
    p7->Parents.push_back(p4);
    p7->Parents.push_back(p2);
    p7->Parents.push_back(p5);
    p7->Parents.push_back(p3);
    
    Point* p8 = new Point();
    p8->ID = 8; 
    p8->Layer = 3;
	p8->name = 5;
    p8->Parents.push_back(p2);
    p8->Parents.push_back(p3);
    p8->Parents.push_back(p5);
    
    Point* p9 = new Point();
    p9->ID = 9; 
    p9->Layer = 3;
	p9->name = 9;
    p9->Parents.push_back(p6);
    p9->Parents.push_back(p3);
    
    Point* p10 = new Point();
    p10->ID = 10; 
    p10->Layer = 4;
	p10->name = 4;
    p10->Parents.push_back(p9);
    p10->Parents.push_back(p8);
    p10->Parents.push_back(p6);
    p10->Parents.push_back(p3);
    p10->Parents.push_back(p2);
    p10->Parents.push_back(p5);
    
    Point* p11 = new Point();
    p11->ID = 11; 
	p11->name = 7;
    p11->Layer = 4;
    p11->Parents.push_back(p9);
    p11->Parents.push_back(p6);
    p11->Parents.push_back(p3);
    p11->Parents.push_back(p5);
    
    vector<Point*> testPonts;
    testPonts.push_back(p1);
    testPonts.push_back(p2);
    testPonts.push_back(p3);
    testPonts.push_back(p4);
    testPonts.push_back(p5);
    testPonts.push_back(p6);
    testPonts.push_back(p7);
    testPonts.push_back(p8);
    testPonts.push_back(p9);
    testPonts.push_back(p10);
    testPonts.push_back(p11);

	return testPonts;
}

int main()
{

	string prePath = "../../datasets/";
	string path;
	int dim;

	//run the correct program
	dim = 2;
	MAX_LAYER = 4;
	cout<<"correctness checking"<<endl;
	path = prePath.append("test_2.txt");

	clock_t start,finish;
	double totalTime;
	start = clock();
	//构建SkyLine和DSG图
	vector<Point*> points = getPoints(dim,path);
	finish = clock();
	totalTime = (double)(finish-start)/1000.0;
	cout<<"the prgram has create the skyline."<<endl;
	cout<<"the points count is "<<points.size()<<endl;

	
	//使用UWise+方法
	vector<Unit*> unitGroup = UWiseAddMethod(points,MAX_LAYER);
	
	 for(vector<Unit*>::iterator it  = unitGroup.begin(); it != unitGroup.end(); it ++){

		 (*it)->printUnitGroup();
	 }
	 
	//通过多叉树构建SkyLine和DSG图
	
	points = getPoints_T(dim,path);
	
	totalTime = (double)(finish-start)/1000.0;
	cout<<"the prgram has create the skyline by the multi-tree method."<<endl;
	cout<<"the points count is "<<points.size()<<endl;
	 unitGroup = UWiseAddMethod(points,MAX_LAYER);

	 for(vector<Unit*>::iterator it  = unitGroup.begin(); it != unitGroup.end(); it ++){

		 (*it)->printUnitGroup();
	 }

	//通过R树构建SkyLine和DSG图
	points = getPoints_R(dim,path);
	totalTime = (double)(finish-start)/1000.0;
	cout<<"the prgram has create the skyline by the R-tree method."<<endl;
	cout<<"the points count is "<<points.size()<<endl;
	

	 unitGroup = UWiseAddMethod(points,MAX_LAYER);

	 for(vector<Unit*>::iterator it  = unitGroup.begin(); it != unitGroup.end(); it ++){

		 (*it)->printUnitGroup();
	 }
	
	
	// run the normal program

	cout<<"输入文件名:"<<endl;
	cin>>path;
	path = prePath.append(path);
	cout<<"输入维度:"<<endl;
	cin>>dim;
	cout<<"输入k:"<<endl;
	cin>>MAX_LAYER;


	start = clock();
	//构建SkyLine和DSG图
	points = getPoints(dim,path);
	finish = clock();
	totalTime = (double)(finish-start)/1000.0;
	cout<<"the prgram has create the skyline. the points count is "<<points.size()<<endl;
	cout<<" this method cost " << totalTime <<"s"<<endl;
	
	//通过多叉树构建SkyLine和DSG图
	start = clock();
	points = getPoints_T(dim,path);
	finish = clock();
	totalTime = (double)(finish-start)/1000.0;
	cout<<"the prgram has create the skyline by the multi-tree method."<<endl;
	cout<<"the points count is "<<points.size()<<endl;
	cout<<" this method cost " << totalTime <<"s"<<endl;

	//通过R树构建SkyLine和DSG图
	start = clock();
	points = getPoints_R(dim,path);
	finish = clock();
	totalTime = (double)(finish-start)/1000.0;
	cout<<"the prgram has create the skyline by the R-tree method."<<endl<<" the points count is "<<points.size()<<endl;
	cout<<" this method cost " << totalTime <<"s"<<endl;

	
	//使用UWise+方法
	UWiseAddMethod(points,MAX_LAYER);
	start = clock();
	totalTime = (double)(start-finish)/1000.0;
	cout<<" this method cost " << totalTime <<"s"<<endl;
    return 0;
    
}