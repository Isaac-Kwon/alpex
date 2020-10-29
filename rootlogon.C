{
    std::cout << "========================================" <<std::endl<<std::endl;
    std::cout << "ALPIDE Clustering Analysis Framework" <<std::endl;
    std::cout <<std::endl<< "========================================" <<std::endl;
    TString dirname = TString(gSystem->GetWorkingDirectory());
    gInterpreter->AddIncludePath(TString::Format("%s/include", dirname.Data()));
    gSystem->AddIncludePath(TString::Format("%s/include", dirname.Data()));
}