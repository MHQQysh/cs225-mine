#pragma GCC diagnostic ignored "-Wc++11-extensions"
#pragma GCC diagnostic ignored "-Wunqualified-std-cast-call"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class K, class V>
class BTree {
public:
    BTree(int _order) : order(_order), root(nullptr) {
        if (_order < 2) {
            throw invalid_argument("Order must be at least 2");
        }
    }

    ~BTree() {
        clear(root);
    }

    V find(const K& key) const {
        if (!root) return V(); // Handle empty tree
        return find(root, key);
    }

    void insert(const K& key, const V& value) {
        if (!root) {
            root = new BTreeNode(true);
        }

        if (root->keys.size() == 2 * order - 1) {
            BTreeNode* new_root = new BTreeNode(false);
            new_root->children.push_back(root);
            split_child(new_root, 0);
            root = new_root;
        }

        insert_non_full(root, key, value);
    }

private:
    struct BTreeNode {
        vector<K> keys;
        vector<V> values;
        vector<BTreeNode*> children;
        bool is_leaf;

        BTreeNode(bool leaf) : is_leaf(leaf) {}
    };

    BTreeNode* root;
    int order;

    void clear(BTreeNode* node) {
        if (!node) return;
        if (!node->is_leaf) {
            for (auto& child : node->children) {
                clear(child);
            }
        }
        delete node;
    }

    int insertion_idx(const BTreeNode* node, const K& key) const {
        auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
        return it - node->keys.begin();
    }

    V find(const BTreeNode* subroot, const K& key) const {
        if (!subroot) return V(); // Handle nullptr subroot
        int idx = insertion_idx(subroot, key);
        if (idx < subroot->keys.size() && subroot->keys[idx] == key) {
            return subroot->values[idx];
        } else if (subroot->is_leaf) {
            return V();
        } else {
            return find(subroot->children[idx], key);
        }
    }

    void split_child(BTreeNode* parent, int idx) {
        BTreeNode* child = parent->children[idx];
        BTreeNode* new_node = new BTreeNode(child->is_leaf);

        new_node->keys.reserve(order - 1);
        new_node->values.reserve(order - 1);
        new_node->children.reserve(order);

        for (int j = 0; j < order - 1; ++j) {
            new_node->keys.push_back(move(child->keys[j + order]));
            new_node->values.push_back(move(child->values[j + order]));
        }

        if (!child->is_leaf) {
            for (int j = 0; j < order; ++j) {
                new_node->children.push_back(child->children[j + order]);
            }
        }

        child->keys.resize(order - 1);
        child->values.resize(order - 1);
        if (!child->is_leaf) {
            child->children.resize(order);
        }

        parent->children.insert(parent->children.begin() + idx + 1, new_node);

        parent->keys.insert(parent->keys.begin() + idx, move(child->keys.back()));
        child->keys.pop_back();
        child->values.pop_back();
    }

    void insert_non_full(BTreeNode* node, const K& key, const V& value) {
        int idx = insertion_idx(node, key);
        if (idx < node->keys.size() && node->keys[idx] == key) {
            node->values[idx] = value;
            return;
        }

        if (node->is_leaf) {
            node->keys.insert(node->keys.begin() + idx, key);
            node->values.insert(node->values.begin() + idx, value);
        } else {
            if (node->children[idx]->keys.size() == 2 * order - 1) {
                split_child(node, idx);
                if (key > node->keys[idx]) {
                    ++idx;
                }
            }
            insert_non_full(node->children[idx], key, value);
        }
    }
};

int main() {
    BTree<int, string> tree(3); // 创建一个阶为3的B树

    // 插入键值对
    tree.insert(3, "three");
    tree.insert(5, "five");
    tree.insert(12, "twelve");
    tree.insert(22, "twenty-two");
    tree.insert(39, "thirty-nine");

    // 查找键值对
    int key_to_find = 39;
    string value = tree.find(key_to_find);
    if (!value.empty()) {
        cout << "Found value for key " << key_to_find << ": " << value << endl;
    } else {
        cout << "Key " << key_to_find << " not found." << endl;
    }

    // 查找不存在的键
    key_to_find = 100;
    value = tree.find(key_to_find);
    if (!value.empty()) {
        cout << "Found value for key " << key_to_find << ": " << value << endl;
    } else {
        cout << "Key " << key_to_find << " not found." << endl;
    }

    return 0;
}