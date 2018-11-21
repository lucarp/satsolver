#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void printVec(vector<int> vec) {
    for(int k=0; k<vec.size();k++) {
        cout << vec[k] << " "; 
    }
    cout << "\n"; 
}

void printVecVec(vector<vector<int>> vec) {
    for(int k=0; k<vec.size();k++) {
        for(int i=0; i<vec[k].size();i++) {
            cout << vec[k][i] << " "; 
        }
        cout << "\n"; 
    }
}

vector<vector<int>> simplify(int l, vector<vector<int>> clauses, vector<vector<int>> variables) {
    vector<vector<int>> clauses_prim = clauses;
    bool try_negation=true;
    for(int clause_idx=0; clause_idx<variables[abs(l)].size();clause_idx++) {
        int vec_size = clauses_prim[variables[abs(l)][clause_idx]].size();
        cout << "Tentando remover "  << l <<  " de "  << variables[abs(l)][clause_idx] <<  " \n";
        clauses_prim[variables[abs(l)][clause_idx]].erase(std::remove(clauses_prim[variables[abs(l)][clause_idx]].begin(),clauses_prim[variables[abs(l)][clause_idx]].end(),l), clauses_prim[variables[abs(l)][clause_idx]].end());
        if(clauses_prim[variables[abs(l)][clause_idx]].size() != vec_size) {
            try_negation = false;
        }
    }
    if(try_negation) {
        for(int clause_idx=0; clause_idx<variables[abs(l)].size();clause_idx++) {
            int vec_size = clauses_prim[variables[abs(l)][clause_idx]].size();
            cout << "Tentando remover "  << -l <<  " de "  << variables[abs(l)][clause_idx] <<  " \n";
            clauses_prim[variables[abs(l)][clause_idx]].erase(std::remove(clauses_prim[variables[abs(l)][clause_idx]].begin(),clauses_prim[variables[abs(l)][clause_idx]].end(),-l), clauses_prim[variables[abs(l)][clause_idx]].end());
        }
    }
    return clauses_prim;
}


///////////// SIMPLIFY TEM QUE EXCLUIR CLAUSES TB!!!


vector<int> solver(vector<int> w, vector<vector<int>> variables,vector<vector<int>> clauses) {
    cout << "Comecando novo solver; Press any key;"; 
    cin.get();
    int v = rand() % variables.size();
    cout << " rand v = " << v << "\n";
   //TODO: Changer cette partie pour prendre une variable que n'a pas ete deja prise.
   
   auto clauses_prim = simplify(v, clauses, variables);
   cout << "Prim clauses = \n"; printVecVec(clauses_prim); 
   
   //Verifie si il n'y ont que clauses vides
   int max_clause_size = 0;
   bool clauses_prim_contains_empty_clause = false;
   for(auto clause : clauses_prim) {
       if(clause.size() > max_clause_size) max_clause_size = clause.size();
       if(clause.size() == 0) clauses_prim_contains_empty_clause = true;
   }
   cout << " max_clause_size = " << max_clause_size << "\n";
   if(max_clause_size == 0) {
       w.push_back(v);
       return w;
   }
   else {
       if(clauses_prim_contains_empty_clause) { 
            auto clauses_prim_prim = simplify(-v, clauses, variables);
            cout << "Prim prim clauses = \n"; printVecVec(clauses_prim_prim);
            int max_clause_size = 0;
            bool clauses_prim_prim_contains_empty_clause = false;
            for(auto clause : clauses_prim_prim) {
                if(clause.size() > max_clause_size) max_clause_size = clause.size();
                if(clause.size() == 0) clauses_prim_prim_contains_empty_clause = true;
            }
            if(max_clause_size == 0) {
                w.push_back(-v);
                return w;
            }
            else {
                if(clauses_prim_prim_contains_empty_clause) {
                    cout << "clauses_prim_prim_contains_empty_clause\n"; 
                    return vector<int>();
                }
                else {
                    return solver(w, variables, clauses_prim_prim);
                }
            }
       }
       else {
           return solver(w, variables, clauses_prim);
       }
   }
}

int main() {
    vector<vector<int>> variables = {{},{0},{2},{1,2},{0,1,2},{1}};
    vector<vector<int>> clauses = {{1,4},{3,-4,-5},{-2,-3,-4}};

    auto result = solver(vector<int>(), variables, clauses);
    cout << "RESULTADO: " << result.size() << "\n"; 
    auto simplified = simplify(1, clauses, variables);
    for(int k=0; k<result.size();k++) {
        cout << result[k] << " "; 
    }

    return 1;
}