#include <iostream>
#include <vector>
#include <tuple>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TLegend.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraph.h>
#include "PathFunc.hh"
#include "CalcAction.hh"
#include "PathVariables.hh"

extern std::vector<TString> pathfuncnames;
extern std::vector<PathFunc> pathfuncs;

int main(const int argc, const char *argv[])
{
    TFile* file = new TFile("Actions.root", "recreate");
    TCanvas* canv_path = new TCanvas("canv_path", "Path", 3000, 2000);
    TCanvas* canv_action = new TCanvas("canv_action", "Action", 3000, 2000);
    std::vector<std::tuple<double, double>> actions;

    std::cerr << "========Path Generation Start========" << std::endl;

    size_t num_func = pathfuncs.size();

    const double maxrange = 1.1 * TMath::Max(zet_ini, zet_fin);
    TH2D* axis = new TH2D("axis", "Paths", 10, the_ini-(the_fin-the_ini)*0.05, the_fin+(the_fin-the_ini)*0.05, 10, zet_ini-(zet_fin-zet_ini)*0.05, zet_fin+(zet_fin-zet_ini)*0.05);
    canv_path->cd();
    axis->SetStats(false);
    axis->Draw();
    TLegend* legend_path = new TLegend(0.8, 0.4, 0.95, 0.6);
    for (size_t i = 0; i < num_func; i++)
    {
        double tau, zeta, theta;
        
        TTree* tree = new TTree(pathfuncnames[i], pathfuncnames[i]);
        tree->Branch("tau", &tau);
        tree->Branch("zeta", &zeta);
        tree->Branch("theta", &theta);

        for (size_t j = 0; j < nbin + 1; j++)
        {
            TF1 linear_tau("linear_tau", "[0] * x + [1]", tau_ini, tau_fin);
            linear_tau.SetParameter(0, (tau_fin - tau_ini) / nbin);
            linear_tau.SetParameter(1, tau_ini);
            tau = linear_tau.Eval(j);
            std::tuple<double, double> ztpair = pathfuncs[i](tau);
            zeta = std::get<0>(ztpair);
            theta = std::get<1>(ztpair);
            tree->Fill();
        }
        file->cd();
        tree->Write(0, TObject::kOverwrite);

        tree->Draw("theta:zeta", "", "goff");
        // tree->Draw("zeta * TMath::Cos(theta):zeta * TMath::Sin(theta)", "", "goff");
        TGraph* path = new TGraph(nbin + 1, tree->GetV1(), tree->GetV2());
        path->SetTitle("Paths;X;Y");
        path->SetLineColor(i + 2);
        path->SetLineWidth(2);
        path->SetMarkerStyle(8);
        path->SetMarkerColor(1);
        path->SetMarkerSize(1);
        path->GetXaxis()->SetRangeUser(-1.1 * TMath::Max(zet_ini, zet_fin), 1.1 * TMath::Max(zet_ini, zet_fin));
        path->GetYaxis()->SetRangeUser(0, TMath::Max(zet_ini, zet_fin));
        legend_path->AddEntry(path, pathfuncnames[i]);
        canv_path->cd();
        path->Draw("p");

        actions.push_back(CalcAction(tree));

        delete tree;
    }

    legend_path->Draw();

    gSystem->Unlink("Path.png");
    canv_path->Modified();
    canv_path->Update();
    canv_path->Print("Path.png");
    
    std::cerr << "========Action Calculation Start========" << std::endl;

    TH1D* graph_kinetic = new TH1D("graph_kinetic", "Actions;;Action", num_func, 0, num_func);
    TH1D* graph_potential = new TH1D("graph_potential", "Actions;;Action", num_func, 0, num_func);
    TH1D* graph_action = new TH1D("graph_action", "Actions;;Action", num_func, 0, num_func);
    TLegend* legend_action = new TLegend(0.8, 0.85, 0.95, 0.95);
    for (size_t i = 0; i < num_func; i++)
    {
        double kinetic = std::get<0>(actions[i]);
        double potential = std::get<1>(actions[i]);
        double action = kinetic + potential;
        graph_kinetic->SetBinContent(i + 1, kinetic);
        graph_potential->SetBinContent(i + 1, potential);
        graph_action->SetBinContent(i + 1, action);
        graph_action->GetXaxis()->SetBinLabel(i + 1, pathfuncnames[i]);

        std::cout << "Action of path '" << pathfuncnames[i] <<"': " << kinetic << " + " << potential << " = " << action << std::endl;
    }
    
    graph_action->GetYaxis()->SetRangeUser(0., 8.);
    graph_action->SetBarWidth(0.25);
    graph_action->SetBarOffset(0.625);
    graph_action->SetFillColor(4);
    graph_action->SetStats(false);
    
    graph_kinetic->SetBarWidth(0.25);
    graph_kinetic->SetBarOffset(0.125);
    graph_kinetic->SetFillColor(2);
    graph_kinetic->SetStats(false);
    
    graph_potential->SetBarWidth(0.25);
    graph_potential->SetBarOffset(0.375);
    graph_potential->SetFillColor(3);
    graph_potential->SetStats(false);

    legend_action->AddEntry(graph_kinetic, "T");
    legend_action->AddEntry(graph_potential, "-U");
    legend_action->AddEntry(graph_action, "S");

    graph_action->Write(0, TObject::kOverwrite);
    graph_kinetic->Write(0, TObject::kOverwrite);
    graph_potential->Write(0, TObject::kOverwrite);

    canv_action->cd();
    graph_action->Draw("BAR");
    graph_kinetic->Draw("BAR SAME");
    graph_potential->Draw("BAR SAME");
    legend_action->Draw();

    gSystem->Unlink("Action.png");
    canv_action->Modified();
    canv_action->Update();
    canv_action->Print("Action.png");
    
    delete graph_kinetic;
    delete graph_potential;
    delete graph_action;
    delete legend_action;
    delete file;
    delete canv_path;
    delete canv_action;

    return 0;
}