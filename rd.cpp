// ────── setup ──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐

#include <iostream>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

#define bold    "\e[1m"
#define red     "\e[31m"
#define reset   "\e[m"

int getcount(path&);
void recurse(path&, string = "");

//#include <windows.h>
void windowsnonsense() {
//SetConsoleOutputCP(65001);SetConsoleMode(GetStdHandle(-11),7);
}

// ────── main ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤

int main(int, char** argv)
{
    cout << "\n";
    windowsnonsense();

// ┌───── validation ───────────────────────────────────────────────────────┐

    if (!argv[1])
    {
        cout << red << "⚠ you didn't specify a directory\n\n" << reset;
        return 1;
    }

    path target = argv[1];

    if (!is_directory(target))
    {
        cout << red << "⚠ invalid directory\n\n" << reset;
        return 1;
    }

// ├───── header ───────────────────────────────────────────────────────────┤

    string name = target.filename().string();
    if (name.empty()) name = "root";
    int count = getcount(target);

    cout << bold << name << " (" << count << ")\n" << reset;

// ├───── recursion ────────────────────────────────────────────────────────┤

    recurse(target);

// └────────────────────────────────────────────────────────────────────────┘

    cout << "\n";
}

// ────── logic ──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤

int getcount(path& target)
{
    int count = 0;
    error_code err;

    for (auto& entry : directory_iterator(target, err)) count++;
    return count;
}

void recurse(path& target, string depth)
{
    int i = 1, count = getcount(target);
    error_code err;

    for (auto& entry : directory_iterator(target, err))
    {
        path epath = entry.path();
        string pfx, indent;

        if (i == count)
        {
            pfx = "└─ ";
            indent = "   ";
        }
        else
        {
            pfx = "├─ ";
            indent = "│  ";
        }

        i++;

        cout << depth << pfx << epath.filename().string() << "\n";

        if (is_directory(epath, err) && !is_symlink(entry.symlink_status())) recurse(epath, depth + indent);
    }
}

// ───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
