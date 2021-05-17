#include <iostream>
#include <cmath>
#include <thread>
#include "vessel_t.h"
#include "graphMaker.h"
#include "stochasticSimulator.h"

#include "graphviz/gvc.h"

/** small first example */
vessel_t tiny_example()
{
    auto lambda = 0.001;
    auto v = vessel_t{};
    auto A = v("A", 25);
    auto B = v("B", 50);
    auto C = v("C", 0);
    auto D = v("D", 2);
    v(A + 2*B >>= C, D, lambda);
    return v;
}

/** direct encoding */
vessel_t circadian_oscillator()
{
    auto alphaA = 50.0;
    auto alpha_A = 500.0;
    auto alphaR = 0.01;
    auto alpha_R = 50.0;
    auto betaA = 50.0;
    auto betaR = 5.0;
    auto gammaA = 1.0;
    auto gammaR = 1.0;
    auto gammaC = 2.0;
    auto deltaA = 1.0;
    auto deltaR = 0.2;
    auto deltaMA = 10.0;
    auto deltaMR = 0.5;
    auto thetaA = 50.0;
    auto thetaR = 100.0;
    auto v = vessel_t{};
    auto env = v.environment();
    auto DA = v("DA", 1);
    auto D_A = v("D_A", 0);
    auto DR = v("DR", 1);
    auto D_R = v("D_R", 0);
    auto MA = v("MA", 0);
    auto MR = v("MR", 0);
    auto A = v("A", 0);
    auto R = v("R", 0);
    auto C = v("C", 0);
    v(A + DA >>= D_A, gammaA);
    v(D_A >>= DA + A, thetaA);
    v(A + DR >>= D_R, gammaR);
    v(D_R >>= DR + A, thetaR);
    v(D_A >>= MA + D_A, alpha_A);
    v(DA >>= MA + DA, alphaA);
    v(D_R >>= MR + D_R, alpha_R);
    v(DR >>= MR + DR, alphaR);
    v(MA >>= MA + A, betaA);
    v(MR >>= MR + R, betaR);
    v(A + R >>= C, gammaC);
    v(C >>= R, deltaA);
    v(A >>= env, deltaA);
    v(R >>= env, deltaR);
    v(MA >>= env, deltaMA);
    v(MR >>= env, deltaMR);
    return v;
}

/** alternative encoding using catalysts */
vessel_t circadian_oscillator2()
{
    auto alphaA = 50.0;
    auto alpha_A = 500.0;
    auto alphaR = 0.01;
    auto alpha_R = 50.0;
    auto betaA = 50.0;
    auto betaR = 5.0;
    auto gammaA = 1.0;
    auto gammaR = 1.0;
    auto gammaC = 2.0;
    auto deltaA = 1.0;
    auto deltaR = 0.2;
    auto deltaMA = 10.0;
    auto deltaMR = 0.5;
    auto thetaA = 50.0;
    auto thetaR = 100.0;
    auto v = vessel_t{};
    auto env = v.environment();
    auto DA = v("DA", 1);
    auto D_A = v("D_A", 0);
    auto DR = v("DR", 1);
    auto D_R = v("D_R", 0);
    auto MA = v("MA", 0);
    auto MR = v("MR", 0);
    auto A = v("A", 0);
    auto R = v("R", 0);
    auto C = v("C", 0);
    v(A + DA >>= D_A, gammaA);
    v(D_A >>= DA + A, thetaA);
    v(DR + A >>= D_R, gammaR);
    v(D_R >>= DR + A, thetaR);
    v(env >>= MA, D_A, alpha_A);
    v(env >>= MA, DA, alphaA);
    v(env >>= MR, D_R, alpha_R);
    v(env >>= MR, DR, alphaR);
    v(env >>= A, MA, betaA);
    v(env >>= R, MR, betaR);
    v(A + R >>= C, gammaC);
    v(C >>= R, deltaA);
    v(A >>= env, deltaA);
    v(R >>= env, deltaR);
    v(MA >>= env, deltaMA);
    v(MR >>= env, deltaMR);
    return v;
}

/** covid-19 example */
vessel_t seihr(uint32_t N)
{
    auto v = vessel_t{};
    const auto eps = 0.0009; // initial fraction of infectious
    const auto I0 = size_t(std::round(eps*N)); // initial infectious
    const auto E0 = size_t(std::round(eps*N*15)); // initial exposed
    const auto S0 = N-I0-E0; // initial susceptible
    const auto R0 = 2.4; // basic reproductive number (initial, without lockdown etc)
    const auto alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
    const auto gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
    const auto beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
    const auto P_H = 0.9e-3; // probability of hospitalization
    const auto kappa = gamma * P_H*(1.0-P_H); // hospitalization rate (I -> H)
    const auto tau = 1.0/10.12; // recovery/death rate in hospital (H -> R) ~10.12 days
    auto S = v("S", S0); // susceptible
    auto E = v("E", E0); // exposed
    auto I = v("I", I0); // infectious
    auto H = v("H", 0); // hospitalized
    auto R = v("R", 0); // removed/immune (recovered + dead)
    v(S >>= E, I, beta/N);
    v(E >>= I, alpha);
    v(I >>= R, gamma);
    v(I >>= H, kappa);
    v(H >>= R, tau);
    return v;
}

int main() {
    // Create vessels via DSEL -RQ1
    auto tinyExample_v = tiny_example();
    auto circadianOscillator_v = circadian_oscillator();
    auto circadianOscillator2_v = circadian_oscillator2();
    auto seihr_NJ_v = seihr(589755);  // Magic number - sorry. Found in assignment.
    auto seihr_DK_v = seihr(10000); // Reduced to produce lov enough amount of output.

    // Output vessels to Graphviz digraph entities -RQ2
    graphMaker::vesselToDigraph(tinyExample_v, "tiny-example-digraph");
    graphMaker::vesselToDigraph(circadianOscillator_v, "CO-digraph");
    graphMaker::vesselToDigraph(circadianOscillator2_v, "CO-2-digraph");
    graphMaker::vesselToDigraph(seihr_NJ_v, "seihr-nj-digraph");
    graphMaker::vesselToDigraph(seihr_DK_v, "seihr-dk-digraph");

    // Simulate the reaction rules and output to .csv -RQ4,5
    std::thread t1(stochasticSimulator::simulate, tinyExample_v, 200, "tiny-example-data");
    std::thread t2(stochasticSimulator::simulate, circadianOscillator2_v, 100, "circadian-oscillator-data");
    std::thread t3(stochasticSimulator::simulate, seihr_NJ_v, 200, "seihr-nj-data");
    std::thread t4(stochasticSimulator::simulate, seihr_DK_v, 200, "seihr-dk-data");

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
