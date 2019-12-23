void plot_spectrum()
{
  gROOT->Reset();

  const Double_t MassDs = 1.9685;
  const Int_t NC = 3;
  const Int_t NPt = 4;
  double pT[NC][NPt], y[NC][NPt], ye[NC][NPt], yes[NC][NPt], yet[NC][NPt];

  TF1 *fitfun[NC];
  // TF1 *fun_tmp = new TF1("fun_tmp",LevyFcn,1.0, 7.0, 4);
  TF1 *fun_tmp = new TF1("fun_tmp","(1/(2*TMath::Pi()))*[0]*([2]-1)*([2]-2)/([2]*[1]*([2]*[1]+[3]*([2]-2)))*TMath::Power(([2]*[1]+TMath::Sqrt(x[0]*x[0]+[3]*[3])-[3])/([2]*[1]),-[2])",1.0, 7.0);
  fun_tmp->SetParameters(0.01, 0.2, 5, MassDs);
  fun_tmp->FixParameter(3, MassDs);

  TFile *fin = new TFile("rootfiles/DsoverD0_AndSpectrum.root");
  TGraphAsymmErrors *gr_stat_tmp[NC];
  TGraphAsymmErrors *gr_sys_tmp[NC];
  for(int i=0;i<NC;i++) {
    gr_stat_tmp[i] = (TGraphAsymmErrors *)fin->Get(Form("Ds_pTSepctrum_Combined_stat%d",i));
    gr_sys_tmp[i] = (TGraphAsymmErrors *)fin->Get(Form("Ds_pTSepctrum_Combined_sys%d",i));
    gr_stat_tmp[i]->Print();

    for(int j=0;j<NPt;j++) {
      pT[i][j] = gr_stat_tmp[i]->GetX()[j];
      y[i][j] = gr_stat_tmp[i]->GetY()[j];
      ye[i][j] = (gr_stat_tmp[i]->GetEYhigh()[j]+gr_stat_tmp[i]->GetEYlow()[j])/2.;
      yes[i][j] = (gr_sys_tmp[i]->GetEYhigh()[j]+gr_sys_tmp[i]->GetEYlow()[j])/2.;
      yet[i][j] = sqrt(ye[i][j]*ye[i][j]+yes[i][j]*yes[i][j]);
    }
  }

  TGraphErrors *gr_stat[NC];
  TGraphErrors *gr_total[NC];
  const Int_t kMarkerStyle[NC] = {20, 24, 20};
  for(int i=0;i<NC;i++) {
    gr_stat[i] = new TGraphErrors(NPt, pT[i], y[i], 0, ye[i]);
    gr_stat[i]->SetMarkerSize(2.0);
    gr_stat[i]->SetMarkerColor(1);
    gr_stat[i]->SetLineWidth(2);
    gr_stat[i]->SetMarkerStyle(kMarkerStyle[i]);
    
    gr_total[i] = new TGraphErrors(NPt, pT[i], y[i], 0, yet[i]);
    gr_total[i]->Fit("fun_tmp","R");
    fitfun[i] = (TF1 *)fun_tmp->Clone();
    fitfun[i]->SetName(Form("Levy_%d",i));
  }

  TCanvas *c1 = new TCanvas("c1", "c1",0,0,800,800);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  //  gStyle->SetEndErrorSize(0);
  c1->SetFillColor(10);
  c1->SetFillStyle(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(0);
  c1->SetFrameFillColor(10);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetLogy();
  c1->SetGridx(0);
  c1->SetGridy(0);
  c1->SetLeftMargin(0.2.0);
  c1->SetBottomMargin(0.16);
  c1->SetTopMargin(0.02);
  c1->SetRightMargin(0.02);

  double x1 = 0.2;
  double x2 = 8.5;
  double y1 = 5e-8;
  double y2 = 1e-1;
  TH1 *h0 = new TH1D("h0","",1,x1, x2);
  h0->SetMinimum(y1);
  h0->SetMaximum(y2);
  h0->GetXaxis()->SetNdivisions(208);
  h0->GetXaxis()->CenterTitle();
  h0->GetXaxis()->SetTitle("Transverse Momentum p_{T} (GeV/c)");
  h0->GetXaxis()->SetTitleOffset(1.1);
  h0->GetXaxis()->SetTitleSize(0.06);
  h0->GetXaxis()->SetLabelOffset(0.01);
  h0->GetXaxis()->SetLabelSize(0.05);
  h0->GetXaxis()->SetLabelFont(42);
  h0->GetXaxis()->SetTitleFont(42);
  h0->GetYaxis()->SetNdivisions(303);
  h0->GetYaxis()->CenterTitle();
  h0->GetYaxis()->SetTitle("d^{2}N/2#pip_{T}dp_{T}dy (c^{2}/GeV^{2})");
  h0->GetYaxis()->SetTitleOffset(1.6);
  h0->GetYaxis()->SetTitleSize(0.06);
  h0->GetYaxis()->SetLabelOffset(0.015);
  h0->GetYaxis()->SetLabelSize(0.05);
  h0->GetYaxis()->SetLabelFont(42);
  h0->GetYaxis()->SetTitleFont(42);
  h0->Draw("c");

  TLine *l1 = new TLine(x1,y1,x2,y1);
  l1->SetLineWidth(2);
  l1->Draw("same");
  TLine *l2 = new TLine(x1,y2,x2,y2);
  l2->SetLineWidth(2);
  l2->Draw("same");
  TLine *l3 = new TLine(x1,y1,x1,y2);
  l3->SetLineWidth(2);
  l3->Draw("same");
  TLine *l4 = new TLine(x2,y1,x2,y2);
  l4->SetLineWidth(2);
  l4->Draw("same");

  const Int_t kLineStyle[NC] = {1, 2, 1};
  for(int i=0;i<NC;i++) {

    for(int j=0;j<NPt;j++) {
      double x1 = pT[i][j] - 0.15;
      double x2 = pT[i][j] + 0.15;
      double y1 = y[i][j] - yes[i][j];
      double y2 = y[i][j] + yes[i][j];
            
      TLine *la = new TLine(x1, y1, x1, y1*1.15);
      la->Draw("same");
      TLine *lb = new TLine(x2, y1, x2, y1*1.15);
      lb->Draw("same");
      TLine *lc = new TLine(x1, y2, x1, y2*0.85);
      lc->Draw("same");
      TLine *ld = new TLine(x2, y2, x2, y2*0.85);
      ld->Draw("same");
      TLine *le = new TLine(x1, y1, x2, y1);
      le->SetLineWidth(2);
      le->Draw("same");
      TLine *lf = new TLine(x1, y2, x2, y2);
      lf->SetLineWidth(2);
      lf->Draw("same");
    }
    
    fitfun[i]->SetLineWidth(2);
    fitfun[i]->SetLineColor(1);
    fitfun[i]->SetLineStyle(kLineStyle[i]);
    fitfun[i]->Draw("same");
    gr_stat[i]->Draw("p");
  }

  double x1 = 0.2;
  double x2 = 8.5;
  double y1 = 5e-8;
  double y2 = 1e-1;
  
  TLatex *tex = new TLatex(3., y2*0.3, "STAR Au+Au#rightarrow D_{s}^{#pm}+X");
  tex->SetTextFont(42);
  tex->SetTextSize(0.055);
  tex->Draw("same");
  TLatex *tex = new TLatex(4.5, y2*0.09, "#sqrt{s_{NN}} = 200 GeV");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->Draw("same");


  const Double_t x_txt[NC] = {7.1, 7.1, 7.1};
  const Double_t y_txt[NC] = {1e-6, 3e-7, 1.e-7};
  const Char_t *txt[NC] = {"0-10%","10-40%","40-80%"};
  for(int i=0;i<NC;i++) {
    TLatex *tex = new TLatex(x_txt[i], y_txt[i], txt[i]);
    tex->SetTextFont(42);
    tex->SetTextSize(0.042);
    tex->Draw("same");
  }

  TLine *ll1 = new TLine(0.6, y1*8, 1.3, y1*8);
  ll1->SetLineWidth(2);
  ll1->SetLineStyle(1);
  ll1->Draw("same");
  TLine *ll2 = new TLine(0.6, y1*6, 1.3, y1*6);
  ll2->SetLineWidth(2);
  ll2->SetLineStyle(2);
  ll2->Draw("same");
  TLatex *tex = new TLatex(1.5, y1*5, "Levy fit");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->Draw("same");

  

  c1->Update();

  c1->SaveAs("Fig/fig2_spectrum.pdf");
  c1->SaveAs("Fig/fig2_spectrum.png");
  
  
}
