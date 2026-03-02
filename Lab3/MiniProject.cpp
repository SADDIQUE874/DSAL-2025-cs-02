/////2025-CS-02
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Row1
{

public:
    vector<string> values;

    Row1(vector<string> v)
    {
        values = v;
    }
};
class Column
{
public:
    string name;
    string type;
    unsigned int constraints;

    Column(string n, string t, unsigned int c)
    {
        name = n;
        type = t;
        constraints = c;
    }
};

class Table
{
public:
    string tablename;
    vector<Column> columns;
    vector<Row1 *> rows;

    Table(string n)
    {
        tablename = n;
    }

    ~Table()
    {
        for (Row1 *r : rows)
        {
            delete r;
        }
        rows.clear();
    }
    void insertrow(vector<string> v)
    {
        if (v.size() != columns.size())
        {
            cout << "Columns count is mismatched!\n";
            return;
        }

        // check karna hai constraints ko 
        for (int i = 0; i < columns.size(); i++)
        {

            if ((columns[i].constraints & 2) && v[i].empty())
            {
                cout << "Erro identified and the Not Null constraint has been violated " << columns[i].name << "\n";
                return;
            }

            if ((columns[i].constraints & 1) || (columns[i].constraints & 4))
            {
                for (Row1 *r : rows)
                {
                    if (r->values[i] == v[i])
                    {
                        cout << "Unique/Primary Key violation " << columns[i].name << "\n";
                        return;
                    }
                }
            }
        }

        Row1 *r = new Row1(v);
        rows.push_back(r);
        cout << "The record has been inserted successfully.\n";
    }

    void SelectAll()
    {
        for (Column c : columns)
            cout << c.name << "\t";
        cout << "\n";
        for (Row1 *r : rows)
        {
            for (string val : r->values)
                cout << val << "\t";
            cout << "\n";
        }
    }

    void SaveTable(string file)
    {
        ofstream fout(file);
        fout << "TABLE " << tablename << "\n";
        for (Column c : columns)
        {
            fout << c.name << " " << c.type << " " << c.constraints << "\n";
        }
        fout << "DATA\n";
        for (Row1 *r : rows)
        {
            for (string val : r->values)
                fout << val << " ";
            fout << "\n";
        }
        fout.close();
        cout << "Table saved to " << file<< "\n";
    }

    void LoadTable(string file)
    {
        ifstream fin(file);
        if (!fin)
        {
            cout << "File not found!\n";
            return;
        }
        string line;
        rows.clear();
        columns.clear();

        getline(fin, line); 
        tablename = line.substr(6);

        while (getline(fin, line))
        {
            if (line == "DATA")
                break;
            stringstream ss(line);
            string name, type;
            unsigned int cons;
            ss >> name >> type >> cons;
            columns.push_back(Column(name, type, cons));
        }

        while (getline(fin, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            vector<string> vals;
            string val;
            while (ss >> val)
                vals.push_back(val);
            Row1 *r = new Row1(vals);
            rows.push_back(r);
        }

        fin.close();
        cout << "Table loaded from " << file<< "\n";
    }
};

int main()
{
    Table *mytable = nullptr;
    char buffer[256];

    while (true)
    {
        cout << "Enter the command (CREATE/INSERT/SELECT/SAVE/LOAD/EXIT): ";
        cin.getline(buffer, 256);
        string cmd(buffer);

        if (cmd.substr(0,6) == "CREATE" || cmd.substr(0,6) == "Create")
        {
            cout << "Enter the  table name: ";
            string tname;
            cin >> tname;
            mytable = new Table(tname);
            int colcount;
            cout << "Number of the columns: ";
            cin >> colcount;
            for (int i = 0; i < colcount; i++)
            {
                string name, type;
                unsigned int cons;
                cout << "Column " << i + 1 << " name: ";
                cin >> name;
                cout << "Enter the Type (int/string): ";
                cin >> type;
                cout << "Constraints (bitwise): ";
                cin >> cons;
                mytable->columns.push_back(Column(name, type, cons));
            }
            cin.ignore();
            cout << "Your command is successful and table has been created successfully\n";
        }
        else if (cmd.substr(0, 6) == "INSERT")
        {
            if (!mytable)
            {
                cout << "First create the table and then proceed ahead!\n";
                continue;
            }
            vector<string> vals;
            for (Column c : mytable->columns)
            {
                string val;
                cout << "Enter the value for " << c.name << ": ";
                cin >> val;
                vals.push_back(val);
            }
            cin.ignore();
            mytable->insertrow(vals);
        }
        else if (cmd.substr(0, 6) == "SELECT")
        {
            if (!mytable)
            {
                cout << "First create the table and then proceed ahead!\n";
                continue;
            }
            mytable->SelectAll();
        }
        else if (cmd.substr(0, 4) == "SAVE")
        {
            if (!mytable)
            {
                cout << "First create the table and then proceed ahead\n";
                continue;
            }
            string fname;
            cout << "Enter the filename: ";
            cin >> fname;
            cin.ignore();
            mytable->SaveTable(fname);
        }
        else if (cmd.substr(0, 4) == "LOAD")
        {
            string fname;
            cout << "Enter the filename: ";
            cin >> fname;
            cin.ignore();
            if (mytable)
                delete mytable;
            mytable = new Table("");
            mytable->LoadTable(fname);
        }
        else if (cmd.substr(0, 4) == "EXIT")
        {
            break;
        }
        else
            cout << "Command is unknown\n";
    }

    if (mytable)
        delete mytable;
    return 0;
}