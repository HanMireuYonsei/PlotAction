#ifndef __PATHFUNC__
#define __PATHFUNC__

#include <vector>
#include <tuple>
#include <TString.h>
#include <TMath.h>

bool rangetest(double tau);

std::tuple<double, double> straight(double tau);
std::tuple<double, double> zetatheta(double tau);
std::tuple<double, double> thetazeta(double tau);
std::tuple<double, double> zetaparadown(double tau);
std::tuple<double, double> zetaparaup(double tau);
std::tuple<double, double> thetaparadown(double tau);
std::tuple<double, double> thetaparaup(double tau);
std::tuple<double, double> bestpath(double tau);

typedef std::tuple<double, double> (*PathFunc)(double);

#endif