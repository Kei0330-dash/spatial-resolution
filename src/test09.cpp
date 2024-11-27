#include "TApplication.h"
#include "TGClient.h"
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"

// イベントハンドラー
void ButtonClicked() {
    printf("Button was clicked!\n");
}

int main(int argc, char **argv) {
    TApplication app("App", &argc, argv);

    // メインフレームの作成
    TGMainFrame *mainFrame = new TGMainFrame(gClient->GetRoot(), 800, 600);

    // キャンバスの埋め込み
    TRootEmbeddedCanvas *embeddedCanvas = new TRootEmbeddedCanvas("Canvas", mainFrame, 800, 600);
    mainFrame->AddFrame(embeddedCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    // ボタンの作成
    TGTextButton *button = new TGTextButton(mainFrame, "Click Me!");
    button->Connect("Clicked()", "ButtonClicked()", 0, 0);
    mainFrame->AddFrame(button, new TGLayoutHints(kLHintsCenterX));

    // メインフレームの設定
    mainFrame->SetWindowName("ROOT GUI Example");
    mainFrame->MapSubwindows();
    mainFrame->Resize(mainFrame->GetDefaultSize());
    mainFrame->MapWindow();

    app.Run();

    return 0;
}
