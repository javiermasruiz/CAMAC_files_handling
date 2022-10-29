
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


void convert2simnra()
{

TStopwatch t;
   
   TFile *finput = new TFile("uncalib_P4.root","read");

   TTree *tree = (TTree*)finput->Get("tree");

   TH1F *h;
   TH1F *hout;

      h = (TH1F*)finput->Get("P4_hist0");

         hout = new TH1F("hout","hout",4096,0,4096);

        ofstream out;
		out.open("P4_hist0.txt",ios::out);
			if(out.fail()){
	 			cout <<" No se pudo abrir el archivo de Salida ";
	 	 		exit(1);
			}

         for(Int_t j = 1; j < h->GetNbinsX(); j++){
             Int_t n;
             n = h->GetBinContent(j);

           hout->SetBinContent(j,n);
           out << j << "\t" << n << endl;
           cout << j << "\t" << n << endl;

         }

	out.close();

   finput->Close();
   delete finput;


   t.Print();

}

