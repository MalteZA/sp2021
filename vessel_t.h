#ifndef EXAM_VESSEL_T_H
#define EXAM_VESSEL_T_H


#include <utility>
#include <vector>
#include <memory>
#include <ostream>
#include "reactant_t.h"
#include "rule_t.h"

class vessel_t {
private:
    std::vector<std::shared_ptr<reactant_t>> reactants{};
    std::vector<std::pair<std::shared_ptr<rule_t<reactant_t>>, std::string>> rules{};
public:
    reactant_t& environment() {
        auto r = std::make_shared<reactant_t>("env", -1);
        reactants.push_back(r);
        return *r;
    }

    reactant_t& operator()(std::string&& name, double amount) {
        auto r = std::make_shared<reactant_t>(name, amount);
        reactants.push_back(r);
        return *r;
    }

    /// New reaction rule in vessel with no catalysts -RQ1
    std::pair<std::shared_ptr<rule_t<reactant_t>>, std::string> operator()(tempRule_t<reactant_t>& tempRule, const double rate) {
        auto inputVector = std::vector<std::shared_ptr<reactant_t>>{};
        for (const auto& reactant : tempRule.input) {
            inputVector.push_back(getReactantByName(reactant.getName()));
        }

        auto outputVector = std::vector<std::shared_ptr<reactant_t>>{};
        for (const auto& reactant : tempRule.output) {
            outputVector.push_back(getReactantByName(reactant.getName()));
        }

        delete &tempRule;

        auto newRule = std::make_shared<rule_t<reactant_t>>();
        newRule->setInput(inputVector);
        newRule->setOutput(outputVector);
        newRule->setReactionRate(rate);

        auto newPair = std::pair<std::shared_ptr<rule_t<reactant_t>>, std::string>(newRule, "r" + std::to_string(rules.size()));

        rules.push_back(newPair);
        return newPair;
    }

    /// New reaction rule in vessel with one catalyst passed as reactant. -RQ1
    void operator()(tempRule_t<reactant_t>& tempRule, const reactant_t& catalyst, const double rate) {
        auto newPair = (*this)(tempRule, rate);
        newPair.first->setCatalysts({getReactantByName(catalyst.getName())});
    }

    /// New reaction rule in vessel with multiple catalysts passed as input in tempRule. -RQ1
    void operator()(tempRule_t<reactant_t>& tempRule, tempRule_t<reactant_t>& catalysts, const double rate) {
        auto newPair = (*this)(tempRule, rate);
        auto catalystVector = std::vector<std::shared_ptr<reactant_t>>{};
        for (const auto& catalyst : catalysts.input) {
            catalystVector.push_back(getReactantByName(catalyst.getName()));
        }
        newPair.first->setCatalysts(catalystVector);
    }

    [[nodiscard]] const std::vector<std::shared_ptr<reactant_t>>& getReactants() const {
        return reactants;
    }

    [[nodiscard]] const std::vector<std::pair<std::shared_ptr<rule_t<reactant_t>>, std::string>>& getRules() const {
        return rules;
    }

    std::shared_ptr<reactant_t> getReactantByName(const std::string& name) {
        for (auto reactant : reactants) {
            if (reactant->getName() == name) {
                return reactant;
            }
        }
        std::cout << "ERROR!! reactant '" + name + "' not found in vessel..." << std::endl;
        exit(1); // fatal failure...
    }

    std::pair<std::shared_ptr<rule_t<reactant_t>>, std::string> getRuleById(const std::string& id) {
        for (auto rule : rules) {
            if (rule.first->getId() == id) {
                return rule;
            }
        }
        std::cout << "ERROR!! Rule '" + id + "' not found in vessel...";
        exit(1);
    }

    friend std::ostream& operator<<(std::ostream& os, const vessel_t& vessel) {

        return os;
    }

    std::string toDigraph() {
        std::string s = "digraph{\n";
        for (const auto& reactant : reactants) {
            auto r = *reactant;
            if (r.getName() == "env") continue;
            s.append(r.toDigraphElement());
        }
        for (const auto& rule : rules) {
            auto r = *(rule.first);
            s.append(r.toDigraphFormat(rule.second));
        }
        s.append("}");
        return s;
    }
};

#endif //EXAM_VESSEL_T_H
