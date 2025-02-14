#ifndef GUIMAIN_HPP
#define GUIMAIN_HPP

#include "../include/alias.hpp"
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

class MyMainFrame : public TGMainFrame {
public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();
    void HandleButton();
	void SearchCluster();
	void OpenFile();
    void CloseWindow();
    int Get_Entry_num();
    bool Get_Option_Red();
    bool Get_Option_Subtract();
    bool Get_Option_Fitting();
	TString Get_EnteredPath();

private:
    TGNumberEntry *numEntry;
    TGCheckButton *Option_Red;
    TGCheckButton *Option_Subtract;
    TGCheckButton *Option_Fitting;
    TGTextButton *Run_analysis;
	TGTextButton *SearchCluster_Button;
	TGTextButton *Open_file;
	TGTextEntry *pathEntry;
	TGNumberEntry *thresholdEntry;

	AnalyzeType state = NO_ACTION;
    ClassDef(MyMainFrame, 0);
};

#endif // GUIMAIN_HPP
