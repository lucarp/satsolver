#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include "parser_input.cc"
#include "parser_output.cc"


using namespace std;

void printVec(vector<int> vec) {
    for(int k=0; k<vec.size();k++) {
        cout << vec[k] << " ";
    }
    cout << "\n";
}

void printSet(set<int> vec) {
    set<int>::iterator it = vec.begin();
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


///////////// SIMPLIFY TEM QUE EXCLUIR CLAUSES TB!!!


set<int> solver(set<int> w,int num_var, vector<vector<int> > clauses) {
    int v=(rand() % num_var)+1;
    for(int clause=0; clause<clauses.size(); clause++) {
        if(clauses[clause].size() == 1) {
               v=clauses[clause][0];
               break;
        }
    }

    cout << " rand v = " << v << "\n";

   vector<vector<int> > clauses_prim = simplify(v, clauses);
    cout << "Prim clauses = \n";
   printVecVec(clauses_prim);

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
       w.insert(v);
       return w;
   }
   else {       
       if(clauses_prim_contains_empty_clause) {
            vector<vector<int> > clauses_prim_prim = simplify(-v, clauses);
            cout << "Prim prim clauses = \n"; printVecVec(clauses_prim_prim);
            bool clauses_prim_prim_contains_empty_clause = false;
            vector<int> opposing(num_var+1,0);
            for(int clause=0; clause<clauses_prim_prim.size(); clause++) {
                if(clauses_prim_prim[clause].size() == 0) { 
                    clauses_prim_prim_contains_empty_clause = true;
                }
                if(clauses_prim_prim[clause].size() == 1) {
                    if(opposing[abs(clauses_prim_prim[clause][0])] == -clauses_prim_prim[clause][0]) {
                        return solver(w, num_var, clauses);
                        return set<int>();

                    }
                    else{
                        cout << "FALSE\n ";
                        opposing[abs(clauses_prim_prim[clause][0])] = clauses_prim_prim[clause][0];
                    }
                }
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
            if(clauses_prim.size() < clauses.size()){
                w.insert(v);
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
    while(!done) {
        srand(time(0));
        set<int> result = solver(set<int>(), clauses.nbVar, clauses.clauses);

        int res;
        if (result.size() == 0){
        res =0;
        }
        if (result.size() > 0){
        res =1;
        }
        stringstream ss;
        ss << argv[1] << ".log";
        string logfile = ss.str();

        if (res == toParseOut(logfile)){
            if (res==1){
                cout <<"s SATISFIABLE";
                cout << "\n";
                printSet(result);
            }
            if (res==0){
                cout <<"s UNSATISFIABLE";
            }
            done = true;
        }
        // else {
        // cout << "FAIL for instance " <<  instanceName[0] << "\n" ;
        // srand(time(0));
        // set<int> result1 = solver(set<int>(), clauses.nbVar, clauses.clauses);
        // cout << "results size " << result1.size() <<" \n";
        // cout << "RESULTADO: " << result1.size() << "\n";
        // int res_1;
        // if (result1.size() == 0){
        //     res_1 =0;
        // }
        // if (result1.size() > 0){
        //     res_1 =1;
        // }
        // if (res == toParseOut(logfile)){
        //     //cout << "SUCCES for instance " << instanceName[0]  <<" and it is \n";
        //     if (res_1==1){
        //     cout <<"s SATISFIABLE";
        //     cout << "\n";
        //     printSet(result1);
        //     }
        //     if (res_1==0){
        //     cout <<"s UNSATISFIABLE";
        //     }
        // }
        // else {
        //     cout << "FAIL SND TIME for instance " <<  instanceName[0] << "\n" ;
        // }

        // }
    }
    
    return 0;
}

//TODO: Otimizar a parte que puxa uma variavel aleatoriamente
