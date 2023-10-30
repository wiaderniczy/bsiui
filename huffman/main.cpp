#include <iostream>
#include <map>
#include <fstream>
#include <cmath>
#include <vector>
#include <bitset>
#include <algorithm>

std::string readFromFile(std::string * filename){
    std::ifstream file;
    file.open(*filename, std::ios::in);
    if (!file.good()){
        std::cout << "No input file";
        return "";
    }
    std::string text;
    getline(file, text);
    return text;
}

void encode(const std::string text){
    int iterator = 0;
    std::map<char, int> alphabet = {};
    for (int i = 0; i < text.size(); i++) {
        auto it = alphabet.find(text[i]);
        if (it == alphabet.end()) {
            alphabet[text[i]] = iterator++;
        }
    }

    std::vector<std::pair<char, int>> sortedVector(alphabet.begin(), alphabet.end());
    std::sort(sortedVector.begin(), sortedVector.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    int N = ceil(log2(iterator));
    int textBitSize = N * text.size();
    int finalBitSize = textBitSize;
    int addition = 0;
    int additionSize = 0;
    if (textBitSize % 8 != 0) {
        finalBitSize = textBitSize + (8 - (textBitSize % 8));
        addition = finalBitSize - textBitSize;
        additionSize = ceil(log2(addition + 1));
    }

    std::bitset<8> additionRep(addition);
    std::string encodedString = additionRep.to_string().substr(8 - additionSize);
    for (auto i = 0; i < text.size(); i++){
        std::bitset<32> rep(alphabet.at(text[i]));
        encodedString += rep.to_string().substr(32 - N);
    }
    std::string additionLastBits = "";
    for (auto i = 0; i < addition - additionSize; i++ ){
        additionLastBits += "0";
    }
    encodedString += additionLastBits;

    int outputSize = iterator + 1 + finalBitSize/8;
    char output[outputSize] = {};
    output[0] = iterator;
    for (auto i = 0; i < sortedVector.size(); i++) {
        output[i+1] = sortedVector.at(i).first;
    }
    int guard = encodedString.length()/8;
    for (int i = 0; i < guard; i++){
        output[1 + sortedVector.size() + i] = std::stoi(encodedString.substr(0, 8), nullptr, 2);
        encodedString.erase(0,8);
    }
    std::ofstream out;
    out.open("encoded", std::ios::app);
    for (char x: output){
        out << x;
    }
    std::cout << output[0];
    out.close();

}

void decode(std::string * filename){

}

int main() {
    std::string file;
    std::string text;
    std::cout << "File location:\n";
    std::cin >> file;
    text = readFromFile(&file);
    encode(text);
    return 0;
}
