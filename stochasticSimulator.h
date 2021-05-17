#ifndef EXAM_STOCHASTICSIMULATOR_H
#define EXAM_STOCHASTICSIMULATOR_H

#include <list>
#include <random>
#include <algorithm>
#include "vessel_t.h"

class stochasticSimulator {
private:
public:
    static std::string saveState(const std::vector<std::shared_ptr<reactant_t>>& vector, double currentTime) {
        std::string s = std::to_string(currentTime);
        for (const auto& v : vector) {
            if (v->getName() == "env") continue;
            s += "; " + std::to_string(v->getAmount());
        }
        s += "\n";
        return s;
    }

    static std::string writeHeaders(const std::vector<std::shared_ptr<reactant_t>>& vector) {
        std::string s = "time";
        for (const auto& v : vector) {
            if (v->getName() == "env") continue;
            s += "; " + v->getName();
        }
        s += "\n";
        return s;
    }

    static double randomExp(double k) {
        std::random_device d;
        std::exponential_distribution<> distribution(k);
        return distribution(d);
    }

    static std::pair<double, std::shared_ptr<rule_t<reactant_t>>>
    calculateDelayK(std::shared_ptr<rule_t<reactant_t>> rule) {
        auto lambda_k = rule->getReactionRate();
        for (const auto& r : rule->getInput()) {
            if (r->getName() == "env") continue;
            lambda_k *= r->getAmount();
        }
        for (const auto& c : rule->getCatalysts()) {
            if (c->getName() == "env") continue;
            lambda_k *= c->getAmount();
        }
        lambda_k = randomExp(lambda_k);
        return std::pair<double, std::shared_ptr<rule_t<reactant_t>>>(lambda_k, rule);
    }

    static bool compareRulesWithDelay(std::pair<double, std::shared_ptr<rule_t<reactant_t>>> a,
                                      std::pair<double, std::shared_ptr<rule_t<reactant_t>>> b) {
        return a.first < b.first;
    }

    static std::pair<double, std::shared_ptr<rule_t<reactant_t>>>
    calculateDelays(const std::vector<std::pair<std::shared_ptr<rule_t<reactant_t>>, std::string>>& vector) {
        auto rulesWithDelay = std::vector<std::pair<double, std::shared_ptr<rule_t<reactant_t>>>>{};
        for (const auto& v : vector) {
            rulesWithDelay.push_back(calculateDelayK(v.first));
        }
        std::sort(rulesWithDelay.begin(), rulesWithDelay.end(), stochasticSimulator::compareRulesWithDelay);
        return rulesWithDelay.front();
    }

    static double applyRule(const std::pair<double, std::shared_ptr<rule_t<reactant_t>>>& pair, bool& changes) {
        // Rule Applicable wrt. inputs?
        for (const auto& r : pair.second->getInput()) {
            if (r->getAmount() <= 0) return pair.first;
        }

        // Rule Applicable wrt. catalysts?
        for (const auto& c : pair.second->getCatalysts()) {
            if (c->getAmount() <= 0) return pair.first;
        }

        // Rule is applicable, if we reached here. Apply.
        changes = true;
        for (const auto& r : pair.second->getInput()) {
            auto current = r->getAmount();
            r->setAmount(current - 1);
        }
        for (const auto& r : pair.second->getOutput()) {
            auto current = r->getAmount();
            r->setAmount(current + 1);
        }
        return pair.first;
    }

    /// Simulate according to algorithm. Doesn't work correctly, as some calculations for reactions with
    /// multiple of the same reactant aren't calculated correctly.
    static void simulate(const vessel_t& vessel, const double MaxTime, const std::string& outputFileName) {
        std::cout << "started calculating " + outputFileName << std::endl;
        auto relativeFileName = "../output/" + outputFileName + ".csv";
        std::ofstream file;
        file.open(relativeFileName);
        file << writeHeaders(vessel.getReactants());

        // write initial state at t = 0
        double currentTime{0.0};
        file << saveState(vessel.getReactants(), currentTime);
        while (currentTime < MaxTime) {
            auto changes = false;
            // Calculate delay for every rule and choose lowest
            auto nextReaction = calculateDelays(vessel.getRules());

            // Apply rule (if applicable)
            currentTime += applyRule(nextReaction, changes);

            // Save state
            if (changes) {
                file << saveState(vessel.getReactants(), currentTime);
            }
        }

        file.close();
        std::cout << "Finished " + outputFileName + "!" << std::endl;
    }
};

#endif //EXAM_STOCHASTICSIMULATOR_H
