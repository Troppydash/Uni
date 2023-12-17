#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cmath>

#define END '\f'

using namespace std;

namespace helpers
{
    // https://stackoverflow.com/a/2072890
    inline bool ends_with(std::string const &value, std::string const &ending)
    {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }
}


/**
 * Radix quicksort algorithm to sort same length strings.
 * Outputs through the offset vector, we are sorting a character shifted
 * series of strings.
 * Performance is O(n log^2 n), O(n log n) on quicksort, and O(log n) on radix
 *
 * @param input
 * @param offset
 * @param start
 * @param end
 * @param digit
 */
void radix_quicksort(
        const string &input,
        vector<size_t> &offset,
        size_t start, size_t end,
        size_t digit
)
{
    // base case
    if (digit == input.size() || start >= end)
        return;

    // choose pivot
    char pivot = input[(offset[start + (end - start) / 2] + digit) %
                       input.size()];
    // three-way partition
    size_t first_even = start;
    size_t first_greater = end;
    size_t iter = start;
    while (iter < first_greater)
    {
        char current = input[(offset[iter] + digit) % input.size()];
        if (current == pivot)
        {
            iter += 1;
        }
        else if (current < pivot)
        {
            swap(offset[iter], offset[first_even]);
            first_even += 1;
            iter += 1;
        }
        else if (current > pivot)
        {
            first_greater -= 1;
            swap(offset[iter], offset[first_greater]);
        }
    }


    // sorting middle at increased depth
    radix_quicksort(input, offset, first_even, first_greater, digit + 1);
    // sorting left and right on the same depth
    radix_quicksort(input, offset, start, first_even, digit);
    radix_quicksort(input, offset, first_greater, end, digit);
}

/**
 * Computes the BWT of the source string,
 * this is a string of the previous characters of the suffix array.
 * The string must contain the END character.
 * Performance is O(n log^2 n) for the radix quicksort on suffix array
 *
 * @param input
 * @return
 */
string bwt(const string &input)
{
    // S[i] = rotated i positions
    vector<size_t> S(input.size());
    iota(S.begin(), S.end(), 0);

    // sort lexicographically
    // this can be sped up using a ternary quicksort
     radix_quicksort(input, S, 0, input.size(), 0);
//    sort(S.begin(), S.end(), [&](const auto &a, const auto &b)
    //    {
    //        for (int i = 0; i < input.size(); ++i)
    //        {
    //            size_t ai = (a + i) % input.size();
    //            size_t bi = (b + i) % input.size();
    //
    //            if (input[ai] != input[bi])
    //            {
    //                return input[ai] < input[bi];
    //            }
    //        }
    //
    //        // else they are equal
    //        return false;
    //    });

    // push the previous (circular) character
    string out;
    for (auto &i: S)
    {
        if (i == 0)
            out += input[input.size() - 1];
        else
            out += input[i - 1];
    }

    return out;
}

/**
 * Returns a frequency sequence on a source text.
 * Performance is O(n)
 *
 * @param text
 * @return
 */
vector<size_t> create_seq(const string &text)
{
    vector<size_t> seq(text.size());
    unordered_map<char, size_t> count;

    for (size_t i = 0; i < text.size(); ++i)
    {
        count[text[i]] += 1;
        seq[i] = count[text[i]];
    }

    return seq;
}

/**
 * Computes the inverse BWT of the transformed string.
 * The string must contain the END character.
 * Performance is O(n log n) for both the initial sort and rotations
 *
 * @param input
 * @return
 */
string ibwt(const string &input)
{
    // sort bwt and order them
    string S = input;
    sort(S.begin(), S.end());

    // compute indices for the sorted bwt
    vector<size_t> seq_sorted = create_seq(S);

    // compute indices for input
    vector<size_t> seq_input = create_seq(input);

    string output;
    output.push_back(END);

    // find END in sorted
    size_t ptr = lower_bound(S.begin(), S.end(), END) - S.begin();
    while (output.size() < input.size())
    {
        // find equivalent index in bwt (attaching column)
        // implicit at input[ptr], seq_input[ptr]

        // binary search to find the index 'i' where S[i] = input[ptr],
        // and seq_sorted[i] = seq_input[ptr]
        size_t lower = 0, upper = input.size();
        size_t index = -1;
        while (lower < upper)
        {
            index = lower + (upper - lower) / 2;
            if (S[index] == input[ptr] && seq_sorted[index] == seq_input[ptr])
                break;

            // if S[i] < input[ptr], and or seq_sorted[i] < seq_input[ptr]
            if (S[index] < input[ptr] ||
                (S[index] == input[ptr] && seq_sorted[index] < seq_input[ptr]))
                lower = index + 1;
            else
                upper = index;
        }

        // index is set
        // push character
        ptr = index;
        output.push_back(S[ptr]);
    }

    std::reverse(output.begin(), output.end());
    return output;
}

/**
 * Computes the move-forward transform using traditional arrays.
 * Performance is O(kn), k=256
 *
 * @param bwt
 * @return
 */
vector<unsigned char> mtf(const string &bwt)
{
    // we don't have to use a linked list, for it does not produce too much
    // of an advantage (check this)

    vector<char> dictionary(256);
    iota(dictionary.begin(), dictionary.end(), 0);

    vector<unsigned char> output(bwt.size());

    for (size_t i = 0; i < bwt.size(); ++i)
    {
        char c = bwt[i];

        // find the index of the character in dictionary
        auto index = std::find(dictionary.begin(), dictionary.end(), c);
        // and append it to the output
        output[i] = (unsigned char) (index - dictionary.begin());

        // bring that character to the front
        dictionary.erase(index);
        dictionary.insert(dictionary.begin(), c);
    }

    return output;
}


string imtf(const vector<unsigned char> &indices)
{
    // must be the same dictionary as the mtf
    vector<char> dictionary(256);
    iota(dictionary.begin(), dictionary.end(), 0);

    string output;
    for (auto i: indices)
    {
        char c = dictionary[i];
        output += c;

        // bring c to the front
        dictionary.erase(dictionary.begin() + i);
        dictionary.insert(dictionary.begin(), c);
    }

    return output;
}

/**
 * Encode a series of bytes using the small integer huffman scheme
 *
 * @param mtf
 * @return
 */
vector<bool> huffman_encoding(
        const vector<unsigned char> &mtf
)
{
    // use the simplified huffman encoding scheme
    // because the default version assumes log(n) probability
    // which is produced by the mtf

    // if we use the true encoding with lookup table
    // when length is small, the table dominates space
    // when length is larger, the simplified scheme equals the lookup table

    // so
    // 0 -> 0 = 2^0 + 0 - 1
    // 10 0 -> 1 = 2^1 + 0 - 1
    // 10 1 -> 2 = 2^1 + 1 - 1
    // ...
    // 110 010 -> 2^2 + 010(b) - 1 = 4 + 1 = 5
    // 1{p}0 {r} -> n = 2^p + r - 1
    // 2^p + r = 1 + n

    // 110

    vector<bool> buffer;

    for (unsigned char i: mtf)
    {
        // compute power
        int n = i + 1;
        int power = -1;
        while (n > 0)
        {
            n >>= 1;
            power += 1;
        }

        // compute remainder
        unsigned int remainder = (i + 1) - (1u << power);

        // append the power part
        for (int k = 0; k < power; ++k)
            buffer.push_back(true);
        buffer.push_back(false);

        // append the digit part
        if (power == 0)
            continue;

        // we only need 2^(p-1) for p digit number
        for (unsigned int mask = (1u << (power - 1)); mask > 0; mask >>= 1)
        {
            buffer.push_back(mask & remainder);
        }
    }

    return buffer;
}


/**
 * Decode a series of bytes using the small integer huffman scheme
 * @param encoded
 * @return
 */
vector<unsigned char> huffman_decoding(
        const vector<bool> &encoded
)
{
    vector<unsigned char> output;

    // counting the powers
    int power = 0;
    // recording the remainder
    unsigned int remainder = 0;
    // working byte
    unsigned char byte = 0;
    // whether we are parsing the header
    bool isheader = true;

    for (bool bit: encoded)
    {
        if (isheader)
        {
            if (bit)
            {
                // a 1 still, increment power
                power += 1;
            }
            else
            {
                // finished header, add header to working byte
                isheader = false;
                remainder = 0;
                byte = (1u << power);
            }
        }
        else
        {
            // record "power" times remainder bits, big endian
            power -= 1;
            remainder = (remainder << 1) | (unsigned int) bit;
        }

        // no more remainder to parse, we reset the header and append offset
        if (power == 0)
        {
            isheader = true;
            output.push_back(byte + remainder - 1);
        }
    }

    return output;
}

/**
 * Write the bit encoding into a file, packing the values in the process.
 *
 * @param encoded
 * @param filename
 */
void write_encoding(const vector<bool> &encoded, const char *filename)
{
    // pad encoded to unsigned chars
    vector<unsigned char> padded;
    for (size_t i = 0; i < encoded.size() / 8; ++i)
    {
        unsigned char byte = 0;
        for (size_t j = 0; j < 8; ++j)
        {
            bool bit = encoded[8 * i + j];
            byte = (byte << 1) | (unsigned int) bit;
        }
        padded.push_back(byte);
    }

    // pad the remaining
    if (encoded.size() % 8 != 0)
    {
        size_t i = (encoded.size() / 8);
        unsigned char byte = 0;
        for (size_t j = 0; j < 8; ++j)
        {
            bool bit = true;
            if (8 * i + j < encoded.size())
                bit = encoded[8 * i + j];

            byte = (byte << 1) | (unsigned int) bit;
        }
        padded.push_back(byte);
    }

    // write to file
    ofstream file(filename, ios::binary);
    file.write((char *) &padded[0], padded.size());
    file.close();
}

/**
 * Read the packed bits from a file.
 *
 * @param filename
 * @return
 */
vector<bool> read_encoding(const char *filename)
{
    ifstream file(filename, ios::binary);

    // unpack bits
    vector<bool> bits;
    char data = 0;
    while (file.read(&data, 1))
    {
        // because char and files are read in big endian
        for (int i = 7; i >= 0; --i)
        {
            bits.push_back(data & (1u << i));
        }
    }

    file.close();

    return bits;
}


// testing rle algorithm on the bwt
vector<unsigned char> rle(const string &bwt)
{
    vector<unsigned char> output;

    unsigned char last = bwt[0];
    int count = 1;
    for (size_t i = 1; i < bwt.size(); ++i)
    {
        if (bwt[i] == last && count < 255)
            count += 1;
        else
        {
            if (count == 1)
            {
                output.push_back(last);
            }
            else
            {
                output.push_back(last);
                output.push_back(last);
                output.push_back(count);
            }

            count = 1;
            last = bwt[i];
        }
    }

    output.push_back(last);
    output.push_back(count);
    return output;
}

// logging
namespace logging
{
    void huffman(const vector<bool> &encoded, int limit = 300)
    {
        for (bool bit: encoded)
        {
            if (--limit < 0)
                break;
            cout << (bit ? '1' : '0');
        }
        cout << endl;
    }

    string format_char(char c)
    {
        if (c >= 33 && c <= 126)
            return {c};

        string text = to_string((int) c);
        return "<" + text + ">";
    }

    void text(const string &text)
    {
        for (auto c: text)
        {
            cout << logging::format_char(c);
        }

        cout << endl;
    }
}


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Bzip2Clone\n"
                "Usage:\n"
                "  bzip2clone.exe pack <filename>\n"
                "  bzip2clone.exe unpack <filename>\n";

        return 0;
    }

    if (strcmp(argv[1], "pack") == 0)
    {
        // pack filename into filename.tzip

        // read file
        const char *infile = argv[2];

        ifstream file(infile);

        std::stringstream buf;
        buf << file.rdbuf();
        file.close();

        string contents = buf.str();
        contents += END;

        string afterbwt = bwt(contents);
        vector<unsigned char> aftermtf = mtf(afterbwt);
        vector<bool> afterhuffman = huffman_encoding(aftermtf);

        string newfile = infile;
        newfile += ".tzip";
        write_encoding(afterhuffman, newfile.c_str());
        cout << "done packing to " << newfile << endl;
    }
    else if (strcmp(argv[1], "unpack") == 0)
    {
        // unpack filename.tzip into filename
        string infile = argv[2];

        // verify file extension
        if (!helpers::ends_with(infile, ".tzip"))
        {
            cout << "error: the file does not end with .tzip" << endl;
            return 1;
        }

        // read contents
        vector<bool> bits = read_encoding(infile.c_str());
        vector<unsigned char> afterihuffman = huffman_decoding(bits);
        string afterimtf = imtf(afterihuffman);
        string text = ibwt(afterimtf);

        // write to new file without extensions
        infile.resize(infile.size() - string{".tzip"}.size());

        ofstream file(infile);
        file.write(text.c_str(), text.size() - 1);
        file.close();

        cout << "done unpacking to " << infile << endl;
    }
    else if (strcmp(argv[1], "test") == 0)
    {
        const char *infile = argv[2];

        ifstream file(infile);

        std::stringstream buf;
        buf << file.rdbuf();
        file.close();

        string contents = buf.str();
        contents += END;

        string afterbwt = bwt(contents);
        vector<unsigned char> aftermtf = mtf(afterbwt);
        vector<bool> afterhuffman = huffman_encoding(aftermtf);
        write_encoding(afterhuffman, "tmp.tzip");

        auto step3 = read_encoding("tmp.tzip");
        auto step2 = huffman_decoding(step3);
        auto step1 = imtf(step2);
        auto contents2 = ibwt(step1);

        if (contents != contents2)
        {
            cout << "failed test, source strings do not match" << endl;
        }
        else
        {
            cout << "all good!" << endl;
        }
    }
    else if (strcmp(argv[1], "bwt") == 0)
    {
        ifstream file(argv[2]);

        std::stringstream buf;
        buf << file.rdbuf();
        file.close();

        string contents = buf.str();
        contents += END;

        logging::text(bwt(contents));
        cout << "done" << endl;
    }
    else if (strcmp(argv[1], "perf") == 0)
    {
        ifstream file(argv[2]);

        std::stringstream buf;
        buf << file.rdbuf();
        file.close();

        string contents = buf.str();
        contents += END;

        cout << "source size: " << contents.size() << " bytes" << endl << endl;

        auto step1 = bwt(contents);
        auto step2 = mtf(step1);

        // print mtf distribution
        cout << "MTF distribution" << endl;
        map<int, size_t> freq;
        for (auto &i: step2)
            freq[i] += 1;

        // 0, 1-2, 3-6
        size_t mtfsize = step2.size();
        size_t total_bits = 0;
        for (int power = 0; power <= 7; ++power)
        {
            int p = (int) (1u << power);
            size_t count = 0;
            for (int k = 0; k < p; ++k)
            {
                count += freq[p + k - 1];
            }

            int start = p - 1;
            int end = start + p - 1;
            double percent = (double) count / (double) mtfsize;

            int bits = power + 1;
            if (power > 0)
                bits += (int) (1u << (power - 1));

            total_bits += bits * count;
            printf("%3d-%-3d:%10lld (%5.2lf%%) %4d bits\n", start, end, count,
                   percent * 100.0, bits);
        }

        printf("MTF size: %lld bytes, averaged %4.2lf bits/byte\n",
               total_bits / 8,
               (double) total_bits / (double) mtfsize);

        cout << "compression rate of "
             << (double) total_bits * 100.0 / 8.0 / (double) mtfsize << "%"
             << endl;

        double entropy = 0.0;
        for (auto &p : freq)
        {
            if (p.second == 0)
                continue;

            double percent = (double) p.second / (double) mtfsize;
            entropy -= percent * log2(percent);
        }
        printf("entropy: %.2lf bits\n", entropy);

        cout << endl;

        auto encoded = rle(step1);
        cout << "RLE size: " << encoded.size() << " bytes" << endl;
    }
    else
    {
        cout << "error: unknown command!" << endl;
    }

    return 0;
}
