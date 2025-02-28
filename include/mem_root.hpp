#ifndef MEM_ROOT_HPP
#define MEM_ROOT_HPP
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TLine.h>
#include <TText.h>
#include <vector>
#include <memory>
#include <mutex>
/// @brief ROOTで動的に確保したポインタをここに共有して消去するクラス
class mem_root{
	private:
	std::vector<std::shared_ptr<void>> sharedPtr;
	std::mutex mtx;
	
	public:
	void share(std::shared_ptr<void> ptr);
	void clear();
};

#endif