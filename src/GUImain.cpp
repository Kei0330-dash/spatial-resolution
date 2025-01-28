#include "../include/GUImain.hpp"
#include "../include/analysis.hpp"
#include <TApplication.h>
#include <TGClient.h>
#include <TGFileDialog.h>
#include <iostream>  // std::coutを使用するために追加

const char *fileTypes[] = {"All files", "*", nullptr};  // 配列を定義

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) 
    : TGMainFrame(p, w, h) {
    // 垂直フレームを作成
    TGVerticalFrame *mainFrame = new TGVerticalFrame(this, w, h);

    // 数値入力フィールドを含む水平フレームを作成
    TGHorizontalFrame *numFrame = new TGHorizontalFrame(mainFrame, 200, 50);
    numEntry = new TGNumberEntry(numFrame, 0, 9, -1, TGNumberFormat::kNESInteger);
    numFrame->AddFrame(numEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    mainFrame->AddFrame(numFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    // チェックボックスを含む垂直フレームを作成
    TGVerticalFrame *checkBoxFrame = new TGVerticalFrame(mainFrame, 200, 100);
    Option_Red = new TGCheckButton(checkBoxFrame, "Option_Red");
    Option_Substract = new TGCheckButton(checkBoxFrame, "Option_Substract");
    Option_Fitting = new TGCheckButton(checkBoxFrame, "Option_Fitting");
    checkBoxFrame->AddFrame(Option_Red, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    checkBoxFrame->AddFrame(Option_Substract, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    checkBoxFrame->AddFrame(Option_Fitting, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    mainFrame->AddFrame(checkBoxFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    // ボタンを含む水平フレームを作成
    TGHorizontalFrame *buttonFrame = new TGHorizontalFrame(mainFrame, 200, 50);
    Run_analysis = new TGTextButton(buttonFrame, "Run");
	SearchCluster_Button = new TGTextButton(buttonFrame, "SearchCluster");
    Open_file = new TGTextButton(buttonFrame, "Open File");
    buttonFrame->AddFrame(Run_analysis, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    buttonFrame->AddFrame(SearchCluster_Button, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    buttonFrame->AddFrame(Open_file, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 5));
    mainFrame->AddFrame(buttonFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    // パス入力フィールドを含む水平フレームを作成
    TGHorizontalFrame *pathFrame = new TGHorizontalFrame(mainFrame, 400, 50);
    pathEntry = new TGTextEntry(pathFrame, new TGTextBuffer(100));
    pathFrame->AddFrame(pathEntry, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    mainFrame->AddFrame(pathFrame, new TGLayoutHints(kLHintsCenterX | kLHintsTop));

    AddFrame(mainFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    Run_analysis->Connect("Clicked()", "MyMainFrame", this, "HandleButton()");
    SearchCluster_Button->Connect("Clicked()", "MyMainFrame", this, "SearchCluster()");
    Open_file->Connect("Clicked()", "MyMainFrame", this, "OpenFile()");

    SetWindowName("Settings screen");
    MapSubwindows();
    Resize(w, h);  // 具体的なサイズを指定
    MapWindow();
}

MyMainFrame::~MyMainFrame() {
    Cleanup();
}

void MyMainFrame::HandleButton() {
	TString path = Get_EnteredPath();
	if (path.Length() == 0) {
		std::cerr << "Error: No file path specified" << std::endl;
		return;
	}
    runMyClass(Get_Entry_num(), Get_Option_Red(), Get_Option_Substract(), Get_Option_Fitting(), false, path);
}

void MyMainFrame::SearchCluster() {
	TString path = Get_EnteredPath();
	if (path.Length() == 0) {
		std::cerr << "Error: No file path specified" << std::endl;
		return;
	}
    runMyClass(0, false, Get_Option_Substract(), false, true, path);
}

void MyMainFrame::OpenFile() {
    TString enteredPath = Get_EnteredPath();
    if (enteredPath.Length() > 0) {
        // ファイルパスが入力されている場合そのパスを消す
    }
	static TString dir(".");
	TGFileInfo fileInfo;
	fileInfo.fFileTypes = fileTypes;
	fileInfo.fIniDir = StrDup(dir);
	new TGFileDialog(gClient->GetDefaultRoot(), this, kFDOpen, &fileInfo);
	if (fileInfo.fFilename) {
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

bool MyMainFrame::Get_Option_Substract() {
    return Option_Substract->IsOn();
}

bool MyMainFrame::Get_Option_Fitting() {
    return Option_Fitting->IsOn();
}

int main(int argc, char **argv) {
    TApplication app("app", &argc, argv);
    MyMainFrame *mainFrame = new MyMainFrame(gClient->GetRoot(), 400, 400);
    app.Run();
    delete mainFrame;
    return 0;
}
