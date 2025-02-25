#include "../include/output.hpp"

outputFrame::outputFrame(const TGWindow *p, const TGWindow *mainWindow, UInt_t w, UInt_t h, param parameter)
    : TGTransientFrame(p, mainWindow, w, h), params(parameter) {
    // 垂直フレームを作成
    TGVerticalFrame *mainFrame = new TGVerticalFrame(this, w, h);
    TGGroupFrame *output_Frame = new TGGroupFrame(mainFrame, "Output");

    TGLabel *thresholdLabel = new TGLabel(output_Frame, Form("Threshold:  %.2f", params.out.threshold));
    TGLabel *countLabel = new TGLabel(output_Frame, Form("Clusters Count:  %d", params.out.cluster_found));

    numEntry = new TGNumberEntry(output_Frame, 0, 9, -1, TGNumberFormat::kNESInteger , TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 2147483648);
    SizeLabel = new TGLabel(output_Frame, Form("ClusterXXX of Size:  Not Found"));
    CenterofGLabel = new TGLabel(output_Frame, Form("ClusterXXX's Center of Gravity:  (XXXX,   XXXX)"));

    numEntry->Connect("ValueSet(Long_t)", "outputFrame", this, "EntryChanged(Long_t)");

    output_Frame->AddFrame(thresholdLabel, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));
    output_Frame->AddFrame(countLabel, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));
    output_Frame->AddFrame(numEntry, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));
    output_Frame->AddFrame(SizeLabel, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));
    output_Frame->AddFrame(CenterofGLabel, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));
    mainFrame->AddFrame(output_Frame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));

    AddFrame(mainFrame, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 10, 10, 10));

    SetWindowName("Outputs screen");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();

	EntryChanged(numEntry->GetIntNumber());
}

outputFrame::~outputFrame() {
    Cleanup();
}

int outputFrame::Get_Entry_num() {
    return numEntry->GetIntNumber();
}

void outputFrame::EntryChanged(Long_t val) {
    int entry_num = Get_Entry_num();
	int max_size = params.out.cluster.size();
	if(entry_num < max_size){
		SizeLabel->SetText(Form("Cluster%d of Size:  %d", entry_num, params.out.cluster[entry_num].Get_pixelsize()));
		CenterofGLabel->SetText(Form("Cluster%d's Center of Gravity:  (%.2f, %.2f)", entry_num, params.out.cluster[entry_num].Get_xcenter(), params.out.cluster[entry_num].Get_ycenter()));
	}
	else{
		SizeLabel->SetText(Form("Cluster%d of Size:  Not Found", entry_num));
    	CenterofGLabel->SetText(Form("Cluster%d's Center of Gravity:  Not Found", entry_num));
	}
    SizeLabel->Layout();
    CenterofGLabel->Layout();
}
