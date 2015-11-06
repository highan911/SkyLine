#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
using namespace std;

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
    }
    
    
    
    bool PointInUnit(vector<Point*> points){
        
        
        bool exits = true;
        
        for(vector<Point*>::iterator it  = points.begin(); it != points.end(); it ++)  {
            
            if(exits)
                exits = std::find(Points.begin(), Points.end(), *it) != Points.end();
            
        }
        return exits;
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
    
    void Query_GetParents(double* coor1, Point* point){
        int cross=CrossEdge(coor1, point->Data);//0:out;1:cross;2:cover;
        
        if(cross==0)return;
        
        else if(cross==2)Query_InsertAllAsParents(point);
        
        else{
            if(Count>1){
                for (int i = 0; i < 1<<Dim; i++)
                {
                    if(SubNodes[i]!=NULL)SubNodes[i]->Query_GetParents(coor1, point);
                }
            }else if(Count==1){
                if(PointInArea(Data, coor1, point->Data)){
                    point->Parents.push_back(Data);
                    return;
                }else return;
            }else{
                return;
            }
        }
    }
    
    void Query_InsertAllAsParents(Point* point){
        if(Count==1){
            point->Parents.push_back(Data);
        }else if(Count>1){
            for (int i = 0; i < 1<<Dim; i++)
            {
                if(SubNodes[i]!=NULL)SubNodes[i]->Query_InsertAllAsParents(point);
            }
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
            if (CanDominate(Layers[Layers.size() - 1], point))
            {
                if(MAX_LAYER>0 && Layers.size()==MAX_LAYER)return false;
                
                Layers.push_back(Node(Dim, Coor1, Coor2));
                point->Layer = Layers.size() - 1;
                Layers[point->Layer].Insert(point);
                
                Layers[point->Layer].Query_GetParents(Coor1, point);
                
            }
            else
            {
                BinarySearchSkyline(0, Layers.size() - 1, point);
            }
        }
        else
        {
            Layers.push_back(Node(Dim, Coor1, Coor2));
            point->Layer = Layers.size() - 1;
            Layers[point->Layer].Insert(point);
        }
        return true;
    }
    
    void BinarySearchSkyline(int a, int b, Point* point){
        if (a == b)
        {
            point->Layer = a;
            Layers[a].Insert(point);
            
            if(a > 0){
                Layers[a-1].Query_GetParents(Coor1, point);
            }
            
            return;
        }
        else
        {
            int i = (a + b) / 2;
            
            if (CanDominate(Layers[i], point))
            {
                BinarySearchSkyline(i + 1, b, point);
            }
            else
            {
                BinarySearchSkyline(a, i, point);
            }
        }
    }
    
    bool CanDominate(Node layer, Point* data){
        return layer.Query_Bool(Coor1, data->Data);
    }
};



void UWiseAddMethod(vector<Point*> AllPoint,int k ){
    
    
    vector<Unit*> unitGroup;
    
    for(vector<Point*>::iterator it  = AllPoint.begin(); it != AllPoint.end(); it ++)  {
        
}

int main()
{
    int SIZE = 10000;
    int Dim = 8;
    string PATH = "D:/workspace_cpp/Skyline/datasets/corr_8.txt";
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
    
    UWiseAddMethod(AllPoint,MAX_LAYER);
    
    
    return 0;
}