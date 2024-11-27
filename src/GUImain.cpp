#ifndef GUIMAIN_CXX
#define GUIMAIN_CXX

#include "GUImain.h"
#include <TApplication.h>
#include <TGClient.h>

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) 
    : TGMainFrame(p, w, h) {
    TGCompositeFrame *frame = new TGCompositeFrame(this, 200, 200);
    TGNumberEntry *numEntry = new TGNumberEntry(frame, 0, 9, -1, TGNumberFormat::kNESInteger);
    frame->AddFrame(numEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    Option_Red = new TGCheckButton(this, "Option_Red");
    Option_Substract = new TGCheckButton(this, "Option_Substract");
    Option_Fitting = new TGCheckButton(this, "Option_Fitting");
    Run_analysis = new TGTextButton(this, "Run");

    Run_analysis->Connect("Clicked()", "MyMainFrame", this, "HandleButton()");

    AddFrame(frame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Option_Red, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Option_Substract, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Option_Fitting, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    AddFrame(Run_analysis, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    SetWindowName("Settings screen");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

MyMainFrame::~MyMainFrame() {
    Cleanup();
}

void MyMainFrame::HandleButton() {
    printf("Text button was clicked!\n");
}

void MyMainFrame::CloseWindow() {
    gApplication->Terminate(0);
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
    MyMainFrame *mainFrame = new MyMainFrame(gClient->GetRoot(), 800, 600);
    app.Run();
    delete mainFrame;
    return 0;
}
#endif