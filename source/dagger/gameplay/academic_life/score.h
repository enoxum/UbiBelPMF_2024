#ifndef SCORE_H
#define SCORE_H

#include <iostream>

class Score {
public:
    virtual void Increase(int amount) = 0;
    virtual void Decrease(int amount) = 0;
    virtual int GetValue() const = 0;
};

#endif