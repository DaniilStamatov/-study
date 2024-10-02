#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <array>

using namespace std;

struct Nucleotide {
    char symbol;
    size_t position;
    int chromosome_num;
    int gene_num;
    bool is_marked;
    char service_info;
};



const static int N = 4;
const std::array<char, N> Symbols = { 'A', 'T' , 'G', 'C' };

struct CompactNucleotide {
    uint32_t position;
    uint16_t gene_num : 15;
    uint16_t is_marked : 1;
    uint16_t symbol : 2;
    uint16_t service_info : 8;
    uint16_t chromosome_num : 6;
};

CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide cn;
    cn.position = static_cast<uint32_t>(n.position);
    cn.gene_num = n.gene_num;
    cn.is_marked = n.is_marked;
    cn.service_info = n.service_info;
    for (size_t i = 0; i < N; i++) {
        if (Symbols[i] == n.symbol) {
            cn.symbol = i;
            continue;
        }
    }
    cn.chromosome_num = n.chromosome_num;
    return cn;
    // напишите вашу реализацию здесь
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide n;
    n.position = cn.position;
    n.gene_num = cn.gene_num;
    n.service_info = cn.service_info;
    n.symbol = Symbols[cn.symbol];
    n.chromosome_num = cn.chromosome_num;
    n.is_marked = cn.is_marked;
    return n;
    // напишите вашу реализацию здесь
}

static_assert(sizeof(CompactNucleotide) <= 8, "Your CompactNucleotide is not compact enough");
static_assert(alignof(CompactNucleotide) == 4, "Don't use '#pragma pack'!");
bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) && (lhs.position == rhs.position) && (lhs.chromosome_num == rhs.chromosome_num)
        && (lhs.gene_num == rhs.gene_num) && (lhs.is_marked == rhs.is_marked) && (lhs.service_info == rhs.service_info);
}
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
}
void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    assert(source == decompressedSource);
}

int main() {
    TestSize();
    TestCompressDecompress();
}