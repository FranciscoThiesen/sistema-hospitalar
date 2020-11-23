#include <bits/stdc++.h>
#include "json.hpp"
#include "persistencia.hpp"
#include "f.hpp"
using json = nlohmann::json;

using namespace std;
#define TRACE(x) x
#define WATCH(x) TRACE( cout << #x" = " << x << endl)
#define PRINT(x) TRACE(printf(x))
#define WATCHR(a, b) TRACE( for(auto c = a; c != b;) cout << *(c++) << " "; cout << endl)
#define WATCHC(V) TRACE({cout << #V" = "; WATCHR(V.begin(), V.end());})
#define rep(i, a, b) for(int i = (a); (i) < (b); ++(i))
#define trav(a, x) for(auto& a : x)
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) static_cast<int>(x.size())
#define mp make_pair
#define fi first
#define se second

using vi = vector<int>;
using vvi = vector<vi>;
using ll = long long;
using vll = vector<ll>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

void buff() { ios::sync_with_stdio(false); cin.tie(nullptr); }

constexpr ll MOD = 1e9 + 7;

inline ll pow_mod(ll a, ll b, ll mod = MOD) {
    ll res = 1; a %= mod; assert(b >= 0);
    for(;b;b>>=1) {
        if(b & 1) res = (res * a) % mod;
        a = (a * a) % mod;
    }
    return res;
}

template<typename T> inline void remin(T& x, const T& y) { x = min(x, y); }
template<typename T> inline void remax(T& x, const T& y) { x = max(x, y); }

template<typename T> ostream& operator<<(ostream &os, const vector<T>& v) {
    os << "{"; string sep; for(const auto& x : v) os << sep << x, sep = ", "; return os << "}"; 
}

template<typename T, size_t size> ostream& operator<<(ostream& os, const array<T, size>& arr) {
    os << "{"; string sep; for(const auto& x : arr) os << sep << x, sep = ", "; return os << "}"; 
}

template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { 
    return os << '(' << p.first << ", " << p.second << ')'; 
}

/*
namespace ns {

    struct v {
        int val;
        string name;
        v() {}
    };

    void to_json(json& j, const v& x) {
        j = json{ {"val", x.val}, {"name", x.name} };
    }

    void from_json(const json& j, v& x) {
        j.at("name").get_to(x.name);
        j.at("val").get_to(x.val);
    }
}*/

struct v {
    int val; 
    string name;
    v() {}
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(v, val, name);


int main() {
    buff();
    auto x = v();
    json r = json::array();
    x.name = "kleber";
    json j = x;
    r.push_back(j);
    x.name = "fuck";
    x.val = 17; 
    json k = x;
    r.push_back(k);
    cout << setw(4) << r << endl;
    /*
    unordered_map< int, medico > x;
    medico m = medico(10, "cleber");
    x[0] = m;
    medico m2 = medico(20, "bernardo");
    x[1] = m2;
    
    auto json_vec = json::array();
    
    for(const auto& val : x) {
        json current;
        current["chave"] = val.first;
        current["valor"]["CRM"] = val.second.id;
        current["valor"]["nome"] = val.second.nome;

        auto z = current["valor"];
        if(z.find("tetinha") == z.end()) cout << "nao acheiiii" << endl;
        json_vec.push_back(current);
    }
    
    json r = json::object();
    r["medicos"] = json_vec;
    ofstream out("testando");
    out << r << endl; 
    out.close();

    json y;
    ifstream in("testando");
    in >> y; 
    for(auto& entry : y["medicos"]) {
        cout << entry["valor"] << endl;
        cout << entry["chave"] << endl << endl;
    }
    cout << y << endl;

    cout << filesystem::exists("aba") << endl;
    */
    optional<int> v;
    v = make_optional(13);
    cout << v.value() << endl;
    return 0;
}

