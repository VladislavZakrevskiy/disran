#include <algorithm>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

static string visible_char(char c) {
    if (c == ' ') return "' '";
    if (c == '\t') return "'\\t'";
    if (c == '\n') return "'\\n'";
    return string("'") + c + "'";
}

static string prefix_of(const string& s, int len) {
    if (len <= 0) return "";
    return s.substr(0, static_cast<size_t>(len));
}

static void print_vector(const vector<int>& v, int upto = -1) {
    if (upto < 0 || upto >= static_cast<int>(v.size())) {
        upto = static_cast<int>(v.size()) - 1;
    }
    cout << "[";
    for (int i = 0; i <= upto; ++i) {
        if (i) cout << " ";
        cout << v[i];
    }
    cout << "]";
}

static void print_header(const string& title) {
    cout << "\n============================================================\n";
    cout << title << "\n";
    cout << "============================================================\n";
}

static vector<int> prefix_function_log(const string& s, const string& label, bool verbose) {
    vector<int> pi(s.size(), 0);

    if (verbose) {
        print_header("PREFIX FUNCTION: " + label);
        cout << "s = \"" << s << "\"\n";
        if (s.empty()) {
            cout << "empty string -> pi = []\n";
            return pi;
        }
        cout << "pi[0] = 0\n";
    }

    for (int i = 1; i < static_cast<int>(s.size()); ++i) {
        int j = pi[i - 1];

        if (verbose) {
            cout << "\n";
            cout << "i = " << i << ", s[i] = " << visible_char(s[i]) << "\n";
            cout << "start: j = pi[i - 1] = pi[" << (i - 1) << "] = " << j;
            cout << " (matched prefix \"" << prefix_of(s, j) << "\")\n";
        }

        while (j > 0 && s[i] != s[j]) {
            if (verbose) {
                cout << "mismatch: s[i] = " << visible_char(s[i]);
                cout << " != s[j] = s[" << j << "] = " << visible_char(s[j]) << "\n";
                cout << "fallback: j = pi[j - 1] = pi[" << (j - 1) << "] = " << pi[j - 1] << "\n";
            }
            j = pi[j - 1];
        }

        if (s[i] == s[j]) {
            if (verbose) {
                cout << "match: s[i] = " << visible_char(s[i]);
                cout << " == s[j] = s[" << j << "] = " << visible_char(s[j]) << "\n";
                cout << "extend border: j = " << (j + 1) << "\n";
            }
            ++j;
        } else if (verbose) {
            cout << "no extension from j = 0: s[i] = " << visible_char(s[i]);
            cout << " != s[0] = " << visible_char(s[0]) << "\n";
        }

        pi[i] = j;

        if (verbose) {
            cout << "pi[" << i << "] = " << pi[i] << "\n";
            cout << "pi so far = ";
            print_vector(pi, i);
            cout << "\n";
        }
    }

    if (verbose) {
        cout << "\nfinal pi = ";
        print_vector(pi);
        cout << "\n";
    }

    return pi;
}

static vector<int> z_function_log(const string& s, const string& label, bool verbose) {
    vector<int> z(s.size(), 0);
    int l = 0;
    int r = 0;

    if (verbose) {
        print_header("Z FUNCTION: " + label);
        cout << "s = \"" << s << "\"\n";
        if (s.empty()) {
            cout << "empty string -> z = []\n";
            return z;
        }
        cout << "z[0] = 0\n";
    }

    for (int i = 1; i < static_cast<int>(s.size()); ++i) {
        if (verbose) {
            cout << "\n";
            cout << "i = " << i << ", s[i] = " << visible_char(s[i]) << "\n";
            cout << "current Z-block [l, r] = [" << l << ", " << r << "]\n";
        }

        if (i <= r) {
            int copied = min(r - i + 1, z[i - l]);
            z[i] = copied;
            if (verbose) {
                cout << "inside block: k = i - l = " << (i - l) << "\n";
                cout << "safe start: z[i] = min(r - i + 1, z[k]) = min(";
                cout << (r - i + 1) << ", " << z[i - l] << ") = " << copied << "\n";
            }
        } else if (verbose) {
            cout << "outside block: start from z[i] = 0\n";
        }

        while (i + z[i] < static_cast<int>(s.size()) && s[z[i]] == s[i + z[i]]) {
            if (verbose) {
                cout << "extend: s[z[i]] = s[" << z[i] << "] = " << visible_char(s[z[i]]);
                cout << " == s[i + z[i]] = s[" << (i + z[i]) << "] = ";
                cout << visible_char(s[i + z[i]]) << "\n";
            }
            ++z[i];
        }

        if (verbose) {
            if (i + z[i] < static_cast<int>(s.size())) {
                cout << "stop: s[" << z[i] << "] = " << visible_char(s[z[i]]);
                cout << " != s[" << (i + z[i]) << "] = " << visible_char(s[i + z[i]]) << "\n";
            } else {
                cout << "stop: reached end of string\n";
            }
        }

        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
            if (verbose) {
                cout << "update block -> [l, r] = [" << l << ", " << r << "]\n";
            }
        } else if (verbose) {
            cout << "block unchanged\n";
        }

        if (verbose) {
            cout << "z[" << i << "] = " << z[i] << "\n";
            cout << "z so far = ";
            print_vector(z, i);
            cout << "\n";
        }
    }

    if (verbose) {
        cout << "\nfinal z = ";
        print_vector(z);
        cout << "\n";
    }

    return z;
}

static vector<int> prefix_from_z_log(const string& s) {
    print_header("BUILD PI FROM Z");
    cout << "s = \"" << s << "\"\n";
    cout << "First build z, then fill pi from right to left inside every Z-segment.\n";

    vector<int> z = z_function_log(s, "source for z2pi", true);
    vector<int> pi(s.size(), 0);

    if (s.empty()) {
        cout << "final pi = []\n";
        return pi;
    }

    cout << "\nConvert z -> pi:\n";
    for (int i = 1; i < static_cast<int>(s.size()); ++i) {
        if (z[i] == 0) {
            cout << "i = " << i << ": z[i] = 0, nothing to write\n";
            continue;
        }

        cout << "i = " << i << ": z[i] = " << z[i] << "\n";
        cout << "segment s[" << i << ".." << (i + z[i] - 1) << "] matches prefix s[0.." << (z[i] - 1) << "]\n";

        int j = z[i] - 1;
        for (; j >= 0 && pi[i + j] == 0; --j) {
            int pos = i + j;
            pi[pos] = j + 1;
            cout << "  write pi[" << pos << "] = " << (j + 1);
            cout << " because prefix length " << (j + 1) << " ends at pos " << pos << "\n";
        }

        if (j >= 0) {
            cout << "  stop at pos " << (i + j) << ": pi is already " << pi[i + j] << "\n";
        }

        cout << "  pi so far = ";
        print_vector(pi);
        cout << "\n";
    }

    cout << "\nfinal pi from z = ";
    print_vector(pi);
    cout << "\n";
    return pi;
}

static char choose_separator(const string& text, const string& pattern) {
    for (char c : {'#', '$', '@', '%', '&', '|'}) {
        if (text.find(c) == string::npos && pattern.find(c) == string::npos) {
            return c;
        }
    }
    return '\x01';
}

static void run_concat_kmp(const string& text, const string& pattern) {
    print_header("KMP VARIANT 1: pattern + separator + text");

    if (pattern.empty()) {
        cout << "Empty pattern: every position is a match by convention.\n";
        return;
    }

    char sep = choose_separator(text, pattern);
    string s = pattern + sep + text;
    int m = static_cast<int>(pattern.size());

    cout << "pattern = \"" << pattern << "\"\n";
    cout << "text    = \"" << text << "\"\n";
    cout << "joined  = pattern + " << visible_char(sep) << " + text = \"" << s << "\"\n";
    cout << "Idea: if pi[i] == pattern length inside the text part, a match ends at i.\n";

    vector<int> pi = prefix_function_log(s, "joined string", true);

    cout << "\nScan positions from text part:\n";
    vector<int> matches;
    for (int i = m + 1; i < static_cast<int>(s.size()); ++i) {
        int text_pos = i - (m + 1);
        cout << "joined i = " << i << ", text position = " << text_pos;
        cout << ", char = " << visible_char(s[i]) << ", pi[i] = " << pi[i];
        if (pi[i] == m) {
            int start = i - 2 * m;
            matches.push_back(start);
            cout << " -> FOUND at text position " << start;
        }
        cout << "\n";
    }

    cout << "matches: ";
    print_vector(matches);
    cout << "\n";
}

static void run_stream_kmp(const string& text, const string& pattern) {
    print_header("KMP VARIANT 2: classic streaming search");

    if (pattern.empty()) {
        cout << "Empty pattern: every position is a match by convention.\n";
        return;
    }

    cout << "pattern = \"" << pattern << "\"\n";
    cout << "text    = \"" << text << "\"\n";
    cout << "Preprocess only the pattern.\n";
    vector<int> pi = prefix_function_log(pattern, "pattern", true);

    cout << "\nRuntime scan of text:\n";
    cout << "q means: how many first pattern characters already match the current text suffix.\n";

    int q = 0;
    int m = static_cast<int>(pattern.size());
    vector<int> matches;

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        cout << "\n";
        cout << "text i = " << i << ", T[i] = " << visible_char(text[i]) << "\n";
        cout << "start q = " << q << " (matched \"" << prefix_of(pattern, q) << "\")\n";

        while (q > 0 && pattern[q] != text[i]) {
            cout << "mismatch: pattern[q] = P[" << q << "] = " << visible_char(pattern[q]);
            cout << " != T[i] = " << visible_char(text[i]) << "\n";
            cout << "fallback: q = pi[q - 1] = pi[" << (q - 1) << "] = " << pi[q - 1] << "\n";
            q = pi[q - 1];
            cout << "after fallback q = " << q << " (matched \"" << prefix_of(pattern, q) << "\")\n";
        }

        if (pattern[q] == text[i]) {
            cout << "match: P[" << q << "] = " << visible_char(pattern[q]);
            cout << " == T[" << i << "] = " << visible_char(text[i]) << "\n";
            ++q;
            cout << "advance q -> " << q << "\n";
        } else {
            cout << "no match at q = 0: P[0] = " << visible_char(pattern[0]);
            cout << " != T[" << i << "] = " << visible_char(text[i]) << "\n";
        }

        if (q == m) {
            int start = i - m + 1;
            matches.push_back(start);
            cout << "FOUND: pattern ends at text i = " << i << ", starts at " << start << "\n";
            cout << "continue with overlaps: q = pi[m - 1] = pi[" << (m - 1) << "] = " << pi[m - 1] << "\n";
            q = pi[m - 1];
        }
    }

    cout << "\nmatches: ";
    print_vector(matches);
    cout << "\n";
}

static vector<char> build_alphabet(const string& text, const string& pattern) {
    set<char> chars;
    for (char c : pattern) chars.insert(c);
    for (char c : text) chars.insert(c);
    return vector<char>(chars.begin(), chars.end());
}

static vector<vector<int>> build_automaton_log(const string& pattern, const vector<int>& pi, const vector<char>& alphabet) {
    int m = static_cast<int>(pattern.size());
    vector<vector<int>> go(m + 1, vector<int>(alphabet.size(), 0));

    print_header("BUILD KMP AUTOMATON");
    cout << "state = length of matched pattern prefix\n";
    cout << "alphabet:";
    for (char c : alphabet) cout << " " << visible_char(c);
    cout << "\n";

    for (int state = 0; state <= m; ++state) {
        cout << "\nstate " << state << ":\n";
        for (int idx = 0; idx < static_cast<int>(alphabet.size()); ++idx) {
            char c = alphabet[idx];
            if (state < m && c == pattern[state]) {
                go[state][idx] = state + 1;
                cout << "  on " << visible_char(c) << ": direct match P[" << state << "] -> " << go[state][idx] << "\n";
            } else if (state == 0) {
                go[state][idx] = 0;
                cout << "  on " << visible_char(c) << ": no prefix to fallback -> 0\n";
            } else {
                int fallback_state = pi[state - 1];
                auto it = find(alphabet.begin(), alphabet.end(), c);
                int fallback_idx = static_cast<int>(it - alphabet.begin());
                go[state][idx] = go[fallback_state][fallback_idx];
                cout << "  on " << visible_char(c) << ": fallback to state pi[" << (state - 1) << "] = ";
                cout << fallback_state << ", then go -> " << go[state][idx] << "\n";
            }
        }
    }

    cout << "\nTransition table:\n";
    cout << setw(8) << "state";
    for (char c : alphabet) cout << setw(8) << visible_char(c);
    cout << "\n";
    for (int state = 0; state <= m; ++state) {
        cout << setw(8) << state;
        for (int value : go[state]) cout << setw(8) << value;
        cout << "\n";
    }

    return go;
}

static void run_automaton_kmp(const string& text, const string& pattern) {
    print_header("KMP VARIANT 3: real-time automaton");

    if (pattern.empty()) {
        cout << "Empty pattern: every position is a match by convention.\n";
        return;
    }

    cout << "pattern = \"" << pattern << "\"\n";
    cout << "text    = \"" << text << "\"\n";
    cout << "Runtime idea: each text character performs exactly one table transition.\n";

    vector<int> pi = prefix_function_log(pattern, "pattern", true);
    vector<char> alphabet = build_alphabet(text, pattern);
    vector<vector<int>> go = build_automaton_log(pattern, pi, alphabet);

    int state = 0;
    int m = static_cast<int>(pattern.size());
    vector<int> matches;

    cout << "\nRuntime scan of text:\n";
    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        char c = text[i];
        int idx = static_cast<int>(find(alphabet.begin(), alphabet.end(), c) - alphabet.begin());
        int old_state = state;
        state = go[state][idx];

        cout << "i = " << i << ", T[i] = " << visible_char(c);
        cout << ": state " << old_state << " -> " << state;
        cout << " (matched \"" << prefix_of(pattern, min(state, m)) << "\")";

        if (state == m) {
            int start = i - m + 1;
            matches.push_back(start);
            cout << " -> FOUND at " << start;
            cout << ", reset state to pi[m - 1] = " << pi[m - 1] << " for overlaps";
            state = pi[m - 1];
        }
        cout << "\n";
    }

    cout << "\nmatches: ";
    print_vector(matches);
    cout << "\n";
}

static void print_usage(const string& program) {
    cout << "Usage:\n";
    cout << "  " << program << " <mode> <text> <pattern>\n\n";
    cout << "Modes:\n";
    cout << "  pi         log prefix-function construction for pattern\n";
    cout << "  z2pi       build prefix-function from Z-function for pattern\n";
    cout << "  concat     KMP through pattern + separator + text\n";
    cout << "  stream     classic streaming KMP\n";
    cout << "  automaton  real-time KMP automaton\n";
    cout << "  all        run all variants\n\n";
    cout << "Defaults:\n";
    cout << "  " << program << " all abacaba aba\n";
}

int main(int argc, char** argv) {
    string mode = "all";
    string text = "abacaba";
    string pattern = "aba";

    if (argc == 2) {
        mode = argv[1];
    } else if (argc == 3) {
        mode = argv[1];
        pattern = argv[2];
    } else if (argc == 4) {
        mode = argv[1];
        text = argv[2];
        pattern = argv[3];
    } else if (argc != 1) {
        print_usage(argv[0]);
        return 1;
    }

    if (mode == "pi") {
        prefix_function_log(pattern, "pattern", true);
    } else if (mode == "z2pi") {
        prefix_from_z_log(pattern);
    } else if (mode == "concat") {
        run_concat_kmp(text, pattern);
    } else if (mode == "stream") {
        run_stream_kmp(text, pattern);
    } else if (mode == "automaton") {
        run_automaton_kmp(text, pattern);
    } else if (mode == "all") {
        prefix_function_log(pattern, "pattern", true);
        prefix_from_z_log(pattern);
        run_concat_kmp(text, pattern);
        run_stream_kmp(text, pattern);
        run_automaton_kmp(text, pattern);
    } else {
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}
