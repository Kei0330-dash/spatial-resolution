#ifndef GUIMAIN_HPP
#define GUIMAIN_HPP

#include "alias.hpp"
#include "param.hpp"
#include "analysis.hpp"
#include "output.hpp"
#include <TGButton.h>
#include <TGNumberEntry.h>
// #include <TGCheckButton.h>
// #include <TGTextButton.h>
#include <TGLabel.h>
#include <TGFrame.h>
#include <TGClient.h>
#include <TGFileDialog.h>
#include <TGTextEntry.h>
#include <iostream>  // std::coutを使用するために追加
#include <memory>

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
	bool Get_Option_meanSubtract();
    bool Get_Option_Fitting();
	double Get_SettingThreshold();
	int Get_Filter_ClusterSize();
	TString Get_EnteredPath();
	param SettingParam();
	void init_ANALYZE();


private:
    TGNumberEntry *numEntry;
    TGCheckButton *Option_Red;
    TGCheckButton *Option_Subtract;
    TGCheckButton *Option_meanSubtract;
    TGCheckButton *Option_Fitting;
    TGTextButton *Run_analysis;
	TGTextButton *SearchCluster_Button;
	TGTextButton *Open_file;
	TGTextEntry *pathEntry;
    TGCheckButton *Option_save;
	TGTextEntry *saveEntry;
	TGNumberEntry *thresholdEntry;
	TGNumberEntry *ClusterFilterEntry;
	AnalyzeType state = NO_ACTION;
	std::unique_ptr<analysis> ANA;
	outputFrame* out = nullptr;
    ClassDef(MyMainFrame, 0);
};

#endif // GUIMAIN_HPP
