#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
using namespace std;

class Unit;

class Point{
public:
    int ID;
    double* Data;
    int Layer;
    vector<Point*> Parents;
    
    Point(int id, double* data){
        ID = id;
        Data = data;
    }
    
    Point(){};
    
    bool operator==(Point &a)
    {
        return ID==a.ID;
    }
};

bool SortByID(  Point*v1,  Point *v2)
{
    return v1->ID > v2->ID;
}

class Unit{
public:
    vector<Point*> Hosts;
    vector<Point*> Points;
    int Count;
    
    Unit(Point* host){
        Hosts.push_back(host);
        Points = host->Parents;
        std::sort(Points.begin(),Points.end(),SortByID);
        Count = Points.size();
    }
    
    Unit(Unit* u1, Unit* u2){//u1<=u2
        
        vector<Point*>::iterator it1  = u1->Points.begin();
        vector<Point*>::iterator it2  = u2->Points.begin();
        
        while(it1!= u1->Points.end() && it1 != u2->Points.end()){
            
            
            if((*it1)->ID != (*it2)->ID){
                if((*it1)->ID > (*it2)->ID){
                    Points.push_back(*it1);
                    it1++;
                }else{
                    Points.push_back(*it2);
                    it2++;
                }
            }else{
                
                Points.push_back(*it1);
                it1++;
                it2++;
            }
        }
        
        while(it1!= u1->Points.end()){
            
            Points.push_back(*it1);
            it1++;
        }
        while(it2!= u2->Points.end()){
            
            Points.push_back(*it2);
            it2++;
        }
        
        Count = Points.size();
    }
    
    
    
    bool PointInUnit(vector<Point*> points){
        
        
        bool exits = true;
        
        for(vector<Point*>::iterator it  = points.begin(); it != points.end(); it ++)  {
            
            if(exits)
                exits = std::find(Points.begin(), Points.end(), *it) != Points.end();
            
        }
        return exits;
    }
    
    void printUnitGroup(){
        
        cout<<" print UnitGroup £∫";
        for(vector<Point*>::iterator it1  = Points.begin(); it1 != Points.end(); it1 ++)  {
            
            cout<< (*it1)->ID<<" ";
        }
        cout<<endl;
    }
};



class Node{
public:
    int Dim;
    double* Coor1;
    double* Coor2;
    double* CoorM;
    Node** SubNodes;
    Point* Data;
    int Count;
    
    Node(int dim, double* coor1, double* coor2){
        Dim=dim;
        Coor1=coor1;
        Coor2=coor2;
        Count=0;
    }
    
    int GetSubNode(bool* judge){
        int ind=0;
        for (int i = 0; i < Dim; i++)
        {
            ind+=(1<<i) * judge[i];
        }
        return ind;
    }
    
    void GetJudge(int ind, bool* judge){
        for (int i = 0; i < Dim; i++)
        {
            judge[i]=ind & 1;
            ind=ind>>1;
        }
    }
    
    void Split(){
        CoorM=new double[Dim];
        for (int i = 0; i < Dim; i++)
        {
            CoorM[i]=(Coor1[i]+Coor2[i])/2;
        }
        SubNodes = new Node*[1<<Dim];
        for (int i = 0; i < 1<<Dim; i++)
        {
            SubNodes[i]=NULL;
        }
    }
    
    void Insert(Point* point){
        switch(Count){
            case 0:
                Data=point;
                break;
                
            case 1:
                Split();
                Dispatch(Data);
                Dispatch(point);
                Data=NULL;
                break;
                
            default://>=2
                Dispatch(point);
                break;
        }
        Count++;
    }
    
    void Dispatch(Point* point){
        bool* judge = new bool[Dim];
        for (int i = 0; i < Dim; i++)
        {
            judge[i] = CoorM[i] <= point->Data[i];
        }
        int ind = GetSubNode(judge);
        
        if(SubNodes[ind]==NULL){
            double* coor1=new double[Dim];
            double* coor2=new double[Dim];
            for (int j = 0; j < Dim; j++)
            {
                if(judge[j]){
                    coor1[j]=CoorM[j];
                    coor2[j]=Coor2[j];
                }else{
                    coor1[j]=Coor1[j];
                    coor2[j]=CoorM[j];
                }
            }
            SubNodes[ind] = new Node(Dim, coor1, coor2);
        }
        SubNodes[ind]->Insert(point);
    }
    
    int CrossEdge(double* coor1, double* coor2){//0:out;1:cross;2:cover;
        bool out;
        for (int i = 0; i < Dim; i++)
        {
            out=coor2[i]<=Coor1[i] || Coor2[i]<coor1[i];
            if(out)return 0;
        }
        
        bool cover;
        for (int i = 0; i < Dim; i++)
        {
            cover=coor1[i]<=Coor1[i] && Coor2[i]<coor2[i];
            if(!cover)break;
        }
        if(cover)return 2;
        
        
        return 1;
    }
    
    bool Query_GetParents(double* coor1, Point* point){
        int cross=CrossEdge(coor1, point->Data);//0:out;1:cross;2:cover;
        
        if(cross==0)return false;
        
        else if(cross==2) return Query_InsertAllAsParents(point);
        
        else{
            if(Count>1){
                bool found = false;
                for (int i = 0; i < 1<<Dim; i++)
                {
                    if(SubNodes[i]!=NULL){
                        if(SubNodes[i]->Query_GetParents(coor1, point))found = true;
                    }
                }
                return found;
            }
            else if(Count==1)
            {
                if(PointInArea(Data, coor1, point->Data)){
                    point->Parents.push_back(Data);
                    return true;
                }
                else return false;
            }else{
                return false;
            }
        }
    }
    
    bool Query_InsertAllAsParents(Point* point){
        if(Count==1){
            point->Parents.push_back(Data);
            return true;
        }
        else if(Count>1)
        {
            bool found = false;
            for (int i = 0; i < 1<<Dim; i++)
            {
                if(SubNodes[i]!=NULL){
                    if(SubNodes[i]->Query_InsertAllAsParents(point))found = true;
                }
            }
            return found;
        }
        else
        {
            return false;
        }
    }
    
    bool Query_Bool(double* coor1, double* coor2){
        int cross=CrossEdge(coor1, coor2);//0:out;1:cross;2:cover;
        
        if(cross==0)return false;
        
        else if(cross==2){
            if(Count>0)return true;
            else return false;
        }
        
        else{
            if(Count>1){
                bool found = false;
                for (int i = 0; i < 1<<Dim; i++)
                {
                    if(SubNodes[i]!=NULL)found = SubNodes[i]->Query_Bool(coor1, coor2);
                    if(found) break;
                }
                return found;
            }else if(Count==1){
                if(PointInArea(Data, coor1, coor2)){
                    return true;
                }else return false;
            }else{
                return false;
            }
        }
    }
    
    bool PointInArea(Point* point, double* coor1, double* coor2){
        bool in;
        for (int i = 0; i < Dim; i++)
        {
            in = coor1[i]<=point->Data[i] && point->Data[i]<coor2[i];
            if(!in) return false;
        }
        return true;
    }
    
};


class Skylines{
public:
    int Dim;
    int MAX_LAYER;
    double* Coor1;
    double* Coor2;
    vector<Node> Layers;
    
    Skylines(int dim, int max_layer, double* coor1, double* coor2){
        Dim=dim;
        MAX_LAYER = max_layer;
        Coor1=coor1;
        Coor2=coor2;
    }
    
    bool Insert(Point* point){
        if (Layers.size() > 0)
        {
            if(MAX_LAYER>0 && Layers.size()==MAX_LAYER){
                if(CanDominate(Layers[MAX_LAYER - 1], point)) return false;
            }
            
            for (int i = 0; i < Layers.size(); i++)
            {
                bool found = Layers[i].Query_GetParents(Coor1, point);
                if(!found){
                    point->Layer = i;
                    Layers[point->Layer].Insert(point);
                    return true;
                }
            }
            
        }
        
        Layers.push_back(Node(Dim, Coor1, Coor2));
        point->Layer = Layers.size() - 1;
        Layers[point->Layer].Insert(point);
        
        return true;
    }
    
    bool CanDominate(Node layer, Point* data){
        return layer.Query_Bool(Coor1, data->Data);
    }
    
};

void UWiseAddMethod(vector<Point*> AllPoint,int k ){
    
    
    vector<Unit*> unitGroups;
    vector<Unit*> unitGroupsHighter;
    
    for(vector<Point*>::iterator it  = AllPoint.begin(); it != AllPoint.end(); it ++)  {
        
        if((*it)->Parents.size() == k){
            cout<<" G-Skyline £∫";
            for(vector<Point*>::iterator it1  = (*it)->Parents.begin(); it1 != (*it)->Parents.end(); it1 ++)  {
                
                cout<< (*it1)->ID<<" ";
            }
            cout<<endl;
        }
        
        if((*it)->Parents.size() < k){
            Unit* unitGroup = new Unit((*it));
            unitGroups.push_back(unitGroup);
        }
    }
    bool flag = true;
    
    while(flag){
        vector<Unit*> temp;
        for(vector<Unit*>::iterator it  = unitGroups.begin(); it != unitGroups.end(); it ++){
            for(vector<Unit*>::iterator it2  = unitGroups.begin(); it2 != unitGroups.end(); it2 ++)  {
                
                Unit* u1 = (*it);
                Unit* u2 = (*it2);
                
                if(u1->Count + u2->Count >= k){
                    
                    
                    Unit* bigUnitGroup = new Unit(u1,u2);
                    
                    if(bigUnitGroup->Count == k)
                        bigUnitGroup->printUnitGroup();
                    else if(bigUnitGroup->Count <k)
                        temp.push_back(bigUnitGroup);
                }
            }
            
        }
        flag = temp.empty();
        unitGroupsHighter = temp;
    }
}

int main()
{
    /*
     int SIZE = 10000;
     int Dim = 8;
     string PATH = "F:\\school\\∂¡—–\\«Âª™\\—–“ª\\course\\œ÷¥˙ ˝æ›ø‚∏≈¬€\\ µ—È\\datasets\\test_8.txt";
     int MAX_LAYER = 2;
     
     double** AllData = new double*[SIZE];
     vector<Point*> AllPoint;
     
     
     ifstream f;
     f.open(PATH,ios::in);
     for (int i = 0; i < SIZE; i++)
     {
     AllData[i] = new double[Dim];
     for (int j = 0; j < Dim; j++)
     {
     f>>AllData[i][j];
     }
     }
     f.close();
     
     double coor1[] = {-500.0,-5.0,-5.0,-5.0,-5.0,-5.0,-5.0,-5.0};
     double coor2[] = {500.0,5.0,5.0,5.0,5.0,5.0,5.0,5.0};
     
     Skylines skylines = Skylines(Dim, MAX_LAYER, coor1, coor2);
     
     for (int i = 0; i < SIZE; i++)
     {
     Point* point = new Point(i,AllData[i]);
     if(skylines.Insert(point)){
     AllPoint.push_back(point);
     }
     }
     */
    Point* p1 = new Point();
    p1->ID = 1; 
    p1->Layer = 1;
    
    Point* p2 = new Point();
    p2->ID = 2; 
    p2->Layer = 1;
    
    Point* p3 = new Point();
    p3->ID = 3; 
    p3->Layer = 1;
    
    Point* p4 = new Point();
    p4->ID = 4; 
    p4->Layer = 2;
    p4->Parents.push_back(p2);
    
    Point* p5 = new Point();
    p5->ID = 5; 
    p5->Layer = 2;
    p5->Parents.push_back(p3);
    
    Point* p6 = new Point();
    p6->ID = 6; 
    p6->Layer = 2;
    p6->Parents.push_back(p3);
    
    Point* p7 = new Point();
    p7->ID = 7; 
    p7->Layer = 3;
    p7->Parents.push_back(p4);
    p7->Parents.push_back(p2);
    p7->Parents.push_back(p5);
    p7->Parents.push_back(p3);
    
    Point* p8 = new Point();
    p8->ID = 8; 
    p8->Layer = 3;
    p8->Parents.push_back(p2);
    p8->Parents.push_back(p3);
    p8->Parents.push_back(p5);
    
    Point* p9 = new Point();
    p9->ID = 9; 
    p9->Layer = 3;
    p9->Parents.push_back(p6);
    p9->Parents.push_back(p3);
    
    Point* p10 = new Point();
    p10->ID = 10; 
    p10->Layer = 4;
    p10->Parents.push_back(p9);
    p10->Parents.push_back(p8);
    p10->Parents.push_back(p6);
    p10->Parents.push_back(p3);
    p10->Parents.push_back(p2);
    p10->Parents.push_back(p5);
    
    Point* p11 = new Point();
    p11->ID = 11; 
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
    //UWiseAddMethod(testPonts,2);
    return 0;
    
}