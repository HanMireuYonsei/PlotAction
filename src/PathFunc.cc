#include <tuple>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include "PathFunc.hh"
#include "PathVariables.hh"

std::vector<TString> pathfuncnames = {"straight", "zeta first", "theta first", "zeta para conv down", "zeta para conv up", "theta para conv down", "theta para conv up", "best path"};
std::vector<PathFunc> pathfuncs = {straight, zetatheta, thetazeta, zetaparadown, zetaparaup, thetaparadown, thetaparaup, bestpath};

bool rangetest(double tau)
{
    return (tau >= tau_ini) && (tau < tau_fin);
}

std::tuple<double, double> straight(double tau)
{
    double zeta, theta;

    zeta = (zet_fin - zet_ini) * (tau - tau_ini) / (tau_fin - tau_ini) + zet_ini;
    theta = (the_fin - the_ini) * (tau - tau_ini) / (tau_fin - tau_ini) + the_ini;
    
    return std::tuple<double, double>(zeta, theta);
}

std::tuple<double, double> zetatheta(double tau)
{
    double zeta, theta;

    if (tau < tau_half)
    {
        zeta = (zet_fin - zet_ini) * (tau - tau_ini) / (tau_half - tau_ini) + zet_ini;
        theta = the_ini;
    }
    else
    {
        zeta = zet_fin;
        theta = (the_fin - the_ini) * (tau - tau_fin) / (tau_fin - tau_half) + the_fin;
    }
    
    return std::tuple<double, double>(zeta, theta);
}

std::tuple<double, double> thetazeta(double tau)
{
    double zeta, theta;

    if (tau < tau_half)
    {
        zeta = zet_ini;
        theta = (the_fin - the_ini) * (tau - tau_ini) / (tau_half - tau_ini) + the_ini;
    }
    else
    {
        zeta = (zet_fin - zet_ini) * (tau - tau_fin) / (tau_fin - tau_half) + zet_fin;
        theta = the_fin;
    }
    
    return std::tuple<double, double>(zeta, theta);
}

std::tuple<double, double> zetaparadown(double tau)
{
    double zeta, theta;

    zeta = (zet_fin - zet_ini) * TMath::Power((tau - tau_ini) / (tau_fin - tau_ini), 2) + zet_ini;
    theta = (the_fin - the_ini) * (tau - tau_ini) / (tau_fin - tau_ini) + the_ini;
    
    return std::tuple<double, double>(zeta, theta);
}

std::tuple<double, double> zetaparaup(double tau)
{
    double zeta, theta;

    zeta = (zet_ini - zet_fin) * TMath::Power((tau - tau_fin) / (tau_ini - tau_fin), 2) + zet_fin;
    theta = (the_fin - the_ini) * (tau - tau_ini) / (tau_fin - tau_ini) + the_ini;
    
    return std::tuple<double, double>(zeta, theta);
}

std::tuple<double, double> thetaparadown(double tau)
{
    double zeta, theta;

    zeta = (zet_fin - zet_ini) * (tau - tau_ini) / (tau_fin - tau_ini) + zet_ini;
    theta = (the_fin - the_ini) * TMath::Power((tau - tau_ini) / (tau_fin - tau_ini), 2) + the_ini;
    
    return std::tuple<double, double>(zeta, theta);
}

std::tuple<double, double> thetaparaup(double tau)
{
    double zeta, theta;

    zeta = (zet_fin - zet_ini) * (tau - tau_ini) / (tau_fin - tau_ini) + zet_ini;
    theta = (the_ini - the_fin) * TMath::Power((tau - tau_fin) / (tau_ini - tau_fin), 2) + the_fin;
    
    return std::tuple<double, double>(zeta, theta);
}

std::tuple<double, double> bestpath(double tau)
{
    double _tau, zeta, theta;

    TFile* file = new TFile("BestPath.root", "read");
    TTree* tree = (TTree*)file->Get("Path");
    tree->SetBranchAddress("tau", &_tau);
    tree->SetBranchAddress("zeta", &zeta);
    tree->SetBranchAddress("theta", &theta);

    for (int i = 0; i < nbin + 1; i++)
    {
        tree->GetEntry(i);

        if (tau == _tau)
        {
            return std::tuple<double, double>(zeta, theta);
        }
    }
    throw;
}