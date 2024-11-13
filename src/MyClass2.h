//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 10 13:40:09 2024 by ROOT version 6.30/04
// from TTree SOFIST_Data/SOFIST_Data_Tree
// found on file: SOFIST3_DATA_HV55V_chip1_alpha_240703.root
//////////////////////////////////////////////////////////

#ifndef MyClass_h
#define MyClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
// Header file for the classes stored in the TTree if any.

class MyClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UShort_t        ADC[256][128];
   UShort_t        Header[5];

   // List of branches
   TBranch        *b_raw;   //!
   TBranch        *b_header;   //!

   MyClass(TTree *tree=0);
   virtual ~MyClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Gaus2D_fitting(double x_center, double y_center, TH2D* h2);
   virtual void     Loop(Int_t entry_num, bool opt_Red, bool opt_sub, bool fit_opt);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};


#ifdef MyClass_cxx
MyClass::MyClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../DATA/SOFIST3_DATA_HV55V_chip1_alpha_240703.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../DATA/SOFIST3_DATA_HV55V_chip1_alpha_240703.root");
      }
      f->GetObject("SOFIST_Data",tree);

   }
   Init(tree);
}

MyClass::~MyClass()
{
   /*このデストラクタのコメントアウトを外すとSegmantationFaultする可能性が高い。*/
   //if (!fChain) return;
   //delete fChain->GetCurrentFile();
}

Int_t MyClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MyClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MyClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ADC", ADC, &b_raw);
   fChain->SetBranchAddress("Header", Header, &b_header);
   Notify();
}

Bool_t MyClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MyClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MyClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MyClass_cxx
#endif // #ifndef MyClass_h
