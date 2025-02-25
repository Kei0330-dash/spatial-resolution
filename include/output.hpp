#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "alias.hpp"
#include "param.hpp"
#include "analysis.hpp"
#include <iostream>  // std::coutを使用するために追加
#include <TGButton.h>
#include <TGNumberEntry.h>
// #include <TGCheckButton.h>
// #include <TGTextButton.h>
#include <TGLabel.h>
#include <TGFrame.h>
#include <TGClient.h>
#include <TGFileDialog.h>
#include <TGTextEntry.h>

class outputFrame : public TGTransientFrame {
public:
    outputFrame(const TGWindow *p, const TGWindow *mainWindow, UInt_t w, UInt_t h, param params);
    virtual ~outputFrame();
    int Get_Entry_num();
	void EntryChanged(Long_t val);


private:
    TGNumberEntry *numEntry;
	param params;
	TGLabel *SizeLabel;
	TGLabel *CenterofGLabel;
    ClassDef(outputFrame, 0);
};

#endif // OUTPUT_HPP
