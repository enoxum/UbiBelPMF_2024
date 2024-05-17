#pragma once

#include "core/graphics/text.h"
#include "core/system.h"
#include "core/core.h"

#include "gameplay/academic_life/academic_life_logic.h"

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

        std::string random_operator_simple() const;
        std::string random_operator_medium() const;
        std::string random_operator_hard() const;
        int random_terminal() const;

        std::string to_string(const std::vector<std::string>& code);
        std::string to_equation(const std::string& expression) const;
        double calculate(const std::string& expression);

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
        for (auto entity : view)
        {
            auto& transform = view.get<Transform>(entity);
            auto& falling_entity = view.get<T>(entity);
            falling_entity.speed = entitySpeed;

            transform.position.y -= falling_entity.speed * Engine::DeltaTime();

            // Umesto da se vrati gore kada padne na zemlju, bolje je da se kreira novi objekat (drugog tipa)
            if (transform.position.y < -boarderY)
            {
                transform.position.y = boarderY;
                fallingEntityCounter += 1;
                if (fallingEntityCounter % 3 == 0)
                {
                    mul += 0.4;
                    entitySpeed = fieldSettings.fieldTileSize * mul;
                }
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
}