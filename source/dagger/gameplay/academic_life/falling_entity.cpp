#include "falling_entity.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include <cmath>
#include <cctype>

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

double Equation::evaluate(const std::string& expression_str) {
    double eps = 0.0001;

    int a, b, c, d;
    char op1, op2, op3;

    sscanf(expression_str.c_str(), "(%d %c %d) %c (%d %c %d)", &a, &op1, &b, &op2, &c, &op3, &d);

    double result1 = (op1 == '+') ? a + b :
        (op1 == '-') ? a - b :
        (op1 == '*') ? a * b :
        (op1 == '/') ? a / b :
        pow(a, b);

    if (result1 < eps || result1 == INFINITE) result1 = 0.0;

    double result2 = (op3 == '+') ? c + d :
        (op3 == '-') ? c - d :
        (op3 == '*') ? c * d :
        (op3 == '/') ? c / d :
        pow(c, d);

    if (result2 < eps || result2 == INFINITE) result2 = 0.0;

    double result = (op2 == '+') ? result1 + result2 :
        (op2 == '-') ? result1 - result2 :
        (op2 == '*') ? result1 * result2 :
        (op2 == '/') ? result1 / result2 :
        pow(result1, result2);

    if (result < eps || result == INFINITE) result = 0.0;

    return result;
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