#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include "parser_input.cc"

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
    for(vector<vector<int>>::iterator it=clauses_prim.begin(); it!=clauses_prim.end();) {
        bool removed_clause = false;
        for(int search_idx=0; search_idx<it->size(); search_idx++) {
            // cout << "Tentando remover da clause "  << (*it)[search_idx] << " - "  << l <<  " \n";
            if((*it)[search_idx] == l) {
                clauses_prim.erase(it);
                try_negation = false;
                removed_clause = true;
                break;
            }
            else {
                (*it).erase(std::remove((*it).begin(),(*it).end(),-l), (*it).end());
            }
        }
        if(!removed_clause) it++;
    }
    return clauses_prim;
}


///////////// SIMPLIFY TEM QUE EXCLUIR CLAUSES TB!!!


set<int> solver(set<int> w,int num_var, vector<vector<int>> clauses) {
    int v=(rand() % num_var)+1;;
    bool choose_another = true;

    cout << " rand v = " << v << "\n";
   //TODO: Changer cette partie pour prendre une variable que n'a pas ete deja prise.

   vector<vector<int>> clauses_prim = simplify(v, clauses);
   cout << "Prim clauses = \n";
   printVecVec(clauses_prim);

   //Verifie si il n'y ont que clauses vides
   bool clauses_prim_contains_empty_clause = false;
   for(int clause=0; clause<clauses_prim.size(); clause++) {
       if(clauses_prim[clause].size() == 0){
        clauses_prim_contains_empty_clause = true;
      }
   }

   if(clauses_prim.size() == 0) {
       w.insert(v);
       return w;
   }
   else {
       if(clauses_prim_contains_empty_clause) {
            vector<vector<int>> clauses_prim_prim = simplify(-v, clauses);
            cout << "Prim prim clauses = \n"; printVecVec(clauses_prim_prim);
            bool clauses_prim_prim_contains_empty_clause = false;
            for(int clause=0; clause<clauses_prim_prim.size(); clause++) {
                if(clauses_prim_prim[clause].size() == 0) clauses_prim_prim_contains_empty_clause = true;
            }
            if(clauses_prim_prim.size() == 0) {
                w.insert(-v);
                return w;
            }
            else {
                if(clauses_prim_prim_contains_empty_clause) {
                    return set<int>();
                }
                else {
                    w.insert(-v);
                    return solver(w, num_var, clauses_prim_prim);
                }
            }
       }
       else {
            if(clauses_prim.size() < clauses.size() && v > 0){
                w.insert(v);
            }

           return solver(w, num_var, clauses_prim);
       }
   }
}

int main(int argc, char* argv[]) {
    // vector<vector<int>> clauses = toParseInput(argv[1]);

    //vector<vector<int>> clauses = {{1,4},{3,-4,-5},{-2,-3,-4}};

    // cout << "Importing file " << argv[1];
    // vector<vector<int>> clauses = toParseInput(argv[1]);
    // cout << " clauses size parser_input " << clauses.size() << "\n";
    // for (int i=0; i<clauses.size(); i++) {
    //   cout << " clause size (of each line ) " << clauses[i].size() << "\n";

    // }
    vector<vector<int>> clauses = {{1,2,-4},{1,-3,2,-5,4},{-3,5},{-4,5},{1,2,-4,-5},{-2,-3,-4},{4},{2,-4,3,5},{1,-2,-3,-5},{-3,-4}};


    //vector<vector<int>> clauses = {{1,2,3},{1,2,-3},{1,-2,-3},{-1,2,3},{-1,2,-3},{-1,-2,3},{-1,-2,-3}};

    set<int> result = solver(set<int>(), 5, clauses);
    cout << "RESULTADO: " << result.size() << "\n";
    printSet(result);

    return 1;
}

//TODO: Otimizar a parte que puxa uma variavel aleatoriamente
