// CacheController.cpp
// C++17 single-file cache timing simulator

#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;

struct Config {
    size_t cache_size = 32 * 1024;
    size_t block_size = 64;
    unsigned associativity = 1;
    bool fully_associative = false;
    bool write_back = false;
    bool write_allocate = true;
    unsigned hit_time = 1;
    unsigned miss_penalty = 100;
    double mem_bytes_per_cycle = 8.0;
    unsigned address_trace_limit = 0;
    unsigned element_size = 4;
};

struct Stats {
    u64 accesses = 0, reads = 0, writes = 0;
    u64 hits = 0, misses = 0, writebacks = 0;
    u64 memory_bytes_transferred = 0;
    unsigned cycles = 0;
};

struct Line {
    bool valid = false;
    bool dirty = false;
    u64 tag = 0;
    unsigned last_use = 0;
};

struct Set { vector<Line> lines; };

class Cache {
public:
    Cache(const Config &cfg): cfg(cfg) {
        if (cfg.fully_associative) {
            associativity = (cfg.cache_size / cfg.block_size);
            n_sets = 1;
        } else if (cfg.associativity == 0) {
            associativity = (cfg.cache_size / cfg.block_size);
            n_sets = 1;
        } else {
            associativity = cfg.associativity;
            n_sets = (cfg.cache_size / cfg.block_size) / associativity;
        }
        sets.resize(n_sets);
        for (auto &s : sets) s.lines.assign(associativity, Line());
        block_offset_bits = log2_floor(cfg.block_size);
        index_bits = log2_floor(n_sets);
    }

    bool access(u64 addr, bool is_write, Stats &st) {
        ++st.accesses;
        if (is_write) ++st.writes; else ++st.reads;
        u64 block_addr = addr >> block_offset_bits;
        u64 index = (index_bits==0) ? 0 : (block_addr & ((1ULL<<index_bits)-1));
        u64 tag = block_addr >> index_bits;
        Set &s = sets[index];

        for (unsigned i=0;i<associativity;++i) {
            Line &L = s.lines[i];
            if (L.valid && L.tag==tag) {
                ++st.hits;
                st.cycles += cfg.hit_time;
                L.last_use = ++use_clock;
                if (is_write) {
                    if (cfg.write_back) L.dirty = true;
                    else {
                        st.memory_bytes_transferred += cfg.block_size;
                        st.cycles += write_through_cost();
                    }
                }
                return true;
            }
        }

        ++st.misses;
        st.cycles += cfg.hit_time;
        st.cycles += cfg.miss_penalty;
        st.memory_bytes_transferred += cfg.block_size;

        unsigned victim = 0; bool found_invalid=false;
        for (unsigned i=0;i<associativity;++i) {
            if (!s.lines[i].valid) { victim = i; found_invalid=true; break; }
        }
        if (!found_invalid) {
            unsigned min_idx=0, min_use=UINT_MAX;
            for (unsigned i=0;i<associativity;++i)
                if (s.lines[i].last_use < min_use) { min_use=s.lines[i].last_use; min_idx=i; }
            victim = min_idx;
            if (cfg.write_back && s.lines[victim].valid && s.lines[victim].dirty) {
                ++st.writebacks;
                st.memory_bytes_transferred += cfg.block_size;
                st.cycles += cfg.miss_penalty;
            }
        }

        if (is_write && !cfg.write_allocate) {
            st.memory_bytes_transferred += cfg.block_size;
            st.cycles += cfg.miss_penalty;
            return false;
        }

        Line &L = s.lines[victim];
        L.valid = true; L.tag = tag; L.last_use = ++use_clock;
        if (is_write) {
            if (cfg.write_back) L.dirty = true;
            else {
                st.memory_bytes_transferred += cfg.block_size;
                st.cycles += write_through_cost();
                L.dirty = false;
            }
        } else L.dirty = false;
        return false;
    }

    void flush(Stats &st) {
        if (!cfg.write_back) return;
        for (auto &s : sets)
            for (auto &L : s.lines)
                if (L.valid && L.dirty) {
                    ++st.writebacks;
                    st.memory_bytes_transferred += cfg.block_size;
                    st.cycles += cfg.miss_penalty;
                    L.dirty=false;
                }
    }

private:
    const Config cfg;
    unsigned associativity=1, n_sets=1;
    vector<Set> sets;
    unsigned block_offset_bits=0, index_bits=0;
    unsigned use_clock=0;

    static unsigned log2_floor(size_t x) {
        unsigned b=0; while ((1ULL<<b) < x) ++b; if ((1ULL<<b)>x) --b; return b;
    }

    unsigned write_through_cost() const {
        return (unsigned)ceil((double)cfg.block_size / cfg.mem_bytes_per_cycle);
    }
};

Config load_config(const string &path) {
    Config c;
    ifstream in(path);
    if (!in) return c;
    string k, v;
    while (in >> k) {
        if (k=="cache_size") in>>c.cache_size;
        else if (k=="block_size") in>>c.block_size;
        else if (k=="associativity") in>>c.associativity;
        else if (k=="fully_associative") { in>>v; c.fully_associative=(v=="1"||v=="true"); }
        else if (k=="write_back") { in>>v; c.write_back=(v=="1"||v=="true"); }
        else if (k=="write_allocate") { in>>v; c.write_allocate=(v=="1"||v=="true"); }
        else if (k=="hit_time") in>>c.hit_time;
        else if (k=="miss_penalty") in>>c.miss_penalty;
        else if (k=="mem_bytes_per_cycle") in>>c.mem_bytes_per_cycle;
        else if (k=="address_trace_limit") in>>c.address_trace_limit;
        else if (k=="element_size") in>>c.element_size;
        else getline(in, v);
    }
    return c;
}

vector<u64> load_trace(const string &path, size_t limit=0) {
    vector<u64> t;
    ifstream in(path);
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        u64 a=0;
        try {
            if (line.rfind("0x",0)==0||line.rfind("0X",0)==0) a=stoull(line,nullptr,16);
            else a=stoull(line,nullptr,10);
        } catch(...) { continue; }
        t.push_back(a);
        if (limit && t.size()>=limit) break;
    }
    return t;
}

void simulate(const Config &cfg, const vector<u64> &trace, const string &label) {
    Cache c(cfg);
    Stats st;
    for (u64 addr : trace) {
        bool is_write = ((addr/cfg.element_size) % 7 == 0);
        c.access(addr, is_write, st);
    }
    c.flush(st);

    double miss_rate = (st.accesses? (double)st.misses/st.accesses : 0);
    double amat = cfg.hit_time + miss_rate * cfg.miss_penalty;
    double bytes_cycle = st.cycles? (double)st.memory_bytes_transferred/st.cycles : 0;

    cout << "--- "<<label<<" ---\n";
    cout << "Accesses: "<<st.accesses<<" Hits: "<<st.hits
         <<" Misses: "<<st.misses<<" MissRate="<<miss_rate<<"\n";
    cout << "Writebacks: "<<st.writebacks<<"\n";
    cout << "MemBytes: "<<st.memory_bytes_transferred<<" Cycles="<<st.cycles<<"\n";
    cout << "AMAT: "<<amat<<"\n";
    cout << "Bytes/Cycle: "<<bytes_cycle<<"\n\n";
}

int main(int argc,char**argv){
    if(argc<3){ cerr<<"Usage: ./CacheController config.cfg trace.txt"; return 1; }
    Config cfg=load_config(argv[1]);
    auto trace=load_trace(argv[2], cfg.address_trace_limit);
    if(trace.empty()){ cerr<<"Empty trace"; return 1; }

    Config c1=cfg; c1.associativity=1; c1.fully_associative=false; c1.write_back=false;
    simulate(c1,trace,"Direct-Mapped + WT");
    c1.write_back=true; simulate(c1,trace,"Direct-Mapped + WB");

    Config c2=cfg; c2.fully_associative=true; c2.write_back=false;
    simulate(c2,trace,"Fully-Assoc + WT");
    c2.write_back=true; simulate(c2,trace,"Fully-Assoc + WB");

    Config c3=cfg; if(c3.associativity<=1) c3.associativity=4;
    c3.fully_associative=false; c3.write_back=false;
    simulate(c3,trace,"Set-Assoc + WT (assoc="+to_string(c3.associativity)+")");
    c3.write_back=true; simulate(c3,trace,"Set-Assoc + WB (assoc="+to_string(c3.associativity)+")");
}