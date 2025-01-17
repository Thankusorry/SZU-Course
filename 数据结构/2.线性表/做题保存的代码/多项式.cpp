#include<iostream>
using namespace std;
struct Node {
	int xishu = 0;
	int zhishu = 0;
	Node* next = nullptr;
};
void display(Node* beg) {
	Node* p = beg;
	while (p) {
		if (p->xishu < 0) {
			cout << "(" << p->xishu << ")";
		}
		else if (p->xishu > 0) {
			cout << p->xishu;
		}
		else if (p->xishu == 0) {
			p = p->next;
			continue;
		}
		if (p->zhishu != 0) {
			cout << "x^";
			if (p->zhishu < 0) {
				cout << "(" << p->zhishu << ")";
			}
			else cout << p->zhishu;
		}
		if (p->next) cout << " + ";
		p = p->next;
	}
	cout << endl;
}
void ex(Node* la, Node* lb) {
	Node* beg = new Node;
	Node* p = beg;
	while (la && lb) {
		if (la->zhishu == lb->zhishu) {
			if (la->xishu + lb->xishu != 0) {
				la->xishu += lb->xishu;
				p->next = la;
				la = la->next;
				lb = lb->next;
				p = p->next;
			}
			else {
				la = la->next;
				lb = lb->next;
			}
		}
		else if (la->zhishu < lb->zhishu) {
			p->next = la;
			la = la->next;
			p = p->next;
		}
		else {
			p->next = lb;
			lb = lb->next;
			p = p->next;
		}
	}
	if (la == nullptr) {
		p->next = lb;
	}
	else p->next = la;
	display(beg);
}
int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		int n;
		cin >> n;
		Node* beg1 = new Node;
		Node* p = beg1;
		for (int i = 0; i < n; i++) {
			p->next = new Node;
			p = p->next;
			cin >> p->xishu >> p->zhishu;
		}
		display(beg1);
		cin >> n;
		Node* beg2 = new Node;
		p = beg2;
		for (int i = 0; i < n; i++) {
			p->next = new Node;
			p = p->next;
			cin >> p->xishu >> p->zhishu;
		}
		display(beg2);
		ex(beg1, beg2);
	}

}