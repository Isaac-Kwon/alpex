{
    std::cout << "========================================" <<std::endl<<std::endl;
    std::cout << "ALPIDE Clustering Analysis Framework" <<std::endl;\
    std::cout <<std::endl<< "========================================" <<std::endl<<std::endl;
    std::cout << ">> WARNING" << std::endl;
    std::cout << "Now you're in in directory \'AnalysisFramework/macro/analyze\', the input-output filepath may be different with macro\'s expectation." <<std::endl;
    std::cout << "If there's error with file I/O during running, run the macro in above directory (right below the \"AnalysisFramework\")" <<std::endl<<std::endl;

    TString dirname = TString(gSystem->GetWorkingDirectory());
    gInterpreter->AddIncludePath(TString::Format("%s/../../include", dirname.Data()));
    gSystem->AddIncludePath(TString::Format("%s/../../include", dirname.Data()));
}
