#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h> // for compute time
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>
#include <iomanip>
#include<map>
using namespace std;

class Pin//pin
{
public:
	Pin();
	~Pin();
	string pin_name;
	char pin_type;// B/b/i/o/V/G
	int x_shift, y_shift;
private:	
};

class Module//mod
{
public:
	Module();
	~Module();
	string mod_name;
	char mod_type;// G/P
	int width,height;
	int area; //width*height
	int index;
	int llx,lly;//update when packing;
	vector<Pin> mod_pins;
private:
};

class Net
{
public:
	Net();
	~Net();
	string net_name;	
	vector<Module*> net_mods;
	vector<int> pin_id;
private:
};

class Contour{
public:
	Contour();
	~Contour();
	int rx = 0; 
	int lx = 0;
	int y = 0;
	Contour* head = nullptr;
	Contour* tail = nullptr;	
private:
};

class Node
{
public:
	Node();
	~Node();
	int index;
	Module* mod_ptr;
	bool rotat = 0;// 0/1
	int llx, lly;// update when packing
	Node* left_child = nullptr; //module on the left
	Node* right_child = nullptr; //module on the right
	Node* parent = nullptr; //module on the top
	Contour* Con;
	int GetW();
	int GetH();
private:
};

class B_tree{
public:
	B_tree();
	B_tree(B_tree&);
	~B_tree();
	vector<Node> nodes;
	double score;
	int rightmost;
	int upmost;
	double Area;
	double R;
	Node* root;
	void copy(B_tree*);
	void move(Node*, Node*, bool);
	void swap(Node*, Node*);
	void rotat(Node*);
private:
	
};

class NETLIST
{
public:
	NETLIST();
	~NETLIST();
	string name; //benchmark

	B_tree btree_current;
	B_tree btree_new_current;
	B_tree btree_best;
	vector<Module> mods;
	vector<Pin> pins;
	vector<Net> nets;
	Module Bound;
	//-------------------------------------------------------constant
	double Gamma;// White_space Fraction
	double R_star;

	double A_total, A_star;// A_total*Gamma = A_star
	double H_star, W_star;// aspected Height, aspected Width
	//--------------------------------------------------------------
	Contour* C;	
	int H, W;
    double	A;
	int wirelength;
	//
	void initialize();
	//B_tree* initial();
	void initial(B_tree*);
	void DFS(B_tree& btree);
	void FP(char*);
	void FP1();
	void FP2();
	void FP3();
	void FP4();
	void CF();
	void cal_cf(B_tree*);
	Node* last_node(B_tree& btree);
	int cal_maxx(B_tree*);
	int cal_maxy(B_tree*);
	void insert_node_init(Node* be_inserted, B_tree& btree);
	int count_level(int);
	void Fix_Outline_FloorPlan();
	void Packing(B_tree*);
	int HPWL_approx(B_tree*);
private:
	double  best_cost, cost;
	double alpha, beta;
	double T;
	int n;
	//SA parameter 	
	
	double Cost_Estimate();
	void Perturb(B_tree*);
	void place(Node*,B_tree*);
};

int readYal(char* file_name);
void Plot(char* filenam);
void writeTxt(char* filename, double);
void writeOut(char* filename);

extern NETLIST nt;
