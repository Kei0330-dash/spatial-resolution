#include <TApplication.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGFileDialog.h>
#include <TGButton.h>
#include <TGTextView.h>
#include <TSystem.h>

class MyMainFrame : public TGMainFrame {
private:
    TGTextView *fTextView;
public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();
    void OpenFile();
};

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h) {
    // ボタンを追加
    TGTextButton *openButton = new TGTextButton(this, "Open File");
    AddFrame(openButton, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    openButton->Connect("Clicked()", "MyMainFrame", this, "OpenFile()");

    // テキストビューを追加
    fTextView = new TGTextView(this, w, h - 50);
    AddFrame(fTextView, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    // フレームを設定
    SetWindowName("File Open Example");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

MyMainFrame::~MyMainFrame() {
    Cleanup();
}

void MyMainFrame::OpenFile() {
    static TString dir(".");
    TGFileInfo fileInfo;
    fileInfo.fFileTypes = (const char *[]){"All files", "*", nullptr};
    fileInfo.fIniDir = StrDup(dir);
    new TGFileDialog(gClient->GetDefaultRoot(), this, kFDOpen, &fileInfo);
    if (fileInfo.fFilename) {
        dir = fileInfo.fIniDir;
        fTextView->LoadFile(fileInfo.fFilename);
    }
}

void example() {
    TApplication theApp("App", nullptr, nullptr);
    MyMainFrame *main = new MyMainFrame(gClient->GetRoot(), 800, 600);
    theApp.Run();
}

int main(int argc, char **argv) {
    example();
    return 0;
}
