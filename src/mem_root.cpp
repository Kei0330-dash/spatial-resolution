#include "../include/mem_root.hpp"
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TLine.h>
#include <vector>
#include <mutex>
#include <iostream>

void mem_root::share(std::shared_ptr<void> ptr) {
	std::lock_guard<std::mutex> lock(mtx);
	sharedPtr.push_back(ptr);
}

void mem_root::clear() {
	std::lock_guard<std::mutex> lock(mtx);
	sharedPtr.clear();
}