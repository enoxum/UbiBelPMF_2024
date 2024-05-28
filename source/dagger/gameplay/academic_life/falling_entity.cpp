#include "falling_entity.h"
#include "academic_life_main.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include <core/graphics/sprite.h>
#include <gameplay/common/particles.h>
#include <gameplay/common/simple_collisions.h>
#include "enumi.h"

using namespace dagger;
using namespace academic_life;

std::string Equation::generate_expression(Equation& eq)
{
    ESPB& espb = ESPB::Instance();
    int espb_value = espb.GetValue();

    if (espb_value < 60)
        return Equation::generate_expression(eq, &Equation::get_code_simple);
    else if (espb_value < 120)
        return Equation::generate_expression(eq, &Equation::get_code_medium);
    else
        return Equation::generate_expression(eq, &Equation::get_code_hard);
}

void generate_equation_entity(const int randomX, const int randomY)
{
    // TODO:  cuvati poziciju prethodne jednacine i generisati trenutnu podalje

    auto& reg = Engine::Instance().Registry();
    auto entity = reg.create();
    ESPB& espb = ESPB::Instance();

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = { randomX, randomY, zPos };

    auto& falling_text = reg.emplace<FallingText>(entity);
    auto& text = falling_text.text;
    text.scale = { 0.6f, 0.6f };
    text.spacing = { 0.3f };
    text.position = transform.position;
    falling_text.speed = tileSize * (rand() % 5 + 3);

    Equation eq = Equation::Equation(3, 4, -5, 5);
    std::string expression = eq.generate_expression(eq);
    text.message = eq.to_equation(expression);
    text.value = eq.calculate(expression);

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size = { tileSize * 11.5, tileSize * 2 };
}

void generate_lifestyle_entity(const int randomX, const int randomY)
{
    // TODO:  cuvati poziciju prethodne jednacine i generisati trenutnu podalje

    auto& reg = Engine::Instance().Registry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);

    int lifestyle_prob = rand() % 5; //da li ce da deluje pozitivno ili negativno

    setLifestyleEntity_byProbability(lifestyle_prob, reg, entity, sprite);

    float ratio = sprite.size.y / sprite.size.x;
    sprite.size = { 2 * tileSize, 2 * tileSize * ratio };

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = { randomX, randomY, zPos + 0.1f };

    auto& falling_entity = reg.emplace<FallingEntity>(entity);
    falling_entity.speed = tileSize * (rand() % 5 + 3);

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size = sprite.size;
}

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

int calculate_one(const int left, const char op, const int right) {
    if (op == '/' && right == 0)
        return left;
    return (op == '+') ? left + right :
           (op == '-') ? left - right :
           (op == '*') ? left * right :
           (op == '/') ? left / right :
           pow(right, right);
}

int Equation::calculate(const std::string& expression_str) {
    int eps = 0.0001;

    int a, b, c, d;
    char op1, op2, op3;

    sscanf(expression_str.c_str(), "(%d %c %d) %c (%d %c %d)", &a, &op1, &b, &op2, &c, &op3, &d);

    int result1 = calculate_one(a, op1, b);
    int result2 = calculate_one(c, op3, d);

    return calculate_one(result1, op2, result2);
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

void academic_life::setLifestyleEntity_byProbability(int lifestyle_prob, Registry& reg, entt::entity entity, Sprite& sprite)
{
    if (lifestyle_prob == 0) {
        AssignSprite(sprite, "AcademicLife:cigarette2");

        reg.emplace<LifestyleChange>(entity, LifestyleChange::Cigarette);

        //ako je entity cigara - moze da ostavlja dim 
        common_res::ParticleSpawnerSettings settings;
        settings.Setup(0.1f, { 4.f, 4.f }, { -0.2f, 0.4f }, { 0.2f, 1.2f },
            { 0.6f,0.6f,0.6f,1 }, { 1,1,1,1 }, "EmptyWhitePixel");
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }
    else if (lifestyle_prob == 1) {
        AssignSprite(sprite, "AcademicLife:Beer2");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::Beer);
    }
    else if (lifestyle_prob == 2) {
        AssignSprite(sprite, "AcademicLife:whey");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::WheyProtein);
    }
    else if (lifestyle_prob == 3) {
        AssignSprite(sprite, "AcademicLife:Steak");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::fishMeal);
    }
    else {
        AssignSprite(sprite, "AcademicLife:Apple2");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::Apple);
    }
}

void academic_life::createRandomEntity() 
{
    auto& reg = Engine::Instance().Registry();
    int entity_prob = rand() % 2;
    auto randomX = rand() % 300 - (150 + rand() % 90);
    auto randomY = (rand() % 30) * ((rand() % 10) + 5) + 300;

    if (entity_prob == 0)
    {
        generate_equation_entity(randomX, randomY);
    }

    else {
        generate_lifestyle_entity(randomX, randomY);
    }
    
}