#ifndef GUIMAIN_HPP
#define GUIMAIN_HPP

#include <TGButton.h>
#include <TGNumberEntry.h>

class MyMainFrame : public TGMainFrame {
public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();
    void HandleButton();
    void CloseWindow();
    int Get_Entry_num();
    bool Get_Option_Red();
    bool Get_Option_Substract();
    bool Get_Option_Fitting();

private:
    TGNumberEntry *numEntry;
    TGCheckButton *Option_Red;
    TGCheckButton *Option_Substract;
    TGCheckButton *Option_Fitting;
    TGTextButton *Run_analysis; 
    ClassDef(MyMainFrame, 0);
};

#endif // GUI_H
