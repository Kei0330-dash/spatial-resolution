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

void mem_root::share(TText *t){
	ttext.push_back(t);
}

void mem_root::share(std::vector<TLine*>lines){
	tlines.push_back(lines);
}


void mem_root::erase_pointer(){
	if(!th1d.empty()){
		for(auto& h1 : th1d){
			if(h1){
            	delete h1;
				h1 = nullptr;
			}
        }
        th1d.clear();
	}
	if(!th2d.empty()){
		for(auto& h2 : th2d){
			if(h2){
            	delete h2;
				h2 = nullptr;
			}
        }
        th2d.clear();
	}
	if(!tcanvas.empty()){
		for(auto& c1 : tcanvas){
			if(c1){
            	delete c1;
				c1 = nullptr;
			}
        }
        tcanvas.clear();
	}
	if(!tcanvas.empty()){
		for(auto& t : ttext){
			if(t){
            	delete t;
				t = nullptr;
			}
        }
        ttext.clear();
	}
	if(!tlines.empty()){
		for(int i = 0; i < tlines.size(); i++){
			for(auto& l : tlines[i]){
				if(l){
					delete l;
					l = nullptr;
				}
        	}
			tlines[i].clear();
		}
        tlines.clear();
	}
}


