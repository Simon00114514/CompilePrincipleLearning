/*
 * @Author: cos
 * @Date: 2022-04-30 14:20:51
 * @LastEditTime: 2022-05-01 02:34:12
 * @LastEditors: cos
 * @Description: ʵ��4 LR(1) ������
 * @FilePath: \CompileTheory\experiment_4\demo\main.cpp
 */
#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <vector>
using namespace std;
const string ExpFileName = "./exp.txt";
const string GotoFileName = "./goto.txt";
const string ActionFileName = "./action.txt";
const int Null = -1;
// ACTION��
// + * ( ) i #
string action[12][6];
// goto��
// a b #
int _goto[12][3];
string vt = "+*()i#";      // �ս����
string vn = "ETF";        // ���ս����
string LR[6] = { "E->E+T", "E->T", "T->T*F", "T->F", "F->(E)", "F->i" };   // ��Ų���ʽ
// ���ļ�
vector<string> readFile(string fileName) {
    vector<string> res;
    try {
        ifstream fin;
        fin.open(fileName);
        string temp;
        while (getline(fin, temp))
            res.push_back(temp);
        return res;
    }
    catch (const exception& e) {
        cerr << e.what() << '\n';
        return res;
    }
}
void printActions() {
    cout << "-----------------ACTION��------------------" << endl;
    cout << "+\t*\t(\t)\ti\t$" << endl;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 6; ++j)
            cout << action[i][j] << "\t";
        cout << endl;
    }
}
void printGotos() {
    cout << "-----------------GOTO��------------------" << endl;
    cout << "E\tT\tF" << endl;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 3; ++j)
            cout << _goto[i][j] << "\t";
        cout << endl;
    }
}
void init() {
    vector<string> actions = readFile(ActionFileName);
    for (int i = 0; i < 12; ++i) {
        int cnt = 0;
        string row = actions[i];
        int len = actions[i].length();
        for (int j = 0; j < len; ++j) {
            string temp = "";
            while (j < len && row[j] != ' ' && row[j] != '\t') {
                temp += row[j];
                ++j;
            }
            while (j < len && (row[j] == ' ' || row[j] == '\t'))
                ++j;
            --j;
            action[i][cnt++] = temp;
        }

    }
    printActions();
    vector<string> gotos = readFile(GotoFileName);
    for (int i = 0; i < 12; ++i) {
        int cnt = 0;
        string row = gotos[i];
        int len = row.length();
        for (int j = 0; j < len; ++j) {
            string temp = "";
            while (j < len && row[j] != ' ' && row[j] != '\t') {
                temp += row[j];
                ++j;
            }
            while (j < len && (row[j] == ' ' || row[j] == '\t'))
                ++j;
            --j;
            _goto[i][cnt++] = (temp == "N") ? Null : stoi(temp);
        }
    }
    printGotos();
}
bool isTerminator(char c) {
    return vt.find(c) != string::npos;
}
int findTerminator(char c) { // �����ս�������±�
    return vt.find(c);
}
int findNonTerminator(char c) { // ���ط��ս�����±�
    return vn.find(c);
}
// ��ջת��Ϊ�ַ�������
string s2string(stack<int> s) {
    string str = "";
    while(!s.empty()) {
        str += to_string(s.top()) + " ";
        s.pop();
    }
    return str;
}
// ���ʣ�����봮
void printRestInput(string exp, int start, int len) {
    for(int i = start; i < len; ++i) 
        cout << exp[i];
    cout << '\t';
}
void analyzeLR1(string exp) {  // ����һ�����ʽ
    int len = exp.length();
    stack<char> chars;  // ����ջ
    stack<int> state;   // ״̬ջ
    state.push(0);  // ��ʼ״̬Ϊ0
    chars.push('#');  // ��ʼ����Ϊ#
    string charsStr = "#";
    stack<int> copyState;
    copyState.push(0);
    int cnt = 0;    // ���
    int idx = 0;  // ��ǰ����ָ��
    cout << "���\t\t״̬ջ\t\t����ջ\t\t���봮\t\t����" << endl;
    cout << cnt++ << '\t' << s2string(copyState) << '\t' << charsStr << '\t' << exp << '\t' << " ��ʼ״̬ " << endl;
    while(1) {
        int nowState = state.top();
        char nowChar = exp[idx];    // ��ǰ�����ַ�
        int isT = findTerminator(nowChar);
        if(isT == Null) {   // ���ս��
            cout << "Error!" << "���ַǷ��ַ�����������˳�" <<endl;
            return;
        }
        string actionStr = action[nowState][isT];
        if(actionStr == "acc") {
            cout << cnt++ << '\t' << s2string(copyState) << '\t' << charsStr << '\t' << exp << '\t' << " accept ���ܣ� " << endl;
            return;
        } else if(actionStr == "N") {
            cout << cnt++ << '\t' << s2string(copyState) << '\t' << charsStr << '\t' << exp << '\t' << "Error! �����쳣�˳�" << endl;
            return;
        } else if(actionStr[0] == 'r') {   // ��Լ
            int num = stoi(actionStr.substr(1));    // ѡ�õڼ�������ʽ��Լ
            int len = LR[num-1].length()-3;
            while(len--) {
                chars.pop();        // ��ջ����Լ
                state.pop();
                charsStr = charsStr.substr(0, charsStr.length()-1);
                copyState.pop();   // �������
            }
            chars.push(LR[num-1][0]);   // ����ʽ�������ջ
            charsStr += LR[num-1][0];

            int nowState = state.top();
            int gidx = findNonTerminator(LR[num-1][0]);
            int newState = _goto[nowState][gidx];
            state.push(newState);
            copyState.push(newState);

            cout << cnt++ << '\t' << s2string(copyState) << '\t' << charsStr  << '\t';
            printRestInput(exp, idx, len);
            cout << '\t' << " ��Լ " << LR[num-1] << endl;
        } else if(actionStr[0] == 's') {    // �ƽ�
            int newState =  stoi(actionStr.substr(1));
            state.push(newState);
            copyState.push(newState);

            chars.push(nowChar);
            charsStr += nowChar;
            ++idx;  // ����ָ�����

            cout << cnt++ << '\t' << s2string(copyState) << '\t' << charsStr << '\t';
            printRestInput(exp, idx, len);
            cout << '\t' << actionStr << " �ƽ� " << endl;
        } else {
            cout << "Error!" << "�����쳣�˳�" <<endl;
            return;
        }
    }
}
int main() {
    cout << "LR��1���������򣬱����ˣ�xxx xxxxxxxx xxxx��" << endl;
    cout << "��ʾ:������ֻ�ܶ���'i','+','*','/','(',')'���ɵ���'#'�����ı��ʽ���з�����ÿ�ж���һ�����ʽ" << endl;
    cout << "��ȡ���ļ���Ϊ��" << ExpFileName << endl; 
    init();
    vector<string> exps = readFile(ExpFileName);
    int len = exps.size();
    for (int i = 0; i < len; i++) {
        string exp = exps[i];
        cout << "\n------------------���������ʽ" << i+1 << "��"<< exp << "--------------------" << endl;
        bool flag = true;
        for (int j = 0; j < exp.length(); j++) {
            if (!isTerminator(exp[j])) {
                cout << "�� "<<   i+1 << "��������ַ������Ϸ�������������" << endl;
                flag = false;
                break;
            }
        }
        if (flag) {
            cout << "���ʽ"  << i+1 << "��" << exp << "������ʼ" << endl;
            analyzeLR1(exp);
        }
    }
    return 0;
}
