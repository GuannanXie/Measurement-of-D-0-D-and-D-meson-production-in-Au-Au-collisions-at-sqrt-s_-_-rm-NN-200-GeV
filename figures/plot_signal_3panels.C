std::pair<float, float> gMassExclusionRange(1.80, 1.92);
Double_t funResidualBg(Double_t *x, Double_t *par)
{
   if (x[0] > gMassExclusionRange.first && x[0] < gMassExclusionRange.second)
   {
      TF1::RejectPoint();
      return 0;
   }

   return par[0] + par[1] * x[0];
}

TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex)
{
    TLatex *latex = new TLatex(x,y,text);
    latex->SetNDC();
    latex->SetTextFont(textFont);
    latex->SetTextSize(textSize);
    latex->SetTextColor(colorIndex);
    latex->Draw("same");
    return latex;
}

void plot_signal_3panels()
{
   // globalSetting();
   gStyle->Reset("plain");
   char buf[250];
   char dir[250];
   char name[250];
   char title[250];
   char CMD[250];
   TLegend* legend;
   char lname[16][250];
   TPaveStats* ptstates;
   TH1F* htemp;
   TH1F* h0;
   TH1F* h1;
   const double PI = TMath::Pi();

   char abc[3][250] = {"(a)", "(b)", "(c)"};

   //fit and plot
   TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 1100, 450);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetEndErrorSize(0.01);
   c1->SetFillColor(10);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameFillColor(0);
   c1->SetFrameBorderMode(0);
   c1->SetLogy();
   c1->SetLeftMargin(0.18);
   c1->SetBottomMargin(0.15);
   c1->SetTopMargin(0.025);
   c1->SetRightMargin(0.06);

   TPad *pad[3];
   double x1[3] = {0,    1. / 3,  2. / 3 };
   double y1[3] = {0,    0,     0    };
   double x2[3] = {1. / 3, 2. / 3,  1    };
   double y2[3] = {1,    1,     1    };
   double left[3] = {0.19,    0.10,     0.09  };
   double right[3] = {0.0,    0.02,     0.04  };
   double top[3] = {0.06,    0.06,     0.06 };
   double bottom[3] = {0.13,    0.13,     0.13 };

   for (int i = 0; i < 3; i++)
   {
      pad[i] = new TPad(Form("pad_%d", i), "", x1[i], y1[i], x2[i], y2[i]);
      pad[i]->SetFillStyle(4000);
      pad[i]->SetFillColor(10);
      pad[i]->SetBorderMode(0);
      pad[i]->SetBorderSize(0);
      pad[i]->SetFrameFillColor(0);
      pad[i]->SetFrameBorderMode(0);
      pad[i]->SetFrameBorderSize(0);
      pad[i]->SetFrameLineWidth(1);
      pad[i]->SetLeftMargin(left[i]);
      pad[i]->SetRightMargin(right[i]);
      pad[i]->SetTopMargin(top[i]);
      pad[i]->SetBottomMargin(bottom[i]);
      pad[i]->SetTickx();
      pad[i]->SetTicky();
      pad[i]->Draw();

   }


   TF1 *fD0 = new TF1("fD0", "gausn(0)*[3]+pol1(4)", 1.6, 2.1);
   TF1 *resifunFit = new TF1("resifunFit", funResidualBg, 1.6, 2.1, 2);
   resifunFit->SetNpx(501);
   resifunFit->SetLineColor(kRed);
   resifunFit->SetLineWidth(2);
   fD0->SetParNames("N", "#mu", "#sigma", "BinWidth", "A0", "A1");
   float fitRange_lw = 1.72;
   float fitRange_up = 2.1;

   int index = 0;
   // int ipt[3] = {10, 10, 12};
   int ipt[1] = {11};
   TH1F* hInvMassUL[3];
   TH1F* hInvMassLS[3];
   TH1F* hInvMassMixUL[3];
   TH1F* hInvMass[3];
   TFile* fin = new TFile("./rootfiles/D0_HFT_signal.root");


   // const int ncent = 6;
   // char nameCent[ncent][250] = {"0-10%","10-40%","40-80%","40-60%","60-80%","0-80%"};
   // char nameCent1[ncent][250] = {"0_10", "10_40", "40_80", "40_60", "60_80", "0_80"};
   const int ncent = 3;
   int mCent[3] = {5, 0, 4}; //0-80, 0-10, 60-80
   char nameCent[ncent][250] = {"0-80%","0-10%","60-80%"};
   char nameCent1[ncent][250] = {"0_80", "0_10", "60_80"};

   fin->cd(); //for 0-10GeV
   for (int index = 0; index < 3; index++)
   {
      for (int iipt = 0; iipt < 1; iipt++)
      {
         if (iipt != 0) continue;

         sprintf(name, "%s/hInvMassUL_%i_%i", nameCent1[index], mCent[index], ipt[iipt]);
         hInvMassUL[index] = (TH1F*)fin->Get(name);
         hInvMassUL[index]->SetDirectory(0);
         sprintf(name, "%s/hInvMassLS_%i_%i", nameCent1[index], mCent[index], ipt[iipt]);
         hInvMassLS[index] = (TH1F*)fin->Get(name);
         hInvMassLS[index]->SetDirectory(0);
         sprintf(name, "%s/hInvMassMixUL_%i_%i", nameCent1[index], mCent[index], ipt[iipt]);
         hInvMassMixUL[index] = (TH1F*)fin->Get(name);
         hInvMassMixUL[index]->SetDirectory(0);
         sprintf(name, "%s/hInvMassMix_%i_%i", nameCent1[index], mCent[index], ipt[iipt]);// unlike sign - mixed unlike
         hInvMass[index] = (TH1F*)fin->Get(name);
         hInvMass[index]->SetDirectory(0);
         cout << "index = " << index << " , " << hInvMassUL[index]->GetName() << endl;
      }
   }
   fin->Close();


   double pt1[3] = {0, 0, 0};
   double pt2[3] = {10, 10, 8};
   int digits[3] = {3, 3, 3};

   double min[3] = {-2e3, -1e3, -0.5e2};

   for (int i = 0 ; i < 3; i++)
   {
      Float_t mean, sigma, lowx, higx;
      double S, B;
      pad[i]->cd();
      pad[i]->SetLogy(0);

      TGaxis::SetMaxDigits(digits[i]);
      hInvMassUL[i]->Draw("E");
      hInvMassUL[i]->SetTitle("");
      hInvMassUL[i]->GetXaxis()->SetTitle("M_{K#pi} (GeV/c^{2})");
      hInvMassUL[i]->GetYaxis()->SetTitle("Counts (per 10 MeV/c^{2})");
      if (i != 0) hInvMassUL[i]->GetYaxis()->SetTitle("");
      hInvMassUL[i]->SetMaximum(1.4 * hInvMassUL[i]->GetMaximum());
      if (i == 2) hInvMassUL[i]->SetMaximum(1.2 * hInvMassUL[i]->GetMaximum());
      cout << " ****** " << hInvMassUL[i]->GetMinimum() << endl;
      hInvMassUL[i]->SetMinimum(min[i]);
      hInvMassUL[i]->GetXaxis()->SetRangeUser(1.65, 2.08);
      hInvMassUL[i]->GetXaxis()->CenterTitle();
      hInvMassUL[i]->GetYaxis()->CenterTitle();
      hInvMassUL[i]->GetXaxis()->SetTitleSize(0.06);
      hInvMassUL[i]->GetXaxis()->SetLabelOffset(0.005);
      hInvMassUL[i]->GetXaxis()->SetLabelSize(0.055);
      hInvMassUL[i]->GetXaxis()->SetLabelFont(42);
      hInvMassUL[i]->GetXaxis()->SetTitleFont(42);
      hInvMassUL[i]->GetXaxis()->SetTitleOffset(0.9);
      hInvMassUL[i]->GetYaxis()->SetTitleOffset(1.5);
      hInvMassUL[i]->GetYaxis()->SetTitleSize(0.06);
      hInvMassUL[i]->GetYaxis()->SetLabelSize(0.06);
      hInvMassUL[i]->GetYaxis()->SetLabelOffset(0.018);
      hInvMassUL[i]->GetYaxis()->SetLabelFont(42);
      hInvMassUL[i]->GetYaxis()->SetTitleFont(42);
      hInvMassUL[i]->GetXaxis()->SetNdivisions(505);
      hInvMassUL[i]->GetYaxis()->SetNdivisions(505);
      hInvMassUL[i]->SetMarkerStyle(24);
      hInvMassUL[i]->SetMarkerSize(0.8);
      hInvMassUL[i]->SetMarkerColor(1);
      hInvMassUL[i]->SetLineWidth(2);
      hInvMassUL[i]->SetLineColor(1);

      hInvMassMixUL[i]->SetFillColor(kGray + 2);
      hInvMassMixUL[i]->SetLineColor(kGray + 2);
      hInvMassMixUL[i]->SetFillStyle(3003);
      hInvMassMixUL[i]->SetLineWidth(1);
      hInvMassMixUL[i]->Draw("HSAME");

      hInvMassLS[i]->SetLineColor(4);
      hInvMassLS[i]->SetLineWidth(2);
      hInvMassLS[i]->Draw("SAME");

      hInvMass[i]->SetLineColor(2);
      hInvMass[i]->SetLineWidth(2);
      hInvMass[i]->SetMarkerStyle(20);
      hInvMass[i]->SetMarkerColor(2);
      hInvMass[i]->SetMarkerSize(0.8);
      hInvMass[i]->Draw("PSAME");

      if (i == 0) drawLatex(0.22, 0.86, "STAR Au+Au #sqrt{s_{NN}} = 200 GeV", 42, 0.055, 1);
      if (i == 0) drawLatex(0.22, 0.78, Form("%.1f < p_{T} < %.1f GeV/c", pt1[i], pt2[i]), 42, 0.058, 1);
      if (i == 1) drawLatex(0.18, 0.88, Form("%.1f < p_{T} < %.1f GeV/c", pt1[i], pt2[i]), 42, 0.058, 1);
      if (i == 2) drawLatex(0.18, 0.88, Form("%.1f < p_{T} < %.1f GeV/c", pt1[i], pt2[i]), 42, 0.058, 1);

      drawLatex(0.7, 0.68, Form("%s", nameCent[i]), 42, 0.06, 4);
      drawLatex(0.70, 0.50, Form("D^{0}"), 42, 0.065, 1);

      if (i == 1)
      {
         TLegend* legend2 = new TLegend(0.18, .71, .5, .85);
         legend2->SetTextFont(42);
         legend2->SetTextSize(0.05);
         legend2->SetBorderSize(0);
         legend2->SetFillStyle(0);
         legend2->AddEntry(hInvMassUL[i], " US (SE)", "p");
         legend2->AddEntry(hInvMassLS[i], " LS (SE)", "lep");
         legend2->Draw();
      }

      if (i == 2)
      {
         TLegend* legend2 = new TLegend(0.18, .71, .5, .85);
         legend2->SetTextFont(42);
         legend2->SetTextSize(0.05);
         legend2->SetBorderSize(0);
         legend2->SetFillStyle(0);
         legend2->AddEntry(hInvMassMixUL[i], " US (ME)", "fl");
         legend2->AddEntry(hInvMass[i], " US(SE) - US(ME)", "lep");
         legend2->Draw();
      }


      // Fit residual bg and signal (gauss + pol1)
      float N = hInvMass[i]->GetBinContent(hInvMass[i]->FindBin(1.86));
      resifunFit->SetParameters(0.5 * N, -0.5 * N);
      hInvMass[i]->Fit(resifunFit, "INOR", "", fitRange_lw, fitRange_up);
      hInvMass[i]->Fit(resifunFit, "INOR", "", fitRange_lw, fitRange_up);
      fD0->SetParameters(N, 1.866, 0.014, hInvMass[i]->GetBinWidth(4), resifunFit->GetParameter(0), resifunFit->GetParameter(1));
      fD0->SetParLimits(1, 1.85, 1.88);// Mass
      fD0->SetParLimits(2, 0.003, 0.03);// width
      fD0->FixParameter(3, hInvMass[i]->GetBinWidth(4));
      fD0->FixParameter(4, resifunFit->GetParameter(0));
      fD0->FixParameter(5, resifunFit->GetParameter(1));
      hInvMass[i]->Fit(fD0, "INOR", "", fitRange_lw, fitRange_up);
      hInvMass[i]->Fit(fD0, "INOR", "", fitRange_lw, fitRange_up);

      fD0->SetLineColor(2);

      hInvMass[i]->Draw("Esame");
      fD0->SetRange(fitRange_lw, fitRange_up);
      fD0->DrawCopy("same");


      float x11 = 1.65;
      float x22 = 2.079;
      float y11 = min[i];
      float y22 = hInvMassUL[i]->GetMaximum();
      TLine *l1 = new TLine(x11, y11, x22, y11);
      l1->SetLineWidth(2);
      l1->Draw("same");
      TLine *l2 = new TLine(x11, y22, x22, y22);
      l2->SetLineWidth(2);
      l2->Draw("same");
      TLine *l3 = new TLine(x11, y11, x11, y22);
      l3->SetLineWidth(2);
      l3->Draw("same");
      TLine *l4 = new TLine(x22, y11, x22, y22);
      l4->SetLineWidth(2);
      l4->Draw("same");

      drawLatex(0.85, 0.35, Form("%s", abc[i]), 42, 0.073, 1);
   }

   c1->SaveAs("Fig/signal_3panel.pdf");
   c1->SaveAs("Fig/signal_3panel.png");

}
