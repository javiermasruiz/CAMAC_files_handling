
#include <iostream>
#include <fstream>

#include <TH2F.h>
#include <TGraph.h>
#include <TF1.h>
#include <TLegend.h>
#include <TMath.h>
#include <TAxis.h>
#include <stdio.h>
#include <TROOT.h>
#include <TRint.h>
#include <TSpectrum.h>
#include <math.h> // round

using namespace std;


void integral()
{

 TStopwatch t;
   
   TFile *finput = new TFile("calib_P1.root","read");

   TTree *tree = (TTree*)finput->Get("tree");

   Int_t nfiles=43;

   TH1F *h[nfiles];
   TH1F *hist[nfiles];
   
   for(Int_t i = 0; i < nfiles; i++){

      TString calibname = Form("CalibP1_hist%d",i);
      TString histname = Form("hist%d",i);

      h[i] = (TH1F*)finput->Get(calibname);

      hist[i] = new TH1F(histname, histname,2048,0,2048);

         for(Int_t j = 1; j < h[i]->GetXaxis()->GetNbins(); j++){
             Int_t n;
             n = h[i]->GetBinContent(j);
             hist[i]->SetBinContent(j,n);
        }      

   }

      TCanvas *c1 = new TCanvas("c1","c1",800,600);

      hist[0]->SetLineColor(kBlack); 
      hist[0]->GetXaxis()->SetRangeUser(150,2150);
      hist[0]->Draw();

      cout << " Todo bien..." << endl;
         
   //finput->Close();
  
   //delete finput;
   

   t.Print();


}