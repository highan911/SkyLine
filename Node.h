#ifndef NODE_H
#define NODE_H

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

#endif