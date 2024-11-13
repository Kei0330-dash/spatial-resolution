#include <TGClient.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TApplication.h>

class MyMainFrame : public TGMainFrame {
private:
    TGTextEntry *fTextEntry;

public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();
    void HandleButton();
};

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h)
    : TGMainFrame(p, w, h) {
    // ラベル
    TGLabel *label = new TGLabel(this, "Enter a value:");
    AddFrame(label, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));

    // テキストエントリー
    fTextEntry = new TGTextEntry(this, new TGTextBuffer(100));
    AddFrame(fTextEntry, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));

    // ボタン
    TGTextButton *button = new TGTextButton(this, "&Submit");
    button->Connect("Clicked()", "MyMainFrame", this, "HandleButton()");
    AddFrame(button, new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5));

    SetWindowName("Parameter Input");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

MyMainFrame::~MyMainFrame() {
    Cleanup();
}

void MyMainFrame::HandleButton() {
    const char *input = fTextEntry->GetText();
    printf("You entered: %s\n", input);
}

void MyMain() {
    TApplication theApp("App", 0, 0);
    new MyMainFrame(gClient->GetRoot(), 400, 200);
    theApp.Run();
}

int main(int argc, char **argv) {
    MyMain();
    return 0;
}
