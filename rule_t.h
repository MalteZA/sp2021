#ifndef EXAM_RULE_T_H
#define EXAM_RULE_T_H

#include <ostream>

template<typename T>
class rule_t {
private:
    std::vector<std::shared_ptr<T>> input{};
    std::vector<std::shared_ptr<T>> output{};
    std::vector<std::shared_ptr<T>> catalysts{};
    std::string id;
    double reactionRate{};

    std::string rateToSensibleString() {
        char array[50];
        if (reactionRate > (3 * pow(10, 4)) || reactionRate < 1 * pow(10, -2)){
            sprintf(array, "%e", reactionRate);
        }
        else {
            sprintf(array, "%.3f", reactionRate);
        }
        return std::string(array);
    }

public:
    rule_t() = default;

    rule_t<T>(const rule_t<T>& other) {
        input = other.input;
        output = other.output;
        catalysts = other.catalysts;
        reactionRate = other.reactionRate;
    }

    rule_t<T>& operator=(const rule_t<T> other) {
        if (this != &other) {
            input = other.input;
            output = other.output;
            catalysts = other.catalysts;
            reactionRate = other.reactionRate;
        }
        return *this;
    }

    rule_t<T>(rule_t<T>&& other) noexcept {
        input = other.input;
        output = other.output;
        catalysts = other.catalysts;
        reactionRate = other.reactionRate;

        other.input = nullptr;
        other.output = nullptr;
        other.catalysts = nullptr;
        other.reactionRate = 0;
    }

    rule_t<T>& operator=(rule_t<T>&& other) noexcept {
        if (this != &other) {
            input = other.input;
            output = other.output;
            catalysts = other.catalysts;
            reactionRate = other.reactionRate;

            other.input = nullptr;
            other.output = nullptr;
            other.catalysts = nullptr;
            other.reactionRate = 0;
        }
        return *this;
    }

    ~rule_t() = default;

    const std::vector<std::shared_ptr<T>>& getInput() const {
        return input;
    }

    void setInput(const std::vector<std::shared_ptr<T>>& newInput) {
        rule_t::input = newInput;
    }

    const std::vector<std::shared_ptr<T>>& getOutput() const {
        return output;
    }

    void setOutput(const std::vector<std::shared_ptr<T>>& newOutput) {
        rule_t::output = newOutput;
    }

    const std::vector<std::shared_ptr<T>>& getCatalysts() const {
        return catalysts;
    }

    void setCatalysts(const std::vector<std::shared_ptr<T>>& newCatalysts) {
        rule_t::catalysts = newCatalysts;
    }

    [[nodiscard]] double getReactionRate() const {
        return reactionRate;
    }

    void setReactionRate(double newReactionRate) {
        rule_t::reactionRate = newReactionRate;
    }

    [[nodiscard]] const std::string& getId() const {
        return id;
    }

    void setId(const std::string& id) {
        rule_t::id = id;
    }

    friend std::ostream& operator<<(std::ostream& os, const rule_t& rule) {
        os << "reactionRate: " << rule.reactionRate;
        return os;
    }

    std::string toDigraphFormat(const std::string& id) {
        auto a = "\t" + id + "[label=\"" + this->rateToSensibleString() +
                 "\",shape=\"oval\",style=\"filled\",fillcolor=\"yellow\"];\n";
        for (auto reactant : catalysts) {
            auto catalyst = *reactant;
            a += "\t" + catalyst.getName() + " -> " + id + " [arrowhead=\"tee\"];\n";
        }
        for (auto reactant : input) {
            auto i = *reactant;
            if (i.getName() == "env") continue;
            a += "\t" + i.getName() + " -> " + id + ";\n";
        }
        for (auto reactant : output) {
            auto o = *reactant;
            if (o.getName() == "env") continue;
            a += "\t" + id + " -> " + o.getName() + ";\n";
        }
        return a;
    }
};


#endif //EXAM_RULE_T_H
