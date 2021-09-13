#include "TStyle.h"
#include "TROOT.h"
#include "TColor.h"
#include "iostream"

TStyle* SetStyle(){// Add my own options here:
    TStyle* mcStyle = new TStyle("AXStyle","ALPEx Style");
    Style_t font = 42;
    Float_t labelfontsize = 0.02;
    Float_t titlefontsize = 0.02;
    // mcStyle->SetPalette(0.5,0); // avoid horrible default color schem 
    mcStyle->SetOptStat(0);
    mcStyle->SetOptTitle(0);
    mcStyle->SetOptDate(0);
    mcStyle->SetLabelSize(labelfontsize,"xyz"); // size of axis value font 
    mcStyle->SetTitleSize(titlefontsize,"xyz"); // size of axis title font 
    mcStyle->SetLabelFont(font,"xyz");
    mcStyle->SetTitleFont(font,"xyz");
    mcStyle->SetTitleOffset(1.2,"y"); // default canvas options
    mcStyle->SetCanvasDefW(500); 
    mcStyle->SetCanvasDefH(500);
    mcStyle->SetCanvasColor(0); // canvas... 
    mcStyle->SetCanvasBorderMode(0); 
    mcStyle->SetCanvasBorderSize(0); 
    mcStyle->SetPadBorderMode(0);
    mcStyle->SetPadBorderSize(0);
    mcStyle->SetPadBottomMargin(0.05); //margins... 
    mcStyle->SetPadTopMargin(0.05); 
    // mcStyle->SetPadLeftMargin(0.05); 
    mcStyle->SetPadRightMargin(0.20); 
    // mcStyle->SetPadRightMargin(0.05); 
    mcStyle->SetPadGridX(0); // grids, tickmarks 
    mcStyle->SetPadGridY(0); 
    mcStyle->SetPadTickX(0); 
    mcStyle->SetPadTickY(0); 
    mcStyle->SetFrameBorderMode(0); 
    mcStyle->SetPaperSize(20,24); // US letter size 
    mcStyle->SetLegendBorderSize(1);
    mcStyle->SetLegendFont(font);
    
    

    gROOT->SetStyle("AXStyle");

    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.30, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs] = { 0.00, 0.00, 0.57, 0.90, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.65, 0.95, 0.20, 0.00 };
    Double_t blue[NRGBs] = { 0.51, 0.55, 0.15, 0.00, 0.10 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    
    std::cout << "Styles are Set!" << std::endl;
    return mcStyle; 
}