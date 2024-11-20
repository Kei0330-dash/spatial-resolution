#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TCanvas.h>
#include <TGNumberEntry.h>

class MyMainFrame : public TGMainFrame {
public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();
    void HandleButton(){
        printf("Text button was clicked!\n");
    };
    void CloseWindow();
    bool Get_Option_Red(){
        return Option_Red->IsOn();
    };
    bool Get_Option_substract(){
        return Option_Substract->IsOn();
    };
    bool Get_Option_fitting(){
        return Option_Fitting->IsOn();
    };
private:
    TGCheckButton *Option_Red;
    TGCheckButton *Option_Substract;
    TGCheckButton *Option_Fitting;
    TGTextButton *Run_analysis; 
};

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) 
    : TGMainFrame(p, w, h) {
    // フレームの作成
    TGCompositeFrame *frame = new TGCompositeFrame(this, 200, 200);
    // 数字入力ボックスの作成
    TGNumberEntry *numEntry = new TGNumberEntry(frame, 0, 9, -1, TGNumberFormat::kNESInteger);
    // 数字入力ボックスをフレームに追加
    frame->AddFrame(numEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    // チェックボックスの作成
    Option_Red = new TGCheckButton(this, "Option_Red");
    Option_Substract = new TGCheckButton(this, "Option_Substract");
    Option_Fitting = new TGCheckButton(this, "Option_Fitting");
    //テキストボタンの作成
    Run_analysis = new TGTextButton(this, "Run");
    // シグナルとスロットの接続
    Run_analysis->Connect("Clicked()", "MyMainFrame", this, "HandleButton()");
    // チェックボックスをメインフレームに追加
    AddFrame(frame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Option_Red, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Option_Substract, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Option_Fitting, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Run_analysis, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    // メインフレームの設定と表示
    SetWindowName("Settings screen");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

MyMainFrame::~MyMainFrame() {
    Cleanup();
}

void MyMainFrame::CloseWindow() {
    gApplication->Terminate(0);
}


int main(int argc, char **argv) {
    // ROOTアプリケーションの作成
    TApplication app("app", &argc, argv);
    // メインフレームの作成
    MyMainFrame *mainFrame = new MyMainFrame(gClient->GetRoot(), 800, 600);
    // アプリケーションの実行
    app.Run();
    // メインフレームの削除
    delete mainFrame;
    return 0;
}
