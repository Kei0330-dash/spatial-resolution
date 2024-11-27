#include "TApplication.h"
#include "TCanvas.h"
#include "TButton.h"
#include "TText.h"

class MyApplication : public TApplication {
public:
    MyApplication(int &argc, char **argv) : TApplication("MyApp", &argc, argv) {
        // キャンバスを作成
        TCanvas *canvas = new TCanvas("canvas", "My Canvas", 200, 200, 600, 400);
        canvas->SetBorderSize(0);
        
        // ボタンを作成
        TButton *button = new TButton("Click Me", "OnButtonClicked()");
        button->SetMethod("Clicked()");
        button->SetText("Click Me");
        button->SetX1(0.1);
        button->SetY1(0.1);
        button->SetX2(0.3);
        button->SetY2(0.2);

        // テキストを作成
        TText *text = new TText(0.1, 0.5, "Hello, ROOT!");
        text->SetName("text");
        
        // ボタンをキャンバスに追加
        canvas->cd();
        button->Draw();
        text->Draw();
    }

    void OnButtonClicked() {
        TText *text = (TText*)gPad->FindObject("text");
        text->SetText(0.1, 0.5, "Button clicked!");
        gPad->Modified();
        gPad->Update();
    }
};

int main(int argc, char **argv) {
    MyApplication app(argc, argv);
    app.Run();
    return 0;
}
