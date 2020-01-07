void plot_spectra_ratio()
{
  gROOT->Reset();

  const Double_t MassDs = 1.9685;
  const Int_t NC = 3;
  const Int_t NPt = 4;
  double pT[NC][NPt], y[NC][NPt], ye[NC][NPt], yes[NC][NPt], yet[NC][NPt];


  TFile *fin = new TFile("rootfiles/DsoverD0_AndSpectrum.root");
  TGraphAsymmErrors *gr_stat_tmp[NC];
  TGraphAsymmErrors *gr_sys_tmp[NC];
  for(int i=0;i<NC;i++) {
    gr_stat_tmp[i] = (TGraphAsymmErrors *)fin->Get(Form("Ds_over_D0_Combined_stat%d",i));
    gr_sys_tmp[i] = (TGraphAsymmErrors *)fin->Get(Form("Ds_over_D0_Combined_sys%d",i));
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
  const Int_t kMarkerStyle[NC] = {20, 24, 22};
  const Int_t kColor[NC] = {1,2,4};
  for(int i=0;i<NC;i++) {
    gr_stat[i] = new TGraphErrors(NPt, pT[i], y[i], 0, ye[i]);
    gr_stat[i]->SetMarkerSize(2.0);
    gr_stat[i]->SetMarkerColor(kColor[i]);
    gr_stat[i]->SetLineWidth(2);
    gr_stat[i]->SetLineColor(kColor[i]);
    gr_stat[i]->SetMarkerStyle(kMarkerStyle[i]);
    
    gr_total[i] = new TGraphErrors(NPt, pT[i], y[i], 0, yet[i]);
  }

  // pythia
  TFile *f_p = new TFile("rootfiles/ALICE_pp7TeV_PbPb5TeV_Pythia.root");
  TH1D *h_p = (TH1D *)f_p->Get("Pythia_Monash_tune_200GeV");

  TH1D *h_p_LHC = (TH1D *)f_p->Get("Pythia_Monash_tune_7TeV");
  TGraphAsymmErrors *gr_pp7TeV = (TGraphAsymmErrors *)f_p->Get("pp7GeV");  

  // TCanvas *c1 = new TCanvas("c1", "c1",0,0,800,800);
  TCanvas *c1 = new TCanvas("c1", "c1",0,0,800,600);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0.001);
  c1->SetFillColor(10);
  c1->SetFillStyle(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(0);
  c1->SetFrameFillColor(10);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  //  c1->SetLogy();
  c1->SetGridx(0);
  c1->SetGridy(0);
  c1->SetLeftMargin(0.20);
  c1->SetBottomMargin(0.16);
  c1->SetTopMargin(0.02);
  c1->SetRightMargin(0.02);

  double x1 = 0.2;
  double x2 = 7.5;
  double y1 = 0.0;
  double y2 = 0.8;
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
  h0->GetYaxis()->SetNdivisions(305);
  h0->GetYaxis()->CenterTitle();
  h0->GetYaxis()->SetTitle("(D_{?}^{+}+D_{?}^{-})/(D^{0}+#bar{D}^{0})");
  h0->GetYaxis()->SetTitleOffset(1.2);
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

  h_p->SetLineColor(8);
  h_p->SetLineWidth(3);
  h_p->Draw("samec");

  h_p_LHC->SetLineColor(6);
  h_p_LHC->SetLineWidth(3);
  h_p_LHC->Draw("samec");
  
  const Int_t kLineStyle[NC] = {1, 2, 1};
  for(int i=0;i<NC;i++) {

    for(int j=0;j<NPt;j++) {
      double x1 = pT[i][j] - 0.1;
      double x2 = pT[i][j] + 0.1;
      double y1 = y[i][j] - yes[i][j];
      double y2 = y[i][j] + yes[i][j];
            
      TLine *la = new TLine(x1, y1, x1, y1+0.015);
      la->SetLineColor(kColor[i]);
      la->Draw("same");
      TLine *lb = new TLine(x2, y1, x2, y1+0.015);
      lb->SetLineColor(kColor[i]);
      lb->Draw("same");
      TLine *lc = new TLine(x1, y2, x1, y2-0.015);
      lc->SetLineColor(kColor[i]);
      lc->Draw("same");
      TLine *ld = new TLine(x2, y2, x2, y2-0.015);
      ld->SetLineColor(kColor[i]);
      ld->Draw("same");
      TLine *le = new TLine(x1, y1, x2, y1);
      le->SetLineColor(kColor[i]);
      le->SetLineWidth(2);
      le->Draw("same");
      TLine *lf = new TLine(x1, y2, x2, y2);
      lf->SetLineColor(kColor[i]);
      lf->SetLineWidth(2);
      lf->Draw("same");
    }
    
    gr_stat[i]->Draw("p");
  }

  double x1 = 0.2;
  double x2 = 7.5;
  double y1 = 0.0;
  double y2 = 0.8;
  
  // TLatex *tex = new TLatex(0.5, y2*0.9, "STAR Au+Au#rightarrow D_{?}^{#pm}+X,  #sqrt{s_{NN}} = 200 GeV");
  TLatex *tex = new TLatex(0.5, y2*0.9, "STAR Au+Au #sqrt{s_{NN}} = 200 GeV");
  tex->SetTextFont(42);
  tex->SetTextSize(0.055);
  tex->Draw("same");


  const Char_t *txt[NC] = {"0-10%","10-40%","40-80%"};
  TLegend *leg = new TLegend(0.25, 0.64, 0.53, 0.88);
  leg->SetFillStyle(0);
  leg->SetLineStyle(0);
  leg->SetLineColor(10);
  leg->SetLineWidth(0.);
  leg->SetTextSize(0.045);
  for(int i=0;i<NC;i++) {
    leg->AddEntry(gr_stat[i],txt[i],"pl");
  }
  leg->Draw();
    
  TLatex *tex = new TLatex(5.0, 0.12, "PYTHIA8.2");
  tex->SetTextFont(42);
  tex->SetTextAngle(3);
  tex->SetTextSize(0.05);
  tex->Draw("same");

  c1->Update();

  c1->SaveAs("Fig/fig3_DsD0_ALICE.pdf");
  c1->SaveAs("Fig/fig3_DsD0_ALICE.png");
  
  
}
