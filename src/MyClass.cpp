#include "../include/MyClass.hpp"
#include <vector>
int MyClass::Get_EntryMax(){
	// if(fChain == nullptr) return;
	int ans = fChain->GetEntriesFast();
	return ans;
}

ADC_DATA MyClass::Get_ADC(Int_t entry_num){
	Long64_t nentries;
	//エントリー数を指定する
	// if (fChain == nullptr) return;
	nentries = fChain->GetEntriesFast();
	fChain->GetEntry(entry_num);

	ADC_DATA res(128, std::vector<UShort_t>(128));
	for(int i = 0; i < 128; i++){
		for(int j = 0; j < 128; j++){
			res[i][j] = ADC[i][j];
		}
	}
	return res;
}