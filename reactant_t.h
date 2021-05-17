#include <utility>
#include "rule_t.h"
#include "tempRule_t.h"

#ifndef EXAM_REACTANT_T_H
#define EXAM_REACTANT_T_H


class reactant_t {
private:
    std::string name{};
    int amount{};
public:
    [[nodiscard]] const std::string& getName() const {
        return name;
    }

    [[nodiscard]] int getAmount() const {
        return amount;
    }

    void setAmount(int newAmount) {
        reactant_t::amount = newAmount;
    }

public:
    reactant_t(std::string  name, int amount) : name(std::move(name)), amount(amount) {}

    reactant_t(const reactant_t& other) {
        name = other.name;
        amount = other.amount;
    }

    reactant_t& operator=(const reactant_t& other) {
        if (this != &other) {
            name = other.name;
            amount = other.amount;
        }
        return *this;
    }

    ~reactant_t() = default;

    friend tempRule_t<reactant_t>& operator+(const reactant_t& lhs, const reactant_t& rhs) {
        auto rule = new tempRule_t<reactant_t>;
        rule->input = {lhs, rhs};
        return *rule;
    }

    friend tempRule_t<reactant_t>& operator+(const reactant_t& lhs, tempRule_t<reactant_t>& rhs) {
        rhs.input.emplace_back(lhs);
        return rhs;
    }

    friend tempRule_t<reactant_t>& operator+(tempRule_t<reactant_t>& lhs, const reactant_t& rhs) {
        lhs.input.emplace_back(rhs);
        return lhs;
    }

    friend tempRule_t<reactant_t>& operator>>=(const reactant_t& lhs, const reactant_t& rhs) {
        auto rule = new tempRule_t<reactant_t>;
        rule->input = {lhs};
        rule->output = {rhs};
        return *rule;
    }

    friend tempRule_t<reactant_t>& operator>>=(tempRule_t<reactant_t>& lhs, const reactant_t& rhs) {
        lhs.output = {rhs};
        return lhs;
    }

    friend tempRule_t<reactant_t>& operator>>=(const reactant_t& lhs, tempRule_t<reactant_t>& rhs) {
        auto rule = new tempRule_t<reactant_t>;
        rule->input = {lhs};
        rule->output = rhs.input;

        delete &rhs;
        return *rule;
    }

    friend tempRule_t<reactant_t>& operator>>=(tempRule_t<reactant_t>& lhs, tempRule_t<reactant_t>& rhs) {
        auto rule = new tempRule_t<reactant_t>;
        rule->input = lhs.input;
        rule->output = rhs.input;

        delete &lhs;
        delete &rhs;

        return *rule;
    }

    friend tempRule_t<reactant_t>& operator*(int scalar, const reactant_t& rhs) {
        auto rule = new tempRule_t<reactant_t>;
        for (int i = 0; i < scalar; ++i) {
            rule->input.push_back(rhs);
        }
        return *rule;
    }

    std::string toDigraphElement() {
        auto s = "\t" + name + "[label=\"" + name + "\", shape=\"box\", style=\"filled\", fillcolor=\"cyan\"];\n";
        return s;
    }
};


#endif //EXAM_REACTANT_T_H
