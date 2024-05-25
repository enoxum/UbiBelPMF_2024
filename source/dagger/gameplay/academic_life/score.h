#ifndef SCORE_H
#define SCORE_H

#include <iostream>

class Score {
public:
    virtual void Update(int amount) = 0;
    virtual int GetValue() const = 0;
    virtual void Reset() = 0;
};

#endif