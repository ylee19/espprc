
#include "list_label.h"


// int ListLabel::efficientInsertion(Label &lb)
// one uses a < operator with three states
//  L1 < L2 has 2 states:
//    O : false
//    1 : true

//  one uses a = operator with three states
//  L1 = L2 has 2 states:
//    O : false
//    1 : true

// The method goes once through the list of labels
// the list is sorted with a lexicographic order
// It is decomposed in 3 phases:
// - first part of the list: check if L is dominated
// - stops when =(L,current label) is equal to 1
// - second part of the list: dominance is evaluated in both directions
// - stop when <(L,current label)
// - third part: check if L dominates some labels
// return 1 if the label was inserted

int ListLabel::efficientInsertion(Label &lb) {
    ListLabel::iterator iter;
    int temp;

    // empty list
    if (size() == 0) {
        push_back(lb);
        return 1;
    }

    // first part
    iter = begin();
    while (iter != end() && (temp = ((*iter) < lb))) {
        if (temp == 2)
            return 0;
        iter++;
    }

    // second part
    while (iter != end() && (temp = ((*iter) == lb))) {
        if (temp == 2) //complete equality
            return 0;

        if (((*iter) < lb) == 2)  // lb is dominated
            return 0;

        if ((lb < (*iter)) == 2)  // lb dominates the current label
            iter = erase(iter);
        else
            iter++;
    }

    // insert the label
    insert(iter, lb);

    // third part
    while (iter != end()) {
        if ((lb < (*iter)) == 2)
            iter = erase(iter);
        else
            iter++;
    }

    return 1;
}
