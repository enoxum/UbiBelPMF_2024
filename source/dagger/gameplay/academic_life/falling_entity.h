#pragma once

#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"
#include "espb.h"
#include "gameplay/academic_life/academic_life_logic.h"
#include "core/graphics/sprite.h"

using namespace dagger;

namespace academic_life
{
    struct FallingEntity
    {
        float speed;
        Text text;  // does nothing
    };

    struct FallingText
    {
        float speed;
        Text text;
    };

    class Equation {
    public:
        Equation::Equation(const unsigned num_operators, const unsigned num_operands, const int lb, const int ub)
            : m_num_operators(num_operators), m_num_operands(num_operands), m_lb(lb), m_ub(ub)
        {
            initialize_code(m_code_simple, &Equation::random_operator_simple);
            initialize_code(m_code_medium, &Equation::random_operator_medium);
            initialize_code(m_code_hard, &Equation::random_operator_hard);
        }

        template<typename T>
        void initialize_code(T& code, std::string(Equation::* random_operator)() const)
        {
            for (int i = 0; i < m_num_operators; i++) {
                code.push_back((this->*random_operator)());
            }
            for (int i = 0; i < m_num_operands; i++) {
                code.push_back(std::to_string(random_terminal()));
            }
        }

        template<typename T>
        std::string generate_expression(Equation& eq, T& (Equation::* get_code)() const)
        {
            return eq.to_string((eq.*get_code)());
        }

        std::string random_operator_simple() const;
        std::string random_operator_medium() const;
        std::string random_operator_hard() const;
        int random_terminal() const;

        std::string to_string(const std::vector<std::string>& code);
        std::string to_equation(const std::string& expression) const;
        int calculate(const std::string& expression_str);
        std::string Equation::generate_expression(Equation& eq);
        void Equation::generate_equation_entity(const int randomX, const int randomY);

        const std::vector<std::string>& get_code_simple() const;
        const std::vector<std::string>& get_code_medium() const;
        const std::vector<std::string>& get_code_hard() const;

    private:
        std::vector<std::string> m_code_simple;
        std::vector<std::string> m_code_medium;
        std::vector<std::string> m_code_hard;

        unsigned m_num_operators, m_num_operands;
        int m_lb, m_ub;
    };

    template<typename T>
    void ProcessEntity(Float32 boarderY, float mul, float entitySpeed, int& fallingEntityCounter,
        AcademicLifeFieldSettings fieldSettings, bool isText)
    {
        auto view = Engine::Registry().view<Transform, T>();
        ESPB& espb = ESPB::Instance();
        for (auto entity : view)
        {
            auto& transform = view.get<Transform>(entity);
            auto& falling_entity = view.get<T>(entity);
            falling_entity.speed = entitySpeed;

            transform.position.y -= falling_entity.speed * Engine::DeltaTime()*espb.get_speed_factor();

            if (transform.position.y < -boarderY)
            {
                falling_entity.text.Set("pixel-font", "", transform.position);
                Engine::Registry().destroy(entity);  // delete current entity
                createRandomEntity();  // create new random entity
            }

            if (isText)
                falling_entity.text.Set("pixel-font", falling_entity.text.message, transform.position);
        }
    }

    class FallingEntitySystem : public System
    {
    public:
        inline String SystemName() { return "Falling Entity's System"; }

        void Run() override;
    };

    void setLifestyleEntity_byProbability(int lifestyle_prob, Registry& reg, entt::entity entity, Sprite& sprite);
    void createRandomEntity();
}
