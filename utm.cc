#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utm.h"

class Tape {
public:
    // start in the middle of the tape as default if nothing is passed in
    Tape() : tape(0) { reset(); }
    void reset() {
        // Clear the tape and put all zeros
        headPos = 0;
        steps = 0;
        numOnes = 0;
    }
    char read() { return tape[headPos]; }
    void input(string a) {
        if(a == "") return;
        tape.erase(tape.begin());
        for (uint s = 0; s < a.length(); s++)
            tape.push_back(a[s]);
    }
    void clear(char c) {
        // clear the tape
        tape.clear();
        // then init it to the blank char
        blk = c;
        tape.resize(1, blk);
    }
    // perform the action by writing and moving
    void action(const action *a) { write(a->write); move(a->direction); }
    void printStats() { 
        getOnes(numOnes);
        printf("Number of steps = %d\nNumber of ones = %d\n",steps, numOnes);
    }
    // print what is on the tape
    void print() {
        for (vector<char>::iterator c = tape.begin(); c != tape.end(); c++)
            cout << *c;
        cout << endl;
    }
private:
    vector<char> tape;
    int headPos, start, steps, numOnes;
    char blk;
    void move(char d) {
        steps++;
        if (d == 'N') return;
        // move head +1 if right -1 if left
        headPos += d == 'R' ? 1 : -1;
        // if moving beyond max len move to 0
        // this is simulating infinite tape
        if (headPos < 0) {
            tape.insert(tape.begin(), blk);
            headPos = 0;
        }
        if (headPos >= (int)tape.size())
            tape.push_back(blk);
    }
    void write(char b) {
        // if the write bit is not none
        if (b != 'N') {
            char old = read();
            if (old != b)
               tape[headPos] = b; 
        }
    }
    void getOnes(int &count) {
        for (vector<char>::iterator s = tape.begin(); s != tape.end(); s++) {
            if (*s == '1')
                count++;
        }
    }
};

class transitionFunc {
public:
    bool loadTable(string fname) {
        reset();
        ifstream file;
        file.open(fname.c_str());
        if (file.is_open()) {
            string str;
            while (file.good()) {
                getline(file, str);
                // If the string contains '
                // then we are done reading states
                if(str[0] == '\'') break;
                parseState(str);
            } 
            while (file.good()) {
                getline(file, str);
                if (str == "") continue;
                // if the line begins with ! that is the blank symbol
                if (str[0] == '!') blank = str.erase(0,1)[0];
                // if the line begins with ^ that is the state state
                if (str[0] == '^') curState = str.erase(0,1);
                // if the line begins with > that is the input
                if (str[0] == '>') input = str.erase(0,1);
            }
            file.close();
            return true;
        }
        cout << "Error could not open " << fname << endl;
        return false;
    }

    bool action(char symbol, action &a) {
        string lhs = curState+symbol, rhs;
        State::iterator it;
        it = states.find(lhs);
        if (curState == "H") { return false; }
        // If we could not find a transition and we are not 
        // Halted thats a no no
        if (it == states.end()) {
            printf("INVALID TRANSITIONFUNC \nLHS = %s\n", lhs.c_str());
            return false;
        }
        rhs = it->second;
        a.write = rhs[0];
        a.direction = rhs[1];
        curState = rhs.erase(0,2);
        return true;
    }
    
    void reset() {
        states.clear();
        blank = '0';
        input = "";
        curState = input;
    }
    
    string getInput() { return input; }
    char getBlank() { return blank; }
private:
    State states;
    char blank;
    string curState, input;
    void parseState(string str) {
        // every state has an array of 5 attributes
        // name;read symbol;write symbol;move direction;next state
        string arr[5];
        int x = 0;
        for (string::iterator s = str.begin(); s != str.end(); s++) {
            // if s == ; we move to the next attribute
            if ((*s) == ';') x++;
            // otherwise append the char to the attribute
            else
                arr[x].append(&(*s), 1);
        }
        string lhs, rhs;
        lhs = arr[0]+arr[1];
        rhs = arr[2]+arr[3]+arr[4];
        states.emplace(lhs, rhs);
    }
};

class utm {
public:
    utm() {}
    void start() {
        while (true) {
            reset();
            string f = showMenu();
            // If the file entered wasn't valid
            if (!tf.loadTable(f)) continue;
            startMachine();
        }
    }
private:
    Tape tape;
    transitionFunc tf;
    void simulate() {
        char r;
        action a;
        while (true) {
            r = tape.read();
            if (!tf.action(r,a)) break;
            tape.action(&a);
        }
        tape.print();
        tape.printStats();
    }
    
    string showMenu() {
        string machName;
        cout << "\nWhat file do you want to load?\n";
        cin >> machName;
        if (machName == "q") { exit(0); }
        machName = "machines/"+machName;
        return machName;
    }

    void reset() {
        tape.reset();
        tf.reset();
    }

    void startMachine() {
        tape.clear(tf.getBlank());
        tape.input(tf.getInput());
        simulate();
    }
};

int main() {
    utm tm;
    tm.start();
    return 0;
}
