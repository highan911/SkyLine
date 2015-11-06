#ifndef SKYLINE_H
#define SKYLINE_H


#include"Node.h"
#include<vector>


using namespace std;
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

#endif