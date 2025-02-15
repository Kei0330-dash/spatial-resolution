#include "../include/mem_root.hpp"
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TLine.h>
#include <vector>

void mem_root::share(TH1D* h1){
	th1d.push_back(h1);
}

void mem_root::share(TH2D* h2){
	th2d.push_back(h2);
}

void mem_root::share(TCanvas* c1){
	tcanvas.push_back(c1);
}

void mem_root::erase_pointer(){
	if(!th1d.empty()){
		for(auto& h1 : th1d){
            delete h1;
        }
        th1d.clear();
	}
	if(!th2d.empty()){
		for(auto& h2 : th2d){
            delete h2;
        }
        th2d.clear();
	}
	if(!tcanvas.empty()){
		for(auto& c1 : tcanvas){
            delete c1;
        }
        tcanvas.clear();
	}
}


