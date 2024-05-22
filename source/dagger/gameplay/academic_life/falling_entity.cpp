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

int Equation::calculate(const std::string& expression_str) {
    int eps = 0.0001;
    constexpr int MAX_ESPB = 240;

    int a, b, c, d;
    char op1, op2, op3;

    sscanf(expression_str.c_str(), "(%d %c %d) %c (%d %c %d)", &a, &op1, &b, &op2, &c, &op3, &d);

    int result1 = (op1 == '+') ? a + b :
                     (op1 == '-') ? a - b :
                     (op1 == '*') ? a * b :
                     (op1 == '/') ? a / b :
                     pow(a, b);

    if (result1 < eps || result1 == INFINITE) result1 = 0.0;
    if (result1 > MAX_ESPB) result1 = MAX_ESPB;

    int result2 = (op3 == '+') ? c + d :
                     (op3 == '-') ? c - d :
                     (op3 == '*') ? c * d :
                     (op3 == '/') ? c / d :
                     pow(c, d);

    if (result2 < eps || result2 == INFINITE) result2 = 0.0;
    if (result2 > MAX_ESPB) result2 = MAX_ESPB;

    int result = (op2 == '+') ? result1 + result2 :
                    (op2 == '-') ? result1 - result2 :
                    (op2 == '*') ? result1 * result2 :
                    (op2 == '/') ? result1 / result2 :
                    pow(result1, result2);

    if (result < eps || result == INFINITE) result = 0.0;
    if (result > MAX_ESPB) result = MAX_ESPB;

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

void academic_life::setLifestyleEntity_byProbability(int lifestyle_prob, Registry& reg, entt::entity entity, Sprite& sprite)
{
    if (lifestyle_prob == 0) {
        AssignSprite(sprite, "AcademicLife:cigarette");

        reg.emplace<LifestyleChange>(entity, LifestyleChange::Cigarette);

        //ako je entity cigara - moze da ostavlja dim 
        common_res::ParticleSpawnerSettings settings;
        settings.Setup(0.1f, { 4.f, 4.f }, { -0.2f, 0.4f }, { 0.2f, 1.2f },
            { 0.6f,0.6f,0.6f,1 }, { 1,1,1,1 }, "EmptyWhitePixel");
        common_res::ParticleSystem::SetupParticleSystem(entity, settings);
    }
    else if (lifestyle_prob == 1) {
        AssignSprite(sprite, "AcademicLife:beer");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::Beer);
    }
    else if (lifestyle_prob == 2) {
        AssignSprite(sprite, "AcademicLife:whey-protein");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::WheyProtein);
    }
    else if (lifestyle_prob == 3) {
        AssignSprite(sprite, "AcademicLife:fishMeal");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::fishMeal);
    }
    else {
        AssignSprite(sprite, "AcademicLife:apple");
        reg.emplace<LifestyleChange>(entity, LifestyleChange::Apple);
    }
}

void academic_life::createRandomEntity() 
{
    auto& reg = Engine::Instance().Registry();
    int entity_prob = rand() % 2;
    auto randomX = rand() % 200 - 150;
    auto randomY = (rand() % 30) * ((rand() % 10) + 5) + 300;

    // jednacine
    if (entity_prob == 0)
    {
        generate_equation_entity(randomX, randomY);
    }

    // lifestyle objekti
    else {
        int lifestyle_prob = rand() % 5; //da li ce da deluje pozitivno ili negativno
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        setLifestyleEntity_byProbability(lifestyle_prob, reg, entity, sprite);

        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 2 * tileSize, 2 * tileSize * ratio };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { randomX, randomY, zPos };

        auto& falling_entity = reg.emplace<FallingEntity>(entity);
        falling_entity.speed = tileSize * (rand() % 5 + 3);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;
    }
    
}