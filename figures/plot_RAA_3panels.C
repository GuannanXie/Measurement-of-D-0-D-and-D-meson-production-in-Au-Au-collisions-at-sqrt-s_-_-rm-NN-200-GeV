TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex)
{
   TLatex *latex = new TLatex(x, y, text);
   latex->SetNDC();
   latex->SetTextFont(textFont);
   latex->SetTextSize(textSize);
   latex->SetTextColor(colorIndex);
   latex->Draw("same");
   return latex;
}

TLine* drawLine(Double_t xlow, Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor)
{
   TLine *l1 = new TLine(xlow, ylow, xup, yup);
   l1->SetLineWidth(lineWidth);
   l1->SetLineColor(lineColor);
   l1->Draw("same");
   return l1;
}

void drawLines(Double_t xlow, Double_t ylow, Double_t xup, Double_t yup, Double_t lineWidth, Int_t lineColor)
{
   drawLine(xlow, ylow, xup, ylow, lineWidth, lineColor);
   drawLine(xlow, yup, xup, yup, lineWidth, lineColor);
   drawLine(xlow, ylow, xlow, yup, lineWidth, lineColor);
   drawLine(xup, ylow, xup, yup, lineWidth, lineColor);
}

void setpad(TPad *pad, float left, float right, float top, float bottom)
{
   pad->SetFillColor(10);
   pad->SetBorderMode(0);
   pad->SetBorderSize(0);
   pad->SetFrameFillColor(10);
   pad->SetFrameBorderMode(0);
   pad->SetFrameBorderSize(0);
   pad->SetLeftMargin(left);
   pad->SetRightMargin(right);
   pad->SetTopMargin(top);
   pad->SetBottomMargin(bottom);
}

void plot_RAA_3panels()
{
   // globalSetting();
   char dir[250];
   char name[250];
   char title[250];
   char buf[1024];
   //TString CMD = 0;
   char CMD[250];
   TLegend* legend1;
   TLegend* legend2;

   // sprintf(dir, "Fig");
   sprintf(CMD, "[ -d %s ] || mkdir -p %s", dir, dir);
   // gSystem->Exec(CMD);

   const int ncent = 3;
   const char nameCent[ncent][250] = {"0-10%", "10-40%", "40-80%"};
   const char nameCentXL[ncent][250] = {"0_10", "10_40", "40_80"};
   const float scale[ncent] = {1., 1., 1.};
   float Nbin[ncent] = {938.80170, 386.08527, 56.99229};
   float NbinErr[ncent] = {26.28048, 29.86811, 14.85246 };


   //Read spectra
   TGraphErrors* gD0err_xl[ncent];
   TGraphErrors* gD0sys_xl[ncent];
   TGraphAsymmErrors* gD0sys_xl_combine[ncent];
   TGraphAsymmErrors* gD0_RAA_pperr[ncent];
   TF1* fLevy[ncent];
   TFile* fin1 = new TFile("rootfiles/D0RAA_Run14HFT.root");
   for (int icent = 0; icent < ncent; icent++)
   {
      gD0err_xl[icent] = (TGraphErrors*)fin1->Get(Form("D0_RAA_err_%s", nameCentXL[icent]));
      gD0sys_xl[icent] = (TGraphErrors*)fin1->Get(Form("D0_RAA_sys_%s", nameCentXL[icent]));
      gD0_RAA_pperr[icent] = (TGraphAsymmErrors*)fin1->Get(Form("D0_RAA_pperr_%s", nameCentXL[icent]));
      gD0sys_xl_combine[icent] = (TGraphAsymmErrors*)fin1->Get(Form("D0_RAA_sys_combine_%s", nameCentXL[icent]));
   }
   fin1->Close();

   //Set for Draw
   float markerSize = 2.0;
   float lineWidth = 2;
   float markerSizeScale[ncent + 1] = {0.85, 0.75, 1., 1., };
   for (int icent = 0; icent < ncent; icent++)
   {
      gD0err_xl[icent]->SetMarkerStyle(20);
      gD0err_xl[icent]->SetMarkerSize(1.5);
      gD0err_xl[icent]->SetLineWidth(2);
      gD0err_xl[icent]->SetMarkerColor(1);
      gD0err_xl[icent]->SetLineColor(1);

      gD0sys_xl_combine[icent]->SetMarkerStyle(20);
      gD0sys_xl_combine[icent]->SetMarkerSize(1.5);
      gD0sys_xl_combine[icent]->SetLineWidth(2);
      gD0sys_xl_combine[icent]->SetMarkerColor(1);
      gD0sys_xl_combine[icent]->SetLineColor(1);

   }

   gStyle->UseCurrentStyle();

   //plot
   gStyle->Reset("plain");
   TCanvas* c1 = new TCanvas("c1", "A Canvas", 10, 10, 500, 700);
   gStyle->SetOptFit(0);
   gStyle->SetOptStat(0);
   gStyle->SetEndErrorSize(0.01);
   gStyle->SetTitle(0);
   c1->SetGridx(0);
   c1->SetGridy(0);
   setpad(c1, 0., 0.02, 0.01, 0.12);

   double x1 = 0;
   double x2 = 9.6;
   double y1 = 0.001;
   double y2 = 1.65;
   // double y2 = 2.05;

   float small = 0;
   c1->Divide(1, 3, small, small);

   c1->cd(1)->SetLogy(0);
   // gPad->SetPad(0., 0.68, 1., 0.98);
   gPad->SetPad(0., 0.63, 1., 0.98);
   // setpad(gPad, 0.16, 0.05, 0.1, 0.01);
   setpad(gPad, 0.14, 0.03, 0.1, 0.01);
   gPad->SetTickx();
   gPad->SetTicky(0);

   c1->cd(1)->cd();
   TH1* h00 = new TH1F("", "", 1, x1, x2);
   h00->GetXaxis()->SetTitle("p_{T} (GeV/c)");
   h00->GetXaxis()->SetNdivisions(505);
   h00->GetYaxis()->SetNdivisions(505);
   h00->GetYaxis()->SetLabelFont(42);
   h00->GetYaxis()->SetTitleFont(42);
   h00->GetYaxis()->SetLabelOffset(0.021);
   h00->Draw("e");
   h00->SetMinimum(y1);
   h00->SetMaximum(y2);
   h00->GetYaxis()->CenterTitle();
   h00->GetYaxis()->SetTitleSize(0.1);
   h00->GetYaxis()->SetTitleOffset(0.65);
   h00->GetYaxis()->SetLabelSize(0.09);
   // h00->GetListOfFunctions()->FindObject("stats")->Delete();
   h00->GetXaxis()->SetNdivisions(208);

   const float sysw = 0.08;
   legend1 = new TLegend(0.75, 0.55, 0.95, 0.86);
   legend1->SetFillStyle(0);
   legend1->SetFillColor(10);
   legend1->SetBorderSize(0);
   legend1->SetTextSize(0.075);
   legend1->SetTextFont(42);
   legend1->AddEntry(gD0err_xl[0], " 2014", "p");
   legend1->Draw("same");

   // TMarker *mm2 = new TMarker(7.6, 1.455, 20);
   // mm2->SetMarkerStyle(24);
   // mm2->SetMarkerSize(1.4);
   // mm2->SetMarkerColor(1);
   // mm2->Draw("same");

   for (int icent = 0; icent < ncent ; icent++)
   {
      if (icent != 0) continue;

      const float sysw = 0.15;
      gD0err_xl[icent]->Draw("psame");
      //draw systematic error

      const float sysw = 0.15;

      for (int i = 0; i < gD0sys_xl_combine[icent]->GetN(); i++)
      {
         // const float sysl = gD0sys_xl[icent]->GetY()[i] * 0.05;
         const float sysl = 0.06;
         TLine *llw = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i]);
         llw->SetLineWidth(2);
         llw->SetLineColor(1);
         llw->Draw("same");
         TLine *lhi = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i]);
         lhi->SetLineWidth(2);
         lhi->SetLineColor(1);
         lhi->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i] + sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i] + sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i] - sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i] - sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
      }


      gD0err_xl[icent]->Draw("psame");

      sprintf(buf, Form("%s%%", nameCent[icent]));
      // drawLatex(0.78, 0.75, buf, 62, 0.075, 1);
      TLatex *tex = new TLatex(2.7, 0.11, Form("(a)  %s%", nameCent[icent]));
      tex->SetTextFont(42);
      tex->SetTextSize(0.095);
      // tex->SetNDC(kTRUE);
      tex->Draw("same");

   }
   // drawLines(x1, y1, x2, y2, 2, 1);
   drawLine(x1, y1, x1, y2, 3, 1);
   drawLine(x2, y1, x2, y2, 3, 1);
   drawLine(x1, y2, x2, y2, 3, 1);

   sprintf(buf, "Au+Au #sqrt{s_{NN}} = 200 GeV");
   drawLatex(0.22, 0.76, buf, 42, 0.1, 1);

   TLine *l1 = new TLine(x1, 1.0, x2, 1.0);
   l1->SetLineWidth(2);
   l1->SetLineColor(1);
   l1->SetLineStyle(2);
   l1->Draw("same");

   TBox *bx = new TBox(9.3, (1. - NbinErr[0] / Nbin[0]), 9.45, (1. + NbinErr[0] / Nbin[0]));
   bx->SetFillColor(kGreen - 6);
   bx->Draw();
   const float ppnormsys = sqrt(0.081 * 0.081 + 0.052 * 0.052); //pp normalization 0.081%
   TBox *bx = new TBox(9.45, (1. - ppnormsys), 9.58, (1. + ppnormsys));
   bx->SetFillColor(kGreen - 2);
   bx->Draw();

   c1->cd(2)->SetLogy(0);
   c1->cd(2);
   gPad->SetPad(0., 0.36, 1., 0.63);
   setpad(gPad, 0.14, 0.03, 0.03, 0.01);
   gPad->SetTickx();
   gPad->SetTicky(0);

   TH1F* h0 = new TH1F("", "", 1, x1, x2);
   h0->GetXaxis()->SetTitle("p_{T} (GeV/c)");
   h0->GetXaxis()->SetNdivisions(505);
   h0->GetYaxis()->SetNdivisions(505);
   h0->GetYaxis()->SetLabelFont(42);
   h0->GetYaxis()->SetTitleFont(42);
   h0->GetYaxis()->SetLabelOffset(0.021);
   y2 = 1.65;
   h0->Draw();
   h0->SetMinimum(y1);
   h0->SetMaximum(y2);
   h0->GetYaxis()->CenterTitle();
   h0->GetYaxis()->SetTitleSize(0.13);
   h0->GetYaxis()->SetTitleOffset(0.5);
   h0->GetYaxis()->SetLabelSize(0.11);
   for (int icent = 0; icent < ncent ; icent++)
   {
      if (icent != 1) continue;

      const float sysw = 0.15;
      gD0err_xl[icent]->Draw("psame");
      //draw systematic error
      const float sysw = 0.15;

      for (int i = 0; i < gD0sys_xl_combine[icent]->GetN(); i++)
      {
         // const float sysl = gD0sys_xl[icent]->GetY()[i] * 0.05;
         const float sysl = 0.06;
         TLine *llw = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i]);
         llw->SetLineWidth(2);
         llw->SetLineColor(1);
         llw->Draw("same");
         TLine *lhi = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i]);
         lhi->SetLineWidth(2);
         lhi->SetLineColor(1);
         lhi->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i] + sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i] + sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i] - sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i] - sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
      }

      gD0err_xl[icent]->Draw("psame");

      sprintf(buf, Form("%s%%", nameCent[icent]));
      // drawLatex(0.83, 0.38, buf, 42, 0.12, 1);
      // TLatex *tex = new TLatex(7.4, 1.2, Form("(b)  %s%", nameCent[icent]));
      TLatex *tex = new TLatex(2.7, 0.12, Form("(b)  %s%", nameCent[icent]));
      tex->SetTextFont(42);
      tex->SetTextSize(0.115);
      // tex->SetNDC(kTRUE);
      tex->Draw("same");

   }
   drawLine(x1, y1, x1, y2, 3, 1);
   drawLine(x2, y1, x2, y2, 3, 1);
   TLine *l1 = new TLine(x1, 1.0, x2, 1.0);
   l1->SetLineWidth(2);
   l1->SetLineColor(1);
   l1->SetLineStyle(2);
   l1->Draw("same");

   TBox *bx = new TBox(9.3, (1. - NbinErr[1] / Nbin[1]), 9.45, (1. + NbinErr[1] / Nbin[1]));
   bx->SetFillColor(kGreen - 6);
   bx->Draw();
   const float ppnormsys = sqrt(0.081 * 0.081 + 0.052 * 0.052); //pp normalization 0.081%
   TBox *bx = new TBox(9.45, (1. - ppnormsys), 9.58, (1. + ppnormsys));
   bx->SetFillColor(kGreen - 2);
   bx->Draw();

   c1->cd(3)->SetLogy(0);
   // gPad->SetPad(0., 0.28, 1., 0.48);
   gPad->SetPad(0., 0.0, 1., 0.36);
   // setpad(gPad, 0.16, 0.05, 0.03, 0.23);
   setpad(gPad, 0.14, 0.03, 0.03, 0.23);
   gPad->SetTickx();
   gPad->SetTicky(0);

   // y2 = 2.05;
   y2 = 1.65;
   TH1F* h02 = new TH1F("", "", 1, x1, x2);
   h02->GetXaxis()->SetNdivisions(505);
   h02->GetYaxis()->SetNdivisions(505);
   h02->GetXaxis()->SetTitle("p_{T} (GeV/c)");
   h02->GetYaxis()->SetLabelFont(42);
   h02->GetYaxis()->SetTitleFont(42);
   h02->GetYaxis()->SetLabelOffset(0.021);
   h02->Draw();
   h02->SetMinimum(y1);
   h02->SetMaximum(y2);
   h02->GetYaxis()->CenterTitle();
   h02->GetYaxis()->SetTitleSize(0.095);
   h02->GetYaxis()->SetTitleOffset(0.68);
   h02->GetYaxis()->SetLabelSize(0.085);
   h02->GetXaxis()->SetLabelFont(42);
   h02->GetXaxis()->SetTitleFont(42);
   h02->GetXaxis()->CenterTitle();
   h02->GetXaxis()->SetTitleSize(0.1);
   h02->GetXaxis()->SetTitleOffset(1.05);
   h02->GetXaxis()->SetLabelSize(0.10);
   h02->GetXaxis()->SetLabelOffset(0.025);
   for (int icent = 0; icent < ncent ; icent++)
   {
      if (icent != 2) continue;

      const float sysw = 0.15;
      // for (int i = 0; i < gD0_RAA_pperr[icent]->GetN(); i++)
      // {
      //   TBox *bx = new TBox(gD0_RAA_pperr[icent]->GetX()[i]-sysw,gD0_RAA_pperr[icent]->GetY()[i]- gD0_RAA_pperr[icent]->GetEYlow()[i],gD0_RAA_pperr[icent]->GetX()[i]+sysw,gD0_RAA_pperr[icent]->GetY()[i]+gD0_RAA_pperr[icent]->GetEYhigh()[i]);
      //   bx->SetLineColor(18);
      //   bx->SetFillColor(18);
      //   bx->SetLineWidth(1);
      //   bx->Draw("same");
      // }
      gD0err_xl[icent]->Draw("psame");

      const float sysw = 0.15;

      for (int i = 0; i < gD0sys_xl_combine[icent]->GetN(); i++)
      {
         // const float sysl = gD0sys_xl[icent]->GetY()[i] * 0.05;
         const float sysl = 0.06;
         TLine *llw = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i]);
         llw->SetLineWidth(2);
         llw->SetLineColor(1);
         llw->Draw("same");
         TLine *lhi = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i]);
         lhi->SetLineWidth(2);
         lhi->SetLineColor(1);
         lhi->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i] + sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] - gD0sys_xl_combine[icent]->GetEYlow()[i] + sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] - sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i] - sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
         TLine *lv = new TLine(gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i], gD0sys_xl[icent]->GetX()[i] + sysw, gD0sys_xl[icent]->GetY()[i] + gD0sys_xl_combine[icent]->GetEYhigh()[i] - sysl);
         lv->SetLineWidth(2);
         lv->SetLineColor(1);
         lv->Draw("same");
      }

      gD0err_xl[icent]->Draw("psame");

      sprintf(buf, Form("%s%%", nameCent[icent]));
      // drawLatex(0.83, 0.56, buf, 42, 0.085, 1);
      // TLatex *tex = new TLatex(7.3, 1.18, Form("(c)  %s%", nameCent[icent]));
      TLatex *tex = new TLatex(2.7, 0.12, Form("(c)  %s%", nameCent[icent]));
      tex->SetTextFont(42);
      tex->SetTextSize(0.09);
      // tex->SetNDC(kTRUE);
      tex->Draw("same");

   }
   // drawLines(x1, y1, x2, y2, 2, 1);
   drawLine(x1, y1, x1, y2, 3, 1);
   drawLine(x2, y1, x2, y2, 3, 1);
   drawLine(x1, y1, x2, y1, 3, 1);

   sprintf(buf, "Au+Au #sqrt{s_{NN}} = 200 GeV");
   // drawLatex(0.18, 0.88, buf, 42, 0.045, 1);
   TLine *l1 = new TLine(x1, 1.0, x2, 1.0);
   l1->SetLineWidth(2);
   l1->SetLineColor(1);
   l1->SetLineStyle(2);
   l1->Draw("same");

   TBox *bx = new TBox(9.3, (1. - NbinErr[2] / Nbin[2]), 9.45, (1. + NbinErr[2] / Nbin[2]));
   bx->SetFillColor(kGreen - 6);
   bx->Draw();
   const float ppnormsys = sqrt(0.081 * 0.081 + 0.052 * 0.052); //pp normalization 0.081%
   TBox *bx = new TBox(9.45, (1. - ppnormsys), 9.58, (1. + ppnormsys));
   bx->SetFillColor(kGreen - 2);
   bx->Draw();

   c1->cd();
   mpad = new TPad(Form("mpad"), "", 0.06, 0.0, 0.0, 1.0);
   // setpad(mpad, -0.5, 0., 0., 0.);
   // mpad->SetFillStyle(4000);
   mpad->SetFillStyle(1001);
   mpad->SetFillColor(10);
   mpad->SetBorderMode(0);
   mpad->SetBorderSize(0);
   mpad->SetFrameFillColor(10);
   mpad->SetFrameBorderMode(0);
   mpad->SetFrameBorderSize(0);
   mpad->SetFrameLineWidth(1);
   mpad->SetTickx();
   mpad->SetTicky(0);
   mpad->Draw();

   mpad->cd();
   sprintf(buf, Form("R_{AA}", nameCent[ncent - 1]));
   TLatex *tex = new TLatex(0.8, 0.5, buf);
   tex->SetTextFont(42);
   tex->SetTextSize(0.8);
   tex->SetTextAngle(90);
   tex->SetNDC();
   tex->Draw("same");

   c1->cd();
   c1->Update();

   c1->SaveAs("./Fig/D0_RAA.pdf");
   c1->SaveAs("./Fig/D0_RAA.png");
}
