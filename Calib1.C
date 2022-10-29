
#include "TFile.h"
#include "TChain.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TCutG.h"
#include "TCanvas.h"
#include "TStopwatch.h"
#include "TText.h"
#include <TH2F.h>
#include <TGraph.h>
#include <TF1.h>
#include <TLegend.h>
#include <TMath.h>
#include <TAxis.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <TROOT.h>
#include <TRint.h>
#include <TSpectrum.h>
#include <math.h> // round

using namespace std;

Double_t gainP1 = 5.01077, offsetP1 = -12.22693;   // calibracion Detector a 170, 165 grados
Double_t gainP2 = 4.87673, offsetP2 = 32.48537;   // calibracion Detector a 160, 155 grados
Double_t gainP3 = 4.31651, offsetP3 = 24.46997;    // calibracion Detector a 150, 145 grados
Double_t gainP4 = 4.2429, offsetP4 = 21.57937;   // calibracion Detector a 140, 135 grados


void Calib1()
{


TStopwatch t;
   
   TFile *finput = new TFile("uncalib_P1.root","read");
   TFile *output = new TFile("calib_P1.root","recreate");

   TTree *tree = (TTree*)finput->Get("tree");

   Int_t nfiles=43;

   TH1F *CalibDetectors[nfiles];
   TH1F *h[nfiles];
   TH1F *unCalibDetectors[nfiles];
   
   for(Int_t i = 0; i < nfiles; i++){
    
      TString name = Form("P1_hist%d",i);
      TString uncalib = Form("UncalibP1_hist%d",i);
      TString calib = Form("CalibP1_hist%d",i);

      h[i] = (TH1F*)finput->Get(name);
      h[i]->Rebin();

         unCalibDetectors[i] = new TH1F(uncalib, uncalib,2048,0,2048);

         for(Int_t j = 1; j < h[i]->GetNbinsX(); j++){
             Int_t n;
             n = h[i]->GetBinContent(j);

           unCalibDetectors[i]->SetBinContent(j,n);

         }

         CalibDetectors[i] = new TH1F(calib, calib ,unCalibDetectors[i]->GetNbinsX(),unCalibDetectors[i]->GetXaxis()->GetXmin(),gainP1*unCalibDetectors[i]->GetXaxis()->GetXmax() + offsetP1);
         
         for(Int_t j = 1; j < unCalibDetectors[i]->GetNbinsX(); j++){
             Int_t n;
             Double_t k;
             n = unCalibDetectors[i]->GetBinContent(j);

             CalibDetectors[i]->SetBinContent(j,n);

         }    

   }


   for(Int_t i = 0; i < nfiles; i++){

      unCalibDetectors[i]->Write();
      
   }


   for(Int_t i = 0; i < nfiles; i++){

      CalibDetectors[i]->SetLineColor(kRed); 
      CalibDetectors[i]->GetXaxis()->SetRangeUser(150,2400);
      CalibDetectors[i]->Write();
      
   }

// Calculo de la integral Detector 1 restando el trapecio.
// Los intervalos de integracion se recolectaron manualmente para el pico de Al y Au.
// Detector 1
   
   Double_t bin_al1[43] = {254, 243, 233, 226, 216, 204, 196, 188, 178, 170, 166, 158, 151, 141, 133, 133, 142, 151, 160, 168, 177, 
                           189, 194, 201, 213, 216, 225, 236, 246, 254, 262, 274, 284, 288, 297, 309, 317, 327, 336, 346, 354, 362, 361 };

   Double_t bin_al2[43] = {270, 263, 252, 245, 239, 230, 221, 212, 202, 195, 188, 175, 166, 158, 151, 149, 156, 164, 174, 183, 192, 
                           205, 215, 221, 229, 241, 250, 258, 268, 275, 285, 293, 304, 308, 319, 327, 338, 343, 355, 363, 370, 380, 381 };

   Double_t bin_au1[43] = {290, 278, 265, 257, 244, 233, 221, 212, 203, 195, 190, 181, 170, 162, 152, 149, 162, 172, 181, 189, 200, 
                           213, 220, 230, 239, 245, 256, 267, 277, 288, 295, 308, 320, 327, 339, 348, 360, 370, 378, 390, 400, 407, 409 };

   Double_t bin_au2[43] = {308, 297, 288, 277, 271, 265, 255, 246, 233, 228, 219, 206, 195, 187, 176, 168, 179, 186, 201, 208, 221, 
                           239, 242, 253, 263, 277, 286, 295, 304, 312, 325, 335, 347, 354, 363, 371, 383, 395, 402, 412, 420, 431, 432 };

   Double_t a_al[43], c_al[43], h_al[43]; 
   Double_t a_au[43], c_au[43], h_au[43];
   Double_t A_al[43], A_au[43], I_al[43], I_au[43];

   for(Int_t i = 0; i < 43; i++){

      a_al[i] = CalibDetectors[i]->GetBinContent(bin_al1[i]);
      c_al[i] = CalibDetectors[i]->GetBinContent(bin_al2[i]);
      h_al[i] = bin_al2[i] - bin_al1[i];
      A_al[i] = (a_al[i]+c_al[i])*h_al[i]/2;  // Area_Trapecio = (a+c)*h/2
      I_al[i] = CalibDetectors[i]->Integral(bin_al1[i],bin_al2[i]);

      a_au[i] = CalibDetectors[i]->GetBinContent(bin_au1[i]);
      c_au[i] = CalibDetectors[i]->GetBinContent(bin_au2[i]);
      h_au[i] = bin_au2[i] - bin_au1[i];
      A_au[i] = (a_au[i]+c_au[i])*h_au[i]/2;
      I_au[i] = CalibDetectors[i]->Integral(bin_au1[i],bin_au2[i]);

     //cout << i << ", h_al[i]:" << h_al[i] <<  ", h_au[i]: " << h_au[i]<< endl;
     //cout << round(I_al[i]) <<"\t"<< round(A_al[i]) <<  "\t" << round(I_au[i]) <<"\t"<< round(A_au[i]) << endl;
   
      cout << round(I_al[i] - A_al[i]) <<  "\t" << round(I_au[i] - A_au[i]) << endl;

   }


   finput->Close();
   output->Close();

   delete finput;
   delete output;

   t.Print();


}