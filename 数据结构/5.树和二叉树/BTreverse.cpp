#include<iostream>
using namespace std;

typedef struct BiTNode{
    char data;
    BiTNode* left;
    BiTNode* right;
    BiTNode (int a, BiTNode* l, BiTNode* r):data(a), left(l), right(r){}
}BT;

BT* createtree (string s,int &i){
    if (s[i]=='#') return NULL;
    BT *root = new BT(s[i], nullptr, nullptr);
    root->left = createtree(s,++i);
    root->right = createtree(s,++i);
    if(root->right && root->left)
        swap(root->left, root->right);
    return root;
}

void preorder(BT *root){
    if(!root)
        return ;
    cout << root->data;
    preorder(root->left);
    preorder(root->right);
}
void inorder(BT* root){
    if(!root)
        return;
    inorder(root->left);
    cout << root->data;
    inorder(root->right);
}
void postorder(BT *root){
    if(!root)
        return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data;
}
int main(){
    int t;
    cin>>t;
    while(t--){
        string s;
        cin >> s;
        int i=0;
        BT* root = createtree(s,i);
        postorder(root);
        cout << endl;
        inorder(root);
        cout << endl;
        postorder(root);
        cout << endl; 
    }
    return 0;
}