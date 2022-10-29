
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TSpectrum.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <iostream>
#include <fstream>
#include <TAxis.h>
#include <TH2.h>
#include "TStopwatch.h"
#include <iostream>
#include <fstream>

using namespace std;

void HistoGenerator()
{
   TStopwatch t;

   TFile *output = new TFile("uncalib_P1.root","recreate");

   TTree *tree = new TTree("tree","tree");


   Int_t nfiles=43;

   TH1F *P1_hist[nfiles];
  //TH1F *P2_hist[nfiles];
   //TH1F *P3_hist[nfiles];
   //TH1F *P4_hist[nfiles];

   for (Int_t i = 0; i < nfiles; i++){

         TString name1 = Form("P1_hist%d",i);
         //TString name2 = Form("P2_hist%d",i);
         //TString name3 = Form("P3_hist%d",i);
        // TString name4 = Form("P4_hist%d",i);

         P1_hist[i] = new TH1F(name1, "Histogramas",4096,0,4096);
        // P2_hist[i] = new TH1F(name2, "Histogramas",4096,0,4096);
         //P3_hist[i] = new TH1F(name3, "Histogramas",4096,0,4096);
        //P4_hist[i] = new TH1F(name4, "Histogramas",4096,0,4096);

   }

Double_t val1, val2, val3, val4;

   for (Int_t i = 0; i < nfiles; i++){

         TString infile = Form("DATA/run00%d.txt",i);

         fstream in;
         in.open(infile,ios::in);

         if(in.fail()){
         cout <<" No se pudo abrir el archivo de Entrada ";
         exit(1);
         }        

            Int_t entries = 0;

            while(1){ 
                     
                      in >> val1 >> val2 >> val3 >> val4;

                       P1_hist[i]->Fill(val1);
                       //P2_hist[i]->Fill(val2);
                       // P3_hist[i]->Fill(val3);
                       //P4_hist[i]->Fill(val4);
              
               if(in.eof()) break;
               entries++;
              }

         in.close();
         //cout <<" => Numero de eventos: " << entries << endl; 
   
   }

   for(Int_t i = 0; i < nfiles; i++){

      P1_hist[i]->Write();
      //P2_hist[i]->Write();
     //P3_hist[i]->Write();
      //P4_hist[i]->Write();
   }


   t.Print();
}
