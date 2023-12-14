//
//  output.h
//  SP_LAB4v3
//
//  Created by vika on 05.11.2023.
//

#ifndef output_h
#define output_h

void NonTerminals_set_otput(set<NonTerminal> set_out){
    if (set_out.size() == 0){
        cout << "There are none";
        return;
    }
    for (auto& nonTerm : set_out){
        cout << nonTerm.value << " ";
    }
}

void Map_output(map<NonTerminal, set<vector<Terminal>>> map_out){
    for (auto& row : map_out){
        cout << row.first.value << ": ";
        int k = 0;
        for (auto& vect_row : row.second){
            for (auto& Term : vect_row){
                cout << Term.value;
            }
            if (k == row.second.size()-1){
                break;
            }
            k++;
            cout << ", ";
        }
        cout << endl;
    }
}


#endif /* output_h */
