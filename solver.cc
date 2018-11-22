#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

void printVec(vector<int> vec) {
    for(int k=0; k<vec.size();k++) {
        cout << vec[k] << " "; 
    }
    cout << "\n"; 
}

void printSet(set<int> vec) {
    set<int>::iterator it = vec.begin();
    while(it != vec.end()) {
        cout << (*it) << " ";
        it++;
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

vector<vector<int>> simplify(int l, vector<vector<int>> clauses) {
    vector<vector<int>> clauses_prim = clauses;
    bool try_negation=true;
    for(int clause_idx=0; clause_idx<clauses_prim.size();clause_idx++) {
        for(int search_idx=0; search_idx<clauses_prim[clause_idx].size(); search_idx++) {
            if(clauses_prim[clause_idx][search_idx] == l) {
                clauses_prim.erase(std::remove(clauses_prim.begin(),clauses_prim.end(),clauses_prim[clause_idx]), clauses_prim.end());
                try_negation = false;
                break;
            } 
        }
    }
    if(try_negation) {
        for(int clause_idx=0; clause_idx<clauses_prim.size();clause_idx++) {
            int vec_size = clauses_prim[clause_idx].size();
            // cout << "Tentando remover "  << -l <<  " de "  << variables[abs(l)][clause_idx] <<  " \n";
            clauses_prim[clause_idx].erase(std::remove(clauses_prim[clause_idx].begin(),clauses_prim[clause_idx].end(),-l), clauses_prim[clause_idx].end());
        }
    }
    return clauses_prim;
}


///////////// SIMPLIFY TEM QUE EXCLUIR CLAUSES TB!!!


set<int> solver(set<int> w,int num_var, vector<vector<int>> clauses) {
    cout << "Comecando novo solver; Press any key;"; 
    cin.get();
    int v = rand() % num_var;
    cout << " rand v = " << v << "\n";
   //TODO: Changer cette partie pour prendre une variable que n'a pas ete deja prise.
   
   auto clauses_prim = simplify(v, clauses);
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
       w.insert(v);
       return w;
   }
   else {
       if(clauses_prim_contains_empty_clause) { 
            auto clauses_prim_prim = simplify(-v, clauses);
            cout << "Prim prim clauses = \n"; printVecVec(clauses_prim_prim);
            int max_clause_size = 0;
            bool clauses_prim_prim_contains_empty_clause = false;
            for(auto clause : clauses_prim_prim) {
                if(clause.size() > max_clause_size) max_clause_size = clause.size();
                if(clause.size() == 0) clauses_prim_prim_contains_empty_clause = true;
            }
            if(max_clause_size == 0) {
                w.insert(-v); //This means v is false
                return w;
            }
            else {
                if(clauses_prim_prim_contains_empty_clause) {
                    cout << "clauses_prim_prim_contains_empty_clause\n"; 
                    return set<int>();
                }
                else {
                    return solver(w, num_var, clauses_prim_prim);
                }
            }
       }
       else {
            if(clauses_prim.size() < clauses.size()){
                cout << "Clauses_prim.size()="  << clauses_prim.size() << " - Clauses.size()= "  << clauses.size() << "\n";
                cout << " v = "  << v << "\n";
                w.insert(v);
            }
           
           return solver(w, num_var, clauses_prim);
       }
   }
}

int main() {
    vector<vector<int>> variables = {{},{0},{2},{1,2},{0,1,2},{1}};
    vector<vector<int>> clauses = {{1,4},{3,-4,-5},{-2,-3,-4}};

    auto result = solver(set<int>(), 1, clauses);
    cout << "RESULTADO: " << result.size() << "\n"; 
    printSet(result);

    return 1;
}

//TODO: tirar variables (nao estou mais uando
//TODO: Otimizar a parte que puxa uma variavel aleatoriamente