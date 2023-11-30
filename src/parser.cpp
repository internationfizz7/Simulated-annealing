#include "parser.h"
#include <iostream>
#include <queue>
using namespace std;
NETLIST nt;
map<string, int> ModNameToID;
map<string, int> SigNameToID;
#define alpha_base 0.5
//------------------------constructor----------------//
Pin::Pin(){}
Pin::~Pin(){}

Module::Module(){}
Module::~Module(){}

Net::Net(){}
Net::~Net(){}

Contour::Contour(){}
Contour::~Contour(){}

Node::Node(){
	rotat = 0;
}
Node::~Node(){}

B_tree::B_tree(){}
B_tree::B_tree(B_tree&){}
B_tree::~B_tree(){}

NETLIST::NETLIST(){}
NETLIST::~NETLIST(){}

//----------------------------------------------------//

int readYal(char* filenam){
	string filename=filenam;
	double tmp_Area = 0;
	cout << "now Read Yal\n";
	ifstream fin;
	nt.name = filename;
	fin.open(filename);
	if (!fin)
	{
		cerr << "!error  : reader returns bad status in file " << filename << endl;
		exit(EXIT_FAILURE);	
	}
	else
	{
		//===============================start parse=================================//
		string tmpstr;// temporary string buffer
		while (fin>>tmpstr){
			if (tmpstr[0] == '/')
			{
				while(fin>>tmpstr)
				{
					//cout << "//" << tmpstr << endl;
					if(tmpstr[tmpstr.size()-1]=='/')
						break;
				}
			}// comment
			else if(tmpstr == "MODULE")
			{
				Module* thisMod = new Module;// create new module <=----------<<<<<<<
				fin >> tmpstr;
				thisMod->mod_name = tmpstr.substr(0, tmpstr.size()-1);
				fin >> tmpstr;//type;
				fin >> tmpstr;
				if(tmpstr == "GENERAL;"){
					thisMod->mod_type = tmpstr[0];
					int llx, lly, urx,ury;
					fin >> tmpstr;//dimensions;
					fin >> urx >> lly >> urx >> ury >> llx >> ury >> llx >> tmpstr;
					thisMod->width = urx- llx;
					thisMod->height = ury - lly;
					thisMod->area = thisMod->width*thisMod->height;
					tmp_Area += thisMod->area;
					fin >> tmpstr;//iolist;
					fin >> tmpstr;//pin_name;
					while(tmpstr != "ENDIOLIST;"){
						Pin* thisPin = new Pin;
						thisPin->pin_name = tmpstr;
						fin >> tmpstr;
						if(tmpstr == "PWR")
							thisPin->pin_type = 'V';
						else if(tmpstr == "GND")
							thisPin->pin_type = 'G';
						else if(tmpstr == "PB")
							thisPin->pin_type = 'B';
						else if(tmpstr == "PO")
							thisPin->pin_type = 'O';
						else if(tmpstr == "PI")
							thisPin->pin_type = 'I';
						else if(tmpstr == "B")
							thisPin->pin_type = 'b';
						else
							thisPin->pin_type = 'F';
						//pin_type;
						int x,y;
						fin >> x >> y;
						thisPin->x_shift = x - llx - 0.5*(thisMod->width);
						thisPin->y_shift = y - lly - 0.5*(thisMod->height);
						//shift_xy
						while(fin>>tmpstr){
							if(tmpstr[tmpstr.size()-1]==';')
								break;
						}
						//layer, metal, v/c....
						nt.pins.push_back(*thisPin);//<>=----------<<<<<<<<<<<<<<
						thisMod->mod_pins.push_back(nt.pins[nt.pins.size()-1]);
						fin>>tmpstr;
					}
					nt.mods.push_back(*thisMod);//<>=------------------<<<<<<<<<<<<<<
					int index =nt.mods.size()-1;
					nt.mods[index].index = index;
					ModNameToID.insert(pair<string, int>(thisMod->mod_name, index));
					delete thisMod;
					fin>>tmpstr;//ENDMODULE;
				}
				else if(tmpstr == "PARENT;"){
					thisMod->mod_type = tmpstr[0];
					int llx, lly, urx,ury;
					fin >> tmpstr;//dimensions;
					fin >> urx >> lly >> urx >> ury >> llx >> ury >> llx >> tmpstr;
					thisMod->width = urx- llx;
					thisMod->height = ury - lly;
					thisMod->area = thisMod->width*thisMod->height;
					fin >> tmpstr;//iolist;
					fin >> tmpstr;
					while(true){
						Pin* thisPin = new Pin;
						thisPin->pin_name = tmpstr;
						fin >> tmpstr;
						if(tmpstr == "PWR")
							thisPin->pin_type = 'V';
						else if(tmpstr == "GND")
							thisPin->pin_type = 'G';
						else if(tmpstr == "PB")
							thisPin->pin_type = 'B';
						else if(tmpstr == "PO")
							thisPin->pin_type = 'O';
						else if(tmpstr == "PI")
							thisPin->pin_type = 'I';
						else if(tmpstr == "B")
							thisPin->pin_type = 'b';
						else
							thisPin->pin_type = 'F';
						//pin_type;
						int x,y;
						fin >> x >> y;
						thisPin->x_shift = x - llx - 0.5*(thisMod->width);
						thisPin->y_shift = y - lly - 0.5*(thisMod->height);
						//shift_xy
						while(fin>>tmpstr){
							if(tmpstr[tmpstr.size()-1]==';')
								break;
						}
						//layer, metal, v/c....
						nt.pins.push_back(*thisPin);
						thisMod->mod_pins.push_back(nt.pins[nt.pins.size()-1]);
						fin>>tmpstr;
						if(tmpstr == "ENDIOLIST;")
							break;
					}
					fin >> tmpstr;//network;
					fin >> tmpstr;//instance name
					while(tmpstr != "ENDNETWORK;"){
						fin >> tmpstr;// mod name
						map<string, int>::iterator iter;
						iter = ModNameToID.find(tmpstr);
						//cout << tmpstr << endl;
						Module* thisSub = &nt.mods[iter->second];
						fin >> tmpstr;//sig name;
						bool endd = false;
						while(!endd){
							if(tmpstr.back() == ';'){
								//cout<<tmpstr;
								tmpstr = tmpstr.substr(0, tmpstr.size()-1);
								endd = true;
							}
							map<string, int>::iterator net_iter;
							net_iter = SigNameToID.find(tmpstr);
							Net *thisNet;
							if(net_iter != SigNameToID.end())
							{
								thisNet = &nt.nets[net_iter->second];
							}
							else{
								thisNet = new Net;
								thisNet->net_name = tmpstr;
								//cout <<" "<< tmpstr <<endl;
								nt.nets.push_back(*thisNet);
								int index = nt.nets.size() - 1;	
								SigNameToID.insert(pair<string, int>(tmpstr, index));
								thisNet = &nt.nets[index];		
							}
							thisNet->net_mods.push_back(thisSub);
							if(!endd)
								fin >> tmpstr;
						}
						fin>>tmpstr;//next instance name
					}
					nt.Bound = *thisMod;
					fin >> tmpstr; //endmodules;
				}//parent
			}//module
			else{
				//cout<<"x";
			}
		}
	}
	//initialize nt:
	nt.btree_current.nodes.resize(nt.mods.size());
	for(unsigned int i = 0; i<nt.mods.size(); i++ ){
		nt.btree_current.nodes[i].index = i;
		nt.btree_current.nodes[i].mod_ptr = &nt.mods[i]; 
	}
	nt.A_total = tmp_Area;
	nt.A_star = tmp_Area * (1 + nt.Gamma);
	nt.H_star = sqrt(nt.A_star*nt.R_star);
	nt.W_star = sqrt(nt.A_star/nt.R_star);
	cout<<"YAl read complete\n";
	//cout<< nt.mods[0].mod_name;
	cout << nt.mods.size() <<" "<<nt.nets.size()<<" "<< nt.A_total <<" "<< nt.A_star<<endl;
	return 0;
}
//============================================================================================
void NETLIST::initialize()
{
	srand( time(NULL) );
	H_star = sqrt(A_star*R_star);
	W_star = sqrt(A_star/R_star);
	btree_current.root = &btree_current.nodes[0];
	for(unsigned int i = 1; i < btree_current.nodes.size(); i++){
		Node* thisNode = &btree_current.nodes[i];
		Node* hole = btree_current.root;
		while(true){
			int dice = rand()%3;
			if(dice >= 1){
				if(hole->left_child == nullptr)
				{
					hole->left_child = thisNode;
					thisNode->parent = hole;
					break;

				}else{
					hole = hole->left_child;
				}
			}
			else{
				if(hole->right_child == nullptr)
				{
					hole->right_child = thisNode;
					thisNode->parent = hole;
					break;
				}else{
					hole = hole->right_child;
				}
			}
		}
	}//randomlize b star tree
	Packing(&btree_current); 
	cout<<"initial pack completed\n";
	wirelength = HPWL_approx(&btree_current);//wirelength
	btree_best.copy(&btree_current);
	//Packing(&btree_best);
	alpha = 0.5;
	beta = 0;
	best_cost = Cost_Estimate();
}
void NETLIST::initial(B_tree* to_re){
	//B_tree* to_re=new B_tree();
	//Node* new_node=new Node();
	to_re->nodes = btree_current.nodes;
	/*new_node->mod_ptr=& mods[0];
	new_node->index=0;*/
	to_re->root=&to_re->nodes[0];
	for(int i=1;i<to_re->nodes.size();i++){
		/*Node* newnode=new Node();
		newnode->mod_ptr=& mods[i];
		newnode->index=i;*/
		insert_node_init(&to_re->nodes[i], *to_re);

	}
	
	//return to_re;


}
Node* NETLIST::last_node(B_tree& btree){ //DFS to find last node
	queue<Node*> discovered;
	discovered.push(btree.root);
	while(discovered.empty()==0){
		Node* first=discovered.front();
		discovered.pop();
		if(first->left_child==nullptr ||(first->right_child==nullptr)){
			return first;
		}
		else{
			discovered.push(first->left_child);
			discovered.push(first->right_child);
		}
	}
}
void NETLIST::DFS(B_tree& btree){
	queue<Node*> discovered;
	discovered.push(btree.root);
	while(discovered.empty()==0){
		Node* first=discovered.front();
		cout<<first->index<<" ";
		discovered.pop();
		if(first->left_child!=nullptr ){
			discovered.push(first->left_child);
		}
		if(first->right_child!=nullptr ){
			discovered.push(first->right_child);
		}
	}
}
void NETLIST::insert_node_init(Node* be_inserted, B_tree& btree){
	Node* last=last_node(btree);
	if(last->left_child==nullptr){
		last->left_child=be_inserted;
		be_inserted->parent=last;
	}
	else if(last->right_child==nullptr){
		last->right_child=be_inserted;
		be_inserted->parent=last;
	}
	else{
		cout<<"insert wrong(a node with both notnull ptr)\n";
	}

}

int NETLIST::count_level(int num){
	int re=1;
	while(true){
		int compare=pow(2,re);
		int comparep=pow(2,re-1);
		if(num>=comparep && num<compare){
			return re-1;
		}
		else{
			re++;
		}
	}
}

void NETLIST::FP(char* agr){
	alpha=0.000000005;
	string inn=agr;
	if(inn=="ami33.yal"){
		alpha=0.000000005;
	}
	else if(inn=="ami49.yal"){
		alpha=0.000000005;
	}
	else if(inn=="apte.yal"){
		alpha=0.000000005;
	}
	double T=1000000;
	double count=100000;
	srand(time(NULL));
	int num_mod=mods.size();
	B_tree* btree=new B_tree();
	nt.initial(btree);
	nt.btree_current.copy(btree);
	DFS(btree_current);
	bool in=false;
	//for(int i=T;i>0;i--){
	while(true){
		//cout<<i<<endl;
		if(in==true){
			nt.btree_current.copy(btree);
			in=false;
		}
		int random=rand()%8;
		btree_new_current.copy(&btree_current);
		int n1=rand()%num_mod;
		int n2=rand()%num_mod;
		if(random>=0 && random<=1 && n1!=n2){
			//do move
			//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
		}
		else if(random>=2 && random<=5){
			//do swap
			//cout<<"swap"<<endl;
			btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
		}
		else{
			//cout<<"else"<<endl;
			btree_new_current.rotat(&btree_new_current.nodes[n1]);
		}
		//.
		//.
		//.
		//.
		Packing(&btree_current);
		Packing(&btree_new_current);
		CF();
		double score_dif=btree_new_current.score-btree_current.score;
		if(btree_new_current.score<btree_current.score)
			btree_current.copy(&btree_new_current);
		else{
			double prob=exp(-score_dif/T);
			double x=(double) rand() / (RAND_MAX+1.0);
			if(x<prob){
				btree_current.copy(&btree_new_current);
			}
		}
		/*DFS(btree_current);
		cout<<endl;
		DFS(btree_current);
		cout<<endl;*/
		if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
			cout<<"ACCOMPLISH!"<<endl;
			break;
		}
		count--;
		T*=0.99;
		if(count==0){
			T=1000000;
			count=100000;
			in=true;
			//break;
		}
		
	}
	btree_best.copy(&btree_current);
	cout<<endl;
	DFS(btree_current);
	cout<<endl;
	DFS(btree_best);
	Packing(&nt.btree_best);
}

void NETLIST::FP1(){ //alpha low to high
	double T=100000;
	double T_const=100000;
	srand(time(NULL));
	int num_mod=mods.size();
	B_tree* btree=new B_tree();
	nt.initial(btree);
	nt.btree_current.copy(btree);
	Packing(&btree_current);
	double ini_a=0.1*((double)pow(btree_current.R-R_star,2)/(double)btree_current.Area); //initial alpha
	alpha=ini_a;
	bool in=false;
	//for(int i=T;i>0;i--){
	while(true){
		//cout<<i<<endl;
		if(in==true){
			nt.btree_current.copy(btree);
			alpha=ini_a;
			in=false;
		}
		int random=rand()%10;
		btree_new_current.copy(&btree_current);
		int n1=rand()%num_mod;
		int n2=rand()%num_mod;
		if(random>=0 && random<=6 && n1!=n2){
			//do move
			//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
		}
		else if(random>=7 && random<=8){
			//do swap
			//cout<<"swap"<<endl;
			btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
		}
		else{
			//cout<<"else"<<endl;
			btree_new_current.rotat(&btree_new_current.nodes[n1]);
		}
		//.
		//.
		//.
		//.
		Packing(&btree_current);
		Packing(&btree_new_current);
		CF();
		double score_dif=btree_new_current.score-btree_current.score;
		if(btree_new_current.score<btree_current.score)
			btree_current.copy(&btree_new_current);
		else{
			double prob=exp(-score_dif/T);
			double x=(double) rand() / (RAND_MAX+1.0);
			if(x<prob){
				btree_current.copy(&btree_new_current);
			}
		}
		/*DFS(btree_current);
		cout<<endl;
		DFS(btree_current);
		cout<<endl;*/
		if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
			cout<<"ACCOMPLISH!"<<endl;
			break;
		}
		T--;
		alpha+=(double)ini_a/(double)T_const;
		if(T==0){
			T=100000;
			in=true;
		}
	}
	btree_best.copy(&btree_current);
	cout<<endl;
	DFS(btree_current);
	cout<<endl;
	DFS(btree_best);
	Packing(&nt.btree_best);
}

void NETLIST::FP2(){ //alpha low to high
	double T=100000;
	double T_const=100000;
	srand(time(NULL));
	int num_mod=mods.size();
	B_tree* btree=new B_tree();
	nt.initial(btree);
	nt.btree_current.copy(btree);
	Packing(&btree_current);
	double ini_a=10*((double)pow(btree_current.R-R_star,2)/(double)btree_current.Area); //initial alpha
	alpha=ini_a;
	bool in=false;
	//for(int i=T;i>0;i--){
	while(true){
		//cout<<i<<endl;
		if(in==true){
			nt.btree_current.copy(btree);
			alpha=ini_a;
			in=false;
		}
		int random=rand()%10;
		btree_new_current.copy(&btree_current);
		int n1=rand()%num_mod;
		int n2=rand()%num_mod;
		if(random>=0 && random<=6 && n1!=n2){
			//do move
			//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
		}
		else if(random>=7 && random<=8){
			//do swap
			//cout<<"swap"<<endl;
			btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
		}
		else{
			//cout<<"else"<<endl;
			btree_new_current.rotat(&btree_new_current.nodes[n1]);
		}
		//.
		//.
		//.
		//.
		Packing(&btree_current);
		Packing(&btree_new_current);
		CF();
		double score_dif=btree_new_current.score-btree_current.score;
		if(btree_new_current.score<btree_current.score)
			btree_current.copy(&btree_new_current);
		else{
			double prob=exp(-score_dif/T);
			double x=(double) rand() / (RAND_MAX+1.0);
			if(x<prob){
				btree_current.copy(&btree_new_current);
			}
		}
		/*DFS(btree_current);
		cout<<endl;
		DFS(btree_current);
		cout<<endl;*/
		if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
			cout<<"ACCOMPLISH!"<<endl;
			break;
		}
		T--;
		alpha-=(double)ini_a/(double)T_const;
		if(T==0){
			T=100000;
			in=true;
		}
	}
	btree_best.copy(&btree_current);
	cout<<endl;
	DFS(btree_current);
	cout<<endl;
	DFS(btree_best);
	Packing(&nt.btree_best);
}

void NETLIST::FP3(){//先用0.3G做各種operation的比例嘗試，選出最好的比例，然後使用他做FP2 FP3(交錯使用/一起使用)
	double origin_g=Gamma;
	double origin_h=H_star;
	double origin_w=W_star;
	double T=1000000;
	double T_c=1000000;
	Gamma=0.3;
	double n_a=A_total*(1+Gamma);
	H_star=sqrt(n_a*R_star);
	W_star=sqrt(n_a/R_star);
	int num_mod=mods.size();
	B_tree* btree=new B_tree();
	nt.initial(btree);
	nt.btree_current.copy(btree);
	Packing(&btree_current);
	double ini_a=((double)pow(btree_current.R-R_star,2)/(double)btree_current.Area); //initial alpha
	alpha=ini_a;
	int turn=5; ///增加數據量用這個變數;
	int hit[turn];
	int lab[turn][4]={{0,6,7,8},{7,8,0,6},{0,4,5,8},{5,8,0,4},{0,3,4,6}};
	int highest=-1;
	int hig=-1;
	srand(time(NULL));
	for(int i=0;i<turn;i++){
		//
		cout<<i<<endl;
		if(i==0){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=0 && random<=6 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=7 && random<=8){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
		}
		else if(i==1){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=7 && random<=8 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=0 && random<=6){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else if(i==2){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=0 && random<=4 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=5 && random<=8){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else if(i==3){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=5 && random<=8 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=0 && random<=4){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else if(i==4){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=0 && random<=3 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=4 && random<=6){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else{
				//
			}
			

		
	}
			cout<<"start actual FP:"<<hig<<endl;
			
			int a=lab[hig][0];
			int b=lab[hig][1];
			int c=lab[hig][2];
			int d=lab[hig][3];
			T=1000000;
			
			Gamma=origin_g;
			H_star=origin_h;
			W_star=origin_w;
			nt.btree_current.copy(btree);
			Packing(&btree_current);
			ini_a=10*((double)pow(btree_current.R-R_star,2)/(double)btree_current.Area); //initial alpha
			alpha=ini_a;
			bool in=false;
			//for(int i=T;i>0;i--){
			while(true){
				//cout<<i<<endl;
				if(in==true){
					nt.btree_current.copy(btree);
					alpha=ini_a;
					in=false;
				}
				int random=rand()%10;
				btree_new_current.copy(&btree_current);
				int n1=rand()%num_mod;
				int n2=rand()%num_mod;
				if(random>=a && random<=b && n1!=n2){
					//do move
					//cout<<"move"<<endl;
					int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
						btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
				}
				else if(random>=c && random<=d){
					//do swap
					//cout<<"swap"<<endl;
					btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
				}
				else{
					//cout<<"else"<<endl;
					btree_new_current.rotat(&btree_new_current.nodes[n1]);
				}
				//.
				//.
				//.
				//.
				Packing(&btree_current);
				Packing(&btree_new_current);
				CF();
				double score_dif=btree_new_current.score-btree_current.score;
				if(btree_new_current.score<btree_current.score)
					btree_current.copy(&btree_new_current);
				else{
					double prob=exp(-score_dif/T);
					double x=(double) rand() / (RAND_MAX+1.0);
					if(x<prob){
						btree_current.copy(&btree_new_current);
					}
				}
				/*DFS(btree_current);
				cout<<endl;
				DFS(btree_current);
				cout<<endl;*/
				if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
					cout<<"ACCOMPLISH!"<<endl;
					break;
				}
				T--;
				alpha-=(double)ini_a/(double)T_c;
				if(T==0){
					T=1000000;
					in=true;
				}
			}
			btree_best.copy(&btree_current);
			cout<<endl;
			DFS(btree_current);
			cout<<endl;
			DFS(btree_best);
			Packing(&nt.btree_best);
}

void NETLIST::FP4(){//先用0.3G做各種operation的比例嘗試，選出最好的比例，然後使用他做FP2 FP3(交錯使用/一起使用)
	double origin_g=Gamma;
	double origin_h=H_star;
	double origin_w=W_star;
	double T=1000000;
	double T_c=1000000;
	Gamma=0.3;
	double n_a=A_total*(1+Gamma);
	H_star=sqrt(n_a*R_star);
	W_star=sqrt(n_a/R_star);
	int num_mod=mods.size();
	B_tree* btree=new B_tree();
	nt.initial(btree);
	nt.btree_current.copy(btree);
	Packing(&btree_current);
	double ini_a=((double)pow(btree_current.R-R_star,2)/(double)btree_current.Area); //initial alpha
	alpha=ini_a;
	int turn=5; ///增加數據量用這個變數;
	int hit[turn];
	int lab[turn][4]={{0,6,7,8},{7,8,0,6},{0,4,5,8},{5,8,0,4},{0,3,4,6}};
	int highest=-1;
	int hig=-1;
	srand(time(NULL));
	for(int i=0;i<turn;i++){
		//
		cout<<i<<endl;
		if(i==0){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=0 && random<=6 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=7 && random<=8){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
		}
		else if(i==1){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=7 && random<=8 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=0 && random<=6){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else if(i==2){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=0 && random<=4 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=5 && random<=8){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else if(i==3){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=5 && random<=8 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=0 && random<=4){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else if(i==4){
				int h=0;
				bool restart=true;
				for(int j=0;j<1000000;j++){
					if(restart==true){
						nt.btree_current.copy(btree);
						T=T_c;
						restart=false;
					}
					int random=rand()%10;
					btree_new_current.copy(&btree_current);
					int n1=rand()%num_mod;
					int n2=rand()%num_mod;
					if(random>=0 && random<=3 && n1!=n2){
						//do move
						//cout<<"move"<<endl;
						int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
					}
					else if(random>=4 && random<=6){
						//do swap
						//cout<<"swap"<<endl;
						btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
					}
					else{
						//cout<<"else"<<endl;
						btree_new_current.rotat(&btree_new_current.nodes[n1]);
					}
					//.
					//.
					//.
					//.
					Packing(&btree_current);
					Packing(&btree_new_current);
					CF();
					double score_dif=btree_new_current.score-btree_current.score;
					if(btree_new_current.score<btree_current.score)
						btree_current.copy(&btree_new_current);
					else{
						double prob=exp(-score_dif/T);
						double x=(double) rand() / (RAND_MAX+1.0);
						if(x<prob){
							btree_current.copy(&btree_new_current);
						}
					}
					/*DFS(btree_current);
					cout<<endl;
					DFS(btree_current);
					cout<<endl;*/
					if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
						//cout<<"ACCOMPLISH!"<<endl;
						restart=true;
						h++;
					}
					T--;
					if(T==0){
						T=1000000;
					}
				}
				hit[i]=h;
				if(h>highest){
					highest=h;
					hig=i;
				}
			}
			else{
				//
			}
			

		
	}
			cout<<"start actual FP:"<<hig<<endl;
			
			int a=lab[hig][0];
			int b=lab[hig][1];
			int c=lab[hig][2];
			int d=lab[hig][3];
			T=1000000;
			int count=100000;
			Gamma=origin_g;
			H_star=origin_h;
			W_star=origin_w;
			nt.btree_current.copy(btree);
			Packing(&btree_current);
			//ini_a=10*((double)pow(btree_current.R-R_star,2)/(double)btree_current.Area); //initial alpha
			ini_a=0.1*((double)pow(btree_current.R-R_star,2)/(double)btree_current.Area); //initial alpha
			alpha=ini_a;
			bool in=false;
			//for(int i=T;i>0;i--){
			while(true){
				//cout<<i<<endl;
				if(in==true){
					nt.btree_current.copy(btree);
					alpha=ini_a;
					in=false;
				}
				int random=rand()%10;
				btree_new_current.copy(&btree_current);
				int n1=rand()%num_mod;
				int n2=rand()%num_mod;
				if(random>=a && random<=b && n1!=n2){
					//do move
					//cout<<"move"<<endl;
					int rr=rand()%2;
						if(rr==0){
							btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],false);
						}
						else
						btree_new_current.move(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2],true);
				}
				else if(random>=c && random<=d){
					//do swap
					//cout<<"swap"<<endl;
					btree_new_current.swap(&btree_new_current.nodes[n1],&btree_new_current.nodes[n2]);
				}
				else{
					//cout<<"else"<<endl;
					btree_new_current.rotat(&btree_new_current.nodes[n1]);
				}
				//.
				//.
				//.
				//.
				Packing(&btree_current);
				Packing(&btree_new_current);
				CF();
				double score_dif=btree_new_current.score-btree_current.score;
				if(btree_new_current.score<btree_current.score)
					btree_current.copy(&btree_new_current);
				else{
					double prob=exp(-score_dif/T);
					double x=(double) rand() / (RAND_MAX+1.0);
					if(x<prob){
						btree_current.copy(&btree_new_current);
					}
				}
				/*DFS(btree_current);
				cout<<endl;
				DFS(btree_current);
				cout<<endl;*/
				if((double)btree_current.rightmost<=W_star &&(double)btree_current.upmost<=H_star){
					cout<<"ACCOMPLISH!"<<endl;
					break;
				}
				T*=0.99;
				count--;
				//alpha-=(double)ini_a/(double)T_c;
				alpha+=(double)ini_a/(double)T_c;
				if(count==0){
					T=1000000;
					count=100000;
					in=true;
				}
			}
			btree_best.copy(&btree_current);
			cout<<endl;
			DFS(btree_current);
			cout<<endl;
			DFS(btree_best);
			Packing(&nt.btree_best);
}

void NETLIST::CF(){
	//cost function of b_tree_new_current and btree_current
	cal_cf(&btree_new_current);
	cal_cf(&btree_current);
}

void NETLIST::cal_cf(B_tree* btree){
	//v1 only area
	//btree->score=btree->Area;
	//v2 AREA and ASPECT RATIO
	btree->score=alpha*btree->Area+(1-alpha)*((double)pow((R_star-btree->R),2));
	//cout<<endl;
	//cout<<"alpha "<<alpha<<" 1-alpha "<<(1-alpha)<<" A "<<btree->Area<<" R-R* "<<(double)pow((R_star-btree->R),2)<<" R "<<btree->R<<endl;
}

void NETLIST::Fix_Outline_FloorPlan(){
	queue<int> que;
	que.push(0);
	B_tree* b_tmp = new B_tree;
	int n_feas = 0, nn = 20 ;
	T = 100000000;
	int n_prev = 0;
	bool feas = false;
	while(n_prev < 200)
	{
		b_tmp->copy(&btree_current);
		Perturb(b_tmp);
		Packing(b_tmp);
		double cost_new = Cost_Estimate();
		//--------------------Prob()-------------
		bool choose = 0;
		double cost_delta = cost_new - cost;
		if (cost_delta < 0)
			choose = 1;
		else
			choose = 0;
		//------------choose new floorplan----------------
		if(choose)
		{
			cost = cost_new;
			btree_current.copy(b_tmp);
			if(cost < best_cost)
			{
				best_cost = cost;
				btree_best.copy(&btree_current);
			}			
		}
		//------------------------------------------
		n_prev++;	
	}
	do{
		//cout << n << endl;
		b_tmp->copy(&btree_current);
		Perturb(b_tmp);
		Packing(b_tmp);
		double cost_new = Cost_Estimate();
		//--------------------Prob()-------------
		bool choose;
		double cost_delta = cost_new - cost;
		int prob;
		if (cost_delta<=0)
			prob = 10000;
		else
			prob = 10000*exp(-cost_delta/T);
		if(rand()%10000 < prob)
			choose = true;
		else 
			choose = false;
		//------------choose new floorplan----------------
		if(choose)
		{
			//cout<<"choose\n";
			cost = cost_new;
			btree_current.copy(b_tmp);
			if(cost < best_cost)
			{
				best_cost = cost;
				btree_best.copy(&btree_current);
			}			
		}
		n++;
		//-------------modify weight----------------	
		if(n >= nn)
		{
			if(H < H_star && W < W_star)
			{
				n_feas = n_feas + 1 - que.front();
				que.pop();
				que.push(1);
				feas = true;
			}
			else
			{
				n_feas = n_feas - que.front();
				que.pop();
				que.push(0);
				feas = 0;
			}
			alpha = alpha_base + (0.9 - alpha_base) * n_feas / nn;
		}
		else
		{
			if(H < H_star && W < W_star)
			{
				n_feas ++;
				que.push(1);
			}
			else
			{
				que.push(0);
			}

		}
		//--------------modify T--------------------
		T = 0.996*T;
		if(n==600)
			T = 50000;
		if(n%10000==9999)
			T = 10000;	
		//------------------------------------------

	}while(T > 0.00001||!feas);
	Packing(&btree_best);
	wirelength = HPWL_approx(&btree_best);
	return;
}
void NETLIST::Packing(B_tree* btr){
	//cout<<"start packing\n";
	//cout<<btr->root<<endl;
	/*H = 0;
	W = 0;*/
	btr->rightmost=0;
	btr->upmost=0;
	//-------------------------------
	Contour* ctr = C;
	while(ctr != nullptr){
		Contour* tmp = ctr;
		ctr = ctr->tail;
		tmp = NULL;
		delete tmp;
	}
	//--------------------------------
	C = new Contour;
	C->head =nullptr;
	C->tail = nullptr;
	C->lx = 0;
	C->rx = btr->root->GetW();
	C->y = btr->root->GetH();
	btr->root->Con = C;
	btr->root->llx = 0;
	btr->root->lly = 0;
	
	//cout<<endl<<btr->root->mod_ptr->mod_name<<endl;
	place(btr->root,btr);
	//-------------------------------	
	//A = H * W;
	btr->rightmost=cal_maxx(btr);
	btr->upmost=cal_maxy(btr);


	btr->Area=btr->rightmost*btr->upmost;
	btr->R=(double)((double)btr->upmost/(double)btr->rightmost);
	return;
	//cout<<"\nPacking Complete";
}

int NETLIST::cal_maxx(B_tree* btr){
	int re=0;
	for(int i=0;i<btr->nodes.size();i++){
		if(btr->nodes[i].rotat==0){
			if(btr->nodes[i].llx+btr->nodes[i].mod_ptr->width>re){
				re=btr->nodes[i].llx+btr->nodes[i].mod_ptr->width;
			}
		}
		else{
			if(btr->nodes[i].llx+btr->nodes[i].mod_ptr->height>re){
				re=btr->nodes[i].llx+btr->nodes[i].mod_ptr->height;
			}
		}
	}
	return re;
}

int NETLIST::cal_maxy(B_tree* btr){
	int re=0;
	for(int i=0;i<btr->nodes.size();i++){
		if(btr->nodes[i].rotat==0){
			if(btr->nodes[i].lly+btr->nodes[i].mod_ptr->height>re){
				re=btr->nodes[i].lly+btr->nodes[i].mod_ptr->height;
			}
		}
		else{
			if(btr->nodes[i].lly+btr->nodes[i].mod_ptr->width>re){
				re=btr->nodes[i].lly+btr->nodes[i].mod_ptr->width;
			}
		}
	}
	return re;
}

void NETLIST::place(Node* thisNode,B_tree* btr){
	//cout<<thisNode->mod_ptr->mod_name<<"->";
	if(thisNode->left_child != nullptr)
	{
		//cout<<thisNode->mod_ptr->mod_name<<"->";
		//cout<<"[L]"<<thisNode->left_child->mod_ptr->mod_name<<endl;
		Contour* C_R = new Contour; 
		C_R->lx = thisNode->llx + thisNode->GetW();
		//cout<<thisNode->llx<<" "<<thisNode->GetW()<<endl;

		C_R->rx = C_R->lx + thisNode->left_child->GetW();
		C_R->y = 0;
		Contour* thisCon = thisNode->Con;
		C_R->head = thisCon;
		if(thisCon->tail == nullptr)//right most 
		{
			thisCon->tail = C_R;
			C_R->tail = nullptr;
		}
		else
		{
			thisCon = thisCon->tail;
			while(thisCon != nullptr)
			{
				if(thisCon->y > C_R->y)
					C_R->y = thisCon->y;
				// Update C_R's floor height
				if(thisCon->rx >= C_R->rx)
				{		
					break;
				}// the module is inside this contour
				else
				{
					if(thisCon->tail != nullptr )
						thisCon->tail->head = C_R;
					Contour* tmp = thisCon;
					thisCon = thisCon->tail;
					delete tmp;
				}// the module include this contour
			}
			//-----------------------------------
			C_R->head->tail = C_R;
			C_R->tail = thisCon;
			if(thisCon != nullptr)//--n--__e__
				thisCon->lx = C_R->rx;
		}
		//-------------------------------------------------------
		thisNode->left_child->llx = C_R->lx;
		//cout<<"llx l "<<thisNode->left_child->llx<<endl;
		thisNode->left_child->lly = C_R->y;
		C_R->y += thisNode->left_child->GetH();
		//-----------------------------------------------------
		if(C_R->rx > btr->rightmost)
			btr->rightmost = C_R->rx;
		if(C_R->y > btr->upmost)
			btr->upmost = C_R->y;
		//Update H,W
		//----------------------
		thisNode->left_child->Con = C_R;
		//cout<<"left "<<thisNode->left_child->mod_ptr->mod_name<<" "; 
		place(thisNode->left_child,btr);
	}
	//========================================================================================
	if(thisNode->right_child != nullptr)
	{
		//cout<<thisNode->mod_ptr->mod_name<<"->";
		//cout << "[R]" << thisNode->right_child->mod_ptr->mod_name << endl;
		//-----------------------------------------------
		Contour* C_T = new Contour;	
		C_T->lx = thisNode->llx;
		C_T->rx = C_T->lx + thisNode->right_child->GetW();
		C_T->y = 0;
		//------------------------------------------------
		Contour* thisCon = thisNode->Con;
		C_T->head = thisCon->head;
		while(thisCon != nullptr)
		{
			if(thisCon->y > C_T->y){
				C_T->y = thisCon->y;
			}// Update C_L->y
			if(thisCon->rx > C_T->rx){		
				break;
			}// the module is inside the contour
			else{
				if(thisCon->tail != nullptr)
					thisCon->tail->head = C_T;
				Contour* tmp = thisCon;
				thisCon = thisCon->tail;
				delete tmp;
			}// the module include this contour
		}
		//-----------------------------------------------
		if(C_T->head != nullptr)
			C_T->head->tail = C_T;
		C_T->tail = thisCon;
		if(thisCon != nullptr)//--n--____
			thisCon->lx = C_T->rx;
		//-------------------------------------------
		thisNode->right_child->llx = C_T->lx;
		//cout<<"llx r "<<thisNode->right_child->llx<<endl;
		thisNode->right_child->lly = C_T->y;
		C_T->y += thisNode->right_child->GetH();
		//--------------------
		if(C_T->rx > btr->rightmost)
			btr->rightmost = C_T->rx;
		if(C_T->y > btr->upmost)
			btr->upmost = C_T->y;
		//Update x
		//----------------------
		thisNode->right_child->Con = C_T;
		//cout<<"right "<<thisNode->right_child->mod_ptr->mod_name<<" ";
		place(thisNode->right_child,btr);
	}
}
int NETLIST::HPWL_approx(B_tree* btr){
	int w = 0;
	for(unsigned int i =0; i < nets.size(); i++)
	{
		Net* thisNet = &nets[i];
		int llx, urx, lly, ury;
		llx = INT8_MAX;
		lly = INT8_MAX;
		urx = 0;
		ury = 0;
		for(unsigned int j = 0; j < thisNet->net_mods.size(); j++)
		{
			Node* thisNode = &btr->nodes[thisNet->net_mods[j]->index];
			int ctx = thisNode->llx + 0.5*thisNode->GetW();
			int cty = thisNode->lly + 0.5*thisNode->GetH();
			if(llx > ctx)
				llx = ctx;	
			if(urx < ctx)
				urx = ctx;
			if(lly > cty)
				lly = cty;
			if(ury < cty)
				ury = cty;
		}
		w += urx - llx + ury - lly;
	}
	return w;
}
double NETLIST::Cost_Estimate(){	
	double weight_r = 1 - alpha - beta;
	double RR = R_star - H/W;
	double c = 100*alpha*(A/A_star) + beta*wirelength + weight_r*RR*RR;
	return c;
}
void NETLIST::Perturb(B_tree* btr)
{
	int r = rand()%3;
	if(r==0)//rotat
	{
		//cout<<"r\n";
		r = rand()%(nt.mods.size());
		Node* A = &btr->nodes[r];
		btr->rotat(A);
	}
	else if(r==1)
	{
		//cout<<"s\n";
		r = rand()%(nt.mods.size());
		Node* B = &btr->nodes[r];
		r = (r+rand()%(nt.mods.size()-1)+1)%(nt.mods.size());
		Node* C = &btr->nodes[r];
		btr->swap(B, C);
	}
	else{
		//cout<<"m\n";
		r = rand()%(nt.mods.size());
		Node* D = &btr->nodes[r];
		r = (r+rand()%(nt.mods.size()-1)+1)%(nt.mods.size());
		Node* E = &btr->nodes[r];
		r = rand()%2;
		btr->move(D, E, r);
	}
}
void writeOut(char* filenam){
	string filename=filenam;
	ofstream fout(filename);
	for (unsigned int i = 0; i < nt.btree_best.nodes.size(); i++){
		Node* thisNode = &nt.btree_best.nodes[i];
		fout << thisNode->mod_ptr->mod_name << " ";
		fout << thisNode->llx << " ";
		fout << thisNode->lly << " ";
		fout << thisNode->rotat << "\n";
	}
	fout.close();
}
void writeTxt(char* filenam, double runtime){
	string filename=filenam;
	ofstream fout(filename);
	fout << "Benchmark:" << nt.name << "\n\n";
	fout << "ratio:" << double(nt.H) /double( nt.W) << "\n\n";
	fout << "whitespace:" << 100*(nt.A - nt.A_total) / nt.A << "%\n\n";
	fout << "wirelength:" << nt.wirelength << "\n";
	fout << "Chip_width:" << nt.W << "\n";
	fout << "Chip_height:" << nt.H << "\n";
	fout.close();
}
void Plot(char* filenam){
	//cout << "   Write " << fileName << endl;
	//if(fileName == "") fileName = "GP.m";
	string filename=filenam;
	ofstream fout;
	fout.open(filename);
	fout << "axis equal;\nhold on;\ngrid on;\n" << endl;
		double llxf = 0;
		double llyf = 0;
		double urxf = llxf + nt.W_star;
		double uryf = llyf + nt.H_star;
		fout << "block_x=[" << llxf << " " << llxf << " " << urxf << " " << urxf << " " << llxf << " ];" << endl;
		fout << "block_y=[" << llyf << " " << uryf << " " << uryf << " " << llyf << " " << llyf << " ];" << endl;
		fout << "fill(block_x,block_y,'r','facealpha',0.5);" << endl;
		//fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << thisNode->mod_ptr->mod_name << "');" << endl << endl;
	
	for (unsigned int i = 0; i < nt.mods.size(); i++)
	{
		Node* thisNode = &nt.btree_best.nodes[i];
		double llx = thisNode->llx;
		double lly = thisNode->lly;
		double urx = llx + thisNode->GetW();
		double ury = lly + thisNode->GetH();
		fout << "block_x=[" << llx << " " << llx << " " << urx << " " << urx << " " << llx << " ];" << endl;
		fout << "block_y=[" << lly << " " << ury << " " << ury << " " << lly << " " << lly << " ];" << endl;
		fout << "fill(block_x,block_y,'c','facealpha',0.5);" << endl;
		fout << "text(" << (llx + urx) / 2.0 << ", " << (lly + ury) / 2.0 << ", '" << thisNode->mod_ptr->mod_name << "');" << endl << endl;
	}
	fout.close();
}
//===========================B_tree===================================//
void B_tree::copy(B_tree* btr){
	nodes.clear();
	nodes = btr->nodes;
	root = &nodes[btr->root->index];
	for (unsigned int  i = 0; i < btr->nodes.size(); i++)
	{
		//cout<<"yoyo"<<i<<" ";
		if(btr->nodes[i].parent!=nullptr)
			nodes[i].parent = &nodes[btr->nodes[i].parent->index];
		else
			nodes[i].parent = nullptr;
		if(btr->nodes[i].left_child!=nullptr)
			nodes[i].left_child = &nodes[btr->nodes[i].left_child->index];
		else
			nodes[i].left_child = nullptr;
		if(btr->nodes[i].right_child!=nullptr)
			nodes[i].right_child = &nodes[btr->nodes[i].right_child->index];
		else
			nodes[i].right_child = nullptr;
	}
	return;
}
void B_tree::move(Node* target, Node* new_parent, bool left)
{
	/*//-------------------------------------------------
	cout<<"move "<<target->mod_ptr->mod_name<<" to the";
	if(left)
		cout<<" left";
	else
		cout<<" right";
	//cout<<" of "<<new_parent->mod_ptr->mod_name<<"\n";
	*///---------------------------------------------------------------------
	if (target->left_child != nullptr && target->right_child != nullptr)
	{//have both left and right child;
		//------------------------------
		Node* Leaf = target;
		while(true)
		{
			Node* Next;
			if(Leaf->left_child != nullptr)
			{
				Next = Leaf->left_child;
				if(Next->left_child == nullptr && Next->right_child == nullptr)
				{// find leaf on the left
					Leaf->left_child = nullptr;
					Leaf = Next;
					break;
				}
				Leaf = Next;
			}
			if(Leaf->right_child != nullptr)
			{
				Next = Leaf->right_child;
				if(Next->left_child == nullptr && Next->right_child == nullptr)
				{// find leaf on the right
					Leaf->right_child = nullptr;
					Leaf = Next;
					break;
				}
				Leaf = Next;
			}
		}
		//find a leaf to subsitude targer------------
		if(target->parent!=nullptr){
			if (target->parent->left_child == target)
				target->parent->left_child = Leaf;
			else
				target->parent->right_child = Leaf;
		}
		Leaf->parent = target->parent;     
		//parent-------------------------------------
		Leaf->left_child = target->left_child;
		if(target->left_child != nullptr)
			target->left_child->parent = Leaf; 
		//left-----------------------------------------
		Leaf->right_child = target->right_child;
		if(target->right_child != nullptr)
			target->right_child->parent = Leaf;
		//right-------------------------------------
		if(root == target)
			root = Leaf;
		//------------------------------if target is root
	}
	else if (target->left_child != nullptr)
	{//have left only
		//cout<<"-2\n";
		Node* L = target->left_child;
		if(target->parent != nullptr)
		{
			if (target->parent->left_child == target)
			{
				target->parent->left_child = L;
			}
			else
			{
				target->parent->right_child = L;
			}
		}
		else
			root = target->left_child;
		L->parent = target->parent;
	}
	else if (target->right_child != nullptr)
	{//have right only
		//cout<<"-3\n";	
		Node* R = target->right_child;
		if(target->parent != nullptr)
		{
			if (target->parent->left_child == target){
				target->parent->left_child = R;
			}
			else
			{
				target->parent->right_child = R;
			}
		}
		else	
			root = target->right_child;
		R->parent = target->parent;
	}
	else
	{//leaf node
		return;
		if (target->parent->left_child == target)
			target->parent->left_child = nullptr;
		else
			target->parent->right_child = nullptr;
	}
	target->left_child = nullptr;
	target->right_child = nullptr;
	//cout<<"-\n";
	//--------------------deletion---------------------------//
	target->parent = new_parent;
	if (left)
	{
		target->left_child = new_parent->left_child;
		new_parent->left_child = target;
		if(target->left_child != nullptr)
			target->left_child->parent = target;
	}
	else{
		target->right_child = new_parent->right_child;
		new_parent->right_child = target;
		if(target->right_child != nullptr)
			target->right_child->parent = target;
	}
	//----------------------insertion------------------------//
	//cout<<"move_complete\n";
}
void B_tree::swap(Node* A, Node* B)
{
	//cout<<"swap\n";
	//cout<<A->mod_ptr->mod_name<<"|"<<B->mod_ptr->mod_name<<endl;
	if(root == A){
		root = B;
	}else{
		if(root==B){
			root = A;
		}
	}//root
	//----------------------------
	Node* A_lc = A->left_child;
	Node* A_rc = A->right_child;
	Node* A_pr = A->parent;
	//===================
	if(A->left_child != nullptr){
		if(A->left_child != B)
			A->left_child->parent = B;
	}
	//-------------------
	if(A->right_child != nullptr ){
		if(A->right_child != B)
			A->right_child->parent = B;
	}
	//-------------------
	if(A->parent != nullptr && A->parent != B){
		if(A->parent->left_child == A)
			A->parent->left_child = B;
		else
			A->parent->right_child = B;
	}
	//====================
	if(B->left_child != nullptr ){
		if(B->left_child != A)
			B->left_child->parent = A;
	}
	//-------------------
	if(B->right_child != nullptr ){
		if(B->right_child != A)
			B->right_child->parent = A;
	}
	//-------------------
	if(B->parent != nullptr && B->parent != A){
		if(B->parent->left_child == B)
			B->parent->left_child = A;
		else
			B->parent->right_child = A;
	}
	//====================
	if(B->parent != A )
		A->parent = B->parent;
	else
		A->parent = B;
	//--------------------
	if(B->left_child != A)
		A->left_child = B->left_child;
	else
		A->left_child = B;
	//--------------------
	if(B->right_child != A)
		A->right_child = B->right_child;
	else 
		A->right_child = B;
	//======================
	if(A_pr != B )
		B->parent = A_pr;
	else
		B->parent = A;
	//------------------
	if(A_lc != B)
		B->left_child = A_lc;
	else 
		B->left_child = A;
	//-----------------
	if(A_rc != B)
		B->right_child = A_rc;
	else
		B->right_child = A;
	//cout<<"swap completed\n";
	return;
}
void B_tree::rotat(Node* target)
{
	target->rotat = !target->rotat;
}
//==================================Node===========================//
int Node::GetW(){
	if (rotat){
		return this->mod_ptr->height;
	}
	else
	{
		return this->mod_ptr->width;
	}
	return mod_ptr->width;
}
int Node::GetH(){
	if (rotat){
		return this->mod_ptr->width;
	}
	else
	{
		return this->mod_ptr->height;
	}
	return mod_ptr->height;
}
