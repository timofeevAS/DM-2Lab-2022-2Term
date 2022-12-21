#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct node {
	string var;
	int true_c, false_c;

	node(string var, int true_c, int false_c) {
		this->var = var;
		this->true_c = true_c;
		this->false_c = false_c;
	}
};


bool findByBDD(string params, vector<node>& BDD) {

	int answer = 0, cur_param = 0;
	node cur_node = BDD[0];

	while (cur_node.var != "1" || cur_node.var != "0") {
		if (cur_node.true_c == -1) {
			break;
		}
		if (params[cur_param] == '1') {
			cur_param += (BDD[cur_node.true_c].var.back() - '0') - (cur_node.var.back() - '0');
			cur_node = BDD[cur_node.true_c];
		}
		else if (params[cur_param] == '0') {
			cur_param += (BDD[cur_node.false_c].var.back() - '0') - (cur_node.var.back() - '0');
			cur_node = BDD[cur_node.false_c];
		}
	}


	return cur_node.var == "1" ? true : false;
}

void generateNF(vector<string>& PDNF, vector<string>& PCNF, vector<string>& TABLE, string& func_vector) {
	//PDNF n PCNF

	//vector<string>tmp_var = { "x1","x2","x3","x4" };

	vector<string>tmp_var;
	for (int i = 1; i <= TABLE[0].size(); i++) {
		string s;
		s += "x";
		s.push_back(i + '0');
		tmp_var.push_back(s);
	}
	for (int i = 0; i < func_vector.size(); i++) {
		bool pdnf = true;
		if (func_vector[i] == '0') {
			pdnf = false;
		}
		string tmp = "";
		for (int j = 0; j < TABLE[i].size(); j++) {
			if (pdnf) {
				if (TABLE[i][j] == '0') {
					tmp += "!";
				}
				tmp += tmp_var[j];
				//if (j + 1 != TABLE[i].size()) {
				//	tmp += "&";
				//}

			}
			else {
				if (TABLE[i][j] == '1') {
					tmp += "!";
				}
				tmp += tmp_var[j];
				if (j + 1 != TABLE[i].size()) {
					tmp += " v ";
				}

			}
		}
		if (pdnf) {
			PDNF.push_back(tmp);
		}
		else {
			PCNF.push_back(tmp);
		}

	}


}


bool calcByPDNF(string params, vector<string>& PDNF) {
	bool ans = false;
	for (int i = 0; i < PDNF.size(); i++) {
		string expr = PDNF[i];
		bool tmp_x = true;

		while (expr.size() != 0) {
			int isNeg;
			(expr[0] == '!') ? isNeg = 1 : isNeg = 0;

			string tmpexpr = expr.substr(0, 2 + isNeg);
			expr = expr.substr(isNeg + 2, expr.size());

			bool tmp;
			(params[tmpexpr.back() - '0' - 1] == '0') ? tmp=false : tmp=true;

			if (isNeg) {
				tmp = !tmp;
			}

			tmp_x = tmp && tmp_x;
			if (tmp_x == false) {
				break;
			}
		}

		ans = (tmp_x || ans);
		
	}
	return ans;
}


void createZhegalkin(vector<string>& ZHG,string func_vector, vector<string>&TABLE) {
	vector<string>triangle;
	//vector<string>tmp_var = { "x1","x2","x3","x4" };
	vector<string>tmp_var;
	for (int i = 1; i <= TABLE[0].size(); i++) {
		string s;
		s += "x";
		s.push_back(i + '0');
		tmp_var.push_back(s);
	}
	triangle.push_back(func_vector);
 	while (func_vector.size() != 1) {
		string tmp;
		for (int i = 0; i < func_vector.size()-1; i++) {
			func_vector[i] != func_vector[i+1] ? tmp+= '1' : tmp+= '0';
		}
		triangle.push_back(tmp);
		func_vector = tmp;
	}
	if (triangle[0][0] == '1') {
		ZHG.push_back("1");
	}
	for (int i = 1; i < triangle.size(); i++) {
		if (triangle[i][0] == '1') {
			string tmp = "";
			for (int j = 0; j < TABLE[i].size(); j++) {
				if (TABLE[i][j] == '1') {
					tmp += tmp_var[j];
				}
			}
			ZHG.push_back(tmp);
		}
	}

}

int main() {

	vector<node> BDD = {
		node("x1",2,1),//0
		node("x2",6,3),//1
		node("x2",5,4),//2
		node("x3",8,9),//3
		node("x3",6,9),//4
		node("x3",6,7),//5
		node("x4",8,9),//6
		node("x4",9,8),//7
		node("1",-1,-1),//8
		node("0",-1,-1),//9


	};


	vector<string> TABLE;

	string func_vector = "0011010100011001";
	//string func_vector = "00110011";

	bool VARIATIVE = true;

	if (func_vector != "0011010100011001") {
		VARIATIVE = false;
	}
	int N = log2(func_vector.size());
	{
		char x1 = '0', x2 = '0', x3 = '0', x4 = '0';
		char t1 = '1', t2 = '1', t3 = '1', t4 = '1';
		for (int i = 1; i <= func_vector.size(); i++) {
			string str = { x1,x2,x3,x4 };
			
			TABLE.push_back(str.substr(4-N,4));
			if (i % 8 == 0 && func_vector.size()>=16) {
				swap(x1, t1);
			}
			if (i % 4 == 0 && func_vector.size() >= 8) {
				swap(x2, t2);
			}
			if (i % 2 == 0 && func_vector.size() >= 4) {
				swap(x3, t3);
			}
			swap(x4, t4);
		}
	}

	cout << "VECTOR OF FUNC: " << func_vector << "\n";

	cout << "\n\nTRUTH OF TABLE: \n";

	for (int i = 0; i < TABLE.size(); i++) {
		cout << TABLE[i] + ": " + func_vector[i] << '\n';
	}
	if (VARIATIVE) {
		cout << "\n\nBDD : \n";

		for (int i = 0; i < TABLE.size(); i++) {
			if (findByBDD(TABLE[i], BDD)) {
				cout << TABLE[i] << ": 1\n";
			}
			else {
				cout << TABLE[i] << ": 0\n";
			}
		}
	}

	vector<string>PDNF;
	vector<string>PCNF;
	vector<string>ZHG;


	generateNF(PDNF, PCNF, TABLE, func_vector);


	cout << "\n\nPDNF:\n";
	for (int i = 0; i < PDNF.size(); i++) {
		cout << "(" + PDNF[i] + ")";
		if (i + 1 < PDNF.size()) {
			cout << "v";
		}
	}

	cout << "\n\nPCNF:\n";
	for (int i = 0; i < PCNF.size(); i++) {
		cout << "(" + PCNF[i] + ")";
		if (i + 1 < PCNF.size()) {
			cout << "&";
		}
	}
	cout << "\n\nCALCULATE VALUE BY PDNF: \n";
	for (int i = 0; i < TABLE.size(); i++) {
		if (calcByPDNF(TABLE[i], PDNF)) {
			cout << TABLE[i] << ": 1\n";
		}
		else {
			cout << TABLE[i] << ": 0\n";
		}
	}

	createZhegalkin(ZHG, func_vector, TABLE);
	cout << "\n\nZhegalkin polynomial:\n";
	for (int i = 0; i < ZHG.size(); i++) {
		cout << ZHG[i];
		if (i + 1 < ZHG.size()) {
			cout << " + ";
		}
	}


	//autotest
	if (VARIATIVE) {
		cout << "\n\nAUTOTEST CHECKING BY BDD SEARCH AND PDNF:\n";
		for (int i = 0; i < TABLE.size(); i++) {
			if (calcByPDNF(TABLE[i], PDNF) == findByBDD(TABLE[i], BDD)) {
				cout << TABLE[i] << " test confirmed\n";
			}
			else {
				cout << TABLE[i] << " test assert!!!\n";
				return 0;
			}
		}
		cout << "\n\nAUTOTEST CHECKING BY BDD SEARCH AND TABLE:\n";
		for (int i = 0; i < TABLE.size(); i++) {

			if (findByBDD(TABLE[i], BDD) == func_vector[i] == '0' ? false : true) {
				cout << TABLE[i] << " test confirmed\n";
			}
			else {
				cout << TABLE[i] << " test assert!!!\n";
				return 0;
			}
		}


	}

	cout << "\n\nAUTOTEST CHECKING BY PDNF AND TABLE OF TRUTH:\n";
	for (int i = 0; i < TABLE.size(); i++) {
		
		if (calcByPDNF(TABLE[i], PDNF) == func_vector[i]=='0' ? false : true) {
			cout << TABLE[i] << " test confirmed\n";
		}
		else {
			cout << TABLE[i] << " test assert!!!\n";
			return 0;
		}
	}


}
