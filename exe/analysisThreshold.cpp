#include "TThresholdScan.hpp"


int main(){
    TString defRawDir = "../../data/ThresholdScan/raw-data/";
    TString defProcDir = "../../data/ThresholdScan/processed/";


    // std::vector<TString> names = {
    //     "ThresholdScan_210802_105253",
    //     "ThresholdScan_210802_105844",
    //     "ThresholdScan_210802_160358",
    //     "ThresholdScan_210802_162227",
    //     "ThresholdScan_210802_213742",
    //     "ThresholdScan_210802_215840",
    //     "ThresholdScan_210802_220456"
    // };

    std::vector<TString> names = {"ThresholdScan_210907_144809"};

    for(Int_t i=0; i<names.size(); i++){
        auto thres2 = new TThresholdScan(defRawDir+names[i]+".dat", defProcDir+names[i]+".root");
        thres2->AnalyzeFileMT();
    }

}