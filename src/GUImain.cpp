#include "../include/GUImain.hpp"
#include "../include/analysis.hpp"
#include "../include/param.hpp"
#include <TApplication.h>
#include <TGClient.h>
#include <TGFileDialog.h>
#include <iostream>  // std::coutを使用するために追加

const char *fileTypes[] = {"All files", "*", nullptr};  // 配列を定義

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) 
    : TGMainFrame(p, w, h) {
    // 垂直フレームを作成
    TGVerticalFrame *mainFrame = new TGVerticalFrame(this, w, h);

	// General Settings
	TGGroupFrame *SettingFrame_General = new TGGroupFrame(mainFrame, "General Settings");

    // 数値入力フィールドを含む水平フレームを作成
    TGHorizontalFrame *OptionFrame = new TGHorizontalFrame(SettingFrame_General, 200, 50);
	TGVerticalFrame *numFrame = new TGVerticalFrame(OptionFrame, 100, 100);
	TGLabel *numLabel = new TGLabel(numFrame, "Input Your Entry Number:");
    numEntry = new TGNumberEntry(numFrame, 0, 9, -1, TGNumberFormat::kNESInteger);
	numFrame->AddFrame(numLabel, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    numFrame->AddFrame(numEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));
	OptionFrame->AddFrame(numFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));
    // チェックボックスを含む垂直フレームを作成
    TGGroupFrame *checkBoxFrame = new TGGroupFrame(OptionFrame, "Input Your Analysis Option");
    Option_Red = new TGCheckButton(checkBoxFrame, "Option_Red");
    Option_Subtract = new TGCheckButton(checkBoxFrame, "Option_Subtract");
	Option_meanSubtract = new TGCheckButton(checkBoxFrame, "Option_meanSubtract");
    Option_Fitting = new TGCheckButton(checkBoxFrame, "Option_Fitting");
    checkBoxFrame->AddFrame(Option_Red, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    checkBoxFrame->AddFrame(Option_Subtract, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    checkBoxFrame->AddFrame(Option_meanSubtract, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
	checkBoxFrame->AddFrame(Option_Fitting, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
	OptionFrame->AddFrame(checkBoxFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 40, 10, 10, 10));
    SettingFrame_General->AddFrame(OptionFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    // パス入力フィールドを含む水平フレームを作成
    TGHorizontalFrame *pathFrame = new TGHorizontalFrame(SettingFrame_General, 400, 50);
	TGLabel *pathLabel = new TGLabel(pathFrame, "Input Your Data Path:");
    pathEntry = new TGTextEntry(pathFrame, new TGTextBuffer(30));
	Open_file = new TGTextButton(pathFrame, "Open File");
	pathFrame->AddFrame(pathLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 5, 5, 5));
    pathFrame->AddFrame(pathEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	pathFrame->AddFrame(Open_file, new TGLayoutHints(kLHintsRight | kLHintsCenterY, 5, 5, 5, 5));
    SettingFrame_General->AddFrame(pathFrame, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 10, 10, 10, 10));
	//　ここまでパス入力フィールド


	// // セーブ入力フィールドを含む水平フレームを作成
	// TGHorizontalFrame *saveFrame = new TGHorizontalFrame(SettingFrame_General, 400, 50);
	// TGLabel *pathLabel = new TGLabel(saveFrame, "Input Your Data Path:");
	// saveEntry = new TGTextEntry(saveFrame, new TGTextBuffer(30));
	// Option_save = new TGCheckButton(saveFrame, "");
	// saveFrame->AddFrame(pathLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 5, 5, 5));
	// saveFrame->AddFrame(pathEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	// saveFrame->AddFrame(Open_file, new TGLayoutHints(kLHintsRight | kLHintsCenterY, 5, 5, 5, 5));
	// SettingFrame_General->AddFrame(saveFrame, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 10, 10, 10, 10));
	// //　ここまでセーブ入力フィールド

    // ボタンを含む水平フレームを作成
    TGHorizontalFrame *buttonFrame = new TGHorizontalFrame(SettingFrame_General, 200, 50);
    Run_analysis = new TGTextButton(buttonFrame, "Run");
	SearchCluster_Button = new TGTextButton(buttonFrame, "SearchCluster");
    buttonFrame->AddFrame(Run_analysis, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    buttonFrame->AddFrame(SearchCluster_Button, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    SettingFrame_General->AddFrame(buttonFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

	mainFrame->AddFrame(SettingFrame_General, new TGLayoutHints(kLHintsExpandX | kLHintsLeft, 20, 20, 20, 20));
    AddFrame(mainFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

	//イベントパンドラをコネクト
    Run_analysis->Connect("Clicked()", "MyMainFrame", this, "HandleButton()");
    SearchCluster_Button->Connect("Clicked()", "MyMainFrame", this, "SearchCluster()");
    Open_file->Connect("Clicked()", "MyMainFrame", this, "OpenFile()");

	// Parameter Settings

	TGGroupFrame *SettingFrame_Parameter = new TGGroupFrame(mainFrame, "Parameter Settings");
	TGHorizontalFrame *thresholdFrame = new TGHorizontalFrame(SettingFrame_Parameter, 200, 50);
	TGLabel *thresholdLabel = new TGLabel(thresholdFrame, "Threshold:    mean +");
	thresholdEntry = new TGNumberEntry(thresholdFrame, 3.0, 9, -1);
	TGLabel *thresholdLabel2 = new TGLabel(thresholdFrame, "sigma");
    thresholdFrame->AddFrame(thresholdLabel, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    thresholdFrame->AddFrame(thresholdEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    thresholdFrame->AddFrame(thresholdLabel2, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    SettingFrame_Parameter->AddFrame(thresholdFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));


	TGGroupFrame *Cluster_DefineFrame = new TGGroupFrame(SettingFrame_Parameter, "Clusters Define");
	TGLabel *ClusterLabel = new TGLabel(Cluster_DefineFrame, "Filter by Cluster Pixel Size:");
	ClusterFilterEntry = new TGNumberEntry(Cluster_DefineFrame, 1, 9, -1);
	Cluster_DefineFrame->AddFrame(ClusterLabel, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
	Cluster_DefineFrame->AddFrame(ClusterFilterEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    SettingFrame_Parameter->AddFrame(Cluster_DefineFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

	mainFrame->AddFrame(SettingFrame_Parameter, new TGLayoutHints(kLHintsExpandX | kLHintsLeft, 20, 20, 20, 20));


    SetWindowName("Settings screen");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

MyMainFrame::~MyMainFrame() {
	ANA.reset();
    Cleanup();
}

void MyMainFrame::HandleButton() {
	param params = SettingParam();
	init_ANALYZE();
    state = ANA->runMyClass(params);
	out = new outputFrame(gClient->GetRoot(), this, 500, 400, params);
	out->MapWindow();
}

void MyMainFrame::SearchCluster() {
	param params = SettingParam();
	init_ANALYZE();
	params.in.AutoCluster = true;
    state = ANA->runMyClass(params);
}

void MyMainFrame::OpenFile() {
    TString enteredPath = Get_EnteredPath();
	static TString dir(".");
	TGFileInfo fileInfo;
	fileInfo.fFileTypes = fileTypes;
	fileInfo.fIniDir = StrDup(dir);
	new TGFileDialog(gClient->GetDefaultRoot(), this, kFDOpen, &fileInfo);
	if (fileInfo.fFilename) {
		if (state == ANALYZE_ONE_EVENT || state == ANALYZE_ALL_CLUSTERS) {
			state = NO_ACTION;
			ANA->change_file();
			ANA->clear_pointer();
			ANA->closefile();
		}
		dir = fileInfo.fIniDir;
		std::cout << "Opened file: " << fileInfo.fFilename << std::endl;
		pathEntry->SetText(fileInfo.fFilename);  // ファイルパスをテキストエントリーに設定
	}
}

TString MyMainFrame::Get_EnteredPath() {
    return TString(pathEntry->GetText());
}

void MyMainFrame::CloseWindow() {
    gApplication->Terminate(0);
}

int MyMainFrame::Get_Entry_num(){
    return numEntry->GetIntNumber();
}

bool MyMainFrame::Get_Option_Red() {
    return Option_Red->IsOn();
}

bool MyMainFrame::Get_Option_Subtract() {
    return Option_Subtract->IsOn();
}

bool MyMainFrame::Get_Option_meanSubtract() {
    return Option_meanSubtract->IsOn();
}

bool MyMainFrame::Get_Option_Fitting() {
    return Option_Fitting->IsOn();
}

double MyMainFrame::Get_SettingThreshold(){
	return thresholdEntry->GetNumber();
}

int MyMainFrame::Get_Filter_ClusterSize(){
	return ClusterFilterEntry->GetIntNumber();
}

param MyMainFrame::SettingParam(){
	TString path = Get_EnteredPath();
	if (path.Length() == 0) {
		// std::cerr << "Error: No file path specified" << std::endl;
		// return;
		throw std::runtime_error("Error: No file path specified");
	}

	param params(Get_SettingThreshold(), Get_Filter_ClusterSize(), Get_Option_Red(), Get_Option_Subtract(), Get_Option_meanSubtract(), Get_Option_Fitting(), false, Get_Entry_num(), Get_EnteredPath());
	return params;
}

void MyMainFrame::init_ANALYZE(){
	if(ANA == nullptr) {
		ANA = std::make_unique<analysis>();
	}
	else {
		ANA->clear_pointer();
	}
	ANA->init_DataStructure();
}


int main(int argc, char **argv) {
    TApplication app("app", &argc, argv);
    MyMainFrame *mainFrame = new MyMainFrame(gClient->GetRoot(), 400, 400);
    app.Run();
    delete mainFrame;
    return 0;
}
