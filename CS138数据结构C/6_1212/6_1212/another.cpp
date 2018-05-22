#include<iostream> 
#include<queue>
using namespace std;


class tree{
public:
    
    int data;
    tree *left;
    tree *right;
    tree *parent;

        tree() :left(0), right(0),parent(0) {}
    
};

queue<tree *>nodes;

void levelOrder(tree *&t) {
		if (t!=NULL) {
			cout << t->data << ' ';
			if (t->left!=NULL) {
				nodes.push(t->left);
			}
			if (t->right!=NULL) {
				nodes.push(t->right);
			}
			if (!nodes.empty()) {
				t = nodes.front();
				nodes.pop();
				levelOrder(t);
			}
		}
	}
    
int main()
{
    int n;
    cin >> n;
    
    int lt, rt, vi;
    
    tree *node = new tree[n + 1];
    
    for (int i = 1;i<n + 1;i++)
    {
        cin >> lt >> rt>>vi;
        
        node[i].data = vi;
        
        if (lt != 0)
        {
            node[i].left = &node[lt];
            node[lt].parent = &node[i];
        }
        
        if (rt != 0)
        {
            node[i].right = &node[rt];
            node[rt].parent = &node[i];
        }
    }
    
    tree *root = 0;
    
    int i = 1;
    while (node[i].parent != 0)
        ++i;
    
    root = &node[i];
    levelOrder(root);
    return 0;
}