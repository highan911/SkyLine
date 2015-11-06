#ifndef UNIT_H
#define UNIT_H

#include<vector>
#include "Point.h"
#include<algorithm>
using namespace std;



class Unit{
public:
    vector<Point*> Hosts;
    vector<Point*> Points;
    int Count;
    
    Unit(Point* host){
        Hosts.push_back(host);
        Points = host->Parents;
		Points.push_back(host);
        std::sort(Points.begin(),Points.end(),SortByIDDesc);
        Count = Points.size();
    }
    
    Unit(Unit* u1, Unit* u2){
        
        vector<Point*>::iterator it1  = u1->Points.begin();
        vector<Point*>::iterator it2  = u2->Points.begin();
        
		Hosts = u1->Hosts;
		Hosts.insert(Hosts.end(),u2->Hosts.begin(),u2->Hosts.end());
		std::sort(Hosts.begin(),Hosts.end(),SortByIDAsc);
        while(it1!= u1->Points.end() && it2 != u2->Points.end()){
            
            
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
        
        cout<<" print UnitGroup:";
        for(vector<Point*>::iterator it1  = Hosts.begin(); it1 != Hosts.end(); it1 ++)  {
            
            cout<<"Hosts name = "<<(*it1)->name<<" ";
        }
        cout<<endl;
    }

	bool equal(Unit *a)
    {
		bool flag = true;
		 flag = Count==a->Count;
		 if(flag){
		 
			 flag = PointInUnit(a->Points);
		 }

		 return flag;
    }
};


bool SortByCount(  Unit*v1,  Unit *v2)
{
	bool flag = true;
	if(v1->Count == v2->Count)
	{
		Point* point1 = v1->Hosts.front();
		Point* point2 = v2->Hosts.front();
		flag = point1->ID>point2->ID;
	}else{
	 
		flag = v1->Count > v2->Count;
	}
	return flag;
}
#endif
    