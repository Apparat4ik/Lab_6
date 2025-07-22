#include <iostream>
#include <sstream>
#include <vector>


using namespace std;


int CountOfNumbers (const vector<string>& alphabet, const int& k){
    int k_counter = 0;
    for (string word1 : alphabet){
        for (string word2 : alphabet){
            if (word1.size() + word2.size() == k){
                k_counter++;
            }
        }
    }
    return k_counter;
}


int main(){
    int n, k;
    cout << "Введите n и k: ";
    stringstream serr;
    try {
        cin >> n >> k;
        if (cin.fail() || 2 > n || n > 10 || 2 > k || k > 10){
            serr << "Неверный ввод, n должно быть в промежутке [2, 10], а k - в [2, 6]" << endl;
            throw invalid_argument(serr.str());
        }
        
        vector<string> alphabet;
        for (int i = 0; i < n; i++){
            string word;
            cin >> word;
            for (char w : word){
                if (static_cast<int>(w) == 0){
                    serr << "В шифрах букв не должно быть нулей!";
                    throw invalid_argument(serr.str());
                }
            }
            
            if (word.size() > 5 || word.size() < 1){
                serr << "Длина шифра должна быть [1, 5]";
                throw invalid_argument(serr.str());
            }
            alphabet.push_back(word);
        }
        
        cout << "Всего слов длины k: " << CountOfNumbers(alphabet, k) << endl;
    } catch (exception& s){
        cerr << s.what() << endl;
    }
    
    return 0;
}
