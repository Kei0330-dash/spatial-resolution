#include "../include/mem_root.hpp"
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TLine.h>
#include <vector>
#include <mutex>
#include <iostream>

void mem_root::share(TH1D* h1){
	std::lock_guard<std::mutex> lock(mtx);
	th1d.push_back(h1);
}

void mem_root::share(TH2D* h2){
	std::lock_guard<std::mutex> lock(mtx);
	th2d.push_back(h2);
}

void mem_root::share(TCanvas* c1){
	std::lock_guard<std::mutex> lock(mtx);
	tcanvas.push_back(c1);
}

void mem_root::share(TText *t){
	std::lock_guard<std::mutex> lock(mtx);
	ttext.push_back(t);
}

void mem_root::share(TLine* l1){
	std::lock_guard<std::mutex> lock(mtx);
	tline.push_back(l1);	
}

void mem_root::share(std::vector<TLine*>lines){
	std::lock_guard<std::mutex> lock(mtx);
	tlines.push_back(lines);
}


void mem_root::erase_pointer(){
	std::lock_guard<std::mutex> lock(mtx);
	try{
		if(!th1d.empty()){
			for(auto& h1 : th1d){
					delete h1;
					h1 = nullptr;
			}
			th1d.clear();
		}
		if(!th2d.empty()){
			for(auto& h2 : th2d){
				delete h2;
				h2 = nullptr;
			}
			th2d.clear();
		}
		if(!tcanvas.empty()){
			for(auto& c1 : tcanvas){
				delete c1;
				c1 = nullptr;
			}
			tcanvas.clear();
		}
		if(!ttext.empty()){
			for(auto& t : ttext){
				delete t;
				t = nullptr;
			}
			ttext.clear();
		}
		if(!tline.empty()){
			for(auto& l : tline){
				delete l;
				l = nullptr;
			}
			tline.clear();
		}
		if(!tlines.empty()){
			for(int i = 0; i < tlines.size(); i++){
				for(auto& l : tlines[i]){
					delete l;
					l = nullptr;
				}
				tlines[i].clear();
			}
			tlines.clear();
		}
	} catch (...) {
		std::cerr << "POTENSIAL MEMORY LEAK" << std::endl;
	}
}


