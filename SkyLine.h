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

class Skylines_T{
public:
	int Dim;
	int MAX_LAYER;
	double* Coor1;
	double* Coor2;
	vector<Node_T> Layers;

	Skylines_T(int dim, int max_layer, double* coor1, double* coor2){
		Dim=dim;
		MAX_LAYER = max_layer;
		Coor1=coor1;
		Coor2=coor2;
	}

	bool Insert(Point* point){
		if (Layers.size() > 0)
		{
			if(MAX_LAYER>0 && Layers.size()>=MAX_LAYER){
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

		Layers.push_back(Node_T(Dim, Coor1, Coor2));
		point->Layer = Layers.size() - 1;
		Layers[point->Layer].Insert(point);

		return true;
	}

	bool CanDominate(Node_T layer, Point* data){
		return layer.Query_Bool(Coor1, data->Data);
	}

};

class Skylines_R{
public:
	int Dim;
	int MAX_LAYER;
	double* Coor1;
	double* Coor2;
	vector<Node_R*> Layers;

	Skylines_R(int dim, int max_layer, double* coor1, double* coor2){
		Dim=dim;
		MAX_LAYER = max_layer;
		Coor1=coor1;
		Coor2=coor2;
	}

	bool Insert(Point* point){

		if (Layers.size() > 0)
		{
			
			if(MAX_LAYER>0 && Layers.size()>=MAX_LAYER){

				if(Layers[MAX_LAYER - 1]->Root==false){
					Layers[MAX_LAYER - 1] = Layers[MAX_LAYER - 1]->Parent;
				}

				if(CanDominate(Layers[MAX_LAYER - 1], point)) {
					return false;
				}
			}

			for (int i = 0; i < Layers.size(); i++)
			{
				if(Layers[i]->Root==false){
					Layers[i] = Layers[i]->Parent;
				}

				bool found = Layers[i]->Query_GetParents(Coor1, point);
				if(!found){
					point->Layer = i;
					Layers[i]->Insert(point);
					return true;
				}
			}
		}


		Layers.push_back(new Node_R(Dim, NULL, true, Coor1));
		point->Layer = Layers.size() - 1;
		Layers[point->Layer]->Insert(point);

		return true;
	}

	bool CanDominate(Node_R* layer, Point* data){
		return layer->Query_Bool(Coor1, data);
	}

};


#endif