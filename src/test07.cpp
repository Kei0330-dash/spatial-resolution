#include <TApplication.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGFrame.h>

class MyMainFrame : public TGMainFrame {
public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();

    void CloseWindow();
};

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) 
    : TGMainFrame(p, w, h) {
    // チェックボックスの作成
    TGCheckButton *check = new TGCheckButton(this, "Check Me!");
    
    // チェックボックスをメインフレームに追加
    AddFrame(check, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    // メインフレームの設定と表示
    SetWindowName("CheckButton Example");
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
