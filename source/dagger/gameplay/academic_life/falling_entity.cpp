#include "falling_entity.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/academic_life/exprtk.hpp"

using namespace dagger;
using namespace academic_life;

std::string Equation::random_operator_simple() const
{
    std::vector<std::string> operators = { "+", "-" };
    return operators[rand() % 2];
}

std::string Equation::random_operator_medium() const
{
    std::vector<std::string> operators = { "+", "*", "/" };
    return operators[rand() % 3];
}

std::string Equation::random_operator_hard() const
{
    std::vector<std::string> operators = { "-", "*", "^" };
    return operators[rand() % 3];
}

int Equation::random_terminal() const
{
    return rand() % (m_ub - m_lb + 1) + m_lb;
}

std::string Equation::to_string(const std::vector<std::string>& code)
{
    return "(" + code[3] + " " + code[1] + " " + code[4] + ") " +
        code[0] + " (" + code[5] + " " + code[2] + " " + code[6] + ")";
}

std::string Equation::to_equation(const std::string& expression) const
{
    return "X = " + expression;
}

double Equation::evaluate(const std::string& expression) {
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_constants();

    exprtk::expression<double> expr;
    expr.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    if (parser.compile(expression, expr)) {
        return expr.value();
    }
    else {
        throw std::runtime_error("Failed to parse equation");
    }
}

const std::vector<std::string>& Equation::get_code_simple() const
{
    return m_code_simple;
}

const std::vector<std::string>& Equation::get_code_medium() const
{
    return m_code_medium;
}

const std::vector<std::string>& Equation::get_code_hard() const
{
    return m_code_hard;
}

void FallingEntitySystem::Run()
{
    AcademicLifeFieldSettings fieldSettings;
    if (auto* ptr = Engine::GetDefaultResource<AcademicLifeFieldSettings>())
    {
        fieldSettings = *ptr;
    }

    Float32 boarderY = fieldSettings.GetYBoarder();
    float mul = 4;
    static int fallingEntityCounter = 0;
    float entitySpeed = fieldSettings.fieldTileSize * mul;

    ProcessEntity<FallingEntity>(boarderY, mul, entitySpeed, fallingEntityCounter, fieldSettings, false);
    ProcessEntity<FallingText>(boarderY, mul, entitySpeed, fallingEntityCounter, fieldSettings, true);

}