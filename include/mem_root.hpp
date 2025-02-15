#ifndef MEM_ROOT_HPP
#define MEM_ROOT_HPP
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TLine.h>
#include <vector>

class mem_root{
	private:
	std::vector<TH1D*> th1d;
	std::vector<TH2D*> th2d;
	std::vector<TCanvas*> tcanvas;

	public:
	void share(TH1D* h1);
	void share(TH2D* h1);
	void share(TCanvas* c1);

	void erase_pointer();
};

#endif