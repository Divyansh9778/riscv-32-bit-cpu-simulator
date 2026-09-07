// TraceGenerator.cpp
// produces a simple strided trace resembling matrix multiplication access pattern

#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;

int main(int argc, char** argv) {
    if (argc < 6) {
        cerr << "Usage: TraceGenerator N M stride element_size output_file\n";
        cerr << "Example: ./TraceGenerator 256 256 4 4 example_trace.txt\n";
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int stride = atoi(argv[3]);
    int elem = atoi(argv[4]);
    string out = argv[5];

    u64 baseA = 0x00000000ULL;
    u64 baseB = 0x10000000ULL;

    ofstream fo(out);

    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            for (int j = 0; j < N; j++) {
                u64 addrA = baseA + ((u64)i * M + k) * elem;
                u64 addrB = baseB + ((u64)k * N + j) * elem;

                fo << "0x" << hex << addrA << "\n";
                fo << "0x" << hex << addrB << "\n";

                for (int s = 1; s < stride; s++) {
                    fo << "0x" << hex << (addrA + s * elem) << "\n";
                    fo << "0x" << hex << (addrB + s * elem) << "\n";
                }
            }
        }
    }

    fo.close();
    cout << "Generated trace: " << dec << (long)N * M * N * 2 * stride << " addresses written to " << out << "\n";
    return 0;
}
