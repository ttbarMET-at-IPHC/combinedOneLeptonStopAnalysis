{
    gStyle->SetPaintTextFormat("4.1f");
    
    TFile* currentCuts_rawMC      = new TFile("currentCuts_rawMC.root","READ");
    TFile* currentCuts_prediction = new TFile("currentCuts_prediction.root","READ");
    TFile* newCuts_prediction     = new TFile("newCuts_prediction.root","READ");

    ((TCanvas*) currentCuts_rawMC     ->Get("bestDiscoFOM"))->SaveAs("FOMDisco_currentCuts_rawMC.pdf"     );
    ((TCanvas*) currentCuts_prediction->Get("bestDiscoFOM"))->SaveAs("FOMDisco_currentCuts_prediction.pdf");
    ((TCanvas*) newCuts_prediction    ->Get("bestDiscoFOM"))->SaveAs("FOMDisco_newCuts_prediction.pdf"    );
    ((TCanvas*) currentCuts_rawMC     ->Get("bestExcluFOM"))->SaveAs("FOMExclu_currentCuts_rawMC.pdf"     );
    ((TCanvas*) currentCuts_prediction->Get("bestExcluFOM"))->SaveAs("FOMExclu_currentCuts_prediction.pdf");
    ((TCanvas*) newCuts_prediction    ->Get("bestExcluFOM"))->SaveAs("FOMExclu_newCuts_prediction.pdf"    );

    TH2F* FOMdisco_currentCuts_rawMC      =  (TH2F*) ((TCanvas*) currentCuts_rawMC     ->Get("bestDiscoFOM"))->GetPrimitive("bestDiscoFOM");
    TH2F* FOMdisco_currentCuts_prediction =  (TH2F*) ((TCanvas*) currentCuts_prediction->Get("bestDiscoFOM"))->GetPrimitive("bestDiscoFOM");
    TH2F* FOMdisco_newCuts_prediction     =  (TH2F*) ((TCanvas*) newCuts_prediction    ->Get("bestDiscoFOM"))->GetPrimitive("bestDiscoFOM");
    TH2F* FOMexclu_currentCuts_rawMC      =  (TH2F*) ((TCanvas*) currentCuts_rawMC     ->Get("bestExcluFOM"))->GetPrimitive("bestExcluFOM");
    TH2F* FOMexclu_currentCuts_prediction =  (TH2F*) ((TCanvas*) currentCuts_prediction->Get("bestExcluFOM"))->GetPrimitive("bestExcluFOM");
    TH2F* FOMexclu_newCuts_prediction     =  (TH2F*) ((TCanvas*) newCuts_prediction    ->Get("bestExcluFOM"))->GetPrimitive("bestExcluFOM");

    TH2F* ratioDisco_1 = (TH2F*) FOMdisco_currentCuts_prediction->Clone(); 
          ratioDisco_1->Divide(FOMdisco_currentCuts_rawMC);
    TH2F* ratioDisco_2 = (TH2F*) FOMdisco_newCuts_prediction->Clone(); 
          ratioDisco_2->Divide(FOMdisco_currentCuts_prediction);
    TH2F* ratioDisco_3 = (TH2F*) FOMdisco_newCuts_prediction->Clone(); 
          ratioDisco_3->Divide(FOMdisco_currentCuts_rawMC);

    TH2F* ratioExclu_1 = (TH2F*) FOMexclu_currentCuts_prediction->Clone(); 
          ratioExclu_1->Divide(FOMexclu_currentCuts_rawMC);
    TH2F* ratioExclu_2 = (TH2F*) FOMexclu_newCuts_prediction->Clone(); 
          ratioExclu_2->Divide(FOMexclu_currentCuts_prediction);
    TH2F* ratioExclu_3 = (TH2F*) FOMexclu_newCuts_prediction->Clone(); 
          ratioExclu_3->Divide(FOMexclu_currentCuts_rawMC);

    ratioDisco_1->SetMinimum(0.5);
    ratioDisco_2->SetMinimum(0.5);
    ratioDisco_3->SetMinimum(0.5);
    ratioDisco_1->SetMaximum(1.5);
    ratioDisco_2->SetMaximum(1.5);
    ratioDisco_3->SetMaximum(1.5);

    ratioExclu_1->SetMinimum(0.5);
    ratioExclu_2->SetMinimum(0.5);
    ratioExclu_3->SetMinimum(0.5);
    ratioExclu_1->SetMaximum(1.5);
    ratioExclu_2->SetMaximum(1.5);
    ratioExclu_3->SetMaximum(1.5);

    TCanvas* c = new TCanvas("","",850,750); c->cd(); 
    
    ratioDisco_1->Draw("colz text");
    c->SaveAs("ratioDisco_currentCutsPrediction_currentCutsRawMC.pdf");
    ratioExclu_1->Draw("colz text");
    c->SaveAs("ratioExclu_currentCutsPrediction_currentCutsRawMC.pdf");

    ratioDisco_2->Draw("colz text");
    c->SaveAs("ratioDisco_newCutsPrediction_currentCutsPrediction.pdf");
    ratioExclu_2->Draw("colz text");
    c->SaveAs("ratioExclu_newCutsPrediction_currentCutsPrediction.pdf");
    
    ratioDisco_3->Draw("colz text");
    c->SaveAs("ratioDisco_newCutsPrediction_currentCutsRawMC.pdf");
    ratioExclu_3->Draw("colz text");
    c->SaveAs("ratioExclu_newCutsPrediction_currentCutsRawMC.pdf");


}
