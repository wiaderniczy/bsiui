
#include <iostream>
#include <map>
#include <fstream>
#include <cmath>
#include <vector>
#include <bitset>
#include <algorithm>

std::string readFromFile(std::string *filename) {
    std::ifstream file;
    file.open(*filename, std::ios::in);
    if (!file.good()) {
        std::cout << "No input file";
        return "";
    }
    std::string text;
    getline(file, text);
    return text;
}

void encode(const std::string text) {
    int iterator = 0;
    std::map<char, int> alphabet = {};
    for (int i = 0; i < text.size(); i++) {
        auto it = alphabet.find(text[i]);
        if (it == alphabet.end()) {
            alphabet[text[i]] = iterator++;
        }
    }

    std::vector<std::pair<char, int>> sortedVector(alphabet.begin(), alphabet.end());
    std::sort(sortedVector.begin(), sortedVector.end(), [](const auto &a, const auto &b) {
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
    }

    std::string additionFirstBits = "";
    for (auto i = 0; i < addition; i++) {
        additionFirstBits += "1";
    }

    std::bitset<8> additionRep(addition);
    std::string encodedString = additionFirstBits;
    for (auto i = 0; i < text.size(); i++) {
        std::bitset<32> rep(alphabet.at(text[i]));
        encodedString += rep.to_string().substr(32 - N);
    }


    int outputSize = iterator + 1 + finalBitSize / 8;
    char output[outputSize] = {};
    output[0] = iterator;
    for (int i = 0; i < sortedVector.size(); i++) {
        output[i + 1] = sortedVector.at(i).first;
    }
    int guard = encodedString.length() / 8;
    for (int i = 0; i < guard; i++) {
        output[1 + sortedVector.size() + i] = std::stoi(encodedString.substr(0, 8), nullptr, 2);
        encodedString.erase(0, 8);
    }
    std::ofstream out;
    out.open("encoded", std::ios::trunc);
    for (char x: output) {
        out << x;
    }
    out.close();
}

void decode(std::string text) {
    int alphabetLength = (int) text[0];

    std::map<int, char> alphabet = {};
    for (int i = 0; i < alphabetLength; i++) {
        alphabet[i] = text[1 + i];
    }
    text.erase(0, alphabetLength + 1);

    int N = ceil(log2(alphabetLength));

    std::vector<bool> bits;

    for (char c: text) {
        std::bitset<8> charBits(c);
        for (int i = 7; i >= 0; --i) {
            bits.push_back(charBits[i]);
        }
    }

    auto onesEnd = bits.begin();
    while (onesEnd != bits.end() && *onesEnd) {
        ++onesEnd;
    }
    bits.erase(bits.begin(), onesEnd);

    std::string decodedString = "";

    for (bool bit: bits) {
        decodedString += (bit ? '1' : '0');
    }
    std::string final = "";
    for (int i = 0; i < decodedString.size(); i += N) {
        std::string chunk = decodedString.substr(i, N);
        int value = std::stoi(chunk, nullptr, 2);
        final += std::string(1, alphabet.at(value));
    }

    std::ofstream out;
    out.open("decoded.txt", std::ios::trunc);
    out << final;
    out.close();
}

int main() {
    std::string file, text;
    int option;
    std::cout << "File location:\n";
    std::cin >> file;
    std::cout << "1.Decode \n2.Encode\n";
    std::cin >> option;
    text = readFromFile(&file);

    switch (option) {
        case 1:
            decode(text);
            break;
        case 2:
            encode(text);
            break;
    }
    return 0;
}

/*na zajęciach niestety musiałem zająć się postawieniem środowiska na swoim laptopie,
ale też zapoznawałem się z różnymi metodami szyfrowania, przeglądałem możliwe opcje 
oraz zastanawiałem się w jaki sposób je zaimplementować*/