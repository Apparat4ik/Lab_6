#include <iostream>
#include <vector>
#include <stdint.h>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cuchar>

using namespace std;


random_device r;
mt19937 gen(r());
uniform_int_distribution<uint8_t> dist (0x00, 0xff);

vector<vector<vector<uint8_t>>>  Keys;

vector<uint8_t> Sbox = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

vector<vector<uint8_t>> Rcon = {
        {0x01, 0x00, 0x00, 0x00},
        {0x02, 0x00, 0x00, 0x00},
        {0x04, 0x00, 0x00, 0x00},
        {0x08, 0x00, 0x00, 0x00},
        {0x10, 0x00, 0x00, 0x00},
        {0x20, 0x00, 0x00, 0x00},
        {0x40, 0x00, 0x00, 0x00},
        {0x80, 0x00, 0x00, 0x00},
        {0x1b, 0x00, 0x00, 0x00},
        {0x36, 0x00, 0x00, 0x00}
    };


vector<vector<uint8_t>> State = {};
vector<vector<uint8_t>> IV;

vector<vector<uint8_t>> Cypher_text = {{0, 0, 0, 0},
                                       {0, 0, 0, 0},
                                       {0, 0, 0, 0},
                                       {0, 0, 0, 0}};

vector<vector<uint8_t>> Text_block = {{0, 0, 0, 0},
                                      {0, 0, 0, 0},
                                      {0, 0, 0, 0},
                                      {0, 0, 0, 0}};


vector<vector<uint8_t>> MC_table = {
        {0x02, 0x03, 0x01, 0x01},
        {0x01, 0x02, 0x03, 0x01},
        {0x01, 0x01, 0x02, 0x03},
        {0x03, 0x01, 0x01, 0x01}
    };



ostream& operator<<(ostream& os, const vector<vector<uint8_t>>& vc){
    for (vector<uint8_t> it : vc){
        for (unsigned char ltr : it){
            os << hex << ltr << '\t';
        }
    }
    return os;
}


void BytesToMatrix(string& message){
    int row = 0;
    while (!(message.size() % 16 == 0)){
        message += ' ';
    }
    for (int i = 0; i < 16; i++){
        if (i % 4 == 0 && i != 0){
            row++;
        }
        Text_block[row][i % 4] = (static_cast<uint8_t>(message[i]));
    }
    message.erase(0, 16);
}


vector<uint8_t> RotWord(vector<uint8_t> col){
    vector<uint8_t> new_col;
    for (int i = 0; i < 4; i++){
        new_col.push_back(col[(i + 1) % 4]);
    }
    for (uint8_t& a : new_col){
        a = Sbox[a];
    }
    
    return new_col;
}


void GenMasterKey(const string& filename){
    uint8_t master_key[16];
    for (int i = 0; i < 16; i++){
        master_key[i] = (uint8_t)dist(r);
    }
    
    ofstream out_file(filename , ios::binary | ios::trunc);
    
    cout << out_file.is_open() << endl;
    
    out_file.write(reinterpret_cast<char*>(master_key), 16);
    
    out_file.close();
    
    cout << "Мастер-ключ сгенерирован в файл: " << filename << endl;
    cout << "Мастер-ключ: " << endl;
    for (uint8_t w : master_key){
        cout << hex << setw(2) << setfill('0') << (int)w << ' ';
    }
    cout << endl;
    
}





void expand_key(const string& filename){
    ifstream mk_file(filename, ios::binary);
    uint8_t master_key[16];
    mk_file.read((char*)(master_key), 16);
    mk_file.close();
    
    
    Keys.push_back(vector<vector<uint8_t>>());
    int row = 0;
    for (int i = 0; i < 16; i++){
        if (i % 4 == 0){
            Keys[0].push_back(vector<uint8_t>());
            if (i != 0){row++;}
        }
        Keys[0][row].push_back(master_key[i]);
        
    }
    
    for (int num = 1; num <= 10; num++){
        Keys.push_back(vector<vector<uint8_t>>());
        for (int i = 0; i < 4; i++){
            Keys[num].push_back(vector<uint8_t>());
            for (uint8_t w : RotWord(Keys[num - 1][3])){
                w = Sbox[w];
                Keys[num][0].push_back(Keys[num - 1][0][i] ^ w ^ Rcon[num - 1][i]);
            }
        }  // xor-ит первую строку предыдущего ключа, его же последнюю строку, сдвинутую вправо и строку Rcon
        
        
        for (int i = 1; i <= 3; i++){
            for (int j = 0; j < 4; j++){
                Keys[num][i].push_back(Keys[num - 1][i][j] ^ Keys[num][i - 1][j]);
            }
        }
    }
}


void SubBytes(){
    for (vector<uint8_t>& row : State){
        for (uint8_t& a : row){
            a = Sbox[a];
        }
    }
}



uint8_t Gmul(uint8_t a, uint8_t b) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {  // если младший бит = 1, то в result пишется a
            result ^= a;
        }
        bool carry = (a & 0x80);
        a <<= 1; // сдвиг a влево (домножение на x)
        if (carry) {
            a ^= 0x11B;  // если число больше 128, то берем по модулю неприводимого многочлена
        }
        b >>= 1; // сдвиг вправо для обработки всех множителей b
    }
    return result;
}



void MixColumns(){
    vector<vector<uint8_t>> new_matrix = {{}, {}, {}, {}};
    
    for (int col = 0; col < 4; col++){
        for (int row = 0; row < 4; row++){
            uint8_t summ = 0;
            for (int i = 0; i < 4; i++){
                summ ^= Gmul(State[i][col], MC_table[row][i]);  // перемножаем матрицы
            }
            new_matrix[row].push_back(summ);
        }
    }
    State = new_matrix;
}


void ShiftRows(){
    int shift = 0;
    for (vector<uint8_t>& row : State){
        vector<uint8_t> curent = row;
        for (int i = 0; i < 4; i++){
            row[i] = curent[(i + 3 + shift) % 4];
        }
        shift++;
    }
}


void AddRoundKey(auto roundkey){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            State[i][j] ^= roundkey[i][j];
        }
    }
}



void encrypt_block(string& cypher){
    
    if (State.empty()){
        int row = 0;
        for (int i = 0; i < 16; i++){
            if (i % 4 == 0){
                State.push_back(vector<uint8_t>());
                if (i != 0){row++;}
            }
            State[row].push_back(dist(gen));   // инициализация IV
        }
        IV = State;
    }
    
    // первый раунд
    AddRoundKey(Keys[0]);
    
    
    
    // 2 - 10 раунды
    for (int r_num = 1; r_num <= 9; r_num++){
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(Keys[r_num]);
    }
    
    // финальный раунд
    SubBytes();
    ShiftRows();
    AddRoundKey(Keys[10]);
    
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            Cypher_text[i][j] = (State[i][j] ^ Text_block[i][j]);
            cypher += State[i][j] ^ Text_block[i][j];
        }
    }
    
}



void decrypt_block(string& plain_text){
    
    // первый раунд
    AddRoundKey(Keys[0]);
    
    
    
    // 2 - 10 раунды
    for (int r_num = 1; r_num <= 9; r_num++){
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(Keys[r_num]);
    }
    
    // финальный раунд
    SubBytes();
    ShiftRows();
    AddRoundKey(Keys[10]);
    
    // 3. Расшифровка: XOR ciphertext и keystream
       for (int i = 0; i < 4; i++) {
           for (int j = 0; j < 4; j++) {
               plain_text += static_cast<char>(Text_block[i][j] ^ State[i][j]);
           }
       }
}



int main(){
    cout << "Введите сообщение, которое хотите зашифровать" << endl;
    
    string plain_text;
    getline(cin, plain_text);
    string cypher_text;
    
    GenMasterKey("/Users/vladislav/Documents/Lab_6/Lab_6/MasterKey.txt");
    expand_key("/Users/vladislav/Documents/Lab_6/Lab_6/MasterKey.txt");
    while (!plain_text.empty()){
        BytesToMatrix(plain_text);
        encrypt_block(cypher_text);
    }
    cout << endl;
    
    cout << "Зашифрованный текст:" << endl;
    
    for (unsigned char r : cypher_text){
        cout << hex << static_cast<unsigned int>(r) << '\t';
    }
    
    cout << endl;
    State = IV;
    while (!cypher_text.empty()){
        BytesToMatrix(cypher_text);
        decrypt_block(plain_text);
    }
    
    cout << "Расшифровка: " << endl;
    cout << plain_text << endl;
   
    return 0;
}

