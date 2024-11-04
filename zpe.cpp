#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

bool check_pre_text(const string &data, int pos, const string str) {
  int64_t len = str.length();
  for (int i = 0; i < len; ++i)
    if (data[pos + i] != str[i])
      return false;
  return true;
}

vector<int64_t> find_bit_flag(const string &data) {
  vector<int64_t> ans;
  int64_t len = data.length();
  for (int64_t pt = 3; pt < len; ++pt) {
    if (check_pre_text(data, pt - 3, "\x50\x4b\x03\x04"))
      if (pt + 3 < len)
        ans.push_back(pt + 3);
    if (check_pre_text(data, pt - 3, "\x50\x4b\x01\x02"))
      if (pt + 5 < len)
        ans.push_back(pt + 5);
  }
  return ans;
}

string read_and_pe(const char fn[]) {
  if (ifstream is{fn, ios::binary | ios::ate}) {
    auto size = is.tellg();
    string str(size, '\0');
    is.seekg(0);
    if (!is.read(&str[0], size))
      goto fail;
    vector<int64_t> bit_flags = find_bit_flag(str);
    int en = 0x01;
    for (int64_t pos : bit_flags)
      if (str[pos] & 1)
        en = 0x00;
    for (int64_t pos : bit_flags)
      str[pos] = en;
    is.close();
    return str;
  } else {
  fail:
    cerr << "Can't open file " << fn << endl;
    exit(1);
  }
}

void write_data(const char fn[], const string &data) {
  if (ofstream ostrm{fn, ios::binary}) {
    ostrm << data;
  } else {
    cerr << "Can't open file " << fn << endl;
    exit(1);
  }
}

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: ZPE [Zip Src File] [Zip Dst file]" << endl;
    return 1;
  }
  write_data(argv[2], read_and_pe(argv[1]));
  return 0;
}