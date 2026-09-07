#include <bits/stdc++.h>
#include "Executor.cpp"
using namespace std;

int main(){
    // Fetch
    ifstream file("examples/input.txt");
    string line;

    // Decode
    while (getline(file, line)){
        if (line.empty()) continue;
        assembler(line);
    }
    
    // Execute
    execute();
    return 0;
}