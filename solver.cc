#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include "parser_input.cc"
#include "parser_output.cc"


using namespace std;

struct solverResult {
  vector<int> status;
  bool satisfiable;
};

void printVec(vector<int> vec) {
    for(int k=0; k<vec.size();k++) {
        cout << vec[k] << " ";
    }
    cout << "\n";
}

void printSet(vector<int> vec) {
    vector<int>::iterator it = vec.begin();
    if (vec.size()>0){
      cout << "v ";

    }
    while(it != vec.end()) {
        cout << (*it) << " ";
        it++;
    }
    cout << "\n";
}

void printVecVec(vector<vector<int> > vec) {
    for(int k=0; k<vec.size();k++) {
        for(int i=0; i<vec[k].size();i++) {
            cout << vec[k][i] << " ";
        }
        cout << "\n";
    }
}

vector<vector<int> > simplify(int l, vector<vector<int> > clauses) {
    vector<vector<int> > clauses_prim = clauses;
    bool try_negation=true;
    for(vector<vector<int> >::iterator it=clauses_prim.begin(); it!=clauses_prim.end();) {
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



solverResult solver(vector<int> w,int num_var, vector<vector<int> > clauses) {
    int v=(rand() % num_var)+1;
    bool unit_clause_found = false;
    for(int clause=0; clause<clauses.size(); clause++) {
        if(clauses[clause].size() == 1) {
               v=clauses[clause][0];
               break;
        }
    }
    if(!unit_clause_found) {
        for(int clause=0; clause<clauses.size(); clause++) {
            int max =0, min=0;
            for(int literal=0; literal<clauses[clause].size(); literal++) {
                if(clauses[clause][literal] > max ) max = clauses[clause][literal];
                if(clauses[clause][literal] < min ) min = clauses[clause][literal];
            }
            if(max<0) {
                v=clauses[clause][0];
                break;
            }
            if(min>0) {
                v=clauses[clause][0];
                break;
            }
        }
    }

    // cout << " rand v = " << v << "\n";

   vector<vector<int> > clauses_prim = simplify(v, clauses);
//     cout << "Prim clauses = \n";
//    printVecVec(clauses_prim);

   //Verifie si il n'y ont que clauses vides
   bool clauses_prim_contains_empty_clause = false;
   vector<int> opposing(num_var+1,0);
   for(int clause=0; clause<clauses_prim.size(); clause++) {
        if(clauses_prim[clause].size() == 0){
            clauses_prim_contains_empty_clause = true;
        }

        if(clauses_prim[clause].size() == 1) {
            if(opposing[abs(clauses_prim[clause][0])] == -clauses_prim[clause][0]) {
                clauses_prim_contains_empty_clause = true;
            }
            else{
                opposing[abs(clauses_prim[clause][0])] = clauses_prim[clause][0];
            }
        }
   }

   if(clauses_prim.size() == 0) {
       if(std::find(w.begin(), w.end(), v) == w.end()) w.push_back(v);
       solverResult result;
       result.satisfiable = true;
       result.status = w;
       return result;
   }
   else {       
       if(clauses_prim_contains_empty_clause) {
            vector<vector<int> > clauses_prim_prim = simplify(-v, clauses);
            // cout << "Prim prim clauses = \n"; printVecVec(clauses_prim_prim);
            bool clauses_prim_prim_contains_empty_clause = false;
            vector<int> opposing(num_var+1,0);
            for(int clause=0; clause<clauses_prim_prim.size(); clause++) {
                if(clauses_prim_prim[clause].size() == 0) { 
                    clauses_prim_prim_contains_empty_clause = true;
                }
                if(clauses_prim_prim[clause].size() == 1) {
                    if(opposing[abs(clauses_prim_prim[clause][0])] == -clauses_prim_prim[clause][0]) {
                        solverResult result;
                        result.satisfiable = false;
                        result.status = w;
                        return result;

                    }
                    else{
                        opposing[abs(clauses_prim_prim[clause][0])] = clauses_prim_prim[clause][0];
                    }
                }
            }
            if(clauses_prim_prim.size() == 0) {
                if(std::find(w.begin(), w.end(), -v) == w.end()) w.push_back(-v);
                solverResult result;
                result.satisfiable = true;
                result.status = w;
                return result;
            }
            else {
                if(clauses_prim_prim_contains_empty_clause) {
                    solverResult result;
                    result.satisfiable = false;
                    result.status = w;
                    return result;
                }
                else {
                    if(std::find(w.begin(), w.end(), v) == w.end()) w.push_back(-v);
                    return solver(w, num_var, clauses_prim_prim);
                }
            }
       }
       else {
            if(clauses_prim.size() < clauses.size()){
                if(std::find(w.begin(), w.end(), v) == w.end()) w.push_back(v);
            }
           return solver(w, num_var, clauses_prim);
       }
   }
}

int main(int argc, char* argv[]) {
    

    vector<string> instanceFullName = splitStrings(argv[1], '/');
    vector<string> instanceName = splitStrings(instanceFullName[2], '.');

    dataInput clauses = toParseInput(argv[1]);
    string file = argv[1];

    bool done = false;
    srand(time(0));
    solverResult result = solver(vector<int>(), clauses.nbVar, clauses.clauses);
    while(result.status.size() > 0) {
        srand(time(0));
        result = solver(result.status, clauses.nbVar, clauses.clauses);

        if (result.satisfiable){
            cout <<"s SATISFIABLE";
            cout << "\n";
            printSet(result.status);
            return 0;
        }
        else {
            result.status.pop_back();
        }
    }
    printSet(result.status);
    cout <<"s UNSATISFIABLE";
    return 0;
}
