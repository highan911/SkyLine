#ifndef SKYLINE_H
#define SKYLINE_H


#include"Node.h"
#include<vector>


using namespace std;
class Skylines{
public:
	int Dim;
	int MAX_LAYER;
	vector<vector<Point*>> Layers;

	Skylines(int dim, int max_layer){
		Dim=dim;
		MAX_LAYER = max_layer;
	}

	bool Insert(Point* point){
		if (Layers.size() > 0)
		{

			bool can = false;
			if(Dim==2){
				can = CanDominate_Two(Layers[Layers.size() - 1], point);
			}else{
				can = CanDominate(Layers[Layers.size() - 1], point);
			}

            if (can)
            {
				if(MAX_LAYER>0 && Layers.size()>=MAX_LAYER)return false;

				vector<Point*> layer = vector<Point*>();
				Layers.push_back(layer);
				point->Layer=Layers.size() - 1;
				Layers[point->Layer].push_back(point);

				Query_GetAllParents(point);

            }
            else
            {
                BinarySearchSkyline(0, Layers.size() - 1, point);
            }
        }
        else
        {
			vector<Point*> layer = vector<Point*>();
			Layers.push_back(layer);
			point->Layer=0;
			Layers[Layers.size() - 1].push_back(point);
        }
		return true;
    }

	void BinarySearchSkyline(int a, int b, Point* point){
        if (a == b)
        {
			point->Layer=a;
			Layers[a].push_back(point);

			Query_GetAllParents(point);

            return;
        }
        else
        {
            int i = (a + b) / 2;

			bool can = false;
			if(Dim==2){
				can = CanDominate_Two(Layers[i], point);
			}else{
				can = CanDominate(Layers[i], point);
			}

            if (can)
            {
                BinarySearchSkyline(i + 1, b, point);
            }
            else
            {
                BinarySearchSkyline(a, i, point);
            }
        }
    }

	bool CanDominate(vector<Point*> layer, Point* point){
		for (int i = 0; i < layer.size(); i++)
		{
			if(CanDominate(layer[i], point))return true;
		}
		return false;
	}

	bool CanDominate(Point* point1, Point* point2){
		for (int i = 0; i < Dim; i++)
		{
			if(point1->Data[i]>=point2->Data[i])return false;
		}
		return true;
	}

	bool CanDominate_Two(vector<Point*> layer, Point* point){
		if(layer[layer.size()-1]->Data[1] < point->Data[1])return true;
		return false;
	}


	void Query_GetAllParents(Point* point){
		for (int i = 0; i < point->Layer; i++)
		{
			Query_GetParents(Layers[i],point);
		}
	}

	void Query_GetParents(vector<Point*> layer, Point* point){
		for (int i = 0; i < layer.size(); i++)
		{
			if(CanDominate(layer[i],point)){
				point->Parents.push_back(layer[i]);
			}
		}
	}

};

#endif