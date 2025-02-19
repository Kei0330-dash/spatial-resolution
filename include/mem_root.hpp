#ifndef MEM_ROOT_HPP
#define MEM_ROOT_HPP
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TLine.h>
#include <TText.h>
#include <vector>
/// @brief ROOTで動的に確保したポインタをここに共有して消去するクラス
class mem_root{
	private:
	std::vector<TH1D*> th1d;
	std::vector<TH2D*> th2d;
	std::vector<TCanvas*> tcanvas;
	std::vector<TText *> ttext;
	std::vector<std::vector<TLine*>> tlines;

	public:
	void share(TH1D* h1);
	void share(TH2D* h1);
	void share(TCanvas* c1);
	void share(TText *t);
	void share(std::vector<TLine*>lines);
	void erase_pointer();
};

#endif