#include<iostream>
using namespace std;

struct Node {
	int data;
	Node* next;
	Node(int data = 0, Node* next = NULL) :data(data), next(next) {}
};

void createlist(Node*& head, int n) {
	Node* p = head;
	for (int i = 0; i < n; i++) {
		Node* s = new Node(i+1);
		p->next = s;
		p = s;
	}
	head->data=n;
	p->next=head;
}
void yuesefu(Node* &head, int N, int K, int S) {
	Node* p = head;
	for (int i = 0; i < S - 1; i++) p = p->next;
	while (head->data  ) {
		if(p->next==head) p=p->next;
		for (int i = 0; i < K - 1; i++) {
			p = p->next;
			if(p->next==head) 
				p=p->next;
		}
		Node* s = p->next;
		cout << s->data << " ";
		p->next = s->next;
		delete s;
		head->data--;
	}
}
int main() {
	int t;
	cin >> t;
	while (t--) {
		int n, k, s;
		cin >> n >> k >> s;
		int a[1000];
		Node* head = new Node;
		createlist(head, n);
		yuesefu(head, n, k, s);
		cout << endl;
	}
	return 0;
}