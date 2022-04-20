#include <iostream>
#include <vector>
#include <string>

class TreeExpressionNode {
private:
	std::string s = ""; 
	TreeExpressionNode* l = nullptr;
	TreeExpressionNode* r = nullptr;
	TreeExpressionNode* p = nullptr; 
public:
	TreeExpressionNode(std::string& s1) {
		s = s1; l = nullptr; r = nullptr; p = nullptr;
	}
	TreeExpressionNode() {
		l = nullptr; r = nullptr; p = nullptr;
	}
	std::string getName() {
		return s;
	}
	void setName(std::string& s1) {
		s = s1;
	}
	TreeExpressionNode* getLeft() {
		return l;
	}
	TreeExpressionNode* getRight() {
		return r;
	}
	void setL(TreeExpressionNode* newLeft) {
		l = newLeft;
	}
	void setR(TreeExpressionNode* newRight) {
		r = newRight;
	}
	void setR(std::string& s1) {
		setR(new TreeExpressionNode(s1));
	}
	void setL(std::string& s1) {
		setL(new TreeExpressionNode(s1));
	}
	void setParent(TreeExpressionNode* pr) {
		p = pr;
	}
	TreeExpressionNode* getParent() {
		return p;
	}
};

class TreeExpression {
private:
	TreeExpressionNode* root = nullptr;
public:
	TreeExpressionNode* getRoot() {
		return root;
	}
	void setRoot(TreeExpressionNode* rt) {
		root = rt;
	}
	TreeExpression() {
		root = nullptr;
	}
	std::string makePostfix(TreeExpressionNode* currentRoot);
	std::string makePrefix(TreeExpressionNode* currentRoot);
	std::string makeInfix(TreeExpressionNode* currentRoot);
	void buildTreeInfix(std::string& s, TreeExpressionNode* root);
	void buildTreePrefix(std::string& s1, TreeExpressionNode* root);
	void buildTreePostfix(std::string& s1, TreeExpressionNode* root);
};

std::string TreeExpression::makePostfix(TreeExpressionNode* currentNode) {
	std::string ret = ""; if (currentNode == nullptr) return ret;
	ret += makePostfix(currentNode->getLeft());
	ret += makePostfix(currentNode->getRight());
	ret += currentNode->getName();
	ret += " ";
	return ret;
}
std::string TreeExpression::makePrefix(TreeExpressionNode* currentNode) {
	std::string ret = "";
	if (currentNode == nullptr) return ret;
	ret += currentNode->getName();
	ret += " ";
	ret += makePrefix(currentNode->getLeft());
	ret += makePrefix(currentNode->getRight());
	return ret;
}
std::string TreeExpression::makeInfix(TreeExpressionNode* currentNode) {
	std::string ret = "";
	if (currentNode == nullptr) return ret;
	ret += ")";
	ret = makeInfix(currentNode->getRight()) + ret;
	ret = currentNode->getName() + ret;
	ret = " " + ret;
	ret = makeInfix(currentNode->getLeft()) + ret;
	ret = "(" + ret;
	return ret;
}

void TreeExpression::buildTreeInfix(std::string& expression, TreeExpressionNode* currentNode) {
	std::vector<int> blnc;
	std::string expr;
	for (int i = 0; i < expression.size(); ++i) {
		if (expression[i] != ' ') {
			expr.push_back(expression[i]);
		}
	}
	expr = expression;
	if (blnc.empty()) {
		blnc.assign(expr.size(), 0);
		int curBal = 0;
		for (int i = 0; i < expr.size(); ++i) {
			if (i != 0) blnc[i] = blnc[i - 1];
			if (expr[i] == ')') blnc[i]--;
			if (expr[i] == '(') blnc[i]++;
		}
		int mn = 1e9 + 17; int mnpos = 0;
		for (int i = expr.size() - 1; i >= 0; --i) {
			if (expr[i] != '(' && expr[i] != ')' && blnc[i] < mn) {
				mn = blnc[i]; mnpos = i;
			}
		}
		int i = mnpos;
		while (i >= 0 && (expr[i] != '-' && expr[i] != '+' && expr[i] != '/' && expr[i] != '*')) i--;
		if (i == -1) {
			std::string ret = "";
			for (int j = 0; j < expr.size(); ++j) {
				if (expr[j] != ')' && expr[j] != ' ' && expr[j] != '(') ret.push_back(expr[j]);
			}
			currentNode->setName(ret);
			return;
		}
		std::string s1 = expr.substr(i, 1);
		currentNode->setName(s1);
		TreeExpressionNode* R1 = new TreeExpressionNode();
		R1->setParent(currentNode); currentNode->setR(R1);
		TreeExpressionNode* L1 = new TreeExpressionNode();
		L1->setParent(currentNode); currentNode->setL(L1);
		std::string leftString = expr.substr(0, i);
		std::string rightString = expr.substr(i + 1);
		buildTreeInfix(leftString, currentNode->getLeft());
		buildTreeInfix(rightString, currentNode->getRight());

	}
}

void TreeExpression::buildTreePrefix(std::string& s1, TreeExpressionNode* currentNode) {
	if (s1[0] == '+' || s1[0] == '-' || s1[0] == '*' || s1[0] == '/') {
		std::string ret = s1.substr(0, 1);
		currentNode->setName(ret);
		TreeExpressionNode* newRight = new TreeExpressionNode();
		newRight->setParent(currentNode);
		currentNode->setR(newRight);
		TreeExpressionNode* newLeft = new TreeExpressionNode();
		newLeft->setParent(currentNode);
		currentNode->setL(newLeft);
		s1 = s1.substr((2));
		buildTreePrefix(s1, currentNode->getLeft());
		buildTreePrefix(s1, currentNode->getRight());
	}
	else {
		int i = 0;
		while (i < s1.size() && s1[i] != ' ') i++;
		std::string tempName = s1.substr(0, i); currentNode->setName(tempName);
		if (i < s1.size()) s1 = s1.substr(i + 1);
		return;
	}
}

void TreeExpression::buildTreePostfix(std::string& s1, TreeExpressionNode* currentNode) {
	int curLen = s1.size();
	if (s1[curLen - 1] == '+' || s1[curLen - 1] == '/' || s1[curLen - 1] == '-' || s1[curLen - 1] == '*') {
		std::string ret = s1.substr(curLen - 1);
		currentNode->setName(ret);
		TreeExpressionNode* newRight = new TreeExpressionNode();
		newRight->setParent(currentNode);
		currentNode->setR(newRight);
		TreeExpressionNode* newLeft = new TreeExpressionNode();
		newLeft->setParent(currentNode);
		currentNode->setL(newLeft);
		std::string remains = s1.substr(0, curLen - 2);
		buildTreePostfix(remains, currentNode->getRight());
		buildTreePostfix(remains, currentNode->getLeft());
	}
	else {
		int i = s1.size() - 1;
		while (i >= 0 && s1[i] != ' ') --i;
		if (i != -1) {
			std::string curName = s1.substr(i + 1);
			currentNode->setName(curName);
		}
		else {
			currentNode->setName(s1);
		}
		s1 = s1.substr(0, i);
		return;
	}
}



int main() {
	std::string s1 = "";
	std::cin >> s1;
	std::string expression = "";
	std::getline(std::cin, expression);
	std::getline(std::cin, expression);
	TreeExpressionNode* ROOT = new TreeExpressionNode();
	TreeExpression tree;
	tree.setRoot(ROOT);
	if (s1 == "infix") {
		tree.buildTreeInfix(expression, ROOT);
		std::string S1 = tree.makePostfix(ROOT);
		std::string S2 = tree.makePrefix(ROOT);
		std::cout << S1 << "\n" << S2 << std::endl;
	}
	if (s1 == "prefix") {
		tree.buildTreePrefix(expression, ROOT);
		std::string S1 = tree.makeInfix(ROOT);
		std::string S2 = tree.makePostfix(ROOT);
		std::cout << S1 << "\n" << S2 << std::endl;
	}
	if (s1 == "postfix") {
		tree.buildTreePostfix(expression, ROOT);
		std::string S1 = tree.makeInfix(ROOT);
		std::string S2 = tree.makePrefix(ROOT);
		std::cout << S1 << "\n" << S2 << std::endl;
	}
}
