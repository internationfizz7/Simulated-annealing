#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <set>
#include "parser.h"

using namespace std;

int main(int argc, char **argv){
    cout<<"------------------------start program------------------------\n\n";
    clock_t start_overall,end_overall;
    start_overall=clock();
    string ar3=argv[3];
    string ar2=argv[2];
    //int ar7   =stoi(argv[7]);
    nt.Gamma=stod(ar3)/(double)100;
    nt.R_star=stod(ar2);
    readYal(argv[1]);
   
    /*if(ar7==0){
        nt.FP(argv[1]);
    }
    else if(ar7==1){
        nt.FP1();
    }
    else if(ar7==2){
        nt.FP2();
    }
    else if(ar7==3){
        nt.FP3();
    }
    else if(ar7==4){
        nt.FP4();
    }*/
    //nt.FP3();
    nt.FP(argv[1]);
    end_overall=clock();
    cout<<endl;
    writeTxt(argv[4],(double)(end_overall-start_overall)/CLOCKS_PER_SEC);
    writeOut(argv[5]);
    Plot(argv[6]);
    return 0;
}



