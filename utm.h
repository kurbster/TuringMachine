#ifndef UTM_H
#define UTM_H

#include <string>
#include <map>

using namespace std;

#ifdef _WIN32
    const string DATADIR = "machines\\";
#else
    const string DATADIR = "machines/";
#endif

struct action { char write, direction; };
// A state is a lhs -> rhs. Each side is a string
// lhs = currentState+readSymbol
// rhs = writeSymbol+moveDirection+nextState
// Ex:
//      state1;j;a;R;state2
//      lhs = state1j
//      rhs = aRstate2
//      A;0;1;R;B
//      lhs = A0
//      rhs = 1RB
// Each lhs will be unique because every state
// can has a unique tranition for each symbol
typedef map<string, string> State;
#endif
