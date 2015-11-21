#ifndef NODE_H
#define NODE_H

#define infMax 2000.0
#define infMin -2000.0

class Node{
public:
    int Dim;
    double* Coor1;
    double* Coor2;
    double* CoorM;
    Node** SubNodes;
    Point* Data;
    int Count;
    ~Node(){
		delete SubNodes;
		delete Data;
	}
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

class Node_T{
public:
	int Dim;
	double* Coor1;
	double* Coor2;
	double* CoorM;
	Node_T** SubNodes;
	Point* Data;
	int Count;

	Node_T(int dim, double* coor1, double* coor2){
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
		SubNodes = new Node_T*[1<<Dim];
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
			SubNodes[ind] = new Node_T(Dim, coor1, coor2);
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



class Node_R{
public:
	static const unsigned int Kmin=4;
	static const unsigned int Kmax=2*Kmin;
	int Dim;
	bool Root;
	bool Leaf;
	double* Coor1;
	double* Coor2;
	double* Coor0;
	double MidManhattanDist;

	Node_R* Parent;
	vector<Node_R*> SubNodes;
	vector<Point*> Points;
	int Count;

	bool RecHasSet;

	Node_R(int dim, Node_R* parent, bool leaf, double* coor0){
		Dim=dim;
		Leaf=leaf;
		Count=0;
		Parent=parent;
		RecHasSet=false;

		if(parent==NULL){
			Root=true;
			Coor1 = Point::CopyCoor(dim, coor0);
			Coor2 = Point::CopyCoor(dim, coor0);
		}else{
			Root=false;
			Coor1 = Point::CopyCoor(dim, parent->Coor1);
			Coor2 = Point::CopyCoor(dim, parent->Coor2);
		}
		
		Coor0=coor0;

	}


	void Insert(Point* point){
		Count+=1;
		if(Leaf){
			Points.push_back(point);
			if(Points.size()>=Kmax){
				Split();
			}else{
				GetRect_OnePoint(point);
			}
		}else{
			Node_R* node = ChooseNode(point);
			node->Insert(point);
			if(SubNodes.size()>=Kmax){
				Split();
			}else{
				GetRect_OneSubNode(node);
			}
		}
	}

	Node_R* ChooseNode(Point* point){
		Node_R* node;
		double mindist = infMax;
		for (int i = 0; i < SubNodes.size(); i++)
		{
			double dist = Point::GetManhattanDist(Dim, point->Data, SubNodes[i]->Coor1) 
				+ Point::GetManhattanDist(Dim, point->Data, SubNodes[i]->Coor2);
			if(dist<mindist){
				mindist=dist;
				node = SubNodes[i];
			}
		}
		return node;
	}

	void Split(){
		Node_R* right;
		if(Leaf){
			if(Root){
				Root=false;
				Parent=new Node_R(Dim, NULL, false, Coor0);
				right = new Node_R(Dim,Parent, true, Coor0);
				SplitPoints(this, right);
				Parent->SubNodes.push_back(this);
				Parent->SubNodes.push_back(right);
				Parent->Count=this->Count + right->Count;
			}else{
				right = new Node_R(Dim,Parent,true, Coor0);
				SplitPoints(this,right);
				Parent->SubNodes.push_back(right);
			}
		}else{
			if(Root){
				Root=false;
				Parent=new Node_R(Dim, NULL, false, Coor0);
				right = new Node_R(Dim,Parent, false, Coor0);
				SplitSubNodes(this,right);
				Parent->SubNodes.push_back(this);
				Parent->SubNodes.push_back(right);
				Parent->Count=this->Count + right->Count;
			}else{
				right = new Node_R(Dim,Parent, false, Coor0);
				SplitSubNodes(this,right);
				Parent->SubNodes.push_back(right);
			}
		}
		GetRect_Split();
		right->GetRect_Split();
		Parent->GetRect_Split();
	}

	void SplitPoints(Node_R* left, Node_R* right){
		left->SortPoints();
		for (int i = left->Points.size(); i > Kmin; i--)
		{
			right->Points.push_back(left->Points[left->Points.size()-1]);
			left->Points.pop_back();
			left->Count-=1;
			right->Count+=1;
		}
	}

	void SortPoints(){
		for(int i = 1; i < Points.size(); i++)
		{
			Point* p = Points[i];
			int j;
			for(j = i; j>0; j--)
			{
				if(Points[j-1]->ManhattanDist > p->ManhattanDist){
					Points[j]=Points[j-1];
				}else{
					break;
				}
			}
			Points[j]=p;
		}
	}

	void SplitSubNodes(Node_R* left, Node_R* right){
		left->SortSubNodes();
		for (int i = left->SubNodes.size(); i > Kmin; i--)
		{
			Node_R* node = left->SubNodes[left->SubNodes.size()-1];
			right->SubNodes.push_back(node);
			left->SubNodes.pop_back();
			left->Count-=node->Count;
			right->Count+=node->Count;
			node->Parent = right;
		}
	}

	void SortSubNodes(){
		for(int i = 1; i < SubNodes.size(); i++)
		{
			Node_R* p = SubNodes[i];
			int j;
			for(j = i; j>0; j--)
			{
				if(SubNodes[j-1]->MidManhattanDist > p->MidManhattanDist){
					SubNodes[j]=SubNodes[j-1];
				}else{
					break;
				}
			}
			SubNodes[j]=p;
		}
	}

	void GetRect_Split(){
		if(Leaf){
			if(Count==0){
				return;
			}
			else if(Count==1){
				for (int i = 0; i < Dim; i++)
				{
					Coor1[i]=Points[0]->Data[i];
					Coor2[i]=Points[0]->Data[i];
				}
			}
			else
			{
				for (int i = 0; i < Dim; i++)
				{
					Coor1[i]=Points[0]->Data[i];
					Coor2[i]=Points[0]->Data[i];
				}
				for (int i = 0; i < Points.size(); i++)
				{
					for (int j = 0; j < Dim; j++)
					{
						if(Points[i]->Data[j] < Coor1[j])Coor1[j]=Points[i]->Data[j];
						if(Points[i]->Data[j] > Coor2[j])Coor2[j]=Points[i]->Data[j];
					}
				}
			}
		}else{
			for (int i = 0; i < Dim; i++)
			{
				Coor1[i]=SubNodes[0]->Coor1[i];
				Coor2[i]=SubNodes[0]->Coor2[i];
			}
			for (int i = 0; i < SubNodes.size(); i++)
			{
				for (int j = 0; j < Dim; j++)
				{
					if(SubNodes[i]->Coor1[j] < Coor1[j])Coor1[j]=SubNodes[i]->Coor1[j];
					if(SubNodes[i]->Coor2[j] > Coor2[j])Coor2[j]=SubNodes[i]->Coor2[j];
				}
			}
		}
		RecHasSet=true;
		MidManhattanDist = Point::GetManhattanDist(Dim, Coor0, Coor1) 
			+ Point::GetManhattanDist(Dim, Coor0, Coor2);
	}

	void GetRect_OnePoint(Point* point){
		if(Count==1){
			for (int i = 0; i < Dim; i++)
			{
				Coor1[i]=point->Data[i];
				Coor2[i]=point->Data[i];
			}
		}else{
			for (int i = 0; i < Dim; i++)
			{
				if(point->Data[i] < Coor1[i])Coor1[i]=point->Data[i];
				if(point->Data[i] > Coor2[i])Coor2[i]=point->Data[i];
			}
		}
		RecHasSet=true;
		MidManhattanDist = Point::GetManhattanDist(Dim, Coor0, Coor1) 
			+ Point::GetManhattanDist(Dim, Coor0, Coor2);
	}

	void GetRect_OneSubNode(Node_R* node){
		if(!RecHasSet){
			for (int i = 0; i < Dim; i++)
			{
				Coor1[i]=node->Coor1[i];
				Coor2[i]=node->Coor2[i];
			}
			RecHasSet=true;
		}else{
			for (int i = 0; i < Dim; i++)
			{
				if(node->Coor1[i] < Coor1[i])Coor1[i]=node->Coor1[i];
				if(node->Coor2[i] > Coor2[i])Coor2[i]=node->Coor2[i];
			}
		}
		MidManhattanDist = Point::GetManhattanDist(Dim, Coor0, Coor1) 
			+ Point::GetManhattanDist(Dim, Coor0, Coor2);
	}



	bool Query_Bool(double* coor1, Point* point){
		int cross=CrossEdge(coor1, point->Data);//0:out;1:cross;2:cover;

		if(cross==0)return false;

		else if(cross==2){
			if(Count>0)return true;
			else return false;
		}

		else{
			if(Leaf==false){
				bool found = false;
				for (int i = 0; i < SubNodes.size(); i++)
				{
					if(SubNodes[i]->Query_Bool(coor1, point)){
						found = true;
						break;
					}
				}
				return found;
			}else{
				bool found = false;
				for (int i = 0; i < Points.size(); i++)
				{
					if(PointInArea(Points[i], coor1, point->Data)){
						found = true;
						break;
					}
				}
				return found;
			}
		}
	}

	bool Query_GetParents(double* coor1, Point* point){
		int cross=CrossEdge(coor1, point->Data);//0:out;1:cross;2:cover;

		if(cross==0)return false;

		else if(cross==2) return Query_InsertAllAsParents(point);

		else{
			if(Leaf==false){
				bool found = false;
				for (int i = 0; i < SubNodes.size(); i++)
				{
					if(SubNodes[i]->Query_GetParents(coor1, point))found = true;
				}
				return found;
			}else{
				bool found = false;
				for (int i = 0; i < Points.size(); i++)
				{
					if(PointInArea(Points[i], coor1, point->Data)){
						point->Parents.push_back(Points[i]);
						found = true;
					}
				}
				return found;
			}
		}
	}

	bool Query_InsertAllAsParents(Point* point){
		if(Count==0)return false;
		if(Leaf==false){
			for (int i = 0; i < SubNodes.size(); i++)
			{
				SubNodes[i]->Query_InsertAllAsParents(point);
			}
		}else{
			for (int i = 0; i < Points.size(); i++)
			{
				point->Parents.push_back(Points[i]);
			}
		}
		return true;
	}

	

	int CrossEdge(double* coor1, double* coor2){//0:out;1:cross;2:cover;
		bool out;

		if(Count==0)return 0;

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