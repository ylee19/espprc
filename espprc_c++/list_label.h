#pragma once

#include <iostream>
#include <list>

using namespace std;

#include "label.h"

class ListLabel : public list<Label> {
public:
    int efficientInsertion(Label &lb);
};


